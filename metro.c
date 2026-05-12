#include <stdio.h>
#include <string.h>


#define MAX_STATIONS_PER_LINE 50  // 每条线路最大站点数
#define NAME_MAX_LEN 20    //线路/站点名称最大20
#define MAX_LINES 10          // 最大线路数

typedef struct 
{
    int id;              //线路id
    char name[NAME_MAX_LEN]; //线路名称
    int station_count;   //该站点数量
    char stations[MAX_STATIONS_PER_LINE][NAME_MAX_LEN];        //站点名字数组 二维数组，每一行存一个站点
} Line;

Line lines[MAX_LINES];   //定义线路
int line_count = 2;      //定义线路数量 现在为2 即2条线路
// 构建手动换乘表 (临时方案在init_line_data 中设置)
char transfer[MAX_LINES][MAX_LINES][NAME_MAX_LEN];//源线路id 目标线路id 存储换乘站名字符串

void line_init_data()   //线路初始化
{
    lines[0].id=0;
    strcpy(lines[0].name,"一号线");
    lines[0].station_count = 8;
    strcpy(lines[0].stations[0],"蒙德城");
    strcpy(lines[0].stations[1],"璃月港");
    strcpy(lines[0].stations[2],"稻妻城");
    strcpy(lines[0].stations[3],"天空岛");
    strcpy(lines[0].stations[4],"龙脊雪山");
    strcpy(lines[0].stations[5],"层岩巨渊");
    strcpy(lines[0].stations[6],"赤王陵");
    strcpy(lines[0].stations[7],"渊下宫");

    lines[1].id=1;
    strcpy(lines[1].name,"二号线");
    lines[1].station_count = 7;
    strcpy(lines[1].stations[0],"果酒湖");
    strcpy(lines[1].stations[1],"摘星崖");
    strcpy(lines[1].stations[2],"千风神殿");
    strcpy(lines[1].stations[3],"天空岛");
    strcpy(lines[1].stations[4],"沉玉谷");
    strcpy(lines[1].stations[5],"琥牢山");
    strcpy(lines[1].stations[6],"望舒客栈");


    //换程表手动写（临时）
    strcpy(transfer[0][1], "天空岛");   //一号线(0) 和 二号线(1) 可以在 "天空岛" 换乘
    strcpy(transfer[1][0], "天空岛");
}



//打印单挑线路
char *num[NAME_MAX_LEN]={"一","二","三","四","五","六","七"};
void printf_lines(int id)  //参数：线路id
{
   printf("%s号线:",num[id]);
   for (int i = 0; i < lines[id].station_count; i++)
   {
    printf("%s->",lines[id].stations[i]);
   }
   printf("\b\b");
}



//判断某条线路是否包含某个站点，返回值0（不包含）或1（包含）
int line_contains_station(int line_id, const char *station) //参数 线路id，和站点名称 ，const防误修改 字符串用指针而不是const char station，const char station是只传一个字符
{
                                                           for (int i = 0; i < lines[line_id].station_count; i++)   //遍历查找
    {
        if (strcmp(lines[line_id].stations[i],station)==0)    //strcmp相等等于0
        {
            return 1;//包含返回1
        }
    }
    return 0;
}

// 获取某个站点在线路上的下标（从0开始），用于方向判断，（遍历逻辑同上）
int get_station_index(int line_id, const char *station) //参数 线路id，和站点名称
{
                                                      
    for (int i = 0; i < lines[line_id].station_count; i++)   //遍历查找
    {
        if (strcmp(lines[line_id].stations[i],station)==0)    //strcmp相等等于0
        {
            return i;//返回下标
        }
    }
    return -1;
}


// 查找包含某个站点的所有线路 用来查找起点和终点所在线路 例如：假设 station = "天空岛"，一号线和二号线都经过它。调用后：
//line_ids[0] = 0，line_ids[1] = 1
//*count = 2
void find_lines_by_station(const char *station, int line_ids[], int *count)//参数 站点名称 包含该站点的所有线路组成的数组 包含改站点的线路个数
{
    *count=0;
    for (int i = 0; i <line_count; i++)
    {
        if (line_contains_station(i,station))
        {
            line_ids[*count] = i;
            (*count)++;
        }
    }
}


// 打印同一条线路内从A站到B站的路径 查找同一条路线
void print_segment(int line_id, const char *start, const char *end) //参数 线路id 起点站名称 终点站名称
{
    // 获取索引，判断方向，打印沿途站点
    int start_idx = get_station_index(line_id, start);
    int end_idx = get_station_index(line_id, end);
    if (start_idx == -1 || end_idx == -1) return;

    printf("乘坐 %s 从 %s 到 %s", lines[line_id].name, start, end);
    printf("(");
    if (start_idx<=end_idx)
    {
        for (int i = start_idx; i <= end_idx; i++)
        {
            printf("%s",lines[line_id].stations[i]);
            if(i<end_idx) printf("->");
        }
    }
    else
    {
        for (int i = start_idx; i >= end_idx; i--)
        {
            printf("%s",lines[line_id].stations[i]);
            if(i>end_idx) printf("->");
        }
    }
    printf(")\n");
}


//查询路径
void find_path(const char *start, const char *end)  //参数 起点名字 终点名字
{
    int start_lines[MAX_LINES], start_cnt; //起点所包含线路id，start_cnt 计数线路条数 ，例如天空岛有一号线和二号线，这个数就为2
    int end_lines[MAX_LINES], end_cnt;//终点所包含线路
    
    find_lines_by_station(start, start_lines, &start_cnt);  //查找起点所在线路 和 存在的线路条数
    find_lines_by_station(end, end_lines, &end_cnt);

    //情况一 站点不存在于任何当前线路
    if (start_cnt==0||end_cnt==0)
    {
        printf("所输站点中找不到对应线路\n");
        return;
    }
    
    //情况二 站点位于同一条线路，例如皆为一号线
    for (int i = 0; i < start_cnt; i++)  
    {
        if (line_contains_station(start_lines[i],end)) //end和start有公共线路
        {
            printf("无需换乘，直接乘坐 %s\n", lines[start_lines[i]].name);
            print_segment(start_lines[i], start, end);
            return;
        }
    }

    //情况三 一次换乘
    for (int i = 0; i < start_cnt; i++)  //遍历开始站点的线路
    {
        int l_start = start_lines[i];    //临时存放线路id
        for (int j = 0; j < end_cnt; j++)
        {
            int l_end = end_lines[j];
            if (transfer[l_start][l_end][0] != '\0')//换乘表判断有无公共站点
            {
                char *transfer_station = transfer[l_start][l_end];//指针接收公共站点名字字符串
                print_segment(l_start,start,transfer_station);
                printf("在%s换乘\n",transfer_station);
                print_segment(l_end,transfer_station,end);
            }
            
        }
        


    }
    



}










int main(int argc, char const *argv[])
{

    line_init_data();  //线路初始化，目前一条线路
    //printf_lines(0);  //打印一号线路

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

    find_path("蒙德城","沉玉谷");

    return 0;
}
