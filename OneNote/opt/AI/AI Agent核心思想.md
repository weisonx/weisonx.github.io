好像是在hacker news看到的评论：

```python
# LLM Agent核心思想
def loop(llm):
    msg = user_input()
    while True:
        output, tool_calls = llm(msg)
        print("Agent: ", output)
        if tool_calls:
            msg = [handle_tool_call(tc) for tc in tool_calls]
        else:
            msg = user_input()
```
