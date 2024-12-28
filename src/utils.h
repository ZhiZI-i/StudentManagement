#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// 常量定义
#define MAX_PATH_LENGTH 256
#define DATA_DIR "./data/"
#define USERS_FILE "./data/users.dat"
#define STUDENTS_FILE "./data/students.dat"
#define DORMITORIES_FILE "./data/dormitories.dat"

// 错误码定义
#define SUCCESS 0
#define ERROR_FILE_OPEN -1
#define ERROR_FILE_WRITE -2
#define ERROR_FILE_READ -3
#define ERROR_INVALID_PARAM -4
#define ERROR_NOT_FOUND -5
#define ERROR_ALREADY_EXISTS -6
#define ERROR_PERMISSION_DENIED -7

// 分页显示相关常量
#define PAGE_SIZE 10

// 工具函数声明
// 文件操作
int fileExists(const char* filename);
int createDirectory(const char* path);
int backupFile(const char* filename);

// 字符串操作
char* trim(char* str);
int startsWith(const char* str, const char* prefix);
int contains(const char* str, const char* substr);

// 日期时间操作
char* getCurrentDateTime(char* buffer);
int isValidDate(const char* date);

// 数据验证
int isValidPhoneNumber(const char* phone);
int isValidStudentId(const char* student_id);
int isValidDormitoryId(const char* dormitory_id);

// 加密相关
char* hashPassword(const char* password);
int verifyPassword(const char* password, const char* hash);

// 日志函数
void logError(const char* message);
void logInfo(const char* message);
void logDebug(const char* message);

// 分页显示函数
void showPagination(int currentPage, int totalPages);
void clearScreen();
int getPageCount(int totalItems);

#endif // UTILS_H 