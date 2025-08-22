### 分层设计
```plaintext
Logger 接收日志请求 → 创建 LogEvent（封装信息） → 交给 Layout 格式化 → Appender 输出
```

#### Logger(日志记录器):
  - 触发日志事件。 eg: `logger.info("user logging")`
  - 一个`logger`可以关联多个`Appender`

#### Appender(输出目的地):
接收 Logger 传递的 LogEvent，调用关联的 Layout 格式化内容，最终将格式化后的字符串输出到目标（控制台、文件、网络等）

如果创建了一个Appender实例，那么这个Appender就会一直输出比内部存在的等级高的内容。

  - Console
  - file
    - fileAppender

#### Layout(日志格式器)
负责将 LogEvent 格式化为字符串（如包含时间、级别、线程 ID、消息的固定格式），不涉及输出逻辑

自定义日志的输出格式
  - 纯文本
  - HTML
  - JSON
  - PatternLayout：通过占位符可以定义时间、线程、级别、类名、消息等结构

#### LogFormatter

### 日志级别机制
`UNKNOWN < DEBUG < INFO < WARN < ERROR < FATAL`

仅处理级别≥阈值的日志（如INFO、WARN、ERROR会被处理，DEBUG和TRACE被忽略）