#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"
#include "student.h"
#include "dormitory.h"
#include "utils.h"

// 全局变量
static char currentUser[50] = "";
static int currentUserPrivilege = -1;

// 函数声明
void showMainMenu();
void showAdminMenu();
void showNormalUserMenu();
void handleLogin();
void handleLogout();
void initializeSystem();

// 管理员功能
void handleUserManagement();
void handleStudentManagement();
void handleDormitoryManagement();

// 普通用户功能
void handlePasswordChange();
void handleStudentQuery();
void handleDormitoryQuery();

// 用户管理子菜单
void showUserManagementMenu() {
    printf("\n=== 用户管理 ===\n");
    printf("1. 创建用户账号\n");
    printf("2. 删除用户账号\n");
    printf("3. 重置用户密码\n");
    printf("4. 返回上级菜单\n");
}

// 处理用户管理功能
void handleUserManagement() {
    char username[50], password[50];
    int choice;
    
    while (1) {
        showUserManagementMenu();
        printf("请选择操作：");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: // 创建用户账号
                printf("请输入新用户名：");
                scanf("%s", username);
                printf("请输入密码：");
                scanf("%s", password);
                
                if (createUser(username, password, NORMAL_USER) == SUCCESS) {
                    printf("用户创建成功！\n");
                    logInfo("创建新用户成功");
                } else {
                    printf("用户创建失败！\n");
                    logError("创建新用户失败");
                }
                break;
                
            case 2: // 删除用户账号
                printf("请输入要删除的用户名：");
                scanf("%s", username);
                
                if (strcmp(username, currentUser) == 0) {
                    printf("不能删除当前登录用户！\n");
                    break;
                }
                
                if (deleteUser(username) == SUCCESS) {
                    printf("用户删除成功！\n");
                    logInfo("删除用户成功");
                } else {
                    printf("用户删除失败！\n");
                    logError("删除用户失败");
                }
                break;
                
            case 3: // 重置用户密码
                printf("请输入要重置密码的用户名：");
                scanf("%s", username);
                printf("请输入新密码：");
                scanf("%s", password);
                
                if (resetUserPassword(username, password) == SUCCESS) {
                    printf("密码重置成功！\n");
                    logInfo("重置用户密码成功");
                } else {
                    printf("密码重置失败！\n");
                    logError("重置用户密码失败");
                }
                break;
                
            case 4: // 返回上级菜单
                return;
                
            default:
                printf("无效选择，请重试\n");
        }
    }
}

// 处理密码修改
void handlePasswordChange() {
    char oldPassword[50], newPassword[50];
    printf("请输入原密码：");
    scanf("%s", oldPassword);
    printf("请输入新密码：");
    scanf("%s", newPassword);
    
    if (changePassword(currentUser, oldPassword, newPassword) == SUCCESS) {
        printf("密码修改成功！\n");
        logInfo("修改密码成功");
    } else {
        printf("密码修改失败！\n");
        logError("修改密码失败");
    }
}

// 学生管理子菜单
void showStudentManagementMenu() {
    printf("\n=== 学生信息管理 ===\n");
    printf("1. 录入学生信息\n");
    printf("2. 修改学生信息\n");
    printf("3. 删除学生信息\n");
    printf("4. 查询学生信息\n");
    printf("5. 返回上级菜单\n");
}

// 处理学生信息管理
void handleStudentManagement() {
    int choice;
    Student student;
    char student_id[20];
    
    while (1) {
        showStudentManagementMenu();
        printf("请选择操作：");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: // 录入学生信息
                while (1) {
                    printf("\n=== 录入学生信息 ===\n");
                    printf("请按照以下格式要求输入信息（输入Q退出）：\n");
                    printf("宿舍号 (3-8位数字字母组合，如：A101)：");
                    char input[50];
                    scanf("%s", input);
                    if (strcmp(input, "Q") == 0 || strcmp(input, "q") == 0) break;
                    strcpy(student.dormitory_id, input);

                    if (!isValidDormitoryId(student.dormitory_id)) {
                        printf("错误：宿舍号格式不正确！必须是3-8位数字字母组合。\n");
                        printf("按回车继续...\n");
                        getchar(); getchar();
                        continue;
                    }

                    printf("床位数 (正整数)：");
                    scanf("%d", &student.bed_count);
                    if (student.bed_count <= 0) {
                        printf("错误：床位数必须是正整数！\n");
                        printf("按回车继续...\n");
                        getchar(); getchar();
                        continue;
                    }

                    printf("学生姓名：");
                    scanf("%s", student.student_name);

                    printf("学号 (8-12位数字，如：20230001)：");
                    scanf("%s", student.student_id);
                    if (!isValidStudentId(student.student_id)) {
                        printf("错误：学号格式不正确！必须是8-12位数字。\n");
                        printf("按回车继续...\n");
                        getchar(); getchar();
                        continue;
                    }

                    printf("电话 (11位数字，如：13800138000)：");
                    scanf("%s", student.phone);
                    if (!isValidPhoneNumber(student.phone)) {
                        printf("错误：学生电话格式不正确！必须是11位数字。\n");
                        printf("按回车继续...\n");
                        getchar(); getchar();
                        continue;
                    }

                    printf("卧室长姓名：");
                    scanf("%s", student.room_leader_name);

                    printf("卧室长电话 (11位数字，如：13800138000)：");
                    scanf("%s", student.room_leader_phone);
                    if (!isValidPhoneNumber(student.room_leader_phone)) {
                        printf("错误：卧室长电话格式不正确！必须是11位数字。\n");
                        printf("按回车继续...\n");
                        getchar(); getchar();
                        continue;
                    }

                    int result = addStudent(&student);
                    if (result == SUCCESS) {
                        printf("学生信息录入成功！\n");
                        logInfo("录入学生信息成功");
                        break;
                    } else if (result == ERROR_NOT_FOUND) {
                        printf("错误：指定的宿舍不存在！\n");
                    } else if (result == ERROR_PERMISSION_DENIED) {
                        printf("错误：宿舍已满或不可用！\n");
                    } else if (result == ERROR_ALREADY_EXISTS) {
                        printf("错误：该学号已存在！\n");
                    } else {
                        printf("错误：学生信息录入失败！\n");
                        logError("录入学生信息失败");
                    }
                    printf("按回车继续...\n");
                    getchar(); getchar();
                }
                break;
                
            case 2: // 修改学生信息
                printf("请输入要修改的学生学号：");
                scanf("%s", student_id);
                
                Student* oldInfo = findStudentById(student_id);
                if (oldInfo == NULL) {
                    printf("未找到该学生！\n");
                    break;
                }
                
                // 复制原有信息
                student = *oldInfo;
                
                printf("请输入新的信息（直接回车保持不变）：\n");
                char input[50];
                
                printf("宿舍号 [%s]：", student.dormitory_id);
                scanf("%s", input);
                if (strlen(input) > 0) strcpy(student.dormitory_id, input);
                
                printf("床位数 [%d]：", student.bed_count);
                scanf("%s", input);
                if (strlen(input) > 0) student.bed_count = atoi(input);
                
                printf("学生姓名 [%s]：", student.student_name);
                scanf("%s", input);
                if (strlen(input) > 0) strcpy(student.student_name, input);
                
                printf("电话 [%s]：", student.phone);
                scanf("%s", input);
                if (strlen(input) > 0) strcpy(student.phone, input);
                
                printf("卧室长姓名 [%s]：", student.room_leader_name);
                scanf("%s", input);
                if (strlen(input) > 0) strcpy(student.room_leader_name, input);
                
                printf("卧室长电话 [%s]：", student.room_leader_phone);
                scanf("%s", input);
                if (strlen(input) > 0) strcpy(student.room_leader_phone, input);
                
                if (updateStudent(student_id, &student) == SUCCESS) {
                    printf("学生信息修改成功！\n");
                    logInfo("修改学生信息成功");
                } else {
                    printf("学生信息修改失败！\n");
                    logError("修改学生信息失败");
                }
                break;
                
            case 3: // 删除学生信息
                printf("请输入要删除的学生学号：");
                scanf("%s", student_id);
                
                if (deleteStudent(student_id) == SUCCESS) {
                    printf("学生信息删除成功！\n");
                    logInfo("删除学生信息成功");
                } else {
                    printf("学生信息删除失败！\n");
                    logError("删除学生信息失败");
                }
                break;
                
            case 4: // 查询学生信息
                handleStudentQuery();
                break;
                
            case 5: // 返回上级菜单
                return;
                
            default:
                printf("无效选择，请重试\n");
        }
    }
}

// 处理学生信息查询
void handleStudentQuery() {
    int choice;
    char query[50];
    Student** students;
    int count;
    
    while (1) {
        printf("\n=== 学生信息查询 ===\n");
        printf("1. 按学号查询\n");
        printf("2. 按宿舍号查询\n");
        printf("3. 按姓名查询\n");
        printf("4. 按电话查询\n");
        printf("5. 返回上级菜单\n");
        printf("请选择查询方式：");
        scanf("%d", &choice);
        
        if (choice == 5) break;
        
        if (choice < 1 || choice > 4) {
            printf("无效选择，请重试\n");
            continue;
        }
        
        printf("请输入查询关键字：");
        scanf("%s", query);
        
        const char* fields[] = {"id", "dormitory", "name", "phone"};
        students = findStudentsByPattern(query, fields[choice-1], &count);
        
        if (count == 0) {
            printf("未找到匹配的记录！\n");
            continue;
        }
        
        // 分页显示结果
        int currentPage = 0;
        int totalPages = getPageCount(count);
        char op;
        
        while (1) {
            clearScreen();
            printf("\n=== 查询结果 ===\n");
            printf("共找到 %d 条记录\n", count);
            
            // 显示当前页的记录
            int start = currentPage * PAGE_SIZE;
            int end = start + PAGE_SIZE;
            if (end > count) end = count;
            
            for (int i = start; i < end; i++) {
                printf("\n第 %d 条记录：\n", i + 1);
                printf("学号：%s\n", students[i]->student_id);
                printf("姓名：%s\n", students[i]->student_name);
                printf("宿舍号：%s\n", students[i]->dormitory_id);
                printf("床位数：%d\n", students[i]->bed_count);
                printf("电话：%s\n", students[i]->phone);
                printf("卧室长：%s\n", students[i]->room_leader_name);
                printf("卧室长电话：%s\n", students[i]->room_leader_phone);
            }
            
            showPagination(currentPage, totalPages);
            printf("请选择操作：");
            scanf(" %c", &op);
            
            switch (op) {
                case 'P':
                case 'p':
                    if (currentPage > 0) currentPage--;
                    break;
                case 'N':
                case 'n':
                    if (currentPage < totalPages - 1) currentPage++;
                    break;
                case 'G':
                case 'g':
                    printf("请输入页码(1-%d)：", totalPages);
                    int page;
                    scanf("%d", &page);
                    if (page >= 1 && page <= totalPages) {
                        currentPage = page - 1;
                    }
                    break;
                case 'Q':
                case 'q':
                    return;
                default:
                    printf("无效操作，请重试\n");
            }
        }
    }
}

// 宿舍管理子菜单
void showDormitoryManagementMenu() {
    printf("\n=== 宿舍管理 ===\n");
    printf("1. 添加宿舍信息\n");
    printf("2. 修改宿舍信息\n");
    printf("3. 删除宿舍信息\n");
    printf("4. 查询宿舍信息\n");
    printf("5. 查看统计信息\n");
    printf("6. 返回上级菜单\n");
}

// 处理宿舍管理
void handleDormitoryManagement() {
    int choice;
    Dormitory dormitory;
    char dormitory_id[10];
    
    while (1) {
        showDormitoryManagementMenu();
        printf("请选择操作���");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: // 添加宿舍信息
                printf("请输入宿舍信息：\n");
                printf("宿舍号：");
                scanf("%s", dormitory.dormitory_id);
                printf("总床位数：");
                scanf("%d", &dormitory.total_beds);
                printf("已占用床位数：");
                scanf("%d", &dormitory.occupied_beds);
                printf("楼层：");
                scanf("%s", dormitory.floor);
                printf("房间状态（0:正常, 1:维修中, 2:停用）：");
                scanf("%d", &dormitory.room_status);
                
                if (!isValidDormitoryId(dormitory.dormitory_id)) {
                    printf("宿舍号格式不正确！\n");
                    break;
                }
                
                if (dormitory.occupied_beds > dormitory.total_beds) {
                    printf("已占用床位数不能大于总床位数！\n");
                    break;
                }
                
                if (addDormitory(&dormitory) == SUCCESS) {
                    printf("宿舍信息添加成功！\n");
                    logInfo("添加宿舍信息成功");
                } else {
                    printf("宿舍信息添加失败！\n");
                    logError("添加宿舍信息失败");
                }
                break;
                
            case 2: // 修改宿舍信息
                printf("请输入要修改的宿舍号：");
                scanf("%s", dormitory_id);
                
                Dormitory* oldInfo = getDormitoryInfo(dormitory_id);
                if (oldInfo == NULL) {
                    printf("未找到该宿舍！\n");
                    break;
                }
                
                // 复制原有信息
                dormitory = *oldInfo;
                
                printf("请输入新的信息（直接回车保持不变）：\n");
                char input[50];
                
                printf("总床位数 [%d]：", dormitory.total_beds);
                scanf("%s", input);
                if (strlen(input) > 0) dormitory.total_beds = atoi(input);
                
                printf("已占用床位数 [%d]：", dormitory.occupied_beds);
                scanf("%s", input);
                if (strlen(input) > 0) dormitory.occupied_beds = atoi(input);
                
                printf("楼层 [%s]：", dormitory.floor);
                scanf("%s", input);
                if (strlen(input) > 0) strcpy(dormitory.floor, input);
                
                printf("房间状态 [%d]：", dormitory.room_status);
                scanf("%s", input);
                if (strlen(input) > 0) dormitory.room_status = atoi(input);
                
                if (dormitory.occupied_beds > dormitory.total_beds) {
                    printf("已占用床位数不能大于总床位数！\n");
                    break;
                }
                
                if (updateDormitory(dormitory_id, &dormitory) == SUCCESS) {
                    printf("宿舍信息修改成功！\n");
                    logInfo("修改宿舍信息成功");
                } else {
                    printf("宿舍信息修改失败！\n");
                    logError("修改宿舍信息失败");
                }
                break;
                
            case 3: // 删除宿舍信息
                printf("请输入要删除的宿舍号：");
                scanf("%s", dormitory_id);
                
                printf("警告：删除宿舍将永久删除该宿舍的所有信息！\n");
                printf("确认删除吗？(Y/N)：");
                char confirm;
                scanf(" %c", &confirm);
                
                if (confirm == 'Y' || confirm == 'y') {
                    int result = deleteDormitory(dormitory_id);
                    if (result == SUCCESS) {
                        printf("宿舍信息删除成功！\n");
                        logInfo("删除宿舍信息成功");
                    } else if (result == ERROR_PERMISSION_DENIED) {
                        printf("错误：该宿舍中还有学生，不能删除！\n");
                        logError("删除宿舍失败：宿舍中还有学生");
                    } else {
                        printf("错误：宿舍信息删除失败！\n");
                        logError("删除宿舍信息失败");
                    }
                } else {
                    printf("已取消删除操作。\n");
                }
                printf("按回车继续...\n");
                getchar(); getchar();
                break;
                
            case 4: // 查询宿舍信息
                handleDormitoryQuery();
                break;
                
            case 5: // 查看统计信息
                int total_rooms, available_rooms, total_beds, available_beds;
                getDormitoryStats(&total_rooms, &available_rooms, &total_beds, &available_beds);
                
                printf("\n=== 宿舍统计信息 ===\n");
                printf("总宿舍数：%d\n", total_rooms);
                printf("可用宿舍数：%d\n", available_rooms);
                printf("总床位数：%d\n", total_beds);
                printf("可用床位数：%d\n", available_beds);
                printf("宿舍使用率：%.2f%%\n", (total_rooms - available_rooms) * 100.0 / total_rooms);
                printf("床位使用率：%.2f%%\n", (total_beds - available_beds) * 100.0 / total_beds);
                
                logInfo("查看宿舍统计信息");
                break;
                
            case 6: // 返回上级菜单
                return;
                
            default:
                printf("无效选择，请重试\n");
        }
    }
}

// 处理宿舍查询
void handleDormitoryQuery() {
    char dormitory_id[10];
    printf("\n=== 宿舍信息查询 ===\n");
    printf("请输入宿舍号：");
    scanf("%s", dormitory_id);
    
    Dormitory* dormitory = getDormitoryInfo(dormitory_id);
    if (dormitory != NULL) {
        printf("\n查询结果：\n");
        printf("宿舍号：%s\n", dormitory->dormitory_id);
        printf("总床位数：%d\n", dormitory->total_beds);
        printf("已占用床位数：%d\n", dormitory->occupied_beds);
        printf("空闲床位数：%d\n", dormitory->total_beds - dormitory->occupied_beds);
        printf("楼层：%s\n", dormitory->floor);
        printf("房间状态：%s\n", 
               dormitory->room_status == 0 ? "正常" :
               dormitory->room_status == 1 ? "维修中" : "停用");
        
        // 查询该宿舍的空闲床位
        int available = getAvailableBedsCount(dormitory_id);
        if (available >= 0) {
            printf("当前可用床位数：%d\n", available);
        }
        
        logInfo("查询宿舍信息成功");
    } else {
        printf("未找到该宿舍！\n");
        logError("查询宿舍信息失败");
    }
}

void printUsage() {
    printf("使用方法：\n");
    printf("  ./dormitory_system                 - 正常启动程序\n");
    printf("  ./dormitory_system -u 用户名 -p 密码  - 直接使用用户名密码登录\n");
    printf("  ./dormitory_system -h              - 显示帮助信息\n");
}

int main(int argc, char* argv[]) {
    printf("欢迎使用学生宿舍管理系统\n");
    logInfo("系统启动");
    
    // 初始化系统
    initializeSystem();
    
    // 处理命令行参数
    if (argc > 1) {
        if (strcmp(argv[1], "-h") == 0) {
            printUsage();
            return 0;
        }
        
        char* username = NULL;
        char* password = NULL;
        
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-u") == 0 && i + 1 < argc) {
                username = argv[i + 1];
                i++;
            } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
                password = argv[i + 1];
                i++;
            }
        }
        
        if (username && password) {
            int result = userLogin(username, password);
            if (result >= 0) {
                strcpy(currentUser, username);
                currentUserPrivilege = result;
                printf("登录成功！\n");
                logInfo("用户登录成功");
            } else {
                printf("登录失败，请检查用户名和密码\n");
                logError("用户登录失败");
                return 1;
            }
        } else if (username || password) {
            printf("错误：必须同时提供用户名和密码\n");
            printUsage();
            return 1;
        }
    }
    
    while (1) {
        if (strlen(currentUser) == 0) {
            showMainMenu();
        } else if (currentUserPrivilege == ADMIN_USER) {
            showAdminMenu();
        } else {
            showNormalUserMenu();
        }
    }
    
    return 0;
}

void showMainMenu() {
    int choice;
    printf("\n=== 主菜单 ===\n");
    printf("1. 登录\n");
    printf("2. 退出\n");
    printf("请选择：");
    scanf("%d", &choice);
    
    switch (choice) {
        case 1:
            handleLogin();
            break;
        case 2:
            printf("感谢使用，再见！\n");
            logInfo("系统关闭");
            exit(0);
        default:
            printf("无效选择，请重试\n");
    }
}

void showAdminMenu() {
    int choice;
    printf("\n=== 系统管理员菜单 ===\n");
    printf("1. 用户管理\n");
    printf("2. 学生信息管理\n");
    printf("3. 宿舍管理\n");
    printf("4. 修改密码\n");
    printf("5. 注销\n");
    printf("6. 退出\n");
    printf("请选择：");
    scanf("%d", &choice);
    
    switch (choice) {
        case 1:
            handleUserManagement();
            break;
        case 2:
            handleStudentManagement();
            break;
        case 3:
            handleDormitoryManagement();
            break;
        case 4:
            handlePasswordChange();
            break;
        case 5:
            handleLogout();
            break;
        case 6:
            printf("感谢使用，再见！\n");
            logInfo("系统关闭");
            exit(0);
        default:
            printf("无效选择，请重试\n");
    }
}

void showNormalUserMenu() {
    int choice;
    printf("\n=== 住宿管理员菜单 ===\n");
    printf("1. 查询学生信息\n");
    printf("2. 查询宿舍信息\n");
    printf("3. 修改密码\n");
    printf("4. 注销\n");
    printf("5. 退出\n");
    printf("请选择：");
    scanf("%d", &choice);
    
    switch (choice) {
        case 1:
            handleStudentQuery();
            break;
        case 2:
            handleDormitoryQuery();
            break;
        case 3:
            handlePasswordChange();
            break;
        case 4:
            handleLogout();
            break;
        case 5:
            printf("感谢使用，再见！\n");
            logInfo("系统关闭");
            exit(0);
        default:
            printf("无效选择，请重试\n");
    }
}

void handleLogin() {
    char username[50], password[50];
    printf("请输入用户名：");
    scanf("%s", username);
    printf("请输入密码：");
    scanf("%s", password);
    
    int result = userLogin(username, password);
    if (result >= 0) {
        strcpy(currentUser, username);
        currentUserPrivilege = result;
        printf("登录成功！\n");
        logInfo("用户登录成功");
    } else {
        printf("登录失败，请检查用户名和密码\n");
        logError("用户登录失败");
    }
}

void handleLogout() {
    logInfo("用户注销");
    currentUser[0] = '\0';
    currentUserPrivilege = -1;
    printf("已注销\n");
}

void initializeSystem() {
    // 创建必要的目录和文件
    createDirectory(DATA_DIR);
    
    // 如果是首次运行，创建默认管理员账号
    if (!fileExists(USERS_FILE)) {
        printf("首次运行，创建默认管理员账号\n");
        if (createUser("admin", "admin123", ADMIN_USER) == SUCCESS) {
            printf("默认管理员账号创建成功！\n");
            logInfo("创建默认管理员账号成功");
        } else {
            printf("默认管理员账号创建失败！\n");
            logError("创建默认管理员账号失败");
            exit(1);
        }
    }
    
    // 加载数据
    if (loadUserData() != SUCCESS || 
        loadStudentData() != SUCCESS || 
        loadDormitoryData() != SUCCESS) {
        printf("数据加载失败！\n");
        logError("数据加载失败");
        exit(1);
    }
    
    logInfo("系统初始化完成");
} 