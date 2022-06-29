# git规范

## 1. git commit 规范

每次提交可以包含页眉(header)、正文(body)和页脚(footer)，其中**页眉是必选项**。每次提交信息不超过100个字符。

```shell
# 包含 Header(必填)， Body 和 Footer
<type>(<scope>): <subject>  # 描述修改类型和内容，末尾不使用句号
<BLANK LINE>
<body> # 详细描述修改内容及开发思路等
<BLANK LINE>
<footer> # Breaking Changes 或 Closed Issues
```

其中 type 类型如下:

**feat**：新功能（feature）

**fix**：修补bug

**docs**：文档

**style**：格式（不影响代码运行的变动）

**refactor**：重构（不是新增功能，也不是修改bug的代码变动）

**test**：新增测试用例或是更新现有测试

**chore**：构建过程或辅助工具的变动, 例如依赖管理

**perf**: 性能, 体验优化

**build**: 主要目的是修改项目构建系统（例如 cmake等）的提交

**ci**: 主要目的是修改项目继续集成流程

**revert**: 回滚某个更早之前的提交

如果为feat和fix，则该commit将肯定出现在Change log之中。其他标识，建议不要放入Change log中。

## 2. git 分支命名规范

|   分支   |    命名     |                             说明                             |
| :------: | :---------: | :----------------------------------------------------------: |
|  主分支  |   master    |                自动建立，用于发布重大版本更新                |
| 开发分支 |     dev     |           日常开发在此分支上进行，功能最新最全分支           |
| 功能分支 |   feature   | 它是为了开发某种特定功能，从Develop分支上面分出来的。开发完成后，要再并入Develop。可以采用feature-*的形式命名。* |
| 发布分支 |   release   | 指发布正式版本之前（即合并到Master分支之前），我们可能需要有一个预发布的版本进行测试。预发布分支是从Develop分支上面分出来的，预发布结束以后，必须合并进Develop和Master分支。它的命名，可以采用release-*的形式* |
| 修复分支 | bug(hotfix) | 软件正式发布以后，难免会出现bug。这时就需要创建一个分支，进行bug修补。修补bug分支是从Master分支上面分出来的。修补结束以后，再合并进Master和Develop分支。它的命名，可以采用hotfix-*的形式。 |