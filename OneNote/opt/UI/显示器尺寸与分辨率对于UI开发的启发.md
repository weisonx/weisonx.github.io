## 一、 核心开发原则（怎么做）

1. 抛弃绝对像素： 全面使用弹性布局（如 Qt 的 QLayout、QML Anchors），严禁写死坐标和宽高；字体用 pt（磅）代替 px（像素），关键控件设置最小尺寸防挤压。
2. 资源矢量化： 图标尽量用 SVG 或字体图标；复杂位图必须准备 @2x/@3x 多倍图，防止放大发虚。
3. 动态适配多屏： 监听窗口跨屏移动（screenChanged）和 DPI 变化信号；弹窗位置基于当前鼠标/父窗口所在屏幕计算，不要死盯主屏。
4. 极端比例兜底： 带鱼屏（21:9/32:9）限制内容区最大宽度；窄屏或半屏时，借鉴响应式设计自动折叠侧边栏。

## 二、 行业标杆参考（谁做得好）

*  极致跨平台与多屏： Chrome / Edge、VS Code、Figma（自研渲染 + Per-Monitor V2，4K 屏拖到 1080p 屏无感无损重绘）。

*  Qt 生态典范： Telegram Desktop（内置 100%~300% 缩放滑块 + 宽窄屏自动切换单/三栏布局）、KDE Plasma（攻克 Linux 分数倍缩放）、达芬奇（DaVinci Resolve）（多屏 4K/8K 监视器深度适配）。

*  系统级标杆： Blender（支持界面比例自由微调）、Final Cut Pro（macOS 原生像素级对齐）。

## 三、 开启高 DPI 的四个前提（动手前准备）

在打开高 DPI 开关前，必须先满足以下条件，否则界面会错位或局部缩小：

1. 布局去硬编码： 清理代码中写死的 setFixedSize 和 pixelSize。
2. 资源已就位： .png 图标已替换为 .svg 或补齐了 @2x 高清图。
3. 系统级声明： Windows 需在 Manifest 清单中声明 PerMonitorV2（防止系统强行模糊拉伸）；macOS 需开启 NSHighResolutionCapable。
4. 初始化时机与底层渲染： 高 DPI 属性必须在创建 QApplication 之前设置；自绘控件（QPainter）和 OpenGL 视口尺寸需手动乘以 devicePixelRatio（设备像素比）。
