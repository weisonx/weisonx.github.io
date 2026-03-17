ASan（AddressSanitizer）是一个用于检测内存错误的编译期/运行时工具，常用来查内存问题

---

## ASan 是什么？

ASan 是由 Google 开发的内存错误检测工具，一般通过编译器选项启用，例如：

- 在 Clang 或 GCC 中：
  ```bash
  clang -fsanitize=address -g -O1 main.c -o main
  # 或
  g++ -fsanitize=address -g -O1 main.cpp -o main
  ```

- 运行时不需要专门的“分析器程序”，直接运行编译后的二进制即可：
  ```bash
  ./main
  ```

编译器会在编译时插入检查代码，运行时通过额外的“影子内存（shadow memory）”来跟踪内存使用。

---

## ASan 能检测哪些问题？

常见的有：

1. **越界访问**
   - 访问数组/堆/栈的非法索引：
     ```c
     int *p = new int[10];
     p[10] = 1; // 越界写（有效索引为 0~9）
     ```
2. **Use-after-free（释放后继续使用）**
   ```c
   int *p = (int *)malloc(sizeof(int));
   free(p);
   *p = 5; // use-after-free
   ```
3. **栈溢出/栈越界**
4. **双重释放（double free）**
5. **部分内存泄漏检测（配合 LeakSanitizer）**

出现错误时，ASan 会打印详细的堆栈信息、访问类型（读/写）、地址、大小等。

---

##  ASan vs Valgrind（简单对比）

| 特性                    | ASan                                   | Valgrind (memcheck)                       |
|-------------------------|----------------------------------------|-------------------------------------------|
| 工作方式                | 编译期插桩 + 运行时库                  | 动态二进制翻译（解释执行）               |
| 需要重新编译代码        | 是                                     | 否（直接运行已有二进制）                 |
| 运行时开销              | 较小（一般 2～5 倍）                   | 较大（常见 10～50 倍甚至更多）           |
| 能否在生产环境用        | 比较可行（通常用于测试/调试环境）     | 基本不现实（太慢）                       |
| 检测精度/类型           | 越界、use-after-free 等很强            | 也很强，还带较好的泄漏分析               |
| 对 C++ 模板、大工程支持 | 很好                                   | 一般，但通常也够用                       |

简单说：  
- 想要**高性能检测**、可以重新编译：用 ASan。  
- 无法重新编译、只想“直接跑”已有程序：用 Valgrind。

---

## 4. ASan 使用要点（GCC/Clang）

典型编译参数：

```bash
# 调试构建（建议带符号表 -g，优化不要太高）
g++ -fsanitize=address -fno-omit-frame-pointer -g -O1 main.cpp -o main
```

运行后，如果检测到问题，会得到类似：

```text
==12345==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x602000000014 ...
WRITE of size 4 at 0x602000000014 thread T0
    #0 0x400a2b in main /path/to/main.cpp:10
    ...
```

可以直接根据堆栈和源文件行号去定位问题。


## AddressSanitizer (ASan) 支持的平台

ASan 是编译器特性（Clang / GCC），只要编译器和系统支持就行。
