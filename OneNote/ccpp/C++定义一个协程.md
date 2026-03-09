在 C++20 里，“协程”不是一个新的关键字块，而是由编译器自动改写的一种函数形式。  
**要“定义”一个可用的协程，本质上必须具备这些要素：**

---

### 1. 一个包含协程关键字的函数

函数体中必须出现以下三个关键字中的至少一个，编译器才会把它当作协程来生成状态机代码：

- `co_await`
- `co_yield`
- `co_return`

示例：

```cpp
task foo() {      // 注意返回类型不是普通的 void/int，而是可协程的类型
    co_await some_awaiter;
    co_return 42;
}
```

---

### 2. 一个**协程返回类型**（coroutine return type）

这不是语法关键字，而是你自己定义 / 使用的类型，比如：

- `std::generator<T>`（来自库）
- `std::task`、`task<T>`（常见自定义）
- `std::future<T>`（特定实现支持）
- 自己写的 `MyTask` 类型

**要求：**  
这个类型必须在内部定义一个 `promise_type`，并满足协程所需的一套接口。

```cpp
struct task {
    struct promise_type {
        task get_return_object();
        std::suspend_always initial_suspend() noexcept;
        std::suspend_always final_suspend() noexcept;
        void unhandled_exception();
        void return_void();  // 或 return_value(T)
    };
};
```

只要函数的返回类型 `R` 存在成员 `R::promise_type`，编译器就会把这个函数视为协程，并生成对应的 `promise_type` 实例和状态机。

---

### 3. `promise_type` 的必要成员

严格来说，“一个可编译的协程”**至少**需要（形式略可变）：

```cpp
struct promise_type {
    // 创建协程返回对象（task / generator 等）
    ReturnType get_return_object();

    // 决定协程刚创建时是否立即挂起
    auto initial_suspend() noexcept;

    // 决定协程结束时是否挂起
    auto final_suspend() noexcept;

    // 处理协程内部抛出的异常
    void unhandled_exception();

    // 根据你的协程语义选择其一：
    void return_void();          // 如果协程没有返回值
    // 或：
    void return_value(T value);  // 如果有返回值

    // 如果用 co_yield，还需要：
    auto yield_value(T value);
};
```

其中 `initial_suspend` / `final_suspend` / `yield_value` / `co_await` 都要返回“可等待体”（awaiter），即具有：

```cpp
struct awaiter {
    bool await_ready() noexcept;
    void await_suspend(std::coroutine_handle<>) noexcept;
    void await_resume() noexcept;    // 或返回一个值
};
```

---

### 4. 对 `co_await` 对象的支持（awaitable / awaiter）

协程中 `co_await expr` 的 `expr` 必须是：

- 自身有 `operator co_await()`，或  
- 其类型有 `operator co_await(expr)` 的重载，或  
- 具有 `await_ready/await_suspend/await_resume` 成员（即直接是 awaiter）

所以要使用 `co_await`，需要有一个“可等待对象”（awaitable），通常实现为：

```cpp
struct my_awaitable {
    bool await_ready() noexcept;
    void await_suspend(std::coroutine_handle<> h) noexcept;
    int  await_resume() noexcept;
};
```

---

### 5. 协程句柄（一般不必手写）

标准库：`std::coroutine_handle<promise_type>` 用来操控协程（resume / destroy 等）。  
通常你在返回类型或 awaiter 中会用到它：

```cpp
#include <coroutine>

using handle = std::coroutine_handle<promise_type>;
```

正常使用时，只要写好 `promise_type` 和返回类型，句柄的创建与销毁由编译器和你的类型封装完成。

---

### 总结一句话

**要定义一个 C++20 协程函数，必须具备：**

1. 函数体中使用 `co_await` / `co_yield` / `co_return` 中至少一个；
2. 函数的返回类型需要定义 `promise_type`，并实现：
   - `get_return_object`
   - `initial_suspend`
   - `final_suspend`
   - `unhandled_exception`
   - `return_void` 或 `return_value`
3. 如用 `co_yield` / `co_await`，还需对应的 `yield_value` / awaiter 接口。

# 注意

协程函数可以类成员。协程函数在语法层面就是“带了 `co_await/co_yield/co_return` 的普通函数”，所以**完全可以是类成员函数**，包括：

- 普通成员函数
- `static` 成员函数
- `const` 成员函数
- 模板成员函数

只要返回类型满足协程要求（有 `promise_type` 等），就能作为协程。

示例：

```cpp
#include <coroutine>
#include <iostream>

struct task {
    struct promise_type {
        task get_return_object() {
            return task{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        std::suspend_never initial_suspend() noexcept { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void unhandled_exception() { std::terminate(); }
        void return_void() {}
    };

    std::coroutine_handle<promise_type> h;
    explicit task(std::coroutine_handle<promise_type> h) : h(h) {}
    ~task() { if (h) h.destroy(); }
};

struct Foo {
    // 普通成员协程
    task run() {
        std::cout << "member coroutine\n";
        co_return;
    }

    // const 成员协程
    task run_const() const {
        std::cout << "const member coroutine\n";
        co_return;
    }

    // static 成员协程
    static task run_static() {
        std::cout << "static member coroutine\n";
        co_return;
    }
};

int main() {
    Foo f;
    f.run();
    f.run_const();
    Foo::run_static();
}
```

注意点：

1. **`this` 正常可用**  
   在成员协程里可以像普通成员函数一样访问 `this`、成员变量等；协程被挂起时闭包里会保存 `this` 指针的拷贝（或引用/指针），所以要注意对象生命周期不要早于协程使用它。

2. **生命周期问题要自己管理**  
   - 如果协程里用到了 `this`，而对象提前析构，协程继续 `resume` 就是未定义行为。
   - 常见做法：用 `std::shared_ptr`/`std::weak_ptr` 管理对象，或者保证协程结束前对象一定存在。
