#ifndef LINE_H
#define LINE_H

#define MAX_STATIONS_PER_LINE 50
#define NAME_MAX_LEN 20
#define MAX_LINES 10

typedef struct {
    int id;
    char name[NAME_MAX_LEN];
    int station_count;
    char stations[MAX_STATIONS_PER_LINE][NAME_MAX_LEN];
} Line;

extern Line lines[MAX_LINES];
extern int line_count;

void line_init_data(void);
void printf_lines(int id);
int line_contains_station(int line_id, const char *station);
int get_station_index(int line_id, const char *station);
void find_lines_by_station(const char *station, int line_ids[], int *count);

#endif

