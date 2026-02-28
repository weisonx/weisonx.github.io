用QML和ui的项目结构都是怎么样的

这两者的项目结构反映了它们不同的运行机制：一个是纯编译型的 C++ 结构，一个是“C++ 外壳 + 资源文件”的结构。

## .ui 项目结构 (Qt Widgets)
这是最经典的 C++ 结构。每一个窗口通常由三个文件组成：

mainwindow.ui：XML 格式，存界面长什么样。

mainwindow.h：头文件，定义类和信号槽。

mainwindow.cpp：实现业务逻辑。

main.cpp：程序入口，实例化 MainWindow 并 show() 出来。

编译后： Qt 的 uic 工具会把 .ui 转成 ui_mainwindow.h。你的 C++ 代码通过 ui->buttonName 来控制界面控件。

##  QML 项目结构 (Qt Quick)
   
现代 Qt6 项目推荐使用 CMake 管理。其核心思想是：C++ 负责启动引擎，加载并驱动 QML。

main.cpp：

创建一个 QQmlApplicationEngine。
加载 main.qml。
将 C++ 业务类注入到 QML 上下文中。

Main.qml：程序的顶级界面。

qtquickcontrols2.conf（可选）：配置界面皮肤（如 Material 或 Universal 风格）。

CMakeLists.txt：负责把 C++ 代码和 QML 文件打包在一起。

Qt6 的新变化：
在 Qt6 中，QML 文件通常会被 QML 模块系统 (CMake qt_add_qml_module) 自动编译并嵌入到二进制文件中，避免了旧版本资源文件（.qrc）路径复杂的麻烦。
