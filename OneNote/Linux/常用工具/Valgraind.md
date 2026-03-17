Valgrind 是一套开源的程序运行时分析工具集，最常用来做：

1. **内存错误检查（最常用）**  
   工具：`memcheck`  
   能帮你发现：
   - 内存泄漏（malloc/new 了但没 free/delete）
   - 访问已释放的内存
   - 读写未初始化的内存
   - 越界读写（数组/指针越界）
   - 不匹配的分配与释放（malloc + delete 等）

2. **性能分析（CPU 方面）**  
   工具：`callgrind`  
   - 分析函数调用次数、耗时
   - 可配合 `kcachegrind` 做可视化，优化热点代码

3. **线程问题检查**  
   工具：`helgrind` / `drd`  
   - 检查数据竞争（data race）
   - 检查锁使用错误

---

### 基本用法

假设你有一个可执行程序 `a.out`：

```bash
# 1. 内存错误检查（最常用）
valgrind --leak-check=full ./a.out

# 2. 性能分析
valgrind --tool=callgrind ./a.out

# 3. 线程竞争检查
valgrind --tool=helgrind ./a.out
```

常用选项说明（memcheck）- `--leak-check=full`：显示详细的内存泄漏信息  
- `--show-leak-kinds=all`：显示所有类型的泄漏（definite, indirect 等）  
- `--track-origins=yes`：追踪未初始化值的来源（更慢但更有用）  
- `-s`：输出总结信息

示例：

```bash
valgrind --leak-check=full --track-origins=yes -s ./a.out
```

---

### 输出怎么看（简略示例）

输出中会看到类似：

```text
==12345== 40 bytes in 1 blocks are definitely lost in loss record 1 of 1
==12345==    at 0x4C2FB55: malloc (vg_replace_malloc.c:299)
==12345==    by 0x4005ED: main (test.c:10)
```

含义：
- `definitely lost`：确定内存泄漏  
- 显示调用栈（test.c 第 10 行的 main 调用了 malloc 没有释放）

---

### 使用注意事项

- 只支持类 Unix 系统（Linux、部分 BSD、macOS 特定旧版本等），不支持原生 Windows
- 运行速度会明显变慢（通常 10 倍左右），适合作为调试工具，而不是生产环境
- 最适合 C/C++等手动管理内存的语言
