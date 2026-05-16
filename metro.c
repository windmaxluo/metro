#include <stdio.h>
#include <string.h>
#include "line.h"
#include "graph.h"
#include "path.h"

// 打印欢迎界面
void print_welcome(void)
{
    printf("\n========================================\n");
    printf("      欢迎使用地铁换乘查询系统\n");
    printf("========================================\n");
    printf("功能：查询两地之间的最少换乘路线\n");
    printf("说明：输入起点站和终点站，系统将为您规划最优路线\n");
    printf("提示：输入 'q' 可退出程序\n");
    printf("========================================\n\n");
}

// 打印所有可用的站点列表（供用户参考）
void print_available_stations(void)
{
    printf("【当前线路及站点一览】\n");
    for (int i = 0; i < line_count; i++) {
        printf("%s：", lines[i].name);
        for (int j = 0; j < lines[i].station_count; j++) {
            printf("%s", lines[i].stations[j]);
            if (j < lines[i].station_count - 1) {
                printf(" → ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

// 判断用户是否想退出
int is_quit(const char *input)
{
    return (strcmp(input, "q") == 0 || strcmp(input, "Q") == 0);
}

int main(void)
{
    char star[NAME_MAX_LEN];
    char end[NAME_MAX_LEN];
    
    // 初始化数据
    line_init_data();
    build_transfer_graph();
    
    // 打印欢迎界面和站点列表
    print_welcome();
    print_available_stations();
    
    // 主查询循环
    while (1) {
        // 输入起点
        printf("┌──────────────────────────────────────\n");
        printf("│ 请输入起点站：");
        scanf("%s", star);
        
        // 检查是否退出
        if (is_quit(star)) {
            printf("\n感谢使用，再见！\n");
            break;
        }
        
        // 输入终点
        printf("│ 请输入终点站：");
        scanf("%s", end);
        
        // 检查是否退出
        if (is_quit(end)) {
            printf("\n感谢使用，再见！\n");
            break;
        }
        
        printf("└──────────────────────────────────────\n");
        printf("\n【查询结果】\n");
        
        // 执行查询
        find_path(star, end);
        
        printf("\n----------------------------------------\n\n");
    }
    
    return 0;
}