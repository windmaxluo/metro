#ifndef PATH_H
#define PATH_H

// 打印同一条线路内从 start 到 end 的路径（同一线路内）
void print_segment(int line_id, const char *start, const char *end);

// 查询最少换乘路径（主函数）
void find_path(const char *start, const char *end);

#endif