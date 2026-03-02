在 QML 应用程序中，"入口"的概念通常分为两个层面：**QML 文件的入口（程序界面）** 和 **C++ 程序的入口（底层启动）**。

# URL

### 1. QML 文件的入口：`Window` 或 `ApplicationWindow`

在 QML 代码层面，**并没有一个特定的 "main" 函数**。程序的入口就是**那个被加载的 QML 文件**。

通常，这个文件中会定义一个根对象，一般是 `Window`、`ApplicationWindow` 或者 `Item`。当这个文件被加载引擎时，这个根对象会被实例化并显示。

**示例 (`main.qml`)：**

```qml
import QtQuick 2.15
import QtQuick.Controls 2.15 // 如果使用 ApplicationWindow 需要这个

ApplicationWindow {
    id: root
    visible: true        // 必须设为 true，否则窗口不会显示
    width: 640
    height: 480
    title: qsTr("QML 入口演示")

    // 这里的代码在组件加载时立即执行（类似构造函数）
    Component.onCompleted: {
        console.log("QML 界面加载完成！这是 QML 的逻辑入口。")
    }

    Text {
        anchors.centerIn: parent
        text: "Hello World"
    }
}
```

*   **`visible: true`**：这是最关键的属性，它告诉程序显示这个窗口。
*   **`Component.onCompleted`**：这是 QML 中最接近“构造函数”或“程序初始化逻辑”的地方，常用来在界面启动时执行一些初始化代码。

---

### 2. C++ 程序的入口：`main.cpp` (Qt Quick)

虽然你写的是 QML，但 Qt 程序底层依然需要一个 C++ 的 `main` 函数来启动引擎并加载那个 QML 文件。这就是真正的“程序入口”。

**示例 (`main.cpp`)：**

```cpp
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext> // 如果需要注入 C++ 对象

int main(int argc, char *argv[])
{
    // 1. 初始化 Qt 应用程序环境
    QGuiApplication app(argc, argv);

    // 2. 创建 QML 引擎
    QQmlApplicationEngine engine;

    // (可选) 在这里暴露 C++ 类或对象给 QML
    // engine.rootContext()->setContextProperty("myBackend", &backend);

    // 3. 加载入口 QML 文件 (通常是 main.qml)
    // Qt 6 推荐使用 QUrl::fromLocalFile
    const QUrl url(u"qrc:/main.qml"_qs); 
    
    // 添加加载时的错误捕获（Qt 5/6 通用写法）
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1); // 如果加载失败，退出程序
    }, Qt::QueuedConnection);

    // 4. 真正执行加载
    engine.load(url);

    // 5. 进入事件循环
    return app.exec();
}
```

**流程解析：**
1.  `QGuiApplication app`：初始化应用程序。
2.  `QQmlApplicationEngine engine`：创建一个能解析 QML 的引擎。
3.  `engine.load(url)`：**这是连接 C++ 和 QML 的桥梁**。它告诉引擎去加载 `main.qml`。一旦加载，QML 中的 `Window` 就会被创建。
4.  `app.exec()`：程序进入“事件循环”，开始响应用户的点击和输入。

---

### 3. 特殊情况：Qt Quick Controls 1 (旧式) 或 `QQuickView`

在较旧的代码或特定场景中，你可能会看到 `QQuickView`。这种情况下，C++ 需要显式调用 `show()`。

```cpp
// 旧式写法 (不推荐用于新项目，但常见于老代码)
QQuickView view;
view.setSource(QUrl(QStringLiteral("qrc:/main.qml")));
view.show(); // 注意：这里需要 C++ 手动调用 show()
```

在这种模式下，QML 文件中的根对象通常是一个 `Item`，而不是 `Window`，因为窗口的外壳是由 C++ 的 `view` 提供的。

### 总结

*   **对于 QML 开发者**：你的入口是 `main.qml` 文件中的根对象（通常是 `Window`）。你通常在 `Component.onCompleted` 中写启动逻辑。
*   **对于项目结构**：真正的入口是 C++ 中的 `main.cpp`，它负责启动引擎并加载你的 `main.qml`。

# 模块

它改变了传统的“加载文件路径”的方式，转而使用“加载模块”的方式。这是 Qt 6 推荐的现代开发模式。

### 1. C++ 中的写法 (`main.cpp`)

使用 `loadFromModule` 时，你不再需要手动拼写 `.qml` 文件的路径字符串，而是通过 **模块名** 和 **主文件名** 来加载。

```cpp
#include <QGuiApplication>
#include <QQmlApplicationEngine>

// 引入静态导入函数的头文件 (由 CMake 自动生成)
#include "app_myapp.h" // 命名规则通常为 <appname>_<target>.h

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // 【关键点】使用 loadFromModule
    // 参数1: QML 模块的 URI (在 CMakeLists.txt 中定义的)
    // 参数2: 该模块中作为入口的 QML 单例类型 (通常是 Main 或 Root)
    engine.loadFromModule("MyApp", "Main");

    // 检查是否加载成功
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
```

### 2. 必须配置 CMakeLists.txt

`loadFromModule` 无法单独工作，它依赖于 CMake 中的 `qt_add_qml_module`。你需要在这里告诉 Qt：哪个 QML 文件是入口。

```cmake
cmake_minimum_required(VERSION 3.16)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_add_executable(myapp
    main.cpp
)

# 【关键点】定义 QML 模块
qt_add_qml_module(myapp
    URI MyApp            # 模块 URI，对应 C++ 中的第一个参数
    VERSION 1.0
    QML_FILES
        Main.qml         # 你的入口 QML 文件
        OtherPage.qml    # 其他文件
)
```

### 3. QML 文件的写法 (`Main.qml`)

使用 `loadFromModule` 时，QML 文件本身**不需要**做任何特殊的改变来“配合”加载，但通常入口文件需要是一个可视化的组件（如 `Window`）。

**注意文件名**：CMake 中列出的 `Main.qml`，在 `loadFromModule` 中对应的第二个参数就是 `"Main"`。

```qml
// Main.qml
import QtQuick 2.15
import QtQuick.Controls 2.15

Window {
    visible: true // 确保可见
    width: 640
    height: 480
    title: "Qt6 loadFromModule Demo"

    Text {
        anchors.centerIn: parent
        text: "Hello from Module!"
    }
}
```

---

### 4. 为什么要用 `loadFromModule`？(与 `load(QUrl)` 的区别)

这是最核心的理解点：

#### 传统方式 (`engine.load(QUrl("qrc:/Main.qml"))`)
*   **基于文件路径**：你告诉引擎去哪里找文件。
*   **缺点**：如果文件移动了位置，或者文件名变了，字符串容易失效。且无法利用 QML 的类型编译期检查。

#### 现代方式 (`engine.loadFromModule("MyApp", "Main")`)
*   **基于类型系统**：你告诉引擎加载名为 "MyApp" 的模块中，注册名为 "Main" 的那个类型。
*   **优点**：
    1.  **类型安全**：Qt 的编译系统会在编译时检查 `Main.qml` 是否存在，语法是否正确。
    2.  **单例模式**：`loadFromModule` 实际上是实例化了一个 QML 单例。这意味着你在 C++ 中可以更容易地通过 `engine.singletonInstance` 获取这个根对象的指针进行交互。
    3.  **无路径依赖**：你不需要关心文件是在 `qrc` 资源里还是在文件系统里，模块系统会自动处理。

### 总结

如果你在 Qt 6 环境下开发，**推荐使用 `loadFromModule`**。

**入口流程变为：**
1.  **CMake** 定义模块 (`qt_add_qml_module`) 并列出 `Main.qml`。
2.  **C++** 调用 `engine.loadFromModule("模块名", "文件名")`。
3.  **QML** 定义 `Window` 并正常编写界面。
