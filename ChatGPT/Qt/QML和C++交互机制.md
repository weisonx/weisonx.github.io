QML (Qt Quick) 和 C++ 的交互是 Qt 框架的核心强大功能之一。通常的情况是：**QML 负责前端界面展示，C++ 负责后端业务逻辑和数据处理。**

两者交互主要通过以下几种机制实现：

---

### 1. 核心机制概览

| 交互方向 | 核心机制 | 典型应用场景 |
| :--- | :--- | :--- |
| **C++ -> QML** | **属性绑定** | C++ 数据变化自动更新 UI |
| **C++ -> QML** | **信号与槽** | C++ 触发事件，QML 响应更新 |
| **QML -> C++** | **Q_INVOKABLE / 槽函数** | QML 调用 C++ 函数执行逻辑 |
| **QML -> C++** | **直接访问属性** | QML 读取 C++ 对象的成员变量 |
| **对象注入** | **setContextProperty** | 将 C++ 对象直接暴露给 QML 全局使用 |
| **类型注册** | **qmlRegisterType** | 在 QML 中像使用组件一样实例化 C++ 类 |

---

### 2. 详细交互方式

#### 方式一：将 C++ 对象暴露给 QML (setContextProperty)

这是最简单直接的方式。你可以在 C++ 中，然后将其注入到 QML 的根上下文中，使得 QML 可以直接通过对象名访问它。

**C++ 代码:**
```cpp
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "myclass.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // 1. 创建 C++ 对象
    MyClass myObj;

    QQmlApplicationEngine engine;
    
    // 2. 将对象注入 QML 上下文，命名为 "myBackend"
    engine.rootContext()->setContextProperty("myBackend", &myObj);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();
}
```

**QML 代码:**
```qml
import QtQuick 2.0

Item {
    // 直接使用 C++ 对象名调用方法或访问属性
    Component.onCompleted: {
        console.log(myBackend.userName); // 读取属性
        myBackend.doSomething();        // 调用函数
    }
}
```

#### 方式二：注册 C++ 类为 QML 类型

这种方式允许你在 QML 中直接实例化 C++ 类，就像使用 `Rectangle` 或 `Button` 一样。这更适合创建可复用的自定义控件或组件。

**C++ 代码:**
```cpp
#include <QtQml/qqml.h>
#include "myclass.h"

int main(int argc, char *argv[])
{
    // ...
    
    // 注册类，版本 1.0，导入名为 "com.mycompany.components"
    qmlRegisterType<MyClass>("com.mycompany.components", 1, 0, "MyCustomItem");

    QQmlApplicationEngine engine;
    // ...
}
```

**QML 代码:**
```qml
import QtQuick 2.0
import com.mycompany.components 1.0 // 导入注册的模块

Item {
    // 像普通 QML 组件一样实例化
    MyCustomItem {
        id: myItem
        userName: "Alice" // 设置属性
        
        onSomeSignal: console.log("Signal received!") // 连接信号
    }
    
    Button {
        onClicked: myItem.doSomething()
    }
}
```

---

### 3. 具体交互细节 (属性、方法、信号)

为了让 C++ 类能够与 QML 无缝交互，该类必须继承自 `QObject` 并使用 `Q_OBJECT` 宏。

#### A. 属性
QML 需要知道 C++ 属性的变化，这通过 `Q_PROPERTY` 宏实现。它包含一个 `NOTIFY` 信号，当属性变化时发射信号，QML 引擎会自动更新绑定的 UI。

**C++ 头文件:**
```cpp
class MyClass : public QObject
{
    Q_OBJECT
    // 定义属性：可读、可写、变化通知
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)

public:
    QString userName() const;
    void setUserName(const QString &name);

signals:
    void userNameChanged();

private:
    QString m_userName;
};
```
**QML 中:**
```qml
Text {
    // 当 C++ 中 m_userName 变化时，Text 自动更新，无需手动刷新
    text: myBackend.userName 
}
```

#### B. 方法调用
QML 调用 C++ 函数有两种方式：

1.  **Q_INVOKABLE 宏**：标记普通成员函数为“可调用”。
2.  **public slots**：将函数声明为槽函数。

**C++ 头文件:**
```cpp
public:
    Q_INVOKABLE void calculate(int a, int b); // 方式 1

public slots: // 方式 2
    void refreshData();
```

**QML 中:**
```qml
onClicked: {
    myBackend.calculate(10, 20);
    myBackend.refreshData();
}
```

#### C. 信号与槽
这是 Qt 的核心机制。C++ 可以发射信号，QML 可以连接这些信号。

**C++ 类:**
```cpp
signals:
    void dataReady(const QString &data);
```

**QML 中连接信号:**
```qml
Connections {
    target: myBackend
    onDataReady: {
        console.log("Received data:", data)
    }
}
```

---

### 4. 数据模型交互

除了单个对象，QML 经常需要展示列表数据（如 ListView）。C++ 可以通过继承 `QAbstractListModel` 或 `QAbstractTableModel` 将复杂的数据结构传递给 QML。

*   **C++:** 实现 `rowCount()`, `data()`, `roleNames()` 等虚函数。
*   **QML:** 直接将 C++ model 对象赋值给 `ListView.model`。

---

### 5. 总结：如何选择？

1.  **全局单例/控制器**：如果你的 C++ 对象是全局唯一的（如网络管理器、系统设置），使用 `setContextProperty` 或 `qmlRegisterSingletonType`。
2.  **自定义可视化组件**：如果你在 C++ 中绘制自定义图形或创建可复用的控件，使用 `qmlRegisterType`。
3.  **数据展示**：如果是列表或表格，继承 Model 类。
4.  **简单交互**：记住 `Q_PROPERTY` (用于数据绑定) 和 `Q_INVOKABLE` (用于函数调用)。

**核心原则：** 尽量在 C++ 中处理逻辑，通过属性绑定将结果“推”给 QML，而不是让 QML 频繁地去 C++ “拉”数据或处理逻辑。
