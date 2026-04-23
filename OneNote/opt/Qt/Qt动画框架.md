Qt 动画框架提供了一种简单的方法来为 GUI 元素制作平滑动画。它通过控制 Qt 属性（[Qt Properties](https://doc.qt.io/qt-6/properties.html)）的数值变化，让继承自 QObject 的对象（如各种控件）产生动效。
## 1. 核心类结构
动画框架采用树状架构，主要由以下几部分组成：

* [QAbstractAnimation](https://doc.qt.io/qt-6/qabstractanimation.html)：所有动画类的基类，定义了通用的控制功能，如 start()、stop()、pause() 及动画状态（运行、停止、暂停）。
* [QVariantAnimation](https://doc.qt.io/qt-6/qvariantanimation.html)：支持对各种 QVariant 类型值进行插值计算。
* [QPropertyAnimation](https://doc.qt.io/qt-6/qpropertyanimation.html)：最常用的实际动画类，将对象的属性（如位置 pos 或大小 geometry）与动画逻辑连接起来。
* [QAnimationGroup](https://doc.qt.io/qt-6/qanimationgroup.html)：动画容器，用于组合多个动画。
* QParallelAnimationGroup：同时运行组内所有动画（并行）。
   * QSequentialAnimationGroup：按顺序一个接一个运行动画（串行）。

## 2. 关键辅助工具

* [QEasingCurve](https://doc.qt.io/qt-6/qeasingcurve.html)：控制动画的插值速度规律。通过设置不同的曲线（如 OutBounce 弹跳效果或 InBack 回弹效果），可以使动画看起来更自然、生动。
* [QTimeLine](https://doc.qt.io/qt-6/qtimeline.html)：提供一个时间轴来控制动画进度，常用于需要手动控制每一帧变化的复杂场景。 

## 3. 应用场景

* Qt Widgets 动画：用于传统的桌面端界面，例如让侧边栏滑动切出、按钮点击缩放或窗口渐变透明。
* Qt Quick (QML) 动画：虽然 Qt Quick 有自己的一套声明式动画语法（如 NumberAnimation），但其底层逻辑与 C++ 动画框架非常相似，学到的插值和缓动曲线概念通用。
* 图形视图框架：可与 QGraphicsItem 结合，实现复杂的场景元素动画。
