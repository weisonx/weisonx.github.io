# 一、重删（Deduplication）核心技术

重删本质是：

```text
相同数据块 → 只存一份 → 通过引用计数共享
```

但工程难点在于：

* 高并发
* 指纹表巨大
* 延迟敏感
* 与快照 / GC 强耦合

---

## 1️⃣ 分块技术（Chunking）

### 固定块（Fixed-size chunking）

典型粒度：

* 4KB
* 8KB

优点：

* 元数据简单
* CPU 成本低
* 容易并行

缺点：

* 跨块修改破坏匹配率

---

### 可变块（Content-Defined Chunking, CDC）

算法代表：

* Rabin Fingerprint
* FastCDC

原理：

* 滑动窗口
* 根据 hash 模式切分块

优点：

* 插入数据不破坏后续匹配
* 去重率高

缺点：

* CPU 成本高
* 实现复杂

---

## 2️⃣ 指纹计算（Hashing）

核心要求：

* 低碰撞率
* 高吞吐
* 可 SIMD 加速

主流算法：

| 算法         | 特点       |
| ---------- | -------- |
| SHA-1      | 经典，但偏慢   |
| SHA-256    | 更安全      |
| xxHash     | 极快，低安全性  |
| MurmurHash | 高性能      |
| BLAKE3     | 高性能+并行友好 |

现代 AFA 更偏向：

> 组合策略：快速 hash 预过滤 + 强 hash 校验

---

## 3️⃣ 指纹索引结构（Fingerprint Index）

这是重删的性能核心。

### 哈希表（Hash Table）

```text
fingerprint → physical address
```

要求：

* 常驻内存
* O(1) 查找
* NUMA 优化

内存消耗估算：

```
1TB 数据 / 4KB 块 ≈ 2.6e8 块
指纹 20B + pointer 8B ≈ 28B
→ 约 7GB 内存
```

所以必须：

* 分层索引
* 压缩指纹
* 分布式 shard

---

### LSM 存储指纹

用于：

* 大规模分布式 AFA
* 冷数据 dedup

优点：

* 支持落盘
* 可扩展

缺点：

* 读放大

---

## 4️⃣ 引用计数（Refcount）

重删必须维护：

```text
block → refcount
```

实现方式：

* 内存表 + WAL
* B+Tree
* Log-structured refcount

优化：

* 批量更新
* 延迟合并

否则会成为写路径瓶颈。

---

## 5️⃣ Inline vs Post-process

### Inline Dedup

写路径：

```text
hash → 查索引 → 命中则 refcount++ → 未命中写入
```

优点：

* 无“先写后删”写放大
* 空间利用率最高

缺点：

* 写延迟增加

---

### Post-process Dedup

优点：

* 写延迟低
* 异步处理

缺点：

* 双写放大
* GC 复杂

现代高端 AFA 几乎都使用 Inline。

---

# 二、压缩（Compression）核心技术

压缩目标：

* 降低 NAND 写入
* 降低 WA
* 提高容量利用率

---

## 1️⃣ 压缩算法选择

### LZ 系列

| 算法   | 特点       |
| ---- | -------- |
| LZ4  | 极快，压缩率一般 |
| ZSTD | 平衡性能与压缩率 |
| Gzip | 老牌，偏慢    |

企业 AFA 常用：

> LZ4 / ZSTD

---

## 2️⃣ 压缩粒度

* 4KB 内压缩
* 16KB chunk 压缩
* Segment 级压缩

粒度越大：

* 压缩率越高
* 延迟越高

---

## 3️⃣ Inline 压缩

写入路径：

```text
IO → 聚合 → 压缩 → 写入 NAND
```

要求：

* 压缩时间 < IO SLA

通常通过：

* 多线程 pipeline
* lock-free ring buffer

---

## 4️⃣ 硬件加速

使用：

* AVX2 / AVX-512 SIMD
* Intel QAT
* ARM NEON
* FPGA / ASIC

目标：

> 压缩吞吐 > 存储带宽

---

# 三、重删 + 压缩顺序

两种策略：

### 先重删后压缩（主流）

优点：

* 避免对重复数据多次压缩
* 减少 CPU 浪费

流程：

```text
chunk → hash → dedup → new? → compress → write
```

---

### 先压缩后重删

优点：

* 某些数据压缩后更容易匹配

但现实中：

* 压缩会改变数据特征
* hash 不稳定

因此较少使用。

---

# 四、与 GC / 快照 的耦合

## 1️⃣ 重删 + 快照

多个 snapshot 共享同一 block：

```text
block.refcount > 1
```

删除 snapshot：

```text
refcount--
=0 才能 GC
```

因此：

> 快照深度直接影响 GC 成本

---

## 2️⃣ 重删 + GC

GC 迁移块时：

* refcount 不变
* 只更新物理位置

否则会破坏一致性。

---

## 3️⃣ 重删 + RAID/EC

小块重删后：

* 条带对齐困难
* 小 IO 放大

解决方案：

* 逻辑层聚合
* EC 条带缓存

---

# 五、分布式 AFA 中的重删

两种模式：

### 本地去重

* 每节点独立
* 简单
* 去重率低

---

### 全局去重

* 全 cluster 共享指纹表
* 去重率高
* 一致性协议复杂

通常：

* 采用分片
* 每 shard 负责 hash 范围

---

# 六、典型厂商技术方向

## Pure Storage

* 全局 inline 重删
* DirectFlash 优化写放大
* 全阵列压缩

---

## NetApp

* WAFL 快照结构配合去重
* 后期优化转为 inline

---

## Huawei

* 芯片协同压缩
* 智能数据分类

---

# 七、工程难点总结

真正难点不在算法，而在：

1. 指纹表内存规模
2. 多线程锁竞争
3. NUMA 访问局部性
4. 快照 + GC 耦合
5. WA 控制
6. QLC 适配

---

# 八、关键指标

| 指标       | 目标        |
| -------- | --------- |
| Dedup 延迟 | < 50μs    |
| 压缩吞吐     | > 存储带宽    |
| 总写放大     | < 2       |
| 内存开销     | < 5% 原始容量 |

---

# 最终总结

重删与压缩在 AFA 中不是“节省空间的附加功能”，而是：

> 控制写放大、延长寿命、降低成本的核心系统工程

真正成熟的 AFA 必须做到：

* Inline 去重
* Inline 压缩
* 常驻内存指纹索引
* GC 协同调度
* 快照树 + 引用计数一致性保证
