- QPainter（Widget 体系）
- Graphics View Framework（QGraphicsView / QGraphicsScene）
- Qt Quick / Scene Graph（QML + C++ 的 QSG）
---

## 1. QPainter（基于 Widget 的经典绘图）

### 1.1 核心特点

- 典型使用环境：QWidget、QImage、QPixmap 等，重载 `paintEvent()`。
- API 风格：命令式 2D 绘图（线、矩形、路径、图像、文本等）。
- 渲染后端：  
  - 软件渲染（CPU）为主：QImage / QWidget 默认
  - 也能通过 `QOpenGLWidget` + QPainter 进行 GPU 辅助加速（内部仍是“画家模型”）。

### 1.2 适用场景

- 传统桌面应用：表单、对话框、工具型软件。
- 自定义控件：例如绘制一个特定风格的按钮、进度条、图表等。
- 打印 / 导出位图、PDF：QPainter 对打印和 PDF 后端支持很好。
- 动画不多、刷新频率不高的 UI。

### 1.3 优势

- API 简单、学习成本低。
- 文档和示例丰富；大量历史项目都基于此。
- 与 QWidget 生态紧密结合（layout、事件系统、现成控件）。

### 1.4 劣势 / 限制

- 默认是 CPU 渲染，复杂场景 / 高频动画性能有限。
- 对大规模场景、图元管理不方便（没有场景图）。
- 高 DPI / 平滑缩放、旋转、大量 alpha 混合时成本明显。

### 1.5 实践示例（最常见模板）

```cpp
class MyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyWidget(QWidget *parent = nullptr)
        : QWidget(parent)
    {}

protected:
    void paintEvent(QPaintEvent *event) override
    {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        p.fillRect(rect(), Qt::white);

        p.setPen(QPen(Qt::blue, 2));
        p.drawEllipse(QPoint(width()/2, height()/2), 50, 50);
    }
};
```

---

## 2. Graphics View Framework（QGraphicsView / QGraphicsScene）

### 2.1 核心特点

- 架构：场景（QGraphicsScene）+ 视图（QGraphicsView）+ 图元（QGraphicsItem）。
- 支持“场景图”结构：每个 Item 有自己的几何、变换、事件和绘制逻辑。
- 支持视口技术（`QGraphicsView::setViewport()`）在内部用 OpenGL 作为后端。

### 2.2 适用场景

- 大量可交互图元：如流程图、UML 图、地图、CAD 轻量版编辑、连线编辑器。
- 支持视口变换：平移、缩放、旋转整张场景。
- 复杂拾取与碰撞：内置碰撞检测、选中、多选、拖拽。

### 2.3 优势

- 比直接用 QWidget 更适合“大场景 + 多对象”的程序。
- 每个 QGraphicsItem 负责自己的绘制和事件，结构清晰。
- 有一定的视图优化（可见区域裁剪、缓存等）。
- 能切换到 OpenGL 视口提升渲染效率：

```cpp
QGraphicsView *view = new QGraphicsView(scene);
QOpenGLWidget *glWidget = new QOpenGLWidget;
view->setViewport(glWidget);  // 用 OpenGL 作为绘制后台
```

### 2.4 劣势 / 限制

- 属于“老一代”框架：Qt 6 中已经不推荐用于新 UI 项目（但仍然可用）。
- 动画、复杂 shader 特效、GPU 能力利用程度不如 Qt Quick/Scene Graph。
- 对移动端、高刷动画 UI 不是最优选择。

### 2.5 实践示例（基本模式）

```cpp
QGraphicsScene *scene = new QGraphicsScene;

QGraphicsEllipseItem *circle = scene->addEllipse(-50, -50, 100, 100,
                                                 QPen(Qt::blue, 2),
                                                 QBrush(Qt::yellow));
circle->setFlag(QGraphicsItem::ItemIsMovable);
circle->setFlag(QGraphicsItem::ItemIsSelectable);

QGraphicsView *view = new QGraphicsView(scene);
view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
view->show();
```

---

## 3. Qt Quick / Scene Graph（QML + QSG）

### 3.1 核心特点

- UI 层：QML + JavaScript，声明式描述界面和动画。
- 渲染内核：Qt Scene Graph（QSG），使用 GPU 做基于节点的渲染。
- 每个 QQuickItem 对应一个或多个 Scene Graph 节点（QSGNode 派生）。

### 3.2 适用场景

- 现代 UI、高度动态、动画丰富的面：桌面、嵌入式、车机、大屏。
- 需要 GPU 加速：复杂渐变、模糊、粒子特效、大量透明层。
- 容易适配多分辨率、多 DPI、Touch 交互。

### 3.3 优势

- 渲染性能最强：专门为 GPU 优化的场景图架构。
- 声明式 UI + 状态/动画系统，非常适合快速构建复杂交互。
- 自带大量视觉组件（Controls）、动画元素、ShaderEffect。
- 与 C++ 结合灵活：可以把业务逻辑放在 C++，UI 放 QML。

### 3.4 劣势 / 限制

- 学习曲线：需要掌握 QML、Scene Graph 概念；与传统 Widget 思维有差异。
- 对“打印”、“PDF 导出”、“传统桌面控件风格”支持不如 QWidget 流畅。
- 纯 QML 工程维护需要一定工程经验，否则容易结构混乱。

### 3.5 实践开发常见模式

#### 3.5.1 纯 QML UI + C++ 后端

QML 负责界面，C++ 提供数据与逻辑，使用 `QQmlApplicationEngine` 加载 QML：

```cpp
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
```

`main.qml` 简化示例：

```qml
import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    Rectangle {
        anchors.centerIn: parent
        width: 100; height: 100
        color: "tomato"
        radius: 10

        MouseArea {
            anchors.fill: parent
            onClicked: parent.color = parent.color === "tomato" ? "steelblue" : "tomato"
        }
    }
}
```

#### 3.5.2 自定义 QQuickItem + Scene Graph

当内置 QML 元素无法满足需求时，用 C++ 直接操作 Scene Graph：

```cpp
class MyItem : public QQuickItem
{
    Q_OBJECT
public:
    MyItem() { setFlag(ItemHasContents, true); }

protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override
    {
        QSGGeometryNode *node = static_cast<QSGGeometryNode *>(oldNode);
        if (!node) {
            node = new QSGGeometryNode;
            auto geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 4);
            geometry->setDrawingMode(GL_TRIANGLE_STRIP);
            node->setGeometry(geometry);
            node->setFlag(QSGNode::OwnsGeometry);
            node->setMaterial(new QSGFlatColorMaterial);
            node->setFlag(QSGNode::OwnsMaterial);
        }

        // ... 更新 geometry 和颜色 ...

        return node;
    }
};
```

这种方式性能极高，适合自己实现高效的 2D/3D 特效、图表、可视化。

---

## 4. 三者对比与选型建议

### 4.1 从应用类型出发

1. **传统桌面工具/企业管理软件**
   - 主流：`QWidget + QPainter`
   - 如果有大量图形编辑/连线关系图：`QGraphicsView` 更合适。
   - 一般不必上 Qt Quick，除非对动态 UI、动画有明显需求。

2. **复杂交互场景图、编辑器、可视化工具**
   - 几百上千个可交互对象，需要缩放/平移：
     - 纯桌面：`QGraphicsView`
     - 新项目且对未来 GPU 优化有要求：可考虑 `Qt Quick + 自定义 QQuickItem`（但开发复杂度更高）。

3. **现代 UI/车机/嵌入式界面/大屏展示**
   - 优先：`Qt Quick / Scene Graph`
   - 可搭配 `ShaderEffect`、粒子、动画，充分利用 GPU。

4. **高性能图形特效 / 可视化（图表、波形、地图）**
   - 对普通桌面：可以 `QOpenGLWidget + 自己写 OpenGL` 或 `QtQuick + QSG`。
   - 若 UI 已经是 QML，优先用 QSG / QQuickItem；  
     若已有大体量 Widget 工程，可能用 `QOpenGLWidget + QPainter/OpenGL` 过渡。

### 4.2 实际项目常见混合方案

- **Widget + Qt Quick 嵌入**
  - 用 QQuickWidget 或 `QQuickView -> createWindowContainer()` 把 QML 界面嵌入 Widget 程序。
  - 常见于：老项目逐步引入新 UI 模块。

- **Qt Quick UI + QWidget 工具窗口**
  - 用 `QQuickWindow` 作为主 UI，旁边有一些传统工具窗口用 QWidget（如调试工具、开发者选项）。

- **Qt Quick + C++ Scene Graph 扩展**
  - UI 用 QML，核心绘制（大规模可视化）用 C++ 写 QSG 节点以获得最高性能。

---

## 5. 实践中的几个经验点

1. **不要过度优化一开始的选择**  
   对典型桌面业务应用，`QWidget + QPainter` 仍然足够好，且开发效率高。

2. **动画 + 高分辨率 + Touch = Qt Quick 更舒适**  
   同样的 UI，在 Qt Quick 里做动画和视图过渡往往简单很多。

3. **QGraphicsView 在新项目中的定位**  
   如果需求是“图形编辑器/关系图/图形场景”，它仍是性价比最高的选择之一，API 成熟；  
   但如果长期规划是向现代 UI 迁移，建议调研 Qt Quick + Scene Graph 的方案。

4. **性能调试**  
   - QPainter/Widget：关注 paintEvent 次数、过度重绘、透明度、抗锯齿。
   - QGraphicsView：开启/关闭缓存，不要使用过多复杂的 QGraphicsEffect；适当用 OpenGL 视口。
   - Qt Quick：使用 QML Profiler、`QSG_VISUALIZE` 系列环境变量，避免过多层级、频繁绑定评估。
