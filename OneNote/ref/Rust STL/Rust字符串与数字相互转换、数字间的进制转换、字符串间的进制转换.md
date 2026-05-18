下面按 3 类整理 Rust 中常见的转换方式：

1. 字符串与数字相互转换  
2. 数字间的进制转换  
3. 字符串间的进制转换  

---

## 一、字符串与数字相互转换

### 1. 字符串转整数

使用 `parse()`：

```rust
fn main() {
    let s = "123";

    let n: i32 = s.parse().unwrap();

    println!("{}", n); // 123
}
```

也可以显式指定类型：

```rust
fn main() {
    let s = "123";

    let n = s.parse::<i32>().unwrap();

    println!("{}", n);
}
```

更安全的写法是处理错误：

```rust
fn main() {
    let s = "123a";

    match s.parse::<i32>() {
        Ok(n) => println!("转换成功: {}", n),
        Err(e) => println!("转换失败: {}", e),
    }
}
```

---

### 2. 字符串转浮点数

```rust
fn main() {
    let s = "3.14";

    let f: f64 = s.parse().unwrap();

    println!("{}", f);
}
```

---

### 3. 数字转字符串

可以使用 `to_string()`：

```rust
fn main() {
    let n = 123;

    let s = n.to_string();

    println!("{}", s);
}
```

也可以使用 `format!()`：

```rust
fn main() {
    let n = 123;

    let s = format!("{}", n);

    println!("{}", s);
}
```

---

## 二、数字间的进制转换

严格来说，数字本身在内存中就是二进制表示的。

所谓“十进制转二进制”“十进制转十六进制”，通常是指：

> 把一个数字格式化成不同进制的字符串。

例如：

```rust
fn main() {
    let n = 255;

    println!("十进制: {}", n);
    println!("二进制: {:b}", n);
    println!("八进制: {:o}", n);
    println!("十六进制小写: {:x}", n);
    println!("十六进制大写: {:X}", n);
}
```

输出：

```text
十进制: 255
二进制: 11111111
八进制: 377
十六进制小写: ff
十六进制大写: FF
```

---

### 常用格式化符号

| 进制 | 格式 |
|---|---|
| 十进制 | `{}` |
| 二进制 | `{:b}` |
| 八进制 | `{:o}` |
| 十六进制小写 | `{:x}` |
| 十六进制大写 | `{:X}` |

---

### 带前缀输出

```rust
fn main() {
    let n = 255;

    println!("二进制: {:#b}", n);
    println!("八进制: {:#o}", n);
    println!("十六进制: {:#x}", n);
}
```

输出：

```text
二进制: 0b11111111
八进制: 0o377
十六进制: 0xff
```

---

## 三、不同进制字符串转数字

如果字符串本身是某种进制表示，可以用 `from_str_radix`。

### 1. 二进制字符串转整数

```rust
fn main() {
    let s = "11111111";

    let n = i32::from_str_radix(s, 2).unwrap();

    println!("{}", n); // 255
}
```

---

### 2. 八进制字符串转整数

```rust
fn main() {
    let s = "377";

    let n = i32::from_str_radix(s, 8).unwrap();

    println!("{}", n); // 255
}
```

---

### 3. 十六进制字符串转整数

```rust
fn main() {
    let s = "ff";

    let n = i32::from_str_radix(s, 16).unwrap();

    println!("{}", n); // 255
}
```

大写也可以：

```rust
fn main() {
    let s = "FF";

    let n = i32::from_str_radix(s, 16).unwrap();

    println!("{}", n); // 255
}
```

---

### 4. 十进制字符串转整数

```rust
fn main() {
    let s = "255";

    let n = i32::from_str_radix(s, 10).unwrap();

    println!("{}", n);
}
```

等价于：

```rust
let n: i32 = "255".parse().unwrap();
```

---

## 四、字符串间的进制转换

所谓字符串间的进制转换，一般流程是：

```text
源进制字符串 -> 数字 -> 目标进制字符串
```

例如：二进制字符串 `"11111111"` 转十六进制字符串 `"ff"`。

```rust
fn main() {
    let bin = "11111111";

    let n = i32::from_str_radix(bin, 2).unwrap();

    let hex = format!("{:x}", n);

    println!("{}", hex); // ff
}
```

---

### 1. 二进制字符串转十进制字符串

```rust
fn main() {
    let bin = "11111111";

    let n = i32::from_str_radix(bin, 2).unwrap();

    let dec = n.to_string();

    println!("{}", dec); // 255
}
```

---

### 2. 十进制字符串转二进制字符串

```rust
fn main() {
    let dec = "255";

    let n: i32 = dec.parse().unwrap();

    let bin = format!("{:b}", n);

    println!("{}", bin); // 11111111
}
```

---

### 3. 十六进制字符串转二进制字符串

```rust
fn main() {
    let hex = "ff";

    let n = i32::from_str_radix(hex, 16).unwrap();

    let bin = format!("{:b}", n);

    println!("{}", bin); // 11111111
}
```

---

### 4. 二进制字符串转十六进制字符串

```rust
fn main() {
    let bin = "11111111";

    let n = i32::from_str_radix(bin, 2).unwrap();

    let hex = format!("{:x}", n);

    println!("{}", hex); // ff
}
```

---

## 五、封装一个通用函数

可以写一个函数，把任意进制字符串转换为目标进制字符串。

```rust
fn convert_base(s: &str, from_base: u32, to_base: u32) -> Result<String, std::num::ParseIntError> {
    let n = i64::from_str_radix(s, from_base)?;

    let result = match to_base {
        2 => format!("{:b}", n),
        8 => format!("{:o}", n),
        10 => format!("{}", n),
        16 => format!("{:x}", n),
        _ => panic!("暂不支持该目标进制"),
    };

    Ok(result)
}

fn main() {
    let a = convert_base("11111111", 2, 16).unwrap();
    println!("{}", a); // ff

    let b = convert_base("ff", 16, 2).unwrap();
    println!("{}", b); // 11111111

    let c = convert_base("255", 10, 8).unwrap();
    println!("{}", c); // 377
}
```

---

## 六、支持 2 到 36 进制的通用转换

Rust 标准库的 `from_str_radix` 支持 2 到 36 进制。

但是格式化输出只内置支持二进制、八进制、十进制、十六进制。

如果要输出任意 2 到 36 进制，可以自己写函数：

```rust
fn to_radix_string(mut n: u64, radix: u32) -> String {
    assert!((2..=36).contains(&radix));

    if n == 0 {
        return "0".to_string();
    }

    let digits = b"0123456789abcdefghijklmnopqrstuvwxyz";
    let mut result = Vec::new();

    while n > 0 {
        let rem = n % radix as u64;
        result.push(digits[rem as usize] as char);
        n /= radix as u64;
    }

    result.iter().rev().collect()
}

fn convert_base(s: &str, from_base: u32, to_base: u32) -> Result<String, std::num::ParseIntError> {
    let n = u64::from_str_radix(s, from_base)?;
    Ok(to_radix_string(n, to_base))
}

fn main() {
    println!("{}", convert_base("255", 10, 2).unwrap());  // 11111111
    println!("{}", convert_base("255", 10, 8).unwrap());  // 377
    println!("{}", convert_base("255", 10, 16).unwrap()); // ff
    println!("{}", convert_base("255", 10, 36).unwrap()); // 73
}
```

---

## 七、带前缀的字符串处理

`from_str_radix` 默认不能直接解析 `"0xff"`、`"0b1010"` 这种带前缀的字符串。

例如下面会失败：

```rust
let n = i32::from_str_radix("0xff", 16).unwrap();
```

应该去掉前缀：

```rust
fn main() {
    let s = "0xff";

    let s = s.strip_prefix("0x").unwrap_or(s);

    let n = i32::from_str_radix(s, 16).unwrap();

    println!("{}", n); // 255
}
```

二进制同理：

```rust
fn main() {
    let s = "0b11111111";

    let s = s.strip_prefix("0b").unwrap_or(s);

    let n = i32::from_str_radix(s, 2).unwrap();

    println!("{}", n); // 255
}
```

---

## 八、常见总结

### 字符串转数字

```rust
let n: i32 = "123".parse().unwrap();
let f: f64 = "3.14".parse().unwrap();
```

### 数字转字符串

```rust
let s = 123.to_string();
let s = format!("{}", 123);
```

### 进制字符串转数字

```rust
let n = i32::from_str_radix("1111", 2).unwrap();
let n = i32::from_str_radix("17", 8).unwrap();
let n = i32::from_str_radix("ff", 16).unwrap();
```

### 数字转不同进制字符串

```rust
let n = 255;

let bin = format!("{:b}", n);
let oct = format!("{:o}", n);
let dec = format!("{}", n);
let hex = format!("{:x}", n);
```

### 字符串进制转换

```rust
let n = i32::from_str_radix("ff", 16).unwrap();
let bin = format!("{:b}", n);

println!("{}", bin); // 11111111
```
