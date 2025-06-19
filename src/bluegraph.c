#include <ncurses.h>
#include "tui.h"

int main()
{
	WINDOW *lwin = NULL;
	WINDOW *rwin_up = NULL;
	WINDOW *rwin_down = NULL;
	bluegraph_initialize_tui(&lwin, &rwin_up, &rwin_down);

	wgetch(lwin);
	bluegraph_end_tui(lwin, rwin_up, rwin_down);
	return 0;
}