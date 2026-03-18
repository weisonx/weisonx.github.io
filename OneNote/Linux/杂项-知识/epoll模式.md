##  epoll 

基本调用流程：

1. `epoll_create1()` -> 获得 epoll 实例
2. `epoll_ctl()` -> 往 epoll 里添加/修改/删除 fd 及其关注的事件
3. `epoll_wait()` -> 等待事件发生，返回就绪的 fd 列表

##  两种核心“模式”：水平触发（LT）和边缘触发（ET）

在讨论 “epoll 模式”时，最核心的就是 LT（Level-Triggered）和 ET（Edge-Triggered）。

### 2.1 水平触发（LT, 默认模式）

- 默认行为，不设置 `EPOLLET` 就是 LT。
- 只要缓冲区里**还有**数据没读完，或者写缓冲区**还能写**，`epoll_wait` 每次都会把这个 fd 返回。
- 匹配直觉，写起来比较简单，不容易漏事件。

工作方式示例（读事件）：

1. socket 可读，内核中有 100 字节
2. 一次 `read()` 只读了 50 字节，还剩 50 字节
3. 下次 `epoll_wait()` 还会再次返回这个 fd，提示你“还可读”

适合：简单实现、先保证正确，不追求极致性能时。

### 2.2 边缘触发（ET, `EPOLLET`）

- 设置 `EPOLLET` 标志：`event.events = EPOLLIN | EPOLLET;`
- 只在状态**发生变化时**触发一次，比如：
  - 缓冲区从“不可读”变成“可读”的那一刻
  - 从“不可写”变成“可写”的那一刻
- 如果你没把数据“全部读干净”，后面不会再收到事件通知，很容易丢事件。

因此，**使用 ET 时必须保证：一旦收到事件，要把能读的都读完 / 能写的都写完**（通常配合非阻塞 fd）。

典型代码（ET + 非阻塞）：

```c
void handle_read(int fd) {
    while (1) {
        char buf[4096];
        ssize_t n = read(fd, buf, sizeof(buf));
        if (n > 0) {
            // 处理数据
        } else if (n == 0) {
            // 对端关闭
            close(fd);
            break;
        } else {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // 已经没有数据可读了
                break;
            } else {
                // 读错误
                close(fd);
                break;
            }
        }
    }
}
```

特点：

- 优点：系统调用次数少，性能更好，适合高并发高性能服务器
- 缺点：逻辑复杂，**读/写不彻底就会丢事件**，容易出 bug

---

## EPOLLONESHOT 模式（一次性事件）

另一个经常说的“模式”是 `EPOLLONESHOT`：

- 设置：`event.events = EPOLLIN | EPOLLONESHOT;`
- epoll 只会对这个 fd 触发**一次**事件通知
- 触发后，如果你希望继续收到它的事件，需要在处理完之后再调用一次 `epoll_ctl(EPOLL_CTL_MOD)` 重新激活

用途：多线程处理同一个 epoll 时，防止多个线程同时处理同一个 fd。

示例（读完后重新激活）：

```c
void handle_event(int epfd, int fd) {
    handle_read(fd);

    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN | EPOLLONESHOT | EPOLLET; // 需要的话加 EPOLLET
    epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);      // 重新激活
}
```

---

## 4. 常见组合

1. **LT + 非阻塞**  
   - 默认最安全的组合
   - 代码简单，不易出错
   - 适合大部分业务，性能也足够

2. **ET + 非阻塞**  
   - 高性能服务器常用
   - 每个事件到来时必须循环读/写直到 `EAGAIN`
   - 对代码要求高

3. **ET + EPOLLONESHOT + 非阻塞 + 多线程**  
   - 常见于多线程网络库
   - 保证同一时刻只有一个线程处理某个 fd
   - 业务处理完后再激活

---

## 5. 小总结

- “epoll 模式”一般指：
  - 触发模式：LT（默认） vs ET（`EPOLLET`）
  - 单次触发：`EPOLLONESHOT`
- 初学和一般业务：用 **LT + 非阻塞** 就够用
- 高性能、自己写网络库：考虑 **ET + 非阻塞**，可能再配合 `EPOLLONESHOT` 和线程池
