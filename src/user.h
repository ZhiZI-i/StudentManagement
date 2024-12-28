#ifndef USER_H
#define USER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 用户类型定义
#define ADMIN_USER 0    // 系统管理员
#define NORMAL_USER 1   // 住宿管理员

// 用户结构体
typedef struct {
    char username[50];        // 用户账号
    char password[50];        // 密码
    int privilege_level;      // 权限级别
} User;

// 函数声明
// 用户认证
int userLogin(const char* username, const char* password);
// 创建新用户（仅管理员）
int createUser(const char* username, const char* password, int privilege_level);
// 删除用户（仅管理员）
int deleteUser(const char* username);
// 修改密码
int changePassword(const char* username, const char* oldPassword, const char* newPassword);
// 重置用户密码（仅管理员）
int resetUserPassword(const char* username, const char* newPassword);
// 检查用户权限
int checkUserPrivilege(const char* username);
// 保存用户数据
int saveUserData();
// 加载用户数据
int loadUserData();

#endif // USER_H 