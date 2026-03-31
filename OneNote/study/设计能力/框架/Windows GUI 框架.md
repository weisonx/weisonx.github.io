下面这些，都是今天仍然在 Windows 上实际存在、被使用的 GUI 技术：

微软原生框架：

- Win32（1985）—— 仍在，还被广泛使用。Petzold 的那本书到现在依然适用。
- MFC（1992）—— 基于 Win32 的 C++ 封装。进入维护期，在企业软件和 CAD 领域依然活跃。
- WinForms（2002）—— .NET 对 Win32 的封装。“可以用，但不推荐。”不过做数据录入界面依然是最快的选择。
- WPF（2006）—— 基于 XAML、由 DirectX 渲染、已开源。但微软已经不再新增投入。
- WinUI 3 / Windows App SDK（2021）—— 被称为“现代答案”，但路线仍不明朗。
- MAUI（2022）—— Xamarin.Forms 的跨平台继任者，也是 .NET 团队当前押注的方向。
  
微软的 Web 混合方案：

- Blazor Hybrid —— 在原生 WebView 中运行 .NET 的 Razor 组件。
- WebView2 —— 在 Win32 / WinForms / WPF 应用中嵌入 Chromium。
  
第三方方案：

- Electron —— Chromium + Node.js。VS Code、Slack、Discord 都在用。如今 Windows 上部署最广泛的桌面 GUI 技术，但和微软没什么关系。
- Flutter（Google）—— 使用 Dart，自带渲染引擎，跨平台。
- Tauri —— 基于 Rust 的轻量级 Electron 替代方案。
- Qt —— 支持 C++ / Python / JavaScript，严肃的跨平台解决方案。
- React Native for Windows —— 微软支持的 Facebook 移动框架移植版。
- Avalonia —— 开源的“WPF 精神续作”。被 JetBrains、GitHub、Unity 等采用——这些开发者已经不再等待微软。
- Uno Platform —— 把 WinUI API 带到所有平台上，在某种意义上比微软自己更坚持 WinUI。
- Delphi / RAD Studio —— 还活着，还很高效，在垂直行业软件中依然占有一席之地。
- Java Swing / JavaFX —— 是的，还在生产环境中运行。企业世界从不轻易遗忘。
  
一共十七种路径，五种编程语言，三种渲染思路。这已经不能叫“平台”了。也许我没法给“boof-a-rama”下一个精确定义，但我一眼就能认出来。

[来自](https://blog.csdn.net/csdnnews/article/details/159555125?spm=1000.2115.3001.5927)
