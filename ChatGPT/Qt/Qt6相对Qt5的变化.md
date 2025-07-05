Qt 6 是对 Qt 框架的一次重大升级，相比 Qt 5 在架构、模块划分、平台支持、性能、图形渲染等方面都进行了较大幅度的调整和优化。以下是 Qt 6 相对 Qt 5 的主要变化的详细介绍：

---

## 一、架构与核心框架变化

### 1. C++17 标准支持

* **Qt 5** 支持 C++11/14。
* **Qt 6** 要求 **最低 C++17**，这为现代语言特性（如结构化绑定、`std::optional`、`if constexpr` 等）提供了支持，并清理了大量旧代码。

### 2. 移除 Qt 的私有宏

* Qt 6 开始清理大量私有 API 宏（如 `QT_USE_NAMESPACE`），鼓励使用标准 C++ 特性。

### 3. 移除旧功能（废弃 API）

* 清理了在 Qt 5 中标记为 deprecated 的接口，例如旧的 `QRegExp` 被 `QRegularExpression` 取代。

---

## 二、图形和渲染架构

### 1. 基于 **RHI（Rendering Hardware Interface）** 的图形后端

* Qt 6 引入 **RHI** 层，抽象化底层图形 API（如 OpenGL、Vulkan、Direct3D、Metal）。
* Qt Quick 和 Qt 3D 都建立在这个新层上，允许在不同平台使用最佳图形后端。
* 提高了在现代 GPU 上的渲染性能。

### 2. Qt Quick 3D 集成度提高

* 原来 Qt 5 中 Qt Quick 和 Qt 3D 是独立的，现在 Qt Quick 3D 更好地与 Qt Quick 整合。
* 支持高级特性如 PBR 材质、光照、阴影。

---

## 三、模块拆分与重构

Qt 6 对模块结构进行了大幅重构，很多模块被拆分、重命名或独立发布：

| Qt 5 模块             | Qt 6 中的变化                  |
| ------------------- | -------------------------- |
| Qt Multimedia       | 重写，更现代、更接近底层平台 API         |
| Qt Bluetooth, NFC 等 | 拆分为 Qt Connectivity 系列     |
| Qt WebEngine        | 独立版本发布，不再和 Qt 同步更新         |
| Qt XMLPatterns      | 已废弃，不再包含                   |
| Qt Quick Controls 1 | 被移除，保留 Qt Quick Controls 2 |
| Qt Script           | 被移除，推荐使用 QJSEngine 或 QML   |

---

## 四、多平台与设备支持

### 1. 跨平台图形 API 支持

* Qt 6 通过 RHI 支持多种图形后端：

  * Windows: Direct3D 11 / 12、OpenGL
  * macOS/iOS: Metal
  * Linux: Vulkan / OpenGL
  * Android: OpenGL ES / Vulkan

### 2. Wayland 支持增强（Linux）

* Qt 6 增强了对 Wayland 的支持，逐渐脱离对 X11 的依赖。

---

## 五、QML 和 Qt Quick 方面的变化

### 1. QML 语法优化与性能提升

* 改进了 QML 引擎，提升性能。
* 引入 **qmllint、qmlformat** 工具，提高代码质量。

### 2. Qt Quick 3D 的全面融入

* 提供高级 3D 功能的支持，不再需要 Qt 3D。

---

## 六、国际化与本地化支持

* Qt 6 对文本渲染引擎做了改进，提升对复杂脚本（如阿拉伯语、印地语等）的渲染能力。
* 支持 ICU 作为默认的 Unicode 库，提供一致的字符处理。

---

## 七、构建系统变化：CMake 成为主流构建工具

* Qt 5 使用 qmake 构建。
* Qt 6 改为推荐使用 **CMake**，提供更现代和跨平台的构建体验。

---

## 八、性能与内存优化

* 更精细的模块划分，减少不必要依赖。
* 更高效的事件处理系统与内存管理。

---

## 九、长期支持（LTS）策略变化

* Qt 6 LTS 版本（如 6.2、6.5）对商业用户提供长期支持。
* 开源用户仍可使用所有功能，但不享受 LTS 补丁。

---

## 总结：Qt 6 相对 Qt 5 的关键词

| 方面          | Qt 6 特性                            |
| ----------- | ---------------------------------- |
| 标准支持        | C++17、模块化更清晰                       |
| 图形架构        | RHI 支持 Vulkan/Metal/Direct3D       |
| QML & Quick | 性能更高、支持 3D                         |
| 构建系统        | CMake 支持全面                         |
| 多平台支持       | 更好支持 iOS/Android/Linux/Mac/Windows |
| 模块管理        | 更清晰的模块结构，移除废弃功能                    |
