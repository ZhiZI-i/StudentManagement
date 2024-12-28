#include "student.h"
#include "utils.h"
#include "dormitory.h"

// 学生数据
static Student* students = NULL;
static int studentCount = 0;
static int maxStudents = 1000;

// 初始化学生数据
static int initStudentData() {
    if (students == NULL) {
        students = (Student*)malloc(sizeof(Student) * maxStudents);
        if (students == NULL) {
            return ERROR_FILE_OPEN;
        }
        studentCount = 0;  // 初始化学生数量
        memset(students, 0, sizeof(Student) * maxStudents);  // 清零内存
    }
    return SUCCESS;
}

// 保存学生数据到文件
int saveStudentData() {
    if (students == NULL) {
        return ERROR_FILE_OPEN;
    }
    
    FILE* file = fopen(STUDENTS_FILE, "wb");
    if (file == NULL) {
        return ERROR_FILE_OPEN;
    }
    
    fwrite(&studentCount, sizeof(int), 1, file);
    fwrite(students, sizeof(Student), studentCount, file);
    
    fclose(file);
    return SUCCESS;
}

// 从文件加载学生数据
int loadStudentData() {
    if (initStudentData() != SUCCESS) {
        return ERROR_FILE_OPEN;
    }
    
    FILE* file = fopen(STUDENTS_FILE, "rb");
    if (file == NULL) {
        // 如果文件不存在，就创建一个空的数据文件
        studentCount = 0;
        return SUCCESS;
    }
    
    size_t read = fread(&studentCount, sizeof(int), 1, file);
    if (read != 1) {
        fclose(file);
        studentCount = 0;
        return SUCCESS;
    }
    
    if (studentCount > maxStudents) {
        fclose(file);
        studentCount = 0;
        return SUCCESS;
    }
    
    read = fread(students, sizeof(Student), studentCount, file);
    if (read != studentCount) {
        fclose(file);
        studentCount = 0;
        return SUCCESS;
    }
    
    fclose(file);
    return SUCCESS;
}

// 添加学生信息
int addStudent(Student* student) {
    if (initStudentData() != SUCCESS) {
        return ERROR_FILE_OPEN;
    }
    
    if (studentCount >= maxStudents) {
        return ERROR_FILE_WRITE;
    }
    
    // 检查宿舍是否存在
    Dormitory* dorm = getDormitoryInfo(student->dormitory_id);
    if (dorm == NULL) {
        return ERROR_NOT_FOUND;  // 宿舍不存在
    }
    
    // 检查宿舍状态
    if (dorm->room_status != 0) {
        return ERROR_PERMISSION_DENIED;  // 宿舍不可用
    }
    
    // 统计该宿舍现有学生数
    int currentStudents = 0;
    for (int i = 0; i < studentCount; i++) {
        if (strcmp(students[i].dormitory_id, student->dormitory_id) == 0) {
            currentStudents++;
        }
    }
    
    // 检查床位是否足够
    if (currentStudents >= dorm->total_beds) {
        return ERROR_PERMISSION_DENIED;  // 床位已满
    }
    
    // 检查是否已存在该学生
    for (int i = 0; i < studentCount; i++) {
        if (strcmp(students[i].student_id, student->student_id) == 0) {
            return ERROR_ALREADY_EXISTS;
        }
    }
    
    // 安全复制数据
    strncpy(students[studentCount].dormitory_id, student->dormitory_id, sizeof(students[studentCount].dormitory_id) - 1);
    students[studentCount].dormitory_id[sizeof(students[studentCount].dormitory_id) - 1] = '\0';
    
    students[studentCount].bed_count = student->bed_count;
    
    strncpy(students[studentCount].student_name, student->student_name, sizeof(students[studentCount].student_name) - 1);
    students[studentCount].student_name[sizeof(students[studentCount].student_name) - 1] = '\0';
    
    strncpy(students[studentCount].student_id, student->student_id, sizeof(students[studentCount].student_id) - 1);
    students[studentCount].student_id[sizeof(students[studentCount].student_id) - 1] = '\0';
    
    strncpy(students[studentCount].phone, student->phone, sizeof(students[studentCount].phone) - 1);
    students[studentCount].phone[sizeof(students[studentCount].phone) - 1] = '\0';
    
    strncpy(students[studentCount].room_leader_name, student->room_leader_name, sizeof(students[studentCount].room_leader_name) - 1);
    students[studentCount].room_leader_name[sizeof(students[studentCount].room_leader_name) - 1] = '\0';
    
    strncpy(students[studentCount].room_leader_phone, student->room_leader_phone, sizeof(students[studentCount].room_leader_phone) - 1);
    students[studentCount].room_leader_phone[sizeof(students[studentCount].room_leader_phone) - 1] = '\0';
    
    studentCount++;
    return saveStudentData();
}

// 更新学生信息
int updateStudent(const char* student_id, Student* newInfo) {
    if (students == NULL) return ERROR_NOT_FOUND;
    
    for (int i = 0; i < studentCount; i++) {
        if (strcmp(students[i].student_id, student_id) == 0) {
            // 安全复制数据
            strncpy(students[i].dormitory_id, newInfo->dormitory_id, sizeof(students[i].dormitory_id) - 1);
            students[i].dormitory_id[sizeof(students[i].dormitory_id) - 1] = '\0';
            
            students[i].bed_count = newInfo->bed_count;
            
            strncpy(students[i].student_name, newInfo->student_name, sizeof(students[i].student_name) - 1);
            students[i].student_name[sizeof(students[i].student_name) - 1] = '\0';
            
            strncpy(students[i].phone, newInfo->phone, sizeof(students[i].phone) - 1);
            students[i].phone[sizeof(students[i].phone) - 1] = '\0';
            
            strncpy(students[i].room_leader_name, newInfo->room_leader_name, sizeof(students[i].room_leader_name) - 1);
            students[i].room_leader_name[sizeof(students[i].room_leader_name) - 1] = '\0';
            
            strncpy(students[i].room_leader_phone, newInfo->room_leader_phone, sizeof(students[i].room_leader_phone) - 1);
            students[i].room_leader_phone[sizeof(students[i].room_leader_phone) - 1] = '\0';
            
            return saveStudentData();
        }
    }
    return ERROR_NOT_FOUND;
}

// 删除学生信息
int deleteStudent(const char* student_id) {
    if (students == NULL) return ERROR_NOT_FOUND;
    
    for (int i = 0; i < studentCount; i++) {
        if (strcmp(students[i].student_id, student_id) == 0) {
            // 移动后面的学生数据
            for (int j = i; j < studentCount - 1; j++) {
                students[j] = students[j + 1];
            }
            studentCount--;
            return saveStudentData();
        }
    }
    return ERROR_NOT_FOUND;
}

// 按宿舍号查询学生（精确查找）
Student* findStudentByDormitory(const char* dormitory_id) {
    if (students == NULL) return NULL;
    
    for (int i = 0; i < studentCount; i++) {
        if (strcmp(students[i].dormitory_id, dormitory_id) == 0) {
            return &students[i];
        }
    }
    return NULL;
}

// 按宿舍号查询学生（模糊查找）
Student** findStudentsByDormitoryPattern(const char* pattern, int* count) {
    static Student* results[1000];
    *count = 0;
    
    if (students == NULL) return results;
    
    for (int i = 0; i < studentCount; i++) {
        if (strstr(students[i].dormitory_id, pattern) != NULL) {
            if (*count < 1000) {
                results[*count] = &students[i];
                (*count)++;
            }
        }
    }
    
    return results;
}

// 按学号查询学生
Student* findStudentById(const char* student_id) {
    if (students == NULL) return NULL;
    
    for (int i = 0; i < studentCount; i++) {
        if (strcmp(students[i].student_id, student_id) == 0) {
            return &students[i];
        }
    }
    return NULL;
}

// 获取空闲床位信息
int* getAvailableBeds(const char* dormitory_id, int* count) {
    static int beds[10];  // 假设每个宿舍最多10个床位
    *count = 0;
    
    if (students == NULL) return beds;
    
    // 获取该宿舍的所有学生
    Student** dormStudents;
    int studentCount;
    dormStudents = findStudentsByDormitoryPattern(dormitory_id, &studentCount);
    
    // 如果没有找到宿舍
    if (studentCount == 0) {
        return NULL;
    }
    
    // 获取总床位数
    int totalBeds = dormStudents[0]->bed_count;
    
    // 标记已占用的床位
    int occupied[10] = {0};
    for (int i = 0; i < studentCount && i < 10; i++) {
        occupied[i] = 1;
    }
    
    // 找出空闲床位
    for (int i = 0; i < totalBeds && i < 10; i++) {
        if (!occupied[i]) {
            beds[*count] = i + 1;  // 床位号从1开始
            (*count)++;
        }
    }
    
    return beds;
}

// 模糊搜索函数
Student** findStudentsByPattern(const char* pattern, const char* field, int* count) {
    static Student* results[1000];
    *count = 0;
    
    if (students == NULL) return results;
    
    for (int i = 0; i < studentCount && *count < 1000; i++) {
        int match = 0;
        
        if (strcmp(field, "name") == 0) {
            match = strstr(students[i].student_name, pattern) != NULL;
        } else if (strcmp(field, "id") == 0) {
            match = strstr(students[i].student_id, pattern) != NULL;
        } else if (strcmp(field, "dormitory") == 0) {
            match = strstr(students[i].dormitory_id, pattern) != NULL;
        } else if (strcmp(field, "phone") == 0) {
            match = strstr(students[i].phone, pattern) != NULL;
        }
        
        if (match) {
            results[*count] = &students[i];
            (*count)++;
        }
    }
    
    return results;
} 