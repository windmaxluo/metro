#include "path.h"
#include "line.h"
#include "graph.h"
#include <stdio.h>
#include <string.h>

// 打印同一条线路内从A站到B站的路径 查找同一条路线
void print_segment(int line_id, const char *start, const char *end) // 参数 线路id 起点站名称 终点站名称
{
    // 获取索引，判断方向，打印沿途站点
    int start_idx = get_station_index(line_id, start);
    int end_idx = get_station_index(line_id, end);
    if (start_idx == -1 || end_idx == -1)   //有站点不存在
        return;

    printf("乘坐 %s 从 %s 到 %s", lines[line_id].name, start, end);
    printf("(");
    //双向打印，例如1234567从1到6应该打印123456，从6到1打印654321
    if (start_idx <= end_idx)
    {
        for (int i = start_idx; i <= end_idx; i++)
        {
            printf("%s", lines[line_id].stations[i]);
            if (i < end_idx)
                printf("->");
        }
    }
    else
    {
        for (int i = start_idx; i >= end_idx; i--)
        {
            printf("%s", lines[line_id].stations[i]);
            if (i > end_idx)
                printf("->");
        }
    }
    printf(")\n");
}

// 查询路径
void find_path(const char *start, const char *end) // 参数 起点名字 终点名字
{
    int start_lines[MAX_LINES], start_cnt; // 起点所包含线路id，start_cnt 计数线路条数 ，例如天空岛有一号线和二号线，这个数就为2
    int end_lines[MAX_LINES], end_cnt;     // 终点所包含线路

    find_lines_by_station(start, start_lines, &start_cnt); // 查找起点所在线路 和 存在的线路条数
    find_lines_by_station(end, end_lines, &end_cnt);

    // 情况一 站点不存在于任何当前线路
    if (start_cnt == 0 || end_cnt == 0)
    {
        printf("所输站点中找不到对应线路\n");
        return;
    }

    // 情况二 站点位于同一条线路，例如皆为一号线
    for (int i = 0; i < start_cnt; i++)
    {
        if (line_contains_station(start_lines[i], end)) // 起点站所在线路是否包含有终点站end和start有公共线路
        {
            printf("无需换乘，直接乘坐 %s\n", lines[start_lines[i]].name);
            print_segment(start_lines[i], start, end);
            return;
        }
    }

    // //情况三 一次换乘
    // for (int i = 0; i < start_cnt; i++)  //遍历开始站点的线路
    // {
    //     int l_start = start_lines[i];    //临时存放线路id
    //     for (int j = 0; j < end_cnt; j++)
    //     {
    //         int l_end = end_lines[j];
    //         if (transfer[l_start][l_end][0] != '\0')//换乘表判断有无公共站点
    //         {
    //             char *transfer_station = transfer[l_start][l_end];//指针接收公共站点名字字符串
    //             print_segment(l_start,start,transfer_station);
    //             printf("在%s换乘\n",transfer_station);//不要解引用，%s 要的是字符串地址，不是单个字符；char* 指针变量本身就是地址，直接用变量名；
    //             print_segment(l_end,transfer_station,end);
    //         }

    //     }
    // }

    // bfs
    int visited[MAX_LINES] = {0};               // 线路是否被访问过
    
    //这两个绑定
    int prev_line[MAX_LINES];                   // 前驱线路（从哪里来的）
    char prev_station[MAX_LINES][NAME_MAX_LEN]; // 换乘站名（从前驱换乘到本线的站）

    // 这两个队列绑定的
    int queue[MAX_LINES];      // 队列：存储线路ID
    int q_transfer[MAX_LINES]; // 队列：存储到达该线路的换乘次数

    int front = 0, rear = 0; // 队列头尾指针


//开始bfs
    for (int i = 0; i < start_cnt; i++)//处理起点的线路 入队
    {
        int l = start_lines[i];
        visited[l] = 1;
        prev_line[l] = -1;    // -1 表示这是起点，没有前驱
        queue[rear] = l;      // 把包含起点的线路都存入队列，后面在处理
        q_transfer[rear] = 0; // 起点线路换乘次数为 0
        rear++;
    }

    int found = -1, transfers = 0;//found存找到终点站所在线路的id，transfers计数最终换乘次数

    while (front < rear && found == -1)   //出队
    {
        int cur = queue[front];    // 当前线路
        int t = q_transfer[front]; // 当前换乘次数
        front++;

        // 检查当前线路是否包含终点站（起点终点是否在同一条线路）
        if (line_contains_station(cur, end))
        {
            found = cur;
            transfers = t;
            break;
        }

        // 遍历所有线路，找能换乘的
        for (int next = 0; next < line_count; next++) // 找当前所有和当前线路cur可以换乘的线路next
        {
            if (transfer[cur][next][0] != '\0' && visited[next]==0) // 有换乘站且没有访问过
            {
                visited[next] = 1;                               // 标记访问
                prev_line[next] = cur;                           // 记录next的上一个线路，cur
                strcpy(prev_station[next], transfer[cur][next]); // 记录前一个站名，也就是换乘站
                queue[rear] = next;                              // 可以换乘的线路入队，方便后面处理
                q_transfer[rear] = t + 1;                        // 到next线路时的换乘次数
                rear++;
            }
        }
    }
    // 回溯线路
    int path[MAX_LINES]; // 记录回溯路线
    int path_len = 0;
    int p = found;  // 从找到了开始回溯，也就是found
    while (p != -1) // 因为最开始就是-1
    {
        path[path_len++] = p;
        p = prev_line[p];
    }

    for (int i = 0; i < path_len / 2; i++)
    {
        int temp = path[i];
        path[i] = path[path_len - 1 - i];
        path[path_len - 1 - i] = temp;
    }
    // 举例
    /*prev_line[3] = 1, prev_line[1] = 0, prev_line[0] = -1
     回溯：3 → 1 → 0 → 结束
     path 数组（反转前）：[3, 1, 0]
     反转后：[0, 1, 3]*/

    // 打印路线
    printf("换乘 %d 次：\n", transfers);
    char cur_start[NAME_MAX_LEN]; // 临时起点
    strcpy(cur_start, start);

    for (int i = 0; i < path_len; i++)
    {
        int line_id = path[i];                                                              // 线路id
        char *cur_end = (i == path_len - 1) ? (char *)end : transfer[path[i]][path[i + 1]]; // 是最后一段取end，不是就取换乘站 (char *)去const警告
        print_segment(line_id, cur_start, cur_end);                                         // 打印站点
        if (i < path_len - 1)
        {
            printf("在 %s 换乘\n", cur_end);
        }
        strcpy(cur_start, cur_end); // 换乘站变新的起点站
    }
}