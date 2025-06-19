#include <ncurses.h>
#include "tui.h"

int main()
{
	WINDOW *lwin = NULL;
	WINDOW *rwin_up = NULL;
	WINDOW *rwin_down = NULL;
	BluegraphWindows windows = NULL;
	windows = bluegraph_initialize_tui();

	getch();
	bluegraph_end_tui(windows);
	return 0;
}