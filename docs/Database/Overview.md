# 数据库概述

数据库可以按照不同的标准进行分类，可按照数据模型、服务类型、访问方式等分类，同一数据库可归属多个分类。

依照数据模型分类:

- 关系型数据库(SQL): 数据以二维表格的形式存储，由多个表格组成，表格之间通过关系连接，如 MySQL 和Oracle
- 非关系型数据库(NoSQL): 数据以非表格形式存储，包括文档型、键值型、图数据库等，如 MongoDB 和 Redis

依照服务类型分类:

- OLTP(在线事务处理): 以处理大量小型事务为主，要求高并发和低延迟，例如 MySQL
- OLAP(在线分析处理): 以分析复杂查询为主，要求高吞吐量和低延迟，例如 ClickHouse
- HTAP(混合事务/分析处理): 兼顾 OLTP 和 OLAP 的功能，例如 PostgreSQL

依照访问方式分类:

- 嵌入式数据库: 直接嵌入到用户应用程序中，例如 SQLite
- 客户端-服务端数据库: 通过网络链接客户端和数据库服务器，例如 MySQL，PostgreSQL

## SQL

SQL代表“结构化查询语言”，是自20世纪70年代以来广泛用于管理[关系数据库管理系统（RDBMS）中数据的编程语言](https://www.ibm.com/cloud/learn/relational-databases)。在早期，当存储成本很高时，SQL数据库专注于减少数据重复。



## NoSQL

[NoSQL](https://www.ibm.com/cloud/learn/nosql-databases)是一种非关系数据库，这意味着它允许与SQL数据库不同的结构（不是行和列），并且更灵活地使用最适合数据的格式。“NoSQL”一词直到21世纪初才被创造出来。这并不意味着系统不使用SQL，因为NoSQL数据库有时确实支持一些SQL命令。更准确地说，“NoSQL”有时被定义为“不仅仅是SQL”。

## 参考链接

[1] [SQL vs NoSQL Databases: What's the Difference?](https://www.ibm.com/cloud/blog/sql-vs-nosql)

