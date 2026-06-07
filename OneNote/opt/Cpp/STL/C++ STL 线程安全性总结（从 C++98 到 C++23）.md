# C++ STL 线程安全性总结（从 C++98 到 C++23）

在日常 C++ 开发中，一个经常被问到的问题是：**STL 容器是否线程安全？**
很多开发者的印象往往比较模糊，例如“只读好像是安全的”“vector 不能多线程 push_back”。实际上，从语言标准的发展来看，**STL 的线程安全规则在不同标准中逐渐明确，但核心原则几乎没有改变**。

本文按标准版本梳理 STL 线程安全语义，并给出开发中的实践建议。

---

## 一、C++11 之前：几乎没有线程安全保证

在 **C++98 / C++03** 时代，标准库并没有正式的线程模型，因此 STL 的线程安全行为基本没有明确规定。

原因主要有两个：

1. 语言标准本身没有线程库
2. 没有统一的内存模型（memory model）

因此当时 STL 的线程行为属于：

* **实现定义**
* 或 **未定义行为**

典型情况如下：

| 场景          | 行为          |
| ----------- | ----------- |
| 多线程同时读取容器   | 通常可行，但标准未保证 |
| 一个线程读，一个线程写 | 未定义行为       |
| 多线程同时写      | 未定义行为       |

不同标准库实现（如 GCC、Clang、MSVC）可能表现不同。

---

## 二、C++11：正式定义线程安全规则

从 **C++11** 开始，标准引入了完整的并发模型：

* `std::thread`
* `std::mutex`
* `std::atomic`
* C++ memory model

同时，标准库也首次明确规定了 **STL 的线程安全规则**。

核心原则只有三条：

### 1. 不同对象的并发访问是安全的

如果两个线程操作不同的容器对象：

```cpp
std::vector<int> a;
std::vector<int> b;
```

线程：

```cpp
T1: a.push_back(1);
T2: b.push_back(2);
```

这是完全安全的。

---

### 2. 同一对象的并发只读是安全的

如果多个线程只读取同一个容器，而不修改它：

```cpp
T1: v[0];
T2: v[1];
T3: v.size();
```

这种访问是 **线程安全** 的。

常见只读函数包括：

* `size()`
* `empty()`
* `operator[]`
* `front()`
* `back()`
* `find()`

只要没有任何线程修改容器结构，就不会产生 data race。

---

### 3. 任何写操作都必须外部同步

一旦涉及修改操作，例如：

```cpp
v.push_back(x);
v.erase(it);
v.insert(...)
```

就必须使用锁等同步机制：

```cpp
std::mutex m;

{
    std::lock_guard<std::mutex> lock(m);
    v.push_back(x);
}
```

否则会产生：

**data race → 未定义行为**

---

## 三、迭代器与并发问题

STL 中另一个常见问题是 **迭代器失效（iterator invalidation）**。

例如：

```cpp
T1: 遍历 vector
T2: vector.push_back()
```

如果 `push_back` 导致 **vector 扩容**：

* 内存会重新分配
* 所有 iterator / pointer / reference 失效

这在并发情况下非常容易导致：

* 崩溃
* 访问非法内存

因此：

**只要存在结构修改，就不能并发遍历。**

---

## 四、C++14 / C++17 / C++20 / C++23

在后续标准：

* C++14
* C++17
* C++20
* C++23

中，**STL 容器的线程安全规则基本没有变化**。

标准主要新增的是并发工具，例如：

* `std::jthread`
* `std::latch`
* `std::barrier`
* `std::atomic_ref`

但 STL 容器依然遵循同样原则：

> **Concurrent reads are safe, any write requires synchronization.**

---

## 五、STL 容器线程安全总结

常见容器在并发访问下的行为：

| 容器            | 多线程读 | 读+写 | 写+写 |
| ------------- | ---- | --- | --- |
| vector        | ✔    | ✖   | ✖   |
| deque         | ✔    | ✖   | ✖   |
| list          | ✔    | ✖   | ✖   |
| map           | ✔    | ✖   | ✖   |
| unordered_map | ✔    | ✖   | ✖   |
| set           | ✔    | ✖   | ✖   |

可以总结为一句话：

> **STL 容器只保证“读-读并发安全”，不保证任何写操作的并发安全。**

---

## 六、STL 为什么不做内部加锁？

很多人会问：既然并发这么常见，为什么 STL 不在内部加锁？

原因主要有三个：

### 1. 性能

锁会带来：

* 上下文切换
* cache miss
* 锁竞争

STL 的目标是 **零成本抽象（zero-overhead abstraction）**。

---

### 2. 使用场景不同

不同程序需要不同策略：

* coarse lock
* fine-grained lock
* lock-free
* RCU

标准库无法为所有场景选择最优策略。

---

### 3. 可组合性

如果容器内部加锁，用户再加锁可能产生：

* 死锁
* 锁顺序问题
* 不必要的性能损失

因此标准选择：

**把同步责任交给用户。**

---

## 七、如果需要并发容器

STL 本身不是并发容器库，如果需要高并发结构，一般使用：

* Intel TBB

  * `concurrent_vector`
  * `concurrent_hash_map`

* Facebook Folly

* Boost concurrent containers

这些库会提供：

* lock-free
* fine-grained locking
* 分段哈希表

等更适合多线程的实现。

---

## 八、一句话总结

从 **C++11 到 C++23**，STL 的线程安全规则始终非常简单：

**不同对象：线程安全
同一对象只读：线程安全
任何写操作：必须外部同步**

理解这一点，基本就能避免大多数 STL 并发问题。
