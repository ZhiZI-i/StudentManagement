CC = gcc
CFLAGS = -Wall -g
LDFLAGS = 
OBJDIR = obj
SRCDIR = src
BINDIR = bin

# 源文件和目标文件
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TARGET = $(BINDIR)/dormitory_system

# 创建必要的目录
$(shell mkdir -p $(OBJDIR) $(BINDIR))

# 默认目标
all: $(TARGET)

# 链接目标文件
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# 编译源文件
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# 清理编译产物
clean:
	rm -f $(OBJDIR)/*.o $(TARGET)

# 重新编译
rebuild: clean all

.PHONY: all clean rebuild 