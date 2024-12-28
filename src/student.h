#ifndef STUDENT_H
#define STUDENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 学生信息结构体
typedef struct {
    char dormitory_id[10];    // 宿舍号
    int bed_count;            // 床位数
    char student_name[50];    // 学生姓名
    char student_id[20];      // 学号
    char phone[20];           // 电话
    char room_leader_name[50];// 卧室长姓名
    char room_leader_phone[20];// 卧室长电话
} Student;

// 函数声明
// 添加学生信息
int addStudent(Student* student);
// 更新学生信息
int updateStudent(const char* student_id, Student* newInfo);
// 删除学生信息
int deleteStudent(const char* student_id);
// 按宿舍号查询学生（精确查找）
Student* findStudentByDormitory(const char* dormitory_id);
// 按宿舍号查询学生（模糊查找）
Student** findStudentsByDormitoryPattern(const char* pattern, int* count);
// 按学号查询学生
Student* findStudentById(const char* student_id);
// 保存学生数据
int saveStudentData();
// 加载学生数据
int loadStudentData();
// 获取空闲床位信息
int* getAvailableBeds(const char* dormitory_id, int* count);
// 模糊搜索函数
Student** findStudentsByPattern(const char* pattern, const char* field, int* count);

#endif // STUDENT_H 