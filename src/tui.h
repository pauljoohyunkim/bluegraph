#ifndef __TUI_H__
#define __TUI_H__

#include <ncurses.h>

// TUI: One large LHS box. Two RHS boxes where the second one is on the bottom for messaging.

#define MARGIN_L 5
#define MARGIN_R 5
#define MARGIN_T 5
#define MARGIN_B 5
#define MARGIN_BETWEEN_HORIZONTAL 5
#define MARGIN_BETWEEN_VERTICAL 1
#define RWIN_MESSAGE_BOX_HEIGHT 4

typedef struct
{
    WINDOW *lwin;
    WINDOW *rwin_up;
    WINDOW *rwin_down;
} BluegraphWindows_st;

typedef BluegraphWindows_st *BluegraphWindows;

BluegraphWindows bluegraph_initialize_tui();
void bluegraph_end_tui(BluegraphWindows windows);


#endif