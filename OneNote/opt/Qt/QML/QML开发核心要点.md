## 🚀 1. 环境搭建与项目初始化 [4] 
在现代 Qt 6 项目中，推荐使用 CMake 来管理 QML 模块。
## CMakeLists.txt 配置
使用 [Qt 官方推荐的 CMake 函数](https://zhuanlan.zhihu.com/p/661283938) 声明 QML 模块：
```cmake
cmake_minimum_required(VERSION 3.16)project(MyQmlApp VERSION 0.1 LANGUAGES CXX)
find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_add_executable(appMyQmlApp main.cpp)
```
# Qt 6 核心：自动处理 QML 资源注册与 C++ 类型导出
```cmake
qt_add_qml_module(appMyQmlApp
    URI MyCompany.Controls
    VERSION 1.0
    QML_FILES main.qml MyButton.qml
)
target_link_libraries(appMyQmlApp PRIVATE Qt6::Quick)
```
------------------------------
## 🎨 2. 基础语法与核心概念
QML 是一种声明式语言，采用键值对和层级嵌套的方式描述 UI。 

```qml
// main.qml
import QtQuick 6.0
import QtQuick.Window 6.0

Window {
    width: 640
    height: 480
    visible: true
    title: "新 QML 教程示例"

    // Rectangle 组件：最基础的可视化容器
    Rectangle {
        id: myBox                 // 唯一标识符，用于组件间引用
        width: 200
        height: 100
        color: "lightblue"
        
        // 属性绑定（Property Binding）：QML 的灵魂，当 parent 尺寸改变时自动计算
        anchors.centerIn: parent  
        
        Text {
            text: "Hello, QML!"
            anchors.centerIn: parent
            font.pixelSize: 16
        }
    }
}
```
------------------------------
## 📐 3. 布局与定位机制
在 QML 中，不要使用死板的绝对坐标（XY），推荐使用以下两种现代布局方式：

* 锚定布局 (Anchors)：通过相对边界定位。
* anchors.fill: parent 填满父元素。
   * anchors.left: otherItem.right 靠在另一个组件右侧。
* 布局管理器 (QtQuick.Layouts)：自适应网格或行列布局。
* 需要引入 import QtQuick.Layouts 6.0。
   * ColumnLayout：垂直布局。
   * RowLayout：水平布局。
   * GridLayout：网格布局。 

------------------------------
## ⚙️ 4. 属性、信号与槽（高级核心）
QML 组件的动态交互通过属性（Property）和信号（Signal）实现。
## 自定义组件：MyButton.qml 
```qml
import QtQuick 6.0

Item {
    id: root
    width: 120; height: 40
    
    // 1. 定义对外导出的自定义属性
    property alias buttonText: textElement.text
    property color buttonColor: "dodgerblue"
    
    // 2. 定义自定义信号
    signal buttonClicked()

    Rectangle {
        anchors.fill: parent
        color: root.buttonColor
        radius: 4

        Text {
            id: textElement
            anchors.centerIn: parent
            color: "white"
        }

        MouseArea {
            anchors.fill: parent
            // 3. 触发信号
            onClicked: root.buttonClicked() 
        }
    }
}

## 在主页面中使用该组件

MyButton {
    buttonText: "点击我"
    buttonColor: "emerald"
    
    // 槽函数响应：信号名为 buttonClicked，对应的处理器为 onButtonClicked
    onButtonClicked: {
        console.log("按钮被按下了！");
    }
}
```
------------------------------
## ⚡ 5. 动画与状态管理 (States & Transitions)
Qt Quick 的动画功能极其简单，通常配合状态机使用。 
```qml
Rectangle {
    id: rect
    width: 100; height: 100
    color: "red"

    // 定义状态
    states: [
        State {
            name: "clicked"
            PropertyChanges { target: rect; color: "green"; scale: 1.5 }
        }
    ]

    // 定义状态切换时的平滑动画
    transitions: [
        Transition {
            from: "*"; to: "clicked"
            ColorAnimation { duration: 200 }
            NumberAnimation { properties: "scale"; duration: 200; easing.type: Easing.InOutQuad }
        }
    ]

    MouseArea {
        anchors.fill: parent
        onClicked: rect.state = "clicked"
    }
}
```
------------------------------
## 📚 官方首选自学资源
如果你想进一步深入，请直接参考以下官方权威教程：

* 基础入门：阅读官方的 [Qt Quick QML 基础教程](https://doc.qt.io/qt-6/zh/qml-tutorial.html)，重点学习值类型、属性绑定和组件化概念。
* 进阶实战：参考 [Qt Quick QML 高级开发教程](https://doc.qt.io/qt-6/zh/qml-advtutorial.html)，它将指导你通过 JavaScript 集成和持久化数据开发一个完整的“Same Game”小游戏。 
