#include "dormitory.h"
#include "utils.h"
#include "student.h"

// 宿舍数据
static Dormitory* dormitories = NULL;
static int dormitoryCount = 0;
static int maxDormitories = 500;

// 初始化宿舍数据
static int initDormitoryData() {
    if (dormitories == NULL) {
        dormitories = (Dormitory*)malloc(sizeof(Dormitory) * maxDormitories);
        if (dormitories == NULL) {
            return ERROR_FILE_OPEN;
        }
        dormitoryCount = 0;  // 初始化宿舍数量
        memset(dormitories, 0, sizeof(Dormitory) * maxDormitories);  // 清零内存
    }
    return SUCCESS;
}

// 保存宿舍数据到文件
int saveDormitoryData() {
    if (dormitories == NULL) {
        return ERROR_FILE_OPEN;
    }
    
    FILE* file = fopen(DORMITORIES_FILE, "wb");
    if (file == NULL) {
        return ERROR_FILE_OPEN;
    }
    
    fwrite(&dormitoryCount, sizeof(int), 1, file);
    fwrite(dormitories, sizeof(Dormitory), dormitoryCount, file);
    
    fclose(file);
    return SUCCESS;
}

// 从文件加载宿舍数据
int loadDormitoryData() {
    if (initDormitoryData() != SUCCESS) {
        return ERROR_FILE_OPEN;
    }
    
    FILE* file = fopen(DORMITORIES_FILE, "rb");
    if (file == NULL) {
        // 如果文件不存在，就创建一个空的数据文件
        dormitoryCount = 0;
        return SUCCESS;
    }
    
    size_t read = fread(&dormitoryCount, sizeof(int), 1, file);
    if (read != 1) {
        fclose(file);
        dormitoryCount = 0;
        return SUCCESS;
    }
    
    if (dormitoryCount > maxDormitories) {
        fclose(file);
        dormitoryCount = 0;
        return SUCCESS;
    }
    
    read = fread(dormitories, sizeof(Dormitory), dormitoryCount, file);
    if (read != dormitoryCount) {
        fclose(file);
        dormitoryCount = 0;
        return SUCCESS;
    }
    
    fclose(file);
    return SUCCESS;
}

// 添加宿舍信息
int addDormitory(Dormitory* dormitory) {
    if (initDormitoryData() != SUCCESS) {
        return ERROR_FILE_OPEN;
    }
    
    if (dormitoryCount >= maxDormitories) {
        return ERROR_FILE_WRITE;
    }
    
    // 检查是否已存在
    for (int i = 0; i < dormitoryCount; i++) {
        if (strcmp(dormitories[i].dormitory_id, dormitory->dormitory_id) == 0) {
            return ERROR_ALREADY_EXISTS;
        }
    }
    
    // 安全复制数据
    strncpy(dormitories[dormitoryCount].dormitory_id, dormitory->dormitory_id, sizeof(dormitories[dormitoryCount].dormitory_id) - 1);
    dormitories[dormitoryCount].dormitory_id[sizeof(dormitories[dormitoryCount].dormitory_id) - 1] = '\0';
    
    dormitories[dormitoryCount].total_beds = dormitory->total_beds;
    dormitories[dormitoryCount].occupied_beds = dormitory->occupied_beds;
    
    strncpy(dormitories[dormitoryCount].floor, dormitory->floor, sizeof(dormitories[dormitoryCount].floor) - 1);
    dormitories[dormitoryCount].floor[sizeof(dormitories[dormitoryCount].floor) - 1] = '\0';
    
    dormitories[dormitoryCount].room_status = dormitory->room_status;
    
    dormitoryCount++;
    return saveDormitoryData();
}

// 更新宿舍信息
int updateDormitory(const char* dormitory_id, Dormitory* newInfo) {
    if (dormitories == NULL) return ERROR_NOT_FOUND;
    
    for (int i = 0; i < dormitoryCount; i++) {
        if (strcmp(dormitories[i].dormitory_id, dormitory_id) == 0) {
            // 安全复制数据
            strncpy(dormitories[i].dormitory_id, newInfo->dormitory_id, sizeof(dormitories[i].dormitory_id) - 1);
            dormitories[i].dormitory_id[sizeof(dormitories[i].dormitory_id) - 1] = '\0';
            
            dormitories[i].total_beds = newInfo->total_beds;
            dormitories[i].occupied_beds = newInfo->occupied_beds;
            
            strncpy(dormitories[i].floor, newInfo->floor, sizeof(dormitories[i].floor) - 1);
            dormitories[i].floor[sizeof(dormitories[i].floor) - 1] = '\0';
            
            dormitories[i].room_status = newInfo->room_status;
            
            return saveDormitoryData();
        }
    }
    return ERROR_NOT_FOUND;
}

// 删除宿舍信息
int deleteDormitory(const char* dormitory_id) {
    if (dormitories == NULL) return ERROR_NOT_FOUND;
    
    // 检查是否有学生住在该宿舍
    int count = 0;
    findStudentsByDormitoryPattern(dormitory_id, &count);
    if (count > 0) {
        return ERROR_PERMISSION_DENIED;  // 宿舍中还有学生，不能删除
    }
    
    for (int i = 0; i < dormitoryCount; i++) {
        if (strcmp(dormitories[i].dormitory_id, dormitory_id) == 0) {
            // 移动后面的宿舍数据
            for (int j = i; j < dormitoryCount - 1; j++) {
                dormitories[j] = dormitories[j + 1];
            }
            dormitoryCount--;
            return saveDormitoryData();
        }
    }
    return ERROR_NOT_FOUND;
}

// 查询宿舍信息
Dormitory* getDormitoryInfo(const char* dormitory_id) {
    if (dormitories == NULL) return NULL;
    
    for (int i = 0; i < dormitoryCount; i++) {
        if (strcmp(dormitories[i].dormitory_id, dormitory_id) == 0) {
            return &dormitories[i];
        }
    }
    return NULL;
}

// 获取所有空闲宿舍
Dormitory** getAvailableDormitories(int* count) {
    static Dormitory* results[500];
    *count = 0;
    
    if (dormitories == NULL) return results;
    
    for (int i = 0; i < dormitoryCount; i++) {
        if (dormitories[i].occupied_beds < dormitories[i].total_beds &&
            dormitories[i].room_status == 0) {  // 正常状态
            if (*count < 500) {
                results[*count] = &dormitories[i];
                (*count)++;
            }
        }
    }
    
    return results;
}

// 查询特定宿舍的空闲床位数
int getAvailableBedsCount(const char* dormitory_id) {
    Dormitory* dorm = getDormitoryInfo(dormitory_id);
    if (dorm == NULL || dorm->room_status != 0) {  // 宿舍不存在或不是正常状态
        return -1;
    }
    
    return dorm->total_beds - dorm->occupied_beds;
}

// 统计宿舍信息
void getDormitoryStats(int* total_rooms, int* available_rooms, int* total_beds, int* available_beds) {
    *total_rooms = 0;
    *available_rooms = 0;
    *total_beds = 0;
    *available_beds = 0;
    
    if (dormitories == NULL) return;
    
    for (int i = 0; i < dormitoryCount; i++) {
        if (dormitories[i].room_status == 0) {  // 只统计正常状态的宿舍
            (*total_rooms)++;
            *total_beds += dormitories[i].total_beds;
            *available_beds += (dormitories[i].total_beds - dormitories[i].occupied_beds);
            
            if (dormitories[i].occupied_beds < dormitories[i].total_beds) {
                (*available_rooms)++;
            }
        }
    }
} 