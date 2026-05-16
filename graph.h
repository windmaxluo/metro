#ifndef GRAPH_H
#define GRAPH_H

#include "line.h"  // 需要使用 MAX_LINES 和 NAME_MAX_LEN

// 换乘关系表：transfer[源线路][目标线路] = 换乘站名
// 如果为空字符串 "" 表示两条线路不能直接换乘
extern char transfer[MAX_LINES][MAX_LINES][NAME_MAX_LEN];

// 自动构建换乘图：遍历所有线路，找出公共站点，填入 transfer 表
void build_transfer_graph(void);

#endif