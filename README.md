#  基于C语言实现的学生宿舍管理系统

自己练手做了一个基于C语言开发的学生宿舍管理系统，应该能满足大部分学校的课业要求了把。

## 功能包含
- 用户管理
  - 管理员和普通用户两种角色
  - 用户登录/注销
  - 密码修改
  - 用户创建/删除（管理员）

- 学生信息管理
  - 学生信息录入
  - 学生信息修改
  - 学生信息删除
  - 多条件查询（学号、姓名、宿舍号、电话）
  - 分页显示查询结果

- 宿舍管理
  - 宿舍信息添加
  - 宿舍信息修改
  - 宿舍信息删除
  - 宿舍查询
  - 统计信息查看

## 测试系统环境
- Ubutun 22.04下运行

## 编译和运行
1. 克隆仓库：
```bash
git clone 
```

2. 编译：
```bash
make
```

3. 运行：
```bash
# 方式1：使用启动脚本
./start.sh         # 正常启动
./start.sh admin   # 管理员账号登录
./start.sh help    # 显示帮助信息

# 方式2：直接运行
./bin/dormitory_system                  # 正常启动
./bin/dormitory_system -u admin -p 密码  # 带参数启动
./bin/dormitory_system -h              # 显示帮助
```

## 默认账号
- 管理员账号：
  - 用户名：admin
  - 密码：admin123

## 目录结构

```
.
├── bin/                # 可执行文件目录
├── data/              # 数据文件目录
├── docs/              # 文档目录
├── obj/               # 编译中间文件
├── src/               # 源代码
│   ├── main.c         # 主程序
│   ├── student.c      # 学生管理
│   ├── dormitory.c    # 宿舍管理
│   ├── user.c         # 用户管理
│   └── utils.c        # 工具函数
├── Makefile           # 编译配置
├── start.sh           # 启动脚本
└── README.md          # 说明文档
```

## 数据存储

系统使用文件存储数据：
- `data/users.dat`: 用户数据
- `data/students.dat`: 学生数据
- `data/dormitories.dat`: 宿舍数据

## 开发者

- [ZhiZi]

## 许可证

MIT License 
