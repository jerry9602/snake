#ifndef _SNAKE__H
#define _SNAKE__H

#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>
#include <unistd.h>

static int m = 0;
struct snake
{
	int node_x;
	int node_y;
	int direction;
	struct snake * prev;
	struct snake * next;
};

struct food                        //食物位置
{
	int food_x;
	int food_y;
};

struct save                        // 当需要在蛇尾添加节点时，保存前一个节点的位置
{
	int x;
	int y;
};

void initial();
int Get_Rand_Number(int k);                                               //取得随机数
struct food * New_Food(void);                                             //产生随机食物
struct snake * snake_init(void);                                          //初始化蛇身
void show(struct snake * head);                                           //显示蛇身
struct save * move_body(struct snake * head);                             //移动蛇身
void insert_node(struct snake * head,struct save * sp);                   //插入节点
int search_node(struct snake * head);                                     //查找节点
int is_alive(struct snake * head);                                        //判断蛇是否死亡
void delete_snake(struct snake * head);                                   //删除蛇
void start_game(void);
void dire_choose(int ch,struct snake * head);                             //方向选择
struct save * coord_change(struct snake * head);                  //蛇头坐标变换
void draw_ui(void);                                                       //游戏ui

#endif