---
name: build-qt6
description: Compile Qt 6 from source on Windows with MinGW GCC
---

# Build Qt 6 from Source (Windows + MinGW)

## Prerequisites

| Tool | Minimum Version | Install |
|------|----------------|---------|
| MinGW-w64 GCC | 13.1+ (POSIX SEH) | https://github.com/niXman/mingw-builds-binaries/releases |
| CMake | 3.16+ | https://cmake.org/download/ |
| Ninja | any | `pip install ninja` |
| Python 3 | 3.x | https://www.python.org/downloads/ |
| Perl | 5.x | Git for Windows 自带，或 MSYS2 |

> **注意**: VS2017 MSVC 不支持 Qt 6（需 C++17 完整支持，要求 VS2022+）。MinGW GCC 是轻量替代方案。

## Step 1: 下载源码

**推荐下载单体包**（包含所有子模块），不要下载 supermodule-only 包（子模块目录为空）。

- 清华镜像: `https://mirrors.tuna.tsinghua.edu.cn/qt/official_releases/qt/{MAJOR}.{MINOR}/{VERSION}/single/qt-everywhere-src-{VERSION}.zip`
- 官方: `https://download.qt.io/official_releases/qt/{MAJOR}.{MINOR}/{VERSION}/single/qt-everywhere-src-{VERSION}.zip`

Windows 选 `.zip` 格式，Linux/macOS 选 `.tar.xz` 格式。

> 如果只有 supermodule 包，子模块目录为空无法编译，必须用 `init-repository` 或 git submodule 初始化。

## Step 2: 解压并整理目录结构

解压后整理为标准目录布局：

```
D:\Qt\qt{VERSION}\
  src\           ← 源码（所有子模块目录 + 顶层构建文件）
  mingw_64\      ← 编译安装目标（编译器_架构）
```

```bat
:: 解压到临时位置后，将内容移入 src/
mkdir D:\Qt\qt6.x.x\src
move qt-everywhere-src-6.x.x\* D:\Qt\qt6.x.x\src\
```

## Step 3: 配置

**关键**: 必须显式指定 MinGW 编译器，否则 CMake 会自动选 VS2017 MSVC（如果存在）导致编译失败。

```bat
cd D:\Qt\qt6.x.x
mkdir build
cd build

cmake -G "Ninja" ^
  -DCMAKE_C_COMPILER=gcc ^
  -DCMAKE_CXX_COMPILER=g++ ^
  -DCMAKE_INSTALL_PREFIX=D:/Qt/qt6.x.x/mingw_64 ^
  -DBUILD_qtwebengine=OFF ^
  -DQT_INTERNAL_CALLED_FROM_CONFIGURE:BOOL=TRUE ^
  D:/Qt/qt6.x.x/src
```

常用配置选项：
- `-DCMAKE_INSTALL_PREFIX=...` — 安装路径，格式为 `{Qt根目录}/{编译器}_{架构}`
- `-DBUILD_qtwebengine=OFF` — 跳过 qtwebengine（依赖 Chromium，编译极慢且容易出错）
- `-DCMAKE_BUILD_TYPE=Release` — 发布版本（默认）
- `-DBUILD_qt3d=OFF -DBUILD_qtcharts=OFF` — 跳过不需要的模块加速编译

## Step 4: 编译

**关键**: MinGW 运行时 DLL 必须在 PATH 中，否则 syncqt.exe 等工具运行时报 `0xC0000139` (DLL NOT FOUND)。

### 方案 A: 确保 MinGW bin 在 PATH 最前面

```bat
set PATH=D:\bin\mingw64\bin;%PATH%
cmake --build . --parallel
```

### 方案 B: 复制 DLL 到 qtbase/bin（更可靠）

```bat
:: 编译开始后，syncqt.exe 生成时立即复制 DLL
copy D:\bin\mingw64\bin\libgcc_s_seh-1.dll qtbase\bin\
copy D:\bin\mingw64\bin\libstdc++-6.dll qtbase\bin\
cmake --build . --parallel
```

> **诊断**: 如果编译报 `Exit code 0xc0000139`，就是 MinGW 运行时 DLL 找不到。用 `objdump -p xxx.exe | grep "DLL Name"` 查看依赖。

### 并行数控制

```bat
cmake --build . --parallel 8    :: 限制8线程，减少 Windows 文件锁冲突
```

> **诊断**: 如果报 `CreateProcess: Access is denied`，是并发太多导致文件锁冲突，重新运行编译即可（增量编译会从未完成处继续）。

## Step 5: 安装

```bat
cmake --build . --target install
```

安装后目录结构：

```
D:\Qt\qt6.x.x\
  mingw_64\
    bin\       ← qmake.exe, moc.exe, rcc.exe, uic.exe, designer.exe 等
    lib\       ← .a 静态库, .dll 动态库
    include\   ← 头文件
    mkspecs\   ← qmake 平台配置
    cmake\     ← CMake 配置文件
    plugins\   ← Qt 插件
    qml\       ← QML 模块
  src\         ← 源码（可保留或删除）
  build\       ← 构建临时文件（安装后可删除）
```

## Step 6: 验证

```bat
D:\Qt\qt6.x.x\mingw_64\bin\qmake.exe --version
:: 应输出: Using Qt version 6.x.x in D:/Qt/qt6.x.x/mingw_64/lib
```

## 常见问题

| 错误 | 原因 | 解决 |
|------|------|------|
| `0xC0000139` / DLL NOT FOUND | MinGW 运行时 DLL 不在 PATH | 复制 libgcc_s_seh-1.dll + libstdc++-6.dll 到 qtbase/bin/ |
| `MSVC 19.16 broken` | CMake 自动选了 VS2017 | 配置时加 `-DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++` |
| `CreateProcess: Access is denied` | Windows 并发文件锁冲突 | 重新运行编译，或减少 --parallel 数 |
| 子模块目录为空 | 下载的是 supermodule 而非单体包 | 下载 qt-everywhere-src-{VERSION}.zip 单体包 |
| `non-prefix build, install not supported` | 未设置 CMAKE_INSTALL_PREFIX | 配置时指定 -DCMAKE_INSTALL_PREFIX |

## 编译器_架构命名规范

| 编译器 | 目录名 |
|--------|--------|
| MinGW 64-bit | `mingw_64` |
| MinGW 32-bit | `mingw_32` |
| MSVC 2017 64-bit | `msvc2017_64` |
| MSVC 2022 64-bit | `msvc2022_64` |
