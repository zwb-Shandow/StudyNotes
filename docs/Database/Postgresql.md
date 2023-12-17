# Postgresql 教程

1. 登录

```shell
psql -h 192.168.0.29 -p 5432 -U postgre -d <you db>
```

2. 查询数据库

```sql
\l
```

3. 枚举类型定义

```sql
-- 创建枚举
CREATE TYPE IF NOT EXISTS Device_type AS ENUM ('ftu', 'vftu');
-- 删除枚举
DROP TYPE IF EXISTS Device_type;
```

查询枚举

```
\dT+
```

4. 查看表

```sql
-- 查看某个schema的表
\dt server.*
\d table_name
```

5. 创建数据库

```sql
```

