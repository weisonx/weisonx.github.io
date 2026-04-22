## 一、 核心架构：MVD 是什么？
Qt 的 Model-View-Delegate 是 MVC 模式的变体：

* Model（模型）：数据的守门员。不存储 UI 逻辑，只负责提供数据映射。
* View（视图）：数据的展示窗口。不直接持有数据，只负责布局。
* Delegate（委托）：数据的翻译官。负责单元格的自定义渲染和交互编辑器（如下拉框）。
* 核心优势：实现数据与表现的完全解耦，支持一套数据源对应多个视图（如同时用表格和饼图显示同一组数据）。

## 二、 索引（QModelIndex）：通信的桥梁

* 本质：视图定位数据的“地址凭证”，包含 row, column, internalPointer。
* 生存规则（必考）：即用即弃。绝对不能长期存储 QModelIndex，因为模型结构改变（增删行）后，索引会失效。如需长期持有，请使用 QPersistentModelIndex。
* 映射机制：在自定义模型中，索引是由模型根据视图的需求动态生成的。

## 三、 代理模型（Proxy Model）：三明治架构
当需要在不修改原始数据的情况下进行排序、过滤时使用。

* 索引分裂：存在“源索引”与“代理索引”两套坐标系。
* 转换关键：必须使用 mapToSource() 和 mapFromSource() 进行翻译。切记：View 拿到的索引不能直接传给 Source Model。

## 四、 性能分水岭：QStandardItemModel vs 自定义 Model

* QStandardItemModel（存储式）：
* 场景：小数据量、快速原型、复杂树结构。
   * 弊端：每个格子都是一个 C++ 对象，百万级数据会导致内存崩溃。
* 自定义 QAbstractItemModel（映射式）：
* 场景：大数据量、高性能需求。
   * 原理：模型不存数据，只持有一个指向原始容器（如 std::vector）的指针。在 data() 函数中通过行号实时返回数据。

## 五、 委托（Delegate）：何时出手？

* 外观定制：想在单元格画进度条、图标、五角星。
* 编辑器定制：双击单元格不想弹输入框，想弹 QComboBox 或 QDateEdit。
* 高效渲染：通过 paint() 函数用画笔绘制，而非创建真实的 Widget 控件，极大地节省资源。

## 六、 大数据量处理：分批加载（Lazy Loading）
当面临亿级数据时，不能一次性读入，需利用 canFetchMore() 和 fetchMore()：

   1. rowCount()：只返回当前已加载的数量。
   2. canFetchMore()：判断是否还有未读数据。
   3. fetchMore()：当视图滚动到底部时，触发此函数，通过 beginInsertRows() 动态追加数据，实现无限滚动效果。

------------------------------

## 一、 模型核心接口 (QAbstractItemModel)
模型是数据源的代理，它通过以下接口告诉视图“我有多少数据”以及“数据是什么”。

* rowCount() / columnCount()
* 作用：定义模型的维度（几行几列）。
   * 注意：在分批加载中，返回的是已加载的行数，而非总数。
* index(row, col, parent)
* 作用：为视图创建并返回 QModelIndex。
   * 重写关键：树状模型中需要通过 createIndex 将原始指针（InternalPointer）塞进去。
* data(index, role)
* 作用：核心读取接口。根据视图请求的 role（如 DisplayRole 文字, DecorationRole 图标）返回对应数据。
* setData(index, value, role)
* 作用：核心写入接口。当用户在视图中完成编辑，数据通过此函数写回模型。
* flags(index)
* 作用：告诉视图该单元格是否可选、可编辑（Qt::ItemIsEditable）。
* beginInsertRows() / endInsertRows()
* 作用：结构变更通知。在修改底层容器前/后必须调用，否则视图索引会错乱导致崩溃。

------------------------------
## 二、 视图关键接口 (QAbstractItemView)
视图负责显示和触发交互，通常直接使用系统提供的 QTableView 等，但需了解其控制接口。

* setModel(model)
* 作用：绑定模型。一旦绑定，视图会立即调用模型的 rowCount 和 data 开始渲染。
* setSelectionModel(selectionModel)
* 作用：设置选择模型，用于多个视图同步选中同一行数据。
* setRootIndex(index)
* 作用：在树状模型或分级结构中，设置当前视图显示的“根”节点。
* dataChanged(topLeft, bottomRight)
* 作用：信号。当模型数据变化时触发，视图收到后会局部重绘。

------------------------------
## 三、 委托核心接口 (QStyledItemDelegate)
委托控制单元格“怎么看”和“怎么改”，是面试中自定义功能的核心。

* paint(painter, option, index)
* 作用：自定义渲染。使用 QPainter 手动绘制单元格内容（如画进度条）。它不创建控件，只画图，性能极高。
* createEditor(parent, option, index)
* 作用：创建编辑器。返回一个真正的 Widget（如 new QComboBox），仅在双击进入编辑状态时触发。
* setEditorData(editor, index)
* 作用：初始化编辑器。将模型里的数据（如 "Active"）设置到下拉框中。
* setModelData(editor, model, index)
* 作用：提交编辑。用户关闭编辑器时，将下拉框选中的新值写回模型。
* updateEditorGeometry(editor, option, index)
* 作用：布局控制。确保编辑器的大小、位置完全覆盖对应的单元格区域。

------------------------------
## 四、 代理模型关键接口 (QAbstractProxyModel)
主要处理排序和过滤逻辑。

* mapToSource(proxyIndex) / mapFromSource(sourceIndex)
* 作用：坐标系转换。在视图索引（排序后）与源模型索引（原始）之间互转。
* filterAcceptsRow(sourceRow, sourceParent) (特定于 QSortFilterProxyModel)
* 作用：行过滤。返回 false 则该行在视图中隐藏。

------------------------------
## 总结图示：数据流向

   1. 显示：View -> Model::data() -> Delegate::paint()
   2. 编辑：View -> Delegate::createEditor() -> 用户操作 -> Delegate::setModelData() -> Model::setData()



