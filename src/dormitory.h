#ifndef DORMITORY_H
#define DORMITORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 宿舍信息结构体
typedef struct {
    char dormitory_id[10];    // 宿舍号
    int total_beds;           // 总床位数
    int occupied_beds;        // 已占用床位数
    char floor[10];           // 楼层
    int room_status;          // 房间状态（0:正常, 1:维修中, 2:停用）
} Dormitory;

// 函数声明
// 添加宿舍信息
int addDormitory(Dormitory* dormitory);
// 更新宿舍信息
int updateDormitory(const char* dormitory_id, Dormitory* newInfo);
// 删除宿舍信息
int deleteDormitory(const char* dormitory_id);
// 查询宿舍信息
Dormitory* getDormitoryInfo(const char* dormitory_id);
// 获取所有空闲宿舍
Dormitory** getAvailableDormitories(int* count);
// 查询特定宿舍的空闲床位数
int getAvailableBedsCount(const char* dormitory_id);
// 统计宿舍信息
void getDormitoryStats(int* total_rooms, int* available_rooms, int* total_beds, int* available_beds);
// 保存宿舍数据
int saveDormitoryData();
// 加载宿舍数据
int loadDormitoryData();

#endif // DORMITORY_H 