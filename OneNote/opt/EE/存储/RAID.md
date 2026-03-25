# 一、RAID 基础原理

RAID（Redundant Array of Independent Disks）核心目标：

```text
性能提升 + 数据冗余保护
```

通过：

* 条带化（Striping）
* 镜像（Mirroring）
* 校验（Parity）

实现容量、性能与可靠性的平衡。

---

# 二、传统 RAID 等级

---

## RAID 0（条带化）

![Image](https://www.prepressure.com/images/raid-level-0-striping.svg)

![Image](https://www.techtarget.com/rms/onlineImages/storage_raid_00_mobile.png)

![Image](https://www.researchgate.net/publication/299476295/figure/fig1/AS%3A344984151707649%401459262039076/Example-for-data-block-distribution-of-a-RAID-0-system-with-four-disks.png)

### 原理

数据按条带分布到多个磁盘：

```text
Disk1: A1  A3  A5
Disk2: A2  A4  A6
```

### 特点

* 性能提升
* 无冗余
* 任一磁盘故障 → 数据丢失

适合：

* 临时缓存
* 不重要数据

---

## RAID 1（镜像）

![Image](https://www.prepressure.com/images/raid-level-1-mirroring.svg)

![Image](https://www.researchgate.net/publication/249707459/figure/fig1/AS%3A669541138976782%401536642455406/Data-Layout-in-RAID-1-scheme.png)

![Image](https://images.wondershare.com/recoverit/article/2022/07/what-is-raid-1.jpg)

### 原理

```text
Disk1: A
Disk2: A
```

### 特点

* 可容忍 1 块盘故障
* 读性能提升
* 写性能≈单盘
* 容量利用率 50%

---

## RAID 5（单校验）

![Image](https://upload.wikimedia.org/wikipedia/commons/thumb/6/64/RAID_5.svg/330px-RAID_5.svg.png)

![Image](https://www.fusiondatarecovery.com/images/pages/RAID-5.PNG)

![Image](https://www.prepressure.com/images/raid-level-5-striping-with-parity.svg)

![Image](https://i.sstatic.net/Uy5nd.png)

### 原理

```text
A1 A2 A3 P
```

P = A1 ⊕ A2 ⊕ A3

校验块轮转分布。

### 优点

* 容量利用率高
* 可容忍 1 块盘故障

### 缺点

* 小写惩罚（4 次 IO）
* 重建时间长

---

## RAID 6（双校验）

![Image](https://www.prepressure.com/images/raid-level-6-striping-with-dual-parity.svg)

![Image](https://www.fusiondatarecovery.com/images/pages/RAID-6.PNG)

![Image](https://images.wondershare.com/recoverit/article/2022/07/what-is-raid-6.jpg)

![Image](https://buffaloamericas.com/images/uploads/Raid-6-Diagram.jpg)

### 原理

两个独立校验块：

```text
A1 A2 A3 P Q
```

可容忍 2 块盘故障。

代价：

* 写惩罚更高
* 计算复杂

---

# 三、RAID 写惩罚（Write Penalty）

以 RAID5 为例：

小块写入流程：

```text
1. 读旧数据
2. 读旧校验
3. 写新数据
4. 写新校验
```

→ 4 IO

这在 SSD 上意味着：

* 放大 NAND 写入
* 影响寿命

---

# 四、RAID 在全闪阵列中的问题

在 AFA 中：

1. SSD 重建速度快但容量大
2. 大容量盘（20TB+）重建时间仍然很长
3. UBER（不可恢复读错误）概率升高

因此传统 RAID 需要演进。

---

# 五、RAID 2.0 / 分布式 RAID

核心思想：

```text
打散条带到所有 SSD
```

不再局限于固定 N 块盘。

特征：

* 所有盘参与重建
* Chunk 级虚拟化
* 均匀负载

优点：

* 重建时间从小时级降到分钟级
* 负载更均衡

---

# 六、纠删码（Erasure Coding, EC）

数学基础：

* Galois Field
* Reed-Solomon

例如：

```text
8+2 EC
```

8 数据块 + 2 校验块
可容忍 2 块故障。

优点：

* 比 RAID1 节省容量
* 适合分布式存储

缺点：

* 小写放大
* CPU 开销高

---

# 七、AFA 中 RAID + GC + Dedup 的耦合

在现代 AFA 中：

```text
IO
↓
Dedup
↓
Compress
↓
RAID/EC 编码
↓
Segment 写入
↓
GC
```

注意：

* 重删后数据块大小不均
* 条带对齐复杂
* GC 迁移时必须重新编码

这增加了：

* 写放大
* 计算负载

---

# 八、NVMe-oF 与 RAID 分布

在分布式 AFA 中：

* RAID 可在存储节点计算
* 或卸载到主机端

JBOF 架构中：

```text
Compute Node
    ↓
NVMe-oF
    ↓
Flash Pool
```

RAID 不再是“机箱级”，而是“集群级”。

---

# 九、典型厂商技术方向

## Pure Storage

* 分布式校验
* DirectFlash 减少 FTL 干扰
* 快速重建

---

## NetApp

* WAFL + RAID-DP
* 快照与 RAID 协同

---

## Huawei

* OceanStor RAID 2.0+
* 快速重构算法

---

# 十、工程关键指标

| 指标     | 重要性       |
| ------ | --------- |
| 重建时间   | 决定数据暴露风险  |
| 小写放大   | 决定 SSD 寿命 |
| 校验计算吞吐 | 决定性能上限    |
| 均衡度    | 决定负载热点    |

---

# 十一、RAID 的未来趋势

### 1️⃣ 局部重建（Local Reconstruction Code）

减少重建数据量。

---

### 2️⃣ 分层 RAID

* 热数据 RAID1
* 冷数据 EC

---

### 3️⃣ 计算卸载

* FPGA
* SmartNIC
* DPU

---

# 总结

RAID 在现代 AFA 中已经从：

> “磁盘级冗余技术”

演变为：

> “分布式编码与数据布局算法”

真正核心在于：

* 如何降低写放大
* 如何缩短重建时间
* 如何与 GC / 重删协同

你想深入哪一个方向？
