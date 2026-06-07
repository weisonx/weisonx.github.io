“如何使用”协程，可以分两层理解：

1. **在协程内部怎么写（co_await / co_return / co_yield）**
2. **在协程外部怎么驱动它（resume / 等待结果 / 获取返回值）**

# 在外部“使用”这个协程

使用方式就是：

1. 调用协程函数，得到一个 `task` 对象（协程闭包）
2. 通过 `task` 内部的句柄 `resume()` 它

```cpp
int main() {
    task t = my_coroutine();  // 这里只是创建协程 + 调用 initial_suspend()，不会执行主体

    std::cout << "first resume:\n";
    t.resume();  // 执行到 co_await std::suspend_always{} 挂起

    std::cout << "second resume:\n";
    t.resume();  // 从挂起处继续，执行到结束，进入 final_suspend 挂起

    std::cout << "third resume:\n";
    t.resume();  // done() == true，不再执行
    
    return 0;
}
```

**执行顺序大致是：**

- 调用 `my_coroutine()`：构造协程对象，执行 `initial_suspend()` 后挂起，不打印 `step 1`
- 第一次 `resume()`：打印 `step 1`，遇到 `co_await suspend_always` 再挂起
- 第二次 `resume()`：从 `co_await` 之后继续，打印 `step 2`，`co_return`，进入 `final_suspend`
- 再 `resume()`：协程已结束，`done()==true`，不再执行

---

# 使用 `co_await` 等待其它协程

更常见的用法是：一个协程内部 `co_await` 另一个协程或 awaitable。

定义一个可 `co_await` 的简单类型：

```cpp
struct simple_awaitable {
    bool await_ready() const noexcept { 
        return false;  // 返回 false 表示需要挂起
    }
    void await_suspend(std::coroutine_handle<> h) const noexcept {
        // 这里可以把句柄保存起来，将来再唤醒它；简单起见，直接马上继续
        h.resume();
    }
    void await_resume() const noexcept {
        // 挂起结束恢复时要做的事情，这里什么都不做
    }
};
```

在协程里使用：

```cpp
task child() {
    std::cout << "child start\n";
    co_await simple_awaitable{};
    std::cout << "child end\n";
}

task parent() {
    std::cout << "parent: before child\n";
    co_await simple_awaitable{}; // 可以是 awaitable，也可以是 child() 返回的某种 Task
    std::cout << "parent: after child\n";
    co_return;
}
```

> 真正有用的库（如 async I/O）会提供复杂的 awaitable，用 `co_await` 就能等异步结果。

---

# 使用 `co_yield` 的生成器类

另外一种常见形式是“生成器”：协程一次 `co_yield` 一个值，外部每次 `next()` 取一个。

示意（简化版）：

```cpp
template <typename T>
struct generator {
    struct promise_type {
        T current_value;

        generator get_return_object() {
            return generator{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept return {}; }
        void unhandled_exception() { std::terminate(); }
        void return_void() {}

        std::suspend_always yield_value(T value) noexcept {
            current_value = value;
            return {};
        }
    };

    using handle = std::coroutine_handle<promise_type>;
    handle h;

    generator(handle h) : h(h) {}
    ~generator() { if (h) h.destroy(); }

    bool next() {
        if (!h || h.done()) return false;
        h.resume();
        return !h.done();
    }
    T value() const {
        return h.promise().current_value;
    }
};

generator<int> counter(int n) {
    for (int i = 0; i < n; ++i)
        co_yield i;
}
```

使用：

```cpp
int main() {
    auto g = counter(3);
    while (g.next()) {
        std::cout << g.value() << "\n";  // 依次输出 0 1 2
    }
}
```

# co_return

在“外部使用协程”的时候，其实不会再写 co_return 了。

co_return 是协程内部结束并设置返回结果的语法；在协程外部，只是“观察/获取”这个结果。

---

# 实际工程中“如何使用”

自己从零写 `promise_type` 一般只做一次，之后通常：

1. **用现成库的类型**
   - 有些实现提供 `std::generator`、`std::task` 等；
   - 网络库提供 `co_await socket.read()`、`co_await timer.async_wait()` 等 awaitable。

2. 你的代码中：
   - 写：`task foo()` / `generator<int> gen()` 这样的协程函数；
   - 用：
     - 要顺序的地方：`co_await foo();`
     - 要流式数据：用生成器 `for(...)` 或手写 `next()`。
