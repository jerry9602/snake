#include "snake.h"

int main()
{
	char ch;
	initial();
	
	while(1)
	{
		clear();
		mvprintw(5,15,"menu");
		mvprintw(6,15,"1.start game");
		mvprintw(7,15,"2.quit");
		//mvprintw(16,16,"%l",delay.tv_nsec);
		ch = getch();
		switch(ch)
		{
			case '1':
				start_game();
				break;
			case '2':
				endwin();
				exit(0);
			default:
				break;
		}
	}
}