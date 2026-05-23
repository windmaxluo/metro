#ifndef LINE_H
#define LINE_H

#define MAX_STATIONS_PER_LINE 50  //线路最大站点数量
#define NAME_MAX_LEN 20            //线路，站点名字最大长度
#define MAX_LINES 10              //最大线路数量

typedef struct {
    int id;
    char name[NAME_MAX_LEN];
    int station_count;
    char stations[MAX_STATIONS_PER_LINE][NAME_MAX_LEN];
} Line;

extern Line lines[MAX_LINES];
extern int line_count;

void line_init_data(void);
void printf_lines(int id);  //打印单条线路
int line_contains_station(int line_id, const char *station);   //线路是否包含某个站点  0无1有
int get_station_index(int line_id, const char *station);       //获取某个站点在线路的下标
void find_lines_by_station(const char *station, int line_ids[], int *count);// 查找包含某个站点的所有线路

#endif

