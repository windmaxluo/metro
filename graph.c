#include "graph.h"
#include <string.h>

// 换乘关系表定义
char transfer[MAX_LINES][MAX_LINES][NAME_MAX_LEN];

// 自动构建换乘表
void build_transfer_graph()
{
    // 初始化：全部设置为空 （表示不连通）
    for (int i = 0; i < MAX_LINES; i++)
    {
        for (int j = 0; j < MAX_LINES; j++)
        {
            strcpy(transfer[i][j], "\0");
        }
    }

    // 构建所有线路的公共站点
    for (int i = 0; i < line_count; i++) // 遍历线路
    {
        for (int j = i + 1; j < line_count; j++) // 遍历线路
        {
            for (int i_station = 0; i_station < lines[i].station_count; i_station++) // 遍历站点
            {
                for (int j_station = 0; j_station < lines[j].station_count; j_station++)
                {
                    if (strcmp(lines[i].stations[i_station], lines[j].stations[j_station]) == 0) // 有公共站点
                    {
                        strcpy(transfer[i][j], lines[i].stations[i_station]);
                        strcpy(transfer[j][i], lines[i].stations[i_station]); // 双向

                        // 只存一个公共站点 对于最少换乘次数并没有影响
                        i_station = lines[i].station_count; // 快速跳出i_station的那层循环
                        break;                              // 快速跳出j_station的循环
                    }
                }
            }
        }
    }
}