#include "line.h"
#include <string.h>
#include <stdio.h>

// 线路数据定义
Line lines[MAX_LINES];
int line_count = 6; // 当前有6条线路

// 数字转中文（仅用于打印线路名）
static const char *num[] = {"一", "二", "三", "四", "五", "六", "七", "八", "九", "十"};

// 线路初始化
void line_init_data(void)
{
    // ==================== 一号线 ====================
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

    // ==================== 二号线 ====================
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

    // ==================== 三号线 ====================
    lines[2].id = 2;
    strcpy(lines[2].name, "三号线");
    lines[2].station_count = 6;
    strcpy(lines[2].stations[0], "须弥城");
    strcpy(lines[2].stations[1], "奥摩斯港");
    strcpy(lines[2].stations[2], "桓那兰那");
    strcpy(lines[2].stations[3], "赤王陵");
    strcpy(lines[2].stations[4], "阿如村");
    strcpy(lines[2].stations[5], "沙漠神殿");

    // ==================== 四号线 ====================
    lines[3].id = 3;
    strcpy(lines[3].name, "四号线");
    lines[3].station_count = 6;
    strcpy(lines[3].stations[0], "枫丹廷");
    strcpy(lines[3].stations[1], "沫芒宫");
    strcpy(lines[3].stations[2], "茉洁站");
    strcpy(lines[3].stations[3], "璃月港");
    strcpy(lines[3].stations[4], "明蕴镇");
    strcpy(lines[3].stations[5], "层岩巨渊");

    // ==================== 五号线 ====================
    lines[4].id = 4;
    strcpy(lines[4].name, "五号线");
    lines[4].station_count = 6;
    strcpy(lines[4].stations[0], "纳塔");
    strcpy(lines[4].stations[1], "隆崛谷");
    strcpy(lines[4].stations[2], "晶蝶谷");
    strcpy(lines[4].stations[3], "龙脊雪山");
    strcpy(lines[4].stations[4], "雪葬之都");
    strcpy(lines[4].stations[5], "寒天之钉");

    // ==================== 六号线 ====================
    lines[5].id = 5;
    strcpy(lines[5].name, "六号线");
    lines[5].station_count = 5;
    strcpy(lines[5].stations[0], "至冬宫");
    strcpy(lines[5].stations[1], "挪德卡莱");
    strcpy(lines[5].stations[2], "渊下宫");
    strcpy(lines[5].stations[3], "珊瑚宫");
    strcpy(lines[5].stations[4], "海祇岛");
}

// 打印单条线路（调试用）
void printf_lines(int id)
{
    if (id < 0 || id >= line_count)
    {
        printf("线路不存在\n");
        return;
    }
    printf("%s号线:", num[id]);
    for (int i = 0; i < lines[id].station_count; i++)
    {
        printf("%s", lines[id].stations[i]);
        if (i < lines[id].station_count - 1)
        {
            printf(" → ");
        }
    }
    printf("\n");
}

// 判断某条线路是否包含某个站点
int line_contains_station(int line_id, const char *station)
{
    if (line_id < 0 || line_id >= line_count)
    {
        return 0;
    }
    for (int i = 0; i < lines[line_id].station_count; i++)
    {
        if (strcmp(lines[line_id].stations[i], station) == 0)
        {
            return 1;
        }
    }
    return 0;
}

// 获取某个站点在线路上的下标（从0开始），用于方向判断
int get_station_index(int line_id, const char *station)
{
    if (line_id < 0 || line_id >= line_count)
    {
        return -1;
    }
    for (int i = 0; i < lines[line_id].station_count; i++)
    {
        if (strcmp(lines[line_id].stations[i], station) == 0)
        {
            return i;
        }
    }
    return -1;
}

// 查找包含某个站点的所有线路
void find_lines_by_station(const char *station, int line_ids[], int *count)
{
    *count = 0;
    for (int i = 0; i < line_count; i++)
    {
        if (line_contains_station(i, station))
        {
            line_ids[*count] = i;      //包含该站点的线路存到line_ids[]数组里面记住；
            (*count)++;
        }
    }
}