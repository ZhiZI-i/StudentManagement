#!/bin/bash

# 检查并创建必要的目录
mkdir -p data

# 检查是否需要重新编译
if [ ! -f bin/dormitory_system ] || [ src/main.c -nt bin/dormitory_system ]; then
    echo "正在编译程序..."
    make clean && make
fi

# 检查编译是否成功
if [ ! -f bin/dormitory_system ]; then
    echo "错误：编译失败！"
    exit 1
fi

# 处理命令行参数
if [ "$1" = "admin" ]; then
    # 使用管理员账号登录
    ./bin/dormitory_system -u admin -p admin123
elif [ "$1" = "help" ] || [ "$1" = "-h" ]; then
    # 显示帮助信息
    echo "使用方法："
    echo "  ./start.sh         - 正常启动程序"
    echo "  ./start.sh admin   - 使用管理员账号登录"
    echo "  ./start.sh help    - 显示本帮助信息"
else
    # 正常启动程序
    ./bin/dormitory_system
fi 