Rust 没有像 C++ `<algorithm>` 那样集中在一个模块里的“内置算法库”，但标准库里提供了大量常用算法，主要分布在：

- `Iterator` 迭代器方法
- slice / array / `Vec` 方法
- `Option` / `Result` 方法
- 集合类型如 `HashMap`、`BTreeMap`
- 字符串处理方法

下面按类别整理常用的“内置算法”。

---

## 1. 遍历与迭代

### `for_each`

```rust
let nums = vec![1, 2, 3];

nums.iter().for_each(|x| {
    println!("{}", x);
});
```

### `enumerate`

带索引遍历：

```rust
let arr = ["a", "b", "c"];

for (i, v) in arr.iter().enumerate() {
    println!("{}: {}", i, v);
}
```

### `zip`

两个迭代器配对：

```rust
let a = vec![1, 2, 3];
let b = vec![4, 5, 6];

let pairs: Vec<_> = a.iter().zip(b.iter()).collect();
println!("{:?}", pairs);
```

---

## 2. 映射与转换

### `map`

```rust
let nums =![1, 2, 3];

let doubled: Vec<_> = nums.iter().map(|x| x * 2).collect();

println!("{:?}", doubled); // [2, 4,6]
```

### `filter_map`

过滤并转换：

```rust
let strs = vec!["1", "abc", "3"];

let nums: Vec<i32> = strs
    .iter()
    .filter_map(|s| s.parse::<i32>().ok())
    .collect();

println!("{:?}", nums); // [1, 3]
```

### `flat_map`

扁平化映射：

```rust
let nested = vec![vec![1, 2], vec![3, 4]];

let flat: Vec<_> = nested.into_iter().flat_map(|v| v).collect();

println!("{:?}", flat); // [1, 2, 3, 4]
```

---

## 3. 过滤

### `filter`

```rust
let nums = vec![1, 2, 3, 4, 5];

let even: Vec<_> = nums.into_iter().filter(|x| x % 2 == 0).collect();

println!("{:?}", even); // [2, 4]
```

### `take`

取前 N 个：

```rust
let nums = vec![1, 2, 3, 4, 5];

let first_three: Vec<_> = nums.into_iter().take(3).collect();

println!("{:?}", first_three); // [1, 2, 3]
```

### `skip`

跳过前 N 个：

```rust
let nums = vec![1, 2, 3, 4, 5];

let rest: Vec<_> = nums.into_iter().skip(2).collect();

println!("{:?}", rest); // [3, 4, 5]
```

---

## 4. 查找

### `find`

查找第一个满足条件的元素：

```rust
let nums = vec![1, 2, 3, 4];

let found = nums.iter().find(|&&x| x > 2);

println!("{:?}", found); // Some(3)
```

### `position`

查找元素位置：

```rust
let nums = vec![10, 20, 30];

let pos = nums.iter().position(|&x| x == 20);

println!("{:?}", pos); // Some(1)
```

### `contains`

判断是否包含：

```rust
let nums = vec![1, 2, 3];

println!("{}", nums.contains(&2)); // true
```

---

## 5. 判断

### `any`

是否存在满足条件的元素：

```rust
let nums = vec![1, 2, 3];

let has_even = nums.iter().any(|x| x % 2 == 0);

println!("{}", has_even); // true
```

### `all`

是否全部满足条件：

```rust
let nums = vec![2, 4, 6];

let all_even = nums.iter().all(|x| x % 2 == 0);

println!("{}", all_even); // true
```

---

## 6. 聚合与折叠

### `sum`

```rust
let nums = vec![1, 2, 3];

let s: i32 = nums.iter().sum();

println!("{}", s); // 6
```

### `product`

```rust
let nums = vec![2, 3, 4];

let p: i32 = nums.iter().product();

println!("{}", p); // 24
```

### `fold`

通用折叠：

```rust
let nums = vec![1, 2, 3, 4];

let sum = nums.iter().fold(0, |acc, x| acc + x);

println!("{}", sum); // 10
```

### `reduce`

没有初始值的折叠：

```rust
let nums = vec![1, 2, 3, 4];

let max = nums.into_iter().reduce(|a, b| a.max(b));

println!("{:?}", max); // Some(4)
```

---

## 7. 最大值与最小值

### `max` / `min`

```rust
let nums = vec![3, 1, 4, 2];

println!("{:?}", nums.iter().max()); // Some(4)
println!("{:?}", nums.iter().min()); // Some(1)
```

### `max_by_key` / `min_by_key`

```rust
let words = vec!["rust", "go", "javascript"];

let longest = words.iter().max_by_key(|s| s.len());

println!("{:?}", longest); // Some("javascript")
```

---

## 8. 排序

Rust 的排序主要在 slice 上。

### `sort`

升序排序：

```rust
let mut nums = vec![3, 1, 4, 2];

nums.sort();

println!("{:?}", nums); // [1, 2, 3, 4]
```

### `sort_by`

自定义排序：

```rust
let mut nums = vec![3, 1, 4, 2];

nums.sort_by(|a, b| b.cmp(a));

println!("{:?}", nums); // [4, 3, 2, 1]
```

### `sort_by_key`

按 key 排序：

```rust
let mut words = vec!["rust", "go", "javascript"];

words.sort_by_key(|s| s.len());

println!("{:?}", words); // ["go", "rust", "javascript"]
```

### `sort_unstable`

非稳定排序，通常更快：

```rust
let mut nums = vec![3, 1, 4, 2];

nums.sort_unstable();

println!("{:?}", nums);
```

区别：

```text
sort          稳定排序
sort_unstable 非稳定排序，通常更快
```

---

## 9. 二分查找

适用于已排序 slice。

### `binary_search`

```rust
let nums = vec![1, 2, 3, 4, 5];

let result = nums.binary_search(&3);

println!("{:?}", result); // Ok(2)
```

如果找不到：

```rust
let nums = vec![1, 2, 4, 5];

let result = nums.binary_search(&3);

println!("{:?}", result); // Err(2)
```

`Err(2)` 表示如果插入 `3`，应该插入到索引 `2`。

### `binary_search_by`

```rust
let nums = vec![1, 2, 3, 4, 5];

let result = nums.binary_search_by(|x| x.cmp(&4));

println!("{:?}", result); // Ok(3)
```

### `binary_search_by_key`

```rust
let arr = [(1, "a"), (2, "b"), (3, "c")];

let result = arr.binary_search_by_key(&2, |&(k, _)| k);

println!("{:?}", result); // Ok(1)
```

---

## 10. 去重

### `dedup`

只去除相邻重复元素，通常先排序：

```rust
let mut nums = vec![1, 2, 2, 3, 1];

nums.sort();
nums.dedup();

println!("{:?}", nums); // [1, 2, 3]
```

### `dedup_by`

自定义去重：

```rust
let mut nums = vec![1, 2, 3, 4, 5, 6];

nums.dedup_by(|a, b| (*a % 2) == (*b % 2));

println!("{:?}", nums);
```

---

## 11. 反转

### `reverse`

```rust
let mut nums = vec![1, 2, 3];

nums.reverse();

println!("{:?}", nums); // [3, 2, 1]
```

---

## 12. 切片操作

### `chunks`

分块：

```rust
let nums = vec![1, 2, 3, 4, 5];

for chunk in nums.chunks(2) {
    println!("{:?}", chunk);
}
```

输出：

```text
[1, 2]
[3, 4]
[5]
```

### `windows`

滑动窗口：

```rust
let nums = vec![1, 2, 3, 4];

for w in nums.windows(2) {
    println!("{:?}", w);
}
```

输出：

```text
[1, 2]
[2, 3]
[3, 4]
```

### `split`

按条件切分：

```rust
let nums = vec![1, 2, 0, 3, 4];

let parts: Vec<_> = nums.split(|x| *x == 0).collect();

println!("{:?}", parts); // [[1, 2], [3, 4]]
```

---

## 13. 集合相关算法

### `HashSet` 去重

```rust
use std::collections::HashSet;

let nums = vec![1, 2, 2, 3, 3];

let set: HashSet<_> = nums.into_iter().collect();

println!("{:?}", set);
```

### `HashMap` 统计频率

```rust
use std::collections::HashMap;

let nums = vec![1, 2, 2, 3, 3, 3];

let mut count = HashMap::new();

for x in nums {
    *count.entry(x).or_insert(0) += 1;
}

println!("{:?}", count);
```

### `BTreeSet` 自动排序去重

```rust
use std::collections::BTreeSet;

let nums = vec![3, 1, 2, 3];

let set: BTreeSet<_> = nums.into_iter().collect();

println!("{:?}", set); // {1, 2, 3}
```

---

## 14. 字符串常用算法

### 查找

```rust
let s = "hello rust";

println!("{}", s.contains("rust")); // true
println!("{:?}", s.find("rust"));   // Some(6)
```

### 替换

```rust
let s = "hello rust";

let t = s.replace("rust", "world");

println!("{}", t); // hello world
```

### 分割

```rust
let s = "a,b,c";

let parts: Vec<_> = s.split(',').collect();

println!("{:?}", parts); // ["a", "b", "c"]
```

### 去除首尾空白

```rust
let s = "  hello  ";

println!("{}", s.trim()); // "hello"
```

---

## 15. 常见组合写法

### 求偶数平方和

```rust
let nums = vec![1, 2, 3, 4];

let result: i32 = nums
    .iter()
    .filter(|&&x| x % 2 == 0)
    .map(|&x| x * x)
    .sum();

println!("{}", result); // 20
```

### 找到最长字符串

```rust
let words = vec!["rust", "go", "javascript"];

let longest = words.iter().max_by_key(|s| s.len());

println!("{:?}", longest); // Some("javascript")
```

### 排序后去重

```rust
let mut nums = vec![3, 1, 2, 3, 2];

nums.sort();
nums.dedup();

println!("{:?}", nums); // [1, 2, 3]
```

---

## 总结表

| 功能 | Rust 常用方法 |
|---|---|
| 遍历 | `for`, `for_each`, `enumerate` |
| 映射 | `map`, `filter_map`, `flat_map` |
| 过滤 | `filter`, `take`, `skip` |
| 查找 | `find`, `position`, `contains` |
| 判断 | `any`, `all` |
| 聚合 | `sum`, `product`, `fold`, `reduce` |
| 最大最小 | `max`, `min`, `max_by_key`, `min_by_key` |
| 排序 | `sort`, `sort_by`, `sort_by_key`, `sort_unstable` |
| 二分查找 | `binary_search`, `binary_search_by` |
| 去重 | `dedup`, `HashSet`, `BTreeSet` |
| 反转 | `reverse` |
| 分块 | `chunks`, `windows` |
| 字符串 | `contains`, `find`, `split`, `replace`, `trim` |

简单来说，Rust 里最常用的算法基本都通过：

```rust
.iter()
.map()
.filter()
.fold()
.collect()
```

以及：

```rust
sort()
binary_search()
dedup()
reverse()
```

来完成。
