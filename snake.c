#include "snake.h"

void initial()
{
	initscr();
	cbreak();
	nonl();
	noecho();
	curs_set(0);
	intrflush(stdscr,false);
	nodelay(stdscr, TRUE);            //使程序在没有按键按下时，仍能运行
	keypad(stdscr,true);
	refresh();
}

int Get_Rand_Number(int k)
{
	srand(time(0));
	return rand()%k+2;
}

struct food * New_Food(void)
{
	struct food * newfood = malloc(sizeof(struct food));
	newfood->food_x = Get_Rand_Number(18);
	newfood->food_y = Get_Rand_Number(63);
	
	return newfood;
}
void draw_ui(void)
{
	int i;
	for (i = 1; i <= 70; i++)  
        mvaddch(1, i, '*');  
    for (i = 1; i <= 25; i++)  
        mvaddch(i, 1, '*');  
    for (i = 70; i >= 1; i--)  
        mvaddch(25, i, '*');  
    for (i = 25; i >= 1; i--)  
        mvaddch(i, 70, '*'); //box(stdscr,'*','*');  
    
	mvprintw(4,75,"score: %d",m);
	mvprintw(5,75,"q to quit");
}
struct snake * snake_init(void)
{
	struct snake * head = malloc(sizeof(struct snake));
	struct snake * node = malloc(sizeof(struct snake));
	if(head == NULL || node == NULL)
	{
		mvprintw(20,20,"init failed!\n");
		exit(0);
	}
	head->direction = 1;
	
	head->node_x = 20;
	head->node_y = 20;
	node->node_x = 21;
	node->node_y = 20;
	
	head->prev = node;
	head->next = node;
	node->prev = head;
	node->next = head;
	
	return head;	
}

void show(struct snake * head)
{
	mvprintw(head->node_x,head->node_y,"@");
	struct snake * p = head;
	while(p->next != head)
	{
		mvprintw(p->next->node_x,p->next->node_y,"+");
		p = p->next;
	}
}

struct save * move_body(struct snake * head)
{
	struct snake * p = head;
	struct save * sp = malloc(sizeof(struct save));
	int a,b;
	a = p->node_x;                                 //保存头结点位置
	b = p->node_y;
	while(p->next != head)
	{
		sp->x = p->next->node_x;                    //保存蛇身第一个节点的位置
		sp->y = p->next->node_y;
		p->next->node_x = a;                        //改变第一个节点的位置
		p->next->node_y = b;
		a = sp->x;
		b = sp->y;
		p = p->next;
	}
	return sp;
}

void insert_node(struct snake * head,struct save * sp)
{
	struct snake * new = malloc(sizeof(struct snake));
	new->node_x = sp->x;
	new->node_y = sp->y;
	struct snake * p = head;
	new->next = p;
	new->prev = p->prev;
	p->prev = new;
	new->prev->next = new;
}

int search_node(struct snake * head)
{
	struct snake * p = head;
	while(p->next != head)
	{
		if((head->node_x==p->next->node_x) && (head->node_y==p->next->node_y))
		{
			return 1;
		}
		p = p->next;
	}
	return 0;
}

int is_alive(struct snake * head)
{
	int flag = head->node_y >= 70 || head->node_y <= 1 || head->node_x >= 25 || head->node_x <= 1;
	
	if(flag || search_node(head))                     //在框外或碰到自己就死
		return 0;
	else
		return 1;
}

void delete_snake(struct snake * head)
{
	struct snake * p;
	while(head->next != head)
	{
		p = head->next;
		head->next = p->next;
		p->next->prev = head;
		free(p);
	}
	free(head);
}

void dire_choose(int ch,struct snake * head)
{
		switch(ch)
		{
			case KEY_UP:
			case 'w':                                          //改变蛇的方向
				
				if(head->direction == 3 || head->direction == 4)
					head->direction = 1;
				else if(head->direction == 2)
					head->direction = 2;
				break;
			case KEY_DOWN: 
			case 's':
				if(head->direction == 3 || head->direction == 4)
					head->direction = 2;
				else if(head->direction == 1)
					head->direction = 1;
                break;
            case KEY_RIGHT:
			case 'd':
				if(head->direction == 1 || head->direction == 2)
					head->direction = 4;
				else if(head->direction == 3)
					head->direction = 3;
				break;
            case KEY_LEFT: 
			case 'a':
				if(head->direction == 1 || head->direction == 2)
					head->direction = 3;
				else if(head->direction == 4)
					head->direction = 4;
                break;
			case 'p':
				while(getch() != 'p');
                break;
			case 'q':
				endwin();
				exit(0);
			default:
				break;
		}
}

struct save * coord_change(struct snake * head)
{
	struct save * sp;
	if(head->direction == 1)                   //判断蛇的方向，从而决定蛇的移动
	{
		sp = move_body(head);
		head->node_x--;
	}
	else if(head->direction == 2)
	{
		sp = move_body(head);
		head->node_x++;
	}
	else if(head->direction == 3)
	{
		sp = move_body(head);
		head->node_y--;
	}
	else if(head->direction == 4)
	{
		sp = move_body(head);
		head->node_y++;
	}
	return sp;
}
void start_game(void)
{
	int ch;
	int i;
	struct snake * head = snake_init();
	struct save * sp;
	int m = 0;
	struct timespec ts;
	struct timespec dummy;
	ts.tv_nsec = 100000000;
	//dummy.tv_nsec = 1000000;
	clear();
	
	show(head);
	struct food * newfood = New_Food();
	while(is_alive(head)&&m != 15)
	{
		draw_ui();
        
		mvprintw(newfood->food_x,newfood->food_y,"#");
		//while(ch = getch()!='\n');
		ch = getch();
		dire_choose(ch,head);
		sp = coord_change(head);
		clear();
		show(head);	
		nanosleep(&ts, NULL);
		
		
		//sleep(2);
		
		if((head->node_x == newfood->food_x) && (head->node_y == newfood->food_y))           //碰到食物就增加节点
		{
			newfood = New_Food();
			insert_node(head,sp);
			m++;
		}
		free(sp);
	}
	if(m == 15)                                                                         //吃到15个食物就取得胜利
		mvprintw(15,25,"Congratulation! you are winner!");
	else
		mvprintw(15,25,"Game Over!");
	mvprintw(16,25,"please entr r to return");
	delete_snake(head);
	
	while(ch != 'r')
	{
		ch = getch();
	}
}

