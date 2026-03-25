从事 **C++ + Qt 的音视频开发**，你需要掌握一个相对系统的技术栈，涵盖从音视频编码/解码、播放、采集、传输、同步、UI 到跨平台部署等多个维度。
---

# 技术栈

## 🎯 一、核心语言和基础能力

| 技术              | 说明                                                                                   |
| --------------- | ------------------------------------------------------------------------------------ |
| **C++11/14/17** | 内存管理、智能指针、多线程、STL、RAII                                                               |
| **Qt（重点）**      | 跨平台 GUI、信号槽机制、事件驱动、多媒体模块（`QMediaPlayer`、`QCamera`）、线程、绘图（`QPainter`、`QOpenGLWidget`） |

---

## 🎞️ 二、音视频核心库（务必熟练）

| 库                            | 用途                              |
| ---------------------------- | ------------------------------- |
| **FFmpeg**                   | 音视频解码、编码、封装格式处理（MP4、FLV、RTMP 等） |
| **libswscale / libavfilter** | 图像格式转换、滤镜处理                     |
| **SDL（可选）**                  | 视频显示 / 音频播放（或用 Qt 替代）           |
| **OpenGL / QOpenGL**         | GPU 加速显示视频帧（渲染 YUV / RGB 纹理）    |
| **PortAudio / OpenAL**       | 音频采集 / 播放（低延迟音频）                |

---

## 📡 三、音视频通信与传输（做直播、会议必须懂）

| 技术                             | 说明                         |
| ------------------------------ | -------------------------- |
| **RTMP / RTP / RTSP / WebRTC** | 视频直播/流媒体传输协议               |
| **Socket / UDP / TCP**         | 底层网络通信                     |
| **自定义封包协议 / 流控制**              | 视频帧打包、时间戳、缓冲机制             |
| **多线程与同步**                     | 音视频同步（PTS / DTS），播放控制、缓存管理 |

---

## 🧩 四、实用工具和开发技巧

| 工具                           | 用途                   |
| ---------------------------- | -------------------- |
| **FFmpeg 命令行**               | 转码、测试流、调试解码器         |
| **ffprobe**                  | 媒体文件信息查看             |
| **Wireshark**                | 分析音视频网络流（RTMP、RTP 包） |
| **Qt Creator / CMake / gdb** | 构建和调试                |
| **Git / GitLab / Jenkins**   | 团队协作与持续集成            |

---

## 🧠 五、必须掌握的知识体系（理论+实战）

### 📽️ 音视频基础知识

* 编解码原理（H.264、AAC、VP9、Opus 等）
* 容器格式（MP4、FLV、MKV）
* 帧类型（I/P/B 帧）、GOP、码率、帧率、分辨率
* 色彩空间（YUV、RGB）与格式（YUV420p 等）

### 🎵 音频基础

* 采样率、通道、采样格式（PCM）
* 音视频同步策略（使用时间戳同步）
* 声音回声消除（AEC）、降噪（NS）、增益控制（AGC）

---

## 🎮 六、可选方向拓展（高级开发）

| 方向                          | 应用                                      |
| --------------------------- | --------------------------------------- |
| **多路视频混流 / 推流 / 录像**        | 类似 OBS                                  |
| **视频特效 / 滤镜 / 字幕**          | 用 OpenGL / Shader / libavfilter 实现      |
| **硬件加速**                    | VAAPI、NVENC、DXVA、QSV（大幅提升编码效率）          |
| **跨平台开发**                   | 熟悉在 Windows、Linux、macOS 下部署 FFmpeg + Qt |
| **移动端开发（Qt + Android/iOS）** | 可拓展音视频开发经验到手机端                          |

---

## 🛠️ 七、推荐技术组合实例

| 功能模块  | 推荐技术                                 |
| ----- | ------------------------------------ |
| 视频解码  | FFmpeg（avcodec）                      |
| 音频播放  | SDL2 / QtAudioOutput                 |
| 视频显示  | QOpenGLWidget + OpenGL 纹理渲染          |
| 网络传输  | FFmpeg（libavformat） + TCP/UDP Socket |
| UI 控制 | Qt Widgets / QML                     |
| 播放控制  | 自定义播放器：基于 FFmpeg + Qt 多线程架构          |

---

## 🧪 八、项目实战建议（你可以从这些练手）

1. ✅ 使用 FFmpeg 解码视频并用 Qt 显示（YUV → RGB → OpenGL 渲染）
2. ✅ 录屏 + 麦克风采集 → 音视频混合推流（RTMP）
3. ✅ 模拟简易播放器（支持暂停/快进/倍速）
4. ✅ 编写一个小型会议系统（支持摄像头采集 + 网络传输 + 播放）
5. ✅ 播放网络视频流（RTMP/HTTP）并同步显示音频

---

# 入门到进阶的学习路线或开源项目

## 🎓 一、学习路线总览（从入门到进阶）

| 阶段 | 目标                    | 涉及内容                             |
| -- | --------------------- | -------------------------------- |
| 初级 | 入门 C++/Qt 和音视频基础      | C++11、Qt Widgets、多线程、FFmpeg 基础使用 |
| 中级 | 能用 FFmpeg 进行解码、播放、采集等 | 解码器/采集/显示/同步、多线程播放器架构            |
| 高级 | 构建完整项目（播放器/推流/会议系统）   | OpenGL 渲染、网络传输协议、流控、音视频同步        |

---

## 🧱 二、阶段细化路线（含推荐资源）

### ✅ 阶段 1：基础阶段（1～2 周）

#### 🎯 目标：掌握基本的 Qt 与 C++ 用法，了解 FFmpeg

* C++11 语法：智能指针、lambda、RAII、线程（std::thread）
* Qt 基础：`QWidget`, `QThread`, `QTimer`, `signal/slot`, `QPainter`
* 编译工具链：使用 Qt Creator 或 CMake + Qt + FFmpeg
* FFmpeg 命令行工具基本使用（转码、提取、剪切）

> **推荐学习资源：**

* Qt 官方文档：[https://doc.qt.io](https://doc.qt.io)
* C++ Primer（第五版）
* [FFmpeg 官网](https://ffmpeg.org/documentation.html) 和 B 站 FFmpeg 入门视频

---

### ✅ 阶段 2：中级阶段（2～4 周）

#### 🎯 目标：实现音视频播放器的基础功能（解码 + 播放）

* FFmpeg 解封装（avformat）、解码（avcodec）、YUV 图像转换（libswscale）
* 音视频基本理论：编码、封装、帧、时间戳、PTS/DTS、同步等
* Qt 多线程模型（QThread + signal/slot）
* 使用 `QOpenGLWidget` + OpenGL 渲染 YUV 视频帧
* 音频播放（使用 SDL2、QAudioOutput、PortAudio）

> ✅ **实战练习建议：**

* 【练习 1】：编写一个简单的 MP4 播放器（音画分离但能播放）
* 【练习 2】：Qt 实现 YUV 视频帧播放（OpenGL/Software 渲染）

---

### ✅ 阶段 3：进阶阶段（1～2 个月）

#### 🎯 目标：具备项目实战开发能力

* 实现音视频同步（主时钟选择、同步策略）
* 实现倍速播放、暂停、拖动进度条
* 摄像头、麦克风采集（QtMultimedia / FFmpeg）
* 使用 OpenGL 实现滤镜 / 特效（Shader）
* 网络传输：RTMP、RTP、UDP + 自定义协议
* 推流功能：FFmpeg 实现摄像头/桌面推流
* 音视频混合录制、转码、截图

> ✅ **实战项目建议：**

* 【项目 1】：完整播放器（多线程 + OpenGL + 音视频同步）
* 【项目 2】：本地录制 + 推流功能（类似 OBS）
* 【项目 3】：多人音视频通话 demo（采集 + 传输 + 渲染）

---

## 🌟 三、推荐的开源项目

### 1. **FFmpeg-Qt-Player（C++ + Qt + FFmpeg）**

* 地址：[https://github.com/wang-bin/QtAV（已归档）](https://github.com/wang-bin/QtAV（已归档）)
* 简介：Qt 音视频播放库，基于 FFmpeg，封装较好，功能齐全
* 适合学习：音视频解码、多线程架构、OpenGL 渲染

### 2. **ffplay（FFmpeg 自带播放器源码）**

* 路径：FFmpeg 源码中的 `fftools/ffplay.c`
* 简介：学习 FFmpeg 如何解码并播放（C 写的）
* 适合学习：最权威的 FFmpeg 播放流程、解码原理

### 3. **xiu（基于 FFmpeg 实现 RTMP 服务器）**

* 地址：[https://github.com/ireader/media-server](https://github.com/ireader/media-server)
* 适合学习：如何从底层理解流媒体协议（RTMP、HLS）

### 4. **QtFFmpegPlayer**

* 地址：[https://github.com/zdyxry/QtFFmpegPlayer](https://github.com/zdyxry/QtFFmpegPlayer)
* 简介：一个使用 Qt + FFmpeg 构建的完整播放器
* 有多线程音视频同步、暂停、拖动、音量控制等功能

### 5. **OBS Studio**

* 地址：[https://github.com/obsproject/obs-studio](https://github.com/obsproject/obs-studio)
* 简介：开源直播推流神器（C/C++ + FFmpeg + Qt + OpenGL）
* 适合深入：学习推流、滤镜、图形渲染等高级功能

---

## 🧠 四、思维模型建议（音视频方向）

你在开发过程中，建议建立如下的模块化理解：

```
┌───────────┐
│ 摄像头 / 麦克风采集 │
└───────────┘
         ↓
┌────────────┐
│ 数据封装与编码（FFmpeg） │
└────────────┘
         ↓
┌────────────┐
│ 网络传输（RTMP/WebRTC） │
└────────────┘
         ↓
┌────────────┐
│ 解码与渲染（OpenGL + AudioOut） │
└────────────┘
         ↓
┌────────────┐
│ Qt UI 展示与控制 │
└────────────┘
```

---

## 🚀 五、开发环境建议

| 工具                     | 推荐配置                           |
| ---------------------- | ------------------------------ |
| 编译器                    | GCC / MSVC / Clang             |
| 构建工具                   | CMake + Qt Creator             |
| FFmpeg                 | 自行编译（支持 OpenGL/SDL2）或使用第三方预编译库 |
| SDL2                   | 用于音频输出和窗口测试                    |
| OpenGL / QOpenGLWidget | 用于视频渲染                         |
| Git                    | 版本控制，方便团队协作                    |

---

## ✅ 六、结语

音视频开发是一个很深、很有挑战也很有前景的方向。掌握 Qt + FFmpeg + OpenGL 后，你不仅可以：

* 做一个高性能本地播放器；
* 开发直播系统、录屏工具；
* 进入流媒体、教育、游戏、医疗影像等高薪行业。
