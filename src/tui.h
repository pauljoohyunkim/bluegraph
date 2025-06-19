#ifndef __TUI_H__
#define __TUI_H__

#include <ncurses.h>

// TUI: One large LHS box. Two RHS boxes where the second one is on the bottom for messaging.

void bluegraph_initialize_tui(WINDOW **lwin, WINDOW **rwin_up, WINDOW **rwin_down);
void bluegraph_end_tui(WINDOW *lwin, WINDOW *rwin_up, WINDOW *rwin_down);

#define MARGIN_L 5
#define MARGIN_R 5
#define MARGIN_T 5
#define MARGIN_B 5
#define MARGIN_BETWEEN_HORIZONTAL 5
#define MARGIN_BETWEEN_VERTICAL 1
#define RWIN_MESSAGE_BOX_HEIGHT 4


#endif