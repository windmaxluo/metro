#include <stdio.h>
#include <string.h>

#define MAX_STATIONS_PER_LINE 50 // 每条线路最大站点数
#define NAME_MAX_LEN 20          // 线路/站点名称最大20
#define MAX_LINES 10             // 最大线路数

typedef struct
{
    int id;                                             // 线路id
    char name[NAME_MAX_LEN];                            // 线路名称
    int station_count;                                  // 该站点数量
    char stations[MAX_STATIONS_PER_LINE][NAME_MAX_LEN]; // 站点名字数组 二维数组，每一行存一个站点
} Line;

Line lines[MAX_LINES]; // 定义线路
int line_count = 2;    // 定义线路数量 现在为2 即2条线路
// 构建手动换乘表 (临时方案在init_line_data 中设置)
char transfer[MAX_LINES][MAX_LINES][NAME_MAX_LEN]; // 源线路id 目标线路id 存储换乘站名字符串

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

void line_init_data() // 线路初始化
{
    lines[0].id = 0;
    strcpy(lines[0].name, "一号线");
    lines[0].station_count = 8;
    strcpy(lines[0].stations[0], "蒙德城");
    strcpy(lines[0].stations[1], "璃月港");
    strcpy(lines[0].stations[2], "稻妻城");
    strcpy(lines[0].stations[3], "天空岛");
    strcpy(lines[0].stations[4], "龙脊雪山");
    strcpy(lines[0].stations[5], "层岩巨渊");
    strcpy(lines[0].stations[6], "赤王陵");
    strcpy(lines[0].stations[7], "渊下宫");

    lines[1].id = 1;
    strcpy(lines[1].name, "二号线");
    lines[1].station_count = 7;
    strcpy(lines[1].stations[0], "果酒湖");
    strcpy(lines[1].stations[1], "摘星崖");
    strcpy(lines[1].stations[2], "千风神殿");
    strcpy(lines[1].stations[3], "天空岛");
    strcpy(lines[1].stations[4], "沉玉谷");
    strcpy(lines[1].stations[5], "琥牢山");
    strcpy(lines[1].stations[6], "望舒客栈");

    // 换程表手动写（临时）
    //    strcpy(transfer[0][1], "天空岛");   //一号线(0) 和 二号线(1) 可以在 "天空岛" 换乘
    //    strcpy(transfer[1][0], "天空岛");
}

// 打印单挑线路
char *num[NAME_MAX_LEN] = {"一", "二", "三", "四", "五", "六", "七"};
void printf_lines(int id) // 参数：线路id
{
    printf("%s号线:", num[id]);
    for (int i = 0; i < lines[id].station_count; i++)
    {
        printf("%s->", lines[id].stations[i]);
    }
    printf("\b\b");
}

// 判断某条线路是否包含某个站点，返回值0（不包含）或1（包含）
int line_contains_station(int line_id, const char *station) // 参数 线路id，和站点名称 ，const防误修改 字符串用指针而不是const char station，const char station是只传一个字符
{
    for (int i = 0; i < lines[line_id].station_count; i++) // 遍历查找
    {
        if (strcmp(lines[line_id].stations[i], station) == 0) // strcmp相等等于0
        {
            return 1; // 包含返回1
        }
    }
    return 0;
}

// 获取某个站点在线路上的下标（从0开始），用于方向判断，（遍历逻辑同上）
int get_station_index(int line_id, const char *station) // 参数 线路id，和站点名称
{

    for (int i = 0; i < lines[line_id].station_count; i++) // 遍历查找
    {
        if (strcmp(lines[line_id].stations[i], station) == 0) // strcmp相等等于0
        {
            return i; // 返回下标
        }
    }
    return -1;
}

// 查找包含某个站点的所有线路 用来查找起点和终点所在线路 例如：假设 station = "天空岛"，一号线和二号线都经过它。调用后：
// line_ids[0] = 0，line_ids[1] = 1
//*count = 2
void find_lines_by_station(const char *station, int line_ids[], int *count) // 参数 站点名称 包含该站点的所有线路组成的数组 包含改站点的线路个数
{
    *count = 0;
    for (int i = 0; i < line_count; i++)
    {
        if (line_contains_station(i, station))
        {
            line_ids[*count] = i;
            (*count)++;
        }
    }
}

// 打印同一条线路内从A站到B站的路径 查找同一条路线
void print_segment(int line_id, const char *start, const char *end) // 参数 线路id 起点站名称 终点站名称
{
    // 获取索引，判断方向，打印沿途站点
    int start_idx = get_station_index(line_id, start);
    int end_idx = get_station_index(line_id, end);
    if (start_idx == -1 || end_idx == -1)
        return;

    printf("乘坐 %s 从 %s 到 %s", lines[line_id].name, start, end);
    printf("(");
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
        if (line_contains_station(start_lines[i], end)) // end和start有公共线路
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
    int prev_line[MAX_LINES];                   // 前驱线路（从哪里来的）
    char prev_station[MAX_LINES][NAME_MAX_LEN]; // 换乘站名（从前驱换乘到本线的站）

    // 这两个队列绑定的
    int queue[MAX_LINES];      // 队列：存储线路ID
    int q_transfer[MAX_LINES]; // 队列：存储到达该线路的换乘次数

    int front = 0, rear = 0; // 队列头尾指针

    for (int i = 0; i < start_cnt; i++)
    {
        int l = start_lines[i];
        visited[l] = 1;
        prev_line[l] = -1;    // -1 表示这是起点，没有前驱
        queue[rear] = l;      // 把包含起点的线路都存入队列，后面在处理
        q_transfer[rear] = 0; // 起点线路换乘次数为 0
        rear++;
    }

    int found = -1, transfers = 0;

    while (front < rear && found == -1)
    {
        int cur = queue[front];    // 当前线路
        int t = q_transfer[front]; // 当前换乘次数
        front++;

        // 检查当前线路是否包含终点站
        if (line_contains_station(cur, end))
        {
            found = cur;
            transfers = t;
            break;
        }

        // 遍历所有线路，找能换乘的
        for (int next = 0; next < line_count; next++) // 找当前所有和当前线路cur可以换乘的线路next
        {
            if (transfer[cur][next][0] != '\0' && !visited[next]) // 有换乘站且没有访问过
            {
                visited[next] = 1;                               // 标记访问
                prev_line[next] = cur;                           // 记录next的千一个线路，cur
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
    while (p != -1) // 因为found最开始就是-1
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
        char *cur_end = (i == path_len - 1) ? (char *)end : transfer[path[i]][path[i + 1]]; // 是最后一段取end，不是就取换乘站
        print_segment(line_id, cur_start, cur_end);                                         // 打印站点
        if (i < path_len - 1)
        {
            printf("在 %s 换乘\n", cur_end);
        }
        strcpy(cur_start, cur_end); // 换乘站变新的起点站
    }
}

int main(int argc, char const *argv[])
{

    line_init_data(); // 线路初始化，目前一条线路
    // printf_lines(0);  //打印一号线路

    /*//验证包含函数是否正确
    int line_id=0;
    char station[NAME_MAX_LEN] = "天空岛";
    printf("\n");
    if(line_contains_station(line_id,station))
    {
        printf("%s号线路包含%s\n",num[line_id],station);
    }
    else
    {
        printf("%s号线路不包含%s\n",num[line_id],station);
    }
    */

    /*/验证获取下标函数是否正确
    printf("\n");
    printf("天空岛位置：%d",get_station_index(0,"天空岛"));
    */

    /*验证查找包含某个站点的所有线路的函数
    int start_lines[MAX_LINES], start_cnt;
    find_lines_by_station("天空岛",start_lines,&start_cnt);
    for (int i = 0; i < start_cnt; i++)
    {
        printf("%d,",start_lines[i]);
    }
    printf("\n");
    printf("%d\n",start_cnt);
    */

    // print_segment(0,"蒙德城","天空岛");
    // print_segment(0,"天空岛","蒙德城");
    // print_segment(1,"天空岛","望舒客栈");

    // find_path("蒙德城","沉玉谷");

    build_transfer_graph();

    // for (int i = 0; i < MAX_LINES; i++)
    // {
    //     for (int j = 0; j < MAX_LINES; j++)
    //     {
    //         printf("%-10s ", transfer[i][j]);
    //         if (j == MAX_LINES - 1)
    //         {
    //             printf("\n");
    //         }
    //     }
    // }

    char star[NAME_MAX_LEN], end[NAME_MAX_LEN];

    printf("请输入起点站");
    scanf("%s", star);
    printf("请输入终点站");
    scanf("%s", end);

    find_path(star,end);
    return 0;
}
