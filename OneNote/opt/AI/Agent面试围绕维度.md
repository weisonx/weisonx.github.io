agent面试通常围绕这几个维度：

1）RAG架构和优化（向量检索、重排序、上下文压缩）；

2）Agent设计模式（ReAct、Plan-and-Solve、Multi-Agent协作）；

3）幻觉检测与缓解方案；

4）工具调用和Function Calling的实际落地经验；

5）LangChain/LlamaIndex等框架的选型权衡。

---

## 1）RAG 架构和优化（向量检索、重排序、上下文压缩）

### 1.1 基本 RAG（Retrieval-Augmented Generation，检索增强生成） 架构

- 离线阶段：
  - 文档切分策略：按段落 / 标题 / 语义切分，控制 chunk_size & overlap。
  - 向量化：选用的 embedding 模型（如 bge-m3, text-embedding-3-large 等）和理由。
  - 索引构建：Faiss / Milvus / Elastic / pgvector 等。

- 在线查询阶段：
  - Query 重写 / 扩展（Query Rewriting / Hypothetical Questions）。
  - 检索：向量检索 + BM25 混合（Hybrid Search）。
  - 重排序：基于 cross-encoder / LLM rerank。
  - 上下文组装：Prompt 模板、引用格式等。

### 1.2 检索优化点

- 向量检索：
  - 选择向量维度、embedding 模型，结合语种（中文/多语）与成本。
  - 相似度度量：cosine / dot / L2，向量库支持情况。

- 重排序（Rerank）：
  - 使用 cross-encoder（如 bge-reranker）或 LLM 进行 Rerank。
  - 策略：初筛 k=50 用向量检索，rerank top-10，最终选 top-5 进上下文。

- 上下文压缩（Context Compression）：
  - 抽取式压缩：用模型从文档中抽取与 query 最相关的句子。
  - 摘要式压缩：对 chunk 做 summary 再拼接。
  - Query-aware compression：prompt 里加「只保留和问题强相关的信息」。

**可以举一个实际例子：**
- 原始检索 50 个片段 → rerank → 10 个 → 压缩到 2-3 个精炼段落。
- 说清楚：这样做如何降低 token 成本 & 提升回答相关性。

---

## 2）Agent 设计模式（ReAct、Plan-and-Solve、Multi-Agent 协作）

### 2.1 ReAct

- 核心：在一次对话里交替产生「推理（Thought）」和「行动（Action）」，LLM 根据中间结果不断修正。
- 典型流程：Thought → Action(tool) → Observation → Thought → … → Answer。
- 优点：行为透明，有链式推理；适合工具调用较多、不确定性高的场景。
- 缺点：一步一步走，可能慢；需要设计好工具描述和观察信息的格式。

### 2.2 Plan-and-Solve / Plan-and-Execute

- Plan 阶段：先让模型生成整体计划/子任务列表。
- Execute 阶段：逐个执行子任务，可以由同一个 Agent 或不同 Agent 完成。
- 优点：适合复杂、多步骤任务（如：数据分析、报表生成、自动化运营）。
- 可以提到：有的实现会把 plan 固定下来，有的会动态修正计划。

### 2.3 Multi-Agent 协作

- 常见模式：
  - 角色分工：如「产品经理 Agent + 开发 Agent + 测试 Agent」。
  - 管理者/仲裁者：一个 Supervisor/Router 负责任务分配、结果合并。
- 协作方式：
  - 并行：不同子任务同时跑。
  - 串行：一个 Agent 输出作为下一个的输入。
- 重点要说明：如何避免「瞎聊循环」和「无效协作」：
  - 严格约束每个 Agent 的职责和输出格式。
  - 设定迭代轮数上限和终止条件。

---

## 3）幻觉检测与缓解方案

### 3.1 幻觉来源

- 模型本身的「自信瞎编」：在没信息时依然生成看似合理的内容。
- 检索召回不足：RAG 内容没找到答案，模型只好编。
- Prompt 设计不当：没要求「不知道就说不知道」。

### 3.2 常见缓解方案

- 检索层：
  - 提高召回质量：优化切分、embedding、多路检索（向量 + 关键词）。
  - 增加检索失败检测：如果 top-k 文档都低于相似度阈值，则认为「检索不到」。

- 生成层：
  - 明确指示：在 prompt 里要求「如无依据，必须说明不知道」。
  - 让模型输出「引用」：让模型在回答时标注来源段落，方便检查。
  - 指定输出必须与提供上下文一致，不得引入额外事实。

- 后验验证（post-hoc checking）：
  - 使用第二个模型做 Fact-check：「这个回答是否被上下文充分支持？」。
  - 自一致性（self-consistency）：对同一问题多次生成，看结果是否一致。
  - 针对结构化任务（SQL、代码）：执行 / 编译 / 单元测试来验证。

- 实际经验可以说：
  - 给出一个「限制模型幻想」的系统 prompt 范例。
  - 如何在产品中标注「置信度」或「是否基于知识库」。

---

## 4）工具调用和 Function Calling 的实际落地经验

### 4.1 工具/Function 的设计

- 典型工具类型：
  - 数据库查询工具（SQL / GraphQL）。
  - 内部 HTTP API 调用。
  - 第三方服务：搜索、天气、支付、工单系统等。
- 函数 schema 设计要点：
  - 明确参数类型和取值范围（enum / pattern 等）。
  - 少而精的字段，避免模型填不全。
  - 加入描述信息（description），引导模型正确使用。

### 4.2 调用策略和错误处理

- 单工具 vs 多工具：
  - 如何引导模型在多工具间选择（Tool Routing）。
  - 限制「一次对话中最多调用几次」，防止死循环。

- 错误处理：
  - 工具执行失败时，将错误信息反馈给模型，让其重新规划（如：参数不合法、超时）。
  - 对数据库/接口返回为空/异常时，模型如何解释给用户。

- 安全与权限：
  - 帐号/权限控制：比如某些工具只有在带 token 或特定用户时才能调用。
  - 参数白名单/黑名单：防止模型构造危险请求（比如删库、改价）。

---

## 5）LangChain / LlamaIndex 等框架的选型权衡

### 5.1 LangChain

- 优点：
  - 工具链 / Agent / Chain 模型成熟，生态丰富。
  - 支持多种 LLM、向量库、存储后端，拓展方便。
  - 社区活跃，案例多，上手快。
- 缺点：
  - 抽象层级多，调试复杂，性能/开销不一定最优。
  - 对于简单项目可能显得「过度工程」。

### 5.2 LlamaIndex

- 优点：
  - 专注在「数据接入 + 索引 + RAG」这块。
  - 提供多种 Index 类型：VectorStoreIndex、TreeIndex、KeywordTableIndex 等。
  - 对于多源数据接入（本地文件、数据库、Notion、S3 等）比较方便。
- 缺点：
  - Agent 能力不如 LangChain 「大而全」，但在不断补齐。

### 5.3 选型考虑

- 场景简单（单模型 + 简单 RAG）：
  - 可以直接用官方 SDK（如 OpenAI SDK）+ 自己写少量 glue code；
  - 或用 LlamaIndex 做 RAG，保持清晰结构。

- 场景复杂（多工具、多 Agent、工作流）：
  - LangChain 或者自研一个轻量级 Agent 框架；
  - 重点关注可 observability（logs/trace）、易调试。

- 公司维度：
  - 考虑团队技术栈（Python / JS）、维护成本、社区活跃度。
  - 是否需要「可控」：一些公司宁愿基于 pydantic + 自己的 orchestrator 封装一层，而不完全依赖框架。
