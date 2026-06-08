Qt Quick 是 Qt 框架中用于构建现代、高效、流畅动态图形用户界面（GUI）的标准高级开发库。它通过声明式的 QML 语言设计界面，并使用 JavaScript 处理交互逻辑，特别适合开发移动端、嵌入式以及需要酷炫动画效果的桌面端应用程序。 
## 核心概念：Qt Quick 与 QML 的关系
这两者经常被一同提及，但职责明确不同： 
* QML (Qt Meta-Object Language)：是一种类似于 JSON 的声明式编程语言，用于定义界面的结构和属性。
* Qt Quick：是基于 QML 语言的可视化标准库。它提供了构成界面的所有可视化基本元素（如矩形、文本、图像等）以及动画系统。

## 🚀 为什么选择 Qt Quick？

* 开发效率高：界面排版类似于 HTML/CSS 加上 JSON，代码量远少于传统 C++。
* 原生卓越性能：底层基于渲染树（Scene Graph），利用 GPU 硬件加速，动画和特效极其丝滑。
* 前后端分离：UI 设计师或前端人员可以用 QML 专注写界面，后端程序员用 C++ 编写高性能逻辑，两者可以通过强大的信号与槽无缝对接。 

------------------------------
## 💻 基础代码示例
这是一个标准的 Qt Quick (main.qml) 基础页面，包含一个带圆角的按钮，点击时文字会发生改变：
```
import QtQuick 2.15
import QtQuick.Window 2.15

Window {
    width: 400
    height: 300
    visible: true
    title: "Qt Quick 示例"
    color: "#f5f5f5"

    // 定义一个中央的彩色卡片组件
    Rectangle {
        id: myButton
        width: 150
        height: 50
        color: "#007bff"
        radius: 8
        anchors.centerIn: parent // 屏幕居中
        
        // 按钮内的文本
        Text {
            id: buttonText
            text: "点击我"
            color: "white"
            font.pixelSize: 16
            anchors.centerIn: parent
        }

        // 鼠标交互与属性绑定
        MouseArea {
            anchors.fill: parent
            onClicked: {
                buttonText.text = "Hello, Qt Quick!"
                myButton.color = "#28a745" // 点击后变绿
            }
        }
        
        // 自带丝滑动画
        Behavior on color {
            ColorAnimation { duration: 200 }
        }
    }
}
```
