# 理论

## “窗口（QMainWindow/QWidget）”和“对话框（QDialog）”既有联系又有明显区别

- 本质：窗口和对话框都继承自 QWidget，都是“窗口”。
- 区别在于：
  - 用途：窗口用于主界面或长期界面；对话框用于短期交互。
  - 行为：对话框原生支持模态、返回值、接受/拒绝机制。
  - 结构：`QMainWindow` 有菜单栏、工具栏等专用区域；对话框通常比较简单。
- 联系：对话框也是窗口的一种，只是为特定交互场景做了专门封装。

## 布局是自动调整部件大小的方法

QWidget本身可能没有设置任何布局，或者它的布局没有正确配置。如果QWidget内部没有布局，那么它不会自动调整其子部件的大小。因此，QListView可能不会被拉伸，导致它无法填满整个QWidget的空间。


## 何谓元对象

- 在Qt中，每个QObject派生类都有一个对应的元对象，元对象是在编译时由moc（元对象编译器）生成的。
- 元对象包含了类的属性、方法和信号等信息，它们被用来在运行时进行对象的类型检查、信号与槽的连接等操作。

# 代码

## Qt事件循环
  [一个Qt应用通常至少有一个事件循环](https://blog.csdn.net/mrbone11/article/details/124945582)

## Qt线程收养 `isAdopted`

- 当一个 `QThread` 对象由另一个线程创建（而不是由它自身管理的线程创建）时，这个线程对象会被“收养”
- 如果线程被收养，Qt 不会自动清理它，需要程序员手动管理它的析构
- 大多数情况下，你不需要直接使用 `isAdopted()`，除非你在处理复杂的多线程生命周期管理
- 通常建议在主线程中创建和管理 `QThread` 对象，以避免潜在的问题

## Qt线程和信号槽

- 移动依附线程时，thread对象不能有父对象。
- 槽执行的线程必须有事件循环去处理才能正常执行。--这也是为什么在子线程定义QTimer，不能正常执行的原因
- 异步执行时信号的参数类型必须在元系统中已知。

## Qt与dll
当qt UI程序以dll发布时，其情况有些复杂，涉及到dll加载方式以及QApplication初始化以及事件循环问题。

## 应用程序添加图标

如Windows
- 创建rc文件
- 配置cmake

## Qt原生组件样式设置

- 有些组件内部的样式通过一般选择器设置不了。
- 可以通过打印类结构获取对象名称，然后通过对象名称设置其样式。

## 窗口大小设置类型

初始值（resize）、固定值（fixed）、最小值、最大值

## Windows Hook和Qt事件过滤

- 两者都是用于处理事件的机制，但钩子（Hook）更偏向于系统级的全局事件处理，而Qt的事件过滤机制则更专注于应用程序内部的事件处理。
- 如果你在开发Qt应用程序，通常会优先使用Qt的事件过滤机制，因为它更简单且性能影响较小。

## 分辨率DPI适配

``` C++
QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
```

## setPixelSize和setPointSize

- pt是point，是绝对长度
- px是pixel，是相对长度

## QMenu圆角设置，底部存在直角

- https://blog.csdn.net/qq_41673920/article/details/116980362
- https://blog.51cto.com/xiaohaiwa/5379230
- 这样还不行，会有直角透明的投影阴影，得加上：
```
menu->setWindowFlags(menu->windowFlags() | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
menu->setAttribute(Qt::WA_TranslucentBackground);
```

## 信号槽没执行的原因

- `connect` 没成功
- 对象已经被销毁、 thread affinity 错乱
- 事件循环没跑起来
- 连接类型`Qt::ConnectionType`导致的行为不符合预期
- 信号根本没发出、发错对象
- 信号定义/声明问题 `Q_OBJECT`
- 传参类型没注册元类型

## 模型视图框架中，Qt视图处于排序状态，持久打开的编辑器数据更新问题

此时代理模型更新数据，委托的编辑器的数据不会实时更新。原因在手册中有写：

请注意，当dynamicSortFilter为true时 不应该通过代理模型更新源模型。例如，如果您在 QComboBox上设置代理模型，则使用更新模型的函数（例如addItem()）将无法按预期工作。另一种方法是将dynamicSortFilter设置为false，并在向QComboBox添加项后调用sort()。
默认值为true。

## Qt界面程序运行会带命令窗口.

add_executable加一个WIN32：

`add_executable(WeixTodo WIN32 ${SOURCES})`

## QWdiget作为容器时，`setStyleSheet`背景色等不起作用

改为QFrame。

# Qt Designer

## 布局放置

要放入一个布局到布局，先要拆分该容器的布局，放进去再恢复布局。
