#include <ncurses.h>
#include "tui.h"

int main()
{
	BluegraphWindow windows = NULL;
	windows = bluegraph_initialize_tui();

	getch();
	bluegraph_end_tui(windows);
	return 0;
}