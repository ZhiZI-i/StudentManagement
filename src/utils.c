#include "utils.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <ctype.h>

// 文件操作函数
int fileExists(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

int createDirectory(const char* path) {
#ifdef _WIN32
    return _mkdir(path);
#else
    return mkdir(path, 0777);
#endif
}

int backupFile(const char* filename) {
    char backupName[MAX_PATH_LENGTH];
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    
    snprintf(backupName, sizeof(backupName), "%s.%04d%02d%02d_%02d%02d%02d.bak",
             filename, t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
             t->tm_hour, t->tm_min, t->tm_sec);
    
    FILE* source = fopen(filename, "rb");
    if (!source) {
        return ERROR_FILE_OPEN;
    }
    
    FILE* backup = fopen(backupName, "wb");
    if (!backup) {
        fclose(source);
        return ERROR_FILE_WRITE;
    }
    
    char buffer[4096];
    size_t bytes;
    
    while ((bytes = fread(buffer, 1, sizeof(buffer), source)) > 0) {
        if (fwrite(buffer, 1, bytes, backup) != bytes) {
            fclose(source);
            fclose(backup);
            return ERROR_FILE_WRITE;
        }
    }
    
    fclose(source);
    fclose(backup);
    return SUCCESS;
}

// 字符串操作函数
char* trim(char* str) {
    char* end;
    
    // 去除头部空白字符
    while (isspace((unsigned char)*str)) str++;
    
    if (*str == 0) return str;
    
    // 去除尾部空白字符
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    
    end[1] = '\0';
    return str;
}

int startsWith(const char* str, const char* prefix) {
    return strncmp(str, prefix, strlen(prefix)) == 0;
}

int contains(const char* str, const char* substr) {
    return strstr(str, substr) != NULL;
}

// 日期时间操作函数
char* getCurrentDateTime(char* buffer) {
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    
    sprintf(buffer, "%04d-%02d-%02d %02d:%02d:%02d",
            t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
            t->tm_hour, t->tm_min, t->tm_sec);
    
    return buffer;
}

int isValidDate(const char* date) {
    int year, month, day;
    if (sscanf(date, "%d-%d-%d", &year, &month, &day) != 3) {
        return 0;
    }
    
    if (year < 1900 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    
    // 检查月份对应的天数
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
        daysInMonth[1] = 29;  // 闰年2月有29天
    }
    
    return day <= daysInMonth[month - 1];
}

// 数据验证函数
int isValidPhoneNumber(const char* phone) {
    // 简单的手机号码验证：11位数字
    if (strlen(phone) != 11) return 0;
    
    for (int i = 0; phone[i]; i++) {
        if (!isdigit(phone[i])) return 0;
    }
    
    return 1;
}

int isValidStudentId(const char* student_id) {
    // 学号验证：8-12位数字
    int len = strlen(student_id);
    if (len < 8 || len > 12) return 0;
    
    for (int i = 0; student_id[i]; i++) {
        if (!isdigit(student_id[i])) return 0;
    }
    
    return 1;
}

int isValidDormitoryId(const char* dormitory_id) {
    // 宿舍号验证：3-8位字符，可以包含数字和字母
    int len = strlen(dormitory_id);
    if (len < 3 || len > 8) return 0;
    
    for (int i = 0; dormitory_id[i]; i++) {
        if (!isalnum(dormitory_id[i])) return 0;
    }
    
    return 1;
}

// 加密相关函数
char* hashPassword(const char* password) {
    // 这里使用一个简单的哈希算法，实际应用中应该使用更安全的算法
    static char hash[50];
    unsigned long h = 5381;
    int c;
    
    while ((c = *password++)) {
        h = ((h << 5) + h) + c;
    }
    
    sprintf(hash, "%lu", h);
    return hash;
}

int verifyPassword(const char* password, const char* hash) {
    char* computed_hash = hashPassword(password);
    return strcmp(computed_hash, hash) == 0;
}

// 日志函数
void logError(const char* message) {
    char datetime[20];
    getCurrentDateTime(datetime);
    fprintf(stderr, "[ERROR] [%s] %s\n", datetime, message);
}

void logInfo(const char* message) {
    char datetime[20];
    getCurrentDateTime(datetime);
    printf("[INFO] [%s] %s\n", datetime, message);
}

void logDebug(const char* message) {
#ifdef DEBUG
    char datetime[20];
    getCurrentDateTime(datetime);
    printf("[DEBUG] [%s] %s\n", datetime, message);
#endif
}

// 分页显示函数
void showPagination(int currentPage, int totalPages) {
    printf("\n=== 第 %d/%d 页 ===\n", currentPage + 1, totalPages);
    printf("操作：[P]上一页 [N]下一页 [G]跳转 [Q]返回\n");
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int getPageCount(int totalItems) {
    return (totalItems + PAGE_SIZE - 1) / PAGE_SIZE;
} 