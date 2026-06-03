下面是 Rust 常见容器及其基本操作介绍，主要来自标准库 `std::collections`，也包括数组、切片、字符串等常用数据结构。

# 公共操作

| 操作类型 | 操作说明 | Vec<T> / 切片 | String / &str | HashMap<K, V> | HashSet<T> |
|---|---|---|---|---|---|
| 判空 | 检查是否为空 | v.is_empty() | s.is_empty() | m.is_empty() | s.is_empty() |
| 长度 | 获取元素/字节数量 | v.len() | s.len() (字节数) | m.len() | s.len() |
| 存在性 | 检查是否存在某元素/键 | v.contains(&val) | s.contains("sub") | m.contains_key(&k) | s.contains(&val) |
| 安全获取 | 按索引或键取值 (返回 Option) | v.get(index) | s.get(range) (切片) | m.get(&k) | 无 (直接用 contains) |
| 清空 | 清除所有内容 (保留内存) | v.clear() | s.clear() | m.clear() | s.clear() |
| 预分配 | 创建带容量的容器 | Vec::with_capacity(n) | String::with_capacity(n) | HashMap::with_capacity(n) | HashSet::with_capacity(n) |
| 迭代器 | 获取只读迭代器 | v.iter() | s.chars() / .bytes() | m.iter() | s.iter() |

## 原生数组

| 操作类型 | 操作说明 | 原生数组 [T; N] (如 [1, 2, 3]) | 动态数组 Vec<T> |
|---|---|---|---|
| 判空 | 检查是否为空 | ⚠️ a.is_empty() (固定长度为0时才为true) | v.is_empty() |
| 长度 | 获取元素数量 | a.len() (编译期固定，$O(1)$) | v.len() (运行期动态，$O(1)$) |
| 存在性 | 检查是否存在某元素 | a.contains(&val) ($O(n)$ 线性查找) | v.contains(&val) ($O(n)$ 线性查找) |
| 安全获取 | 按索引取值 (返回 Option) | a.get(index) | v.get(index) |
| 清空 | 清除所有内容 | ❌ 不支持 (长度固定无法清空) | v.clear() |
| 预分配 | 创建带容量的容器 | ❌ 不支持 (声明时即固定大小) | Vec::with_capacity(n) |
| 迭代器 | 获取只读迭代器 | a.iter() 或直接 for x in a | v.iter() |

------------------------------
## 数组是固定长度的底层类型，为什么它也能直接调用 .len()、.contains() 和 .get() 呢？
这得益于 Rust 的 Deref 特征：

   1. 原生数组 [T; N] 在调用这些方法时，会自动隐式强制转换为切片 &[T]（Slice）。
   2. len()、contains()、get()、is_empty() 这些方法，实际上全都是实现给切片 [T] 的。
   3. 因为 Vec<T> 也能自动转换为 &[T]，所以数组和 Vec 在这些只读操作上的语法完全一模一样
---

# 1. 数组 Array

数组长度固定，所有元素类型必须相同。

```rust
let arr = [1, 2, 3, 4, 5];

println!("{}", arr[0]); // 1
println!("{}", arr.len()); // 5
```

也可以指定类型和长度：

```rust
let arr: [i32; 3] = [10, 20, 30];
```

创建重复元素数组：

```rust
let arr = [0; 5]; // [0, 0, 0, 0, 0]
```

## 遍历数组

```rust
let arr = [1, 2, 3];

for x in arr {
    println!("{}", x);
}
```

或借用遍历：

```rust
for x in &arr {
    println!("{}", x);
}
```

---

# 2. 切片 Slice

切片是对数组或 Vec 中一段连续元素的引用，长度不固定。

```rust
let arr = [1, 2, 3, 4, 5];

let s = &arr[1..4]; // [2, 3, 4]

println!("{:?}", s);
```

切片类型通常是：

```rust
&[T]
```

例如：

```rust
fn print_slice(s: &[i32]) {
    println!("{:?}", s);
}

let v = vec![1, 2, 3];
print_slice(&v);
```

---

# 3. Vec 动态数组

`Vec<T>` 是 Rust 中最常用的动态数组，长度可变，存储在堆上。

## 创建 Vec

```rust
let mut v: Vec<i32> = Vec::new();

v.push(1);
v.push(2);
v.push(3);

println!("{:?}", v);
```

使用宏创建：

```rust
let mut v = vec![1, 2, 3];
```

## 常用操作

### 添加元素

```rust
let mut v = vec![1, 2];

v.push(3);

println!("{:?}", v); // [1, 2, 3]
```

### 删除最后一个元素

```rust
let mut v = vec![1, 2, 3];

let x = v.pop();

println!("{}", x); // Some(3)
println!("{:?}", v); // [1, 2]
```

### 按索引访问

```rust
let v = vec![10, 20, 30];

println!("{}", v[0]);
```

注意：索引越界会 panic。

更安全的方式是使用 `get`：

```rust
let v = vec![10, 20, 30];

match v.get(1) {
    Some(value) => println!("{}", value),
    None => println!("不存在"),
}
```

### 修改元素

```rust
let mut v = vec![1, 2, 3];

v[0] = 100;

println!("{:?}", v);
```

### 遍历 Vec

```rust
let v = vec![1, 2, 3];

for x in &v {
    println!("{}", x);
}
```

可变遍历：

```rust
let mut v = vec![1, 2, 3];

for x in &mut v {
    *x *= 2;
}

println!("{:?}", v); // [2, 4, 6]
```

### 排序

```rust
let mut v = vec![3, 1, 2];

v.sort();

println!("{:?}", v); // [1, 2, 3]
```

自定义排序：

```rust
let mut v = vec![3, 1, 2];

v.sort_by(|a, b| b.cmp(a));

println!("{:?}", v); // [3, 2, 1]
```

---

# 4. String 字符串

`String` 是可变、堆分配的 UTF-8 字符串。

## 创建 String

```rust
let s = String::new();

let s = String::from("hello");

let s = "hello".to_string();
```

## 添加内容

```rust
let mut s = String::from("hello");

s.push(' ');
s.push_str("world");

println!("{}", s); // hello world
```

## 拼接字符串

```rust
let s1 = String::from("hello");
let s2 = String::from(" world");

let s3 = s1 + &s2;

println!("{}", s3);
```

注意：`s1 + &s2` 会移动 `s1`。

更推荐使用 `format!`：

```rust
let s1 = String::from("hello");
let s2 = String::from("world");

let s3 = format!("{} {}", s1, s2);

println!("{}", s3);
```

## 遍历字符

```rust
let s = String::from("你好Rust");

for c in s.chars() {
    println!("{}", c);
}
```

遍历字节：

```rust
for b in s.bytes() {
    println!("{}", b);
}
```

---

# 5. HashMap 哈希表

`HashMap<K, V>` 用于存储键值对，查询速度通常很快。

使用前需要导入：

```rust
use std::collections::HashMap;
```

## 创建 HashMap

```rust
use std::collections::HashMap;

let mut map = HashMap::new();

map.insert("name", "Tom");
map.insert("age", "18");

println!("{:?}", map);
```

## 插入和更新

```rust
let mut map = HashMap::new();

map.insert("apple", 3);
map.insert("banana", 5);

// 更新 apple
map.insert("apple", 10);

println!("{:?}", map);
```

## 查询

```rust
let mut map = HashMap::new();

map.insert("apple", 3);

match map.get("apple") {
    Some(value) => println!("数量：{}", value),
    None => println!("不存在"),
}
```

`get` 返回的是 `Option<&V>`。

## 删除

```rust
let mut map = HashMap::new();

map.insert("apple", 3);
map.remove("apple");

println!("{:?}", map);
```

## 只在键不存在时插入

```rust
let mut map = HashMap::new();

map.entry("apple").or_insert(3);
map.entry("apple").or_insert(10);

println!("{:?}", map); // {"apple": 3}
```

## 统计词频

```rust
use std::collections::HashMap;

let text = "hello rust hello world";

let mut map = HashMap::new();

for word in text.split_whitespace() {
    let count = map.entry(word).or_insert(0);
    *count += 1;
}

println!("{:?}", map);
```

---

# 6. HashSet 集合

`HashSet<T>` 用于存储不重复的值。

```rust
use std::collections::HashSet;

let mut set = HashSet::new();

set.insert(1);
set.insert(2);
set.insert(2);

println!("{:?}", set); // 只会有 1 和 2
```

## 常用操作

### 判断是否存在

```rust
println!("{}", set.contains(&1));
```

### 删除元素

```rust
set.remove(&1);
```

### 遍历

```rust
for x in &set {
    println!("{}", x);
}
```

### 集合运算

```rust
use std::collections::HashSet;

let a: HashSet<_> = [1, 2, 3].into_iter().collect();
let b: HashSet<_> = [3, 4, 5].into_iter().collect();

println!("并集: {:?}", a.union(&b).collect::<Vec<_>>());
println!("交集: {:?}", a.intersection(&b).collect::<Vec<_>>());
println!("差集: {:?}", a.difference(&b).collect::<Vec<_>>());
```

---

# 7. BTreeMap 和 BTreeSet

`BTreeMap` 和 `BTreeSet` 会按照键或元素的顺序存储。

适合需要有序遍历的场景。

```rust
use std::collections::BTreeMap;

let mut map = BTreeMap::new();

map.insert(3, "c");
map.insert(1, "a");
map.insert(2, "b");

for (k, v) in &map {
    println!("{}: {}", k, v);
}
```

输出顺序是：

```text
1: a
2: b
3: c
```

`BTreeSet` 类似：

```rust
use std::collections::BTreeSet;

let mut set = BTreeSet::new();

set.insert(3);
set.insert(1);
set.insert(2);

println!("{:?}", set); // {1, 2, 3}
```

---

# 8. VecDeque 双端队列

`VecDeque<T>` 支持从头部和尾部高效插入、删除。

```rust
use std::collections::VecDeque;

let mut queue = VecDeque::new();

queue.push_back(1);
queue.push_back(2);
queue.push_front(0);

println!("{:?}", queue); // [0, 1, 2]
```

## 弹出元素

```rust
let front = queue.pop_front();
let back = queue.pop_back();

println!("{:?}", front);
println!("{:?}", back);
```

适合实现队列：

```rust
use std::collections::VecDeque;

let mut q = VecDeque::new();

q.push_back("task1");
q.push_back("task2");

while let Some(task) = q.pop_front() {
    println!("处理 {}", task);
}
```

---

# 9. LinkedList 链表

`LinkedList<T>` 是双向链表。

```rust
use std::collections::LinkedList;

let mut list = LinkedList::new();

list.push_back(1);
list.push_back(2);
list.push_front(0);

println!("{:?}", list);
```

删除：

```rust
list.pop_front();
list.pop_back();
```

不过在 Rust 中，很多情况下 `Vec` 或 `VecDeque` 比 `LinkedList` 更常用，性能也更好。

---

# 10. BinaryHeap 二叉堆

`BinaryHeap<T>` 默认是最大堆，常用于优先队列。

```rust
use std::collections::BinaryHeap;

let mut heap = BinaryHeap::new();

heap.push(3);
heap.push(1);
heap.push(5);

println!("{:?}", heap.pop()); // Some(5)
println!("{:?}", heap.pop()); // Some(3)
println!("{:?}", heap.pop()); // Some(1)
```

如果想实现最小堆，可以使用 `Reverse`：

```rust
use std::cmp::Reverse;
use std::collections::BinaryHeap;

let mut heap = BinaryHeap::new();

heap.push(Reverse(3));
heap.push(Reverse(1));
heap.push(Reverse(5));

println!("{:?}", heap.pop()); // Some(Reverse(1))
```

---

# 11. 容器中的所有权问题

Rust 容器会持有元素的所有权。

```rust
let s = String::from("hello");

let v = vec![s];

// println!("{}", s); // 错误，s 已经被移动到 v 中
```

如果希望继续使用，可以克隆：

```rust
let s = String::from("hello");

let v = vec![s.clone()];

println!("{}", s);
```

或者存储引用：

```rust
let s = String::from("hello");

let v = vec![&s];

println!("{}", s);
println!("{:?}", v);
```

但存储引用时要注意生命周期。

---

# 12. 常用迭代器操作

Rust 容器通常都支持迭代器。

## iter

不可变借用遍历：

```rust
let v = vec![1, 2, 3];

for x in v.iter() {
    println!("{}", x);
}
```

## iter_mut

可变借用遍历：

```rust
let mut v = vec![1, 2, 3];

for x in v.iter_mut() {
    *x += 10;
}

println!("{:?}", v);
```

## into_iter

消费容器，取得元素所有权：

```rust
let v = vec![String::from("a"), String::from("b")];

for s in v.into_iter() {
    println!("{}", s);
}

// v 不能再使用
```

## map/filter/collect

```rust
let v = vec![1, 2, 3, 4, 5];

let result: Vec<_> = v
    .iter()
    .filter(|x| **x % 2 == 0)
    .map(|x| x * 10)
    .collect();

println!("{:?}", result); // [20, 40]
```

---

# 13. 常见容器选择建议

| 场景 | 推荐容器 |
|---|---|
| 固定长度数据 | 数组 `[T; N]` |
| 动态连续数组 | `Vec<T>` |
| 字符串 | `String` / `&str` |
| 键值对查询 | `HashMap<K, V>` |
| 不重复集合 | `HashSet<T>` |
| 需要有序映射 | `BTreeMap<K, V>` |
| 需要有序集合 | `BTreeSet<T>` |
| 双端队列 | `VecDeque<T>` |
| 优先队列 | `BinaryHeap<T>` |
| 链表 | `LinkedList<T>`，但较少使用 |

---

# 14. 简单总结

Rust 常用容器包括：

```rust
Vec<T>
String
HashMap<K, V>
HashSet<T>
BTreeMap<K, V>
BTreeSet<T>
VecDeque<T>
LinkedList<T>
BinaryHeap<T>
```

使用容器时需要重点注意：

1. 容器会持有元素所有权；
2. 访问元素时常返回引用；
3. `get` 比索引访问更安全；
4. `iter` 是借用遍历；
5. `iter_mut` 是可变借用遍历；
6. `into_iter` 会消费容器；
7. `HashMap` 和 `HashSet` 无序；
8. `BTreeMap` 和 `BTreeSet` 有序。
