#include "user.h"
#include "utils.h"

// 用户数据
static User* users = NULL;
static int userCount = 0;
static int maxUsers = 100;

// 初始化用户数据
static int initUserData() {
    if (users == NULL) {
        users = (User*)malloc(sizeof(User) * maxUsers);
        if (users == NULL) {
            return ERROR_FILE_OPEN;
        }
        userCount = 0;  // 初始化用户数量
        memset(users, 0, sizeof(User) * maxUsers);  // 清零内存
    }
    return SUCCESS;
}

// 保存用户数据到文件
int saveUserData() {
    if (users == NULL) {
        return ERROR_FILE_OPEN;
    }
    
    FILE* file = fopen(USERS_FILE, "wb");
    if (file == NULL) {
        return ERROR_FILE_OPEN;
    }
    
    fwrite(&userCount, sizeof(int), 1, file);
    fwrite(users, sizeof(User), userCount, file);
    
    fclose(file);
    return SUCCESS;
}

// 从文件加载用户数据
int loadUserData() {
    if (initUserData() != SUCCESS) {
        return ERROR_FILE_OPEN;
    }
    
    FILE* file = fopen(USERS_FILE, "rb");
    if (file == NULL) {
        // 如果文件不存在，就创建一个空的数据文件
        userCount = 0;
        return SUCCESS;
    }
    
    size_t read = fread(&userCount, sizeof(int), 1, file);
    if (read != 1) {
        fclose(file);
        userCount = 0;
        return SUCCESS;
    }
    
    if (userCount > maxUsers) {
        fclose(file);
        userCount = 0;
        return SUCCESS;
    }
    
    read = fread(users, sizeof(User), userCount, file);
    if (read != userCount) {
        fclose(file);
        userCount = 0;
        return SUCCESS;
    }
    
    fclose(file);
    return SUCCESS;
}

// 查找用户
static User* findUser(const char* username) {
    if (users == NULL) return NULL;
    
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0) {
            return &users[i];
        }
    }
    return NULL;
}

// 用户登录
int userLogin(const char* username, const char* password) {
    User* user = findUser(username);
    if (user == NULL) {
        return ERROR_NOT_FOUND;
    }
    
    // 验证密码
    if (strcmp(hashPassword(password), user->password) != 0) {
        return ERROR_INVALID_PARAM;
    }
    
    return user->privilege_level;
}

// 创建新用户
int createUser(const char* username, const char* password, int privilege_level) {
    if (initUserData() != SUCCESS) {
        return ERROR_FILE_OPEN;
    }
    
    if (userCount >= maxUsers) {
        return ERROR_FILE_WRITE;
    }
    
    if (findUser(username) != NULL) {
        return ERROR_ALREADY_EXISTS;
    }
    
    strncpy(users[userCount].username, username, sizeof(users[userCount].username) - 1);
    users[userCount].username[sizeof(users[userCount].username) - 1] = '\0';
    
    char* hashedPassword = hashPassword(password);
    strncpy(users[userCount].password, hashedPassword, sizeof(users[userCount].password) - 1);
    users[userCount].password[sizeof(users[userCount].password) - 1] = '\0';
    
    users[userCount].privilege_level = privilege_level;
    
    userCount++;
    return saveUserData();
}

// 删除用户
int deleteUser(const char* username) {
    if (users == NULL) return ERROR_NOT_FOUND;
    
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0) {
            // 移动后面的用户数据
            for (int j = i; j < userCount - 1; j++) {
                users[j] = users[j + 1];
            }
            userCount--;
            return saveUserData();
        }
    }
    return ERROR_NOT_FOUND;
}

// 修改密码
int changePassword(const char* username, const char* oldPassword, const char* newPassword) {
    User* user = findUser(username);
    if (user == NULL) {
        return ERROR_NOT_FOUND;
    }
    
    // 验证旧密码
    if (strcmp(hashPassword(oldPassword), user->password) != 0) {
        return ERROR_INVALID_PARAM;
    }
    
    // 更新密码
    char* hashedPassword = hashPassword(newPassword);
    strncpy(user->password, hashedPassword, sizeof(user->password) - 1);
    user->password[sizeof(user->password) - 1] = '\0';
    
    return saveUserData();
}

// 重置用户密码
int resetUserPassword(const char* username, const char* newPassword) {
    User* user = findUser(username);
    if (user == NULL) {
        return ERROR_NOT_FOUND;
    }
    
    char* hashedPassword = hashPassword(newPassword);
    strncpy(user->password, hashedPassword, sizeof(user->password) - 1);
    user->password[sizeof(user->password) - 1] = '\0';
    
    return saveUserData();
}

// 检查用户权限
int checkUserPrivilege(const char* username) {
    User* user = findUser(username);
    if (user == NULL) {
        return ERROR_NOT_FOUND;
    }
    
    return user->privilege_level;
} 