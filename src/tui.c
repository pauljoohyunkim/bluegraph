#include <stdlib.h>
#include <ncurses.h>
#include "tui.h"

BluegraphWindows bluegraph_initialize_tui()
{
    int ch;
    int terminal_x = 0;
    int terminal_y = 0;
    int box_len = 0;
    int box_height = 0;
    int rbox_up_height = 0;
    BluegraphWindows windows = NULL;
    
    windows = calloc(1, sizeof(BluegraphWindows_st));
    if (!windows)
    {
        fprintf(stderr, "Could not allocate memory for creating windows...\n");
        return NULL;
    }

    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();

    // Get terminal size.
    getmaxyx(stdscr, terminal_y, terminal_x);
    box_len = terminal_x - MARGIN_L - MARGIN_R - MARGIN_BETWEEN_HORIZONTAL;
    box_len /= 2;
    box_height = terminal_y - MARGIN_T - MARGIN_B;

    rbox_up_height = box_height - MARGIN_BETWEEN_VERTICAL - RWIN_MESSAGE_BOX_HEIGHT;

    windows->lwin = newwin(box_height, box_len, MARGIN_T, MARGIN_L);
    windows->rwin_up = newwin(rbox_up_height, box_len, MARGIN_T, MARGIN_L + box_len + MARGIN_BETWEEN_HORIZONTAL);
    windows->rwin_down = newwin(RWIN_MESSAGE_BOX_HEIGHT, box_len, MARGIN_T + rbox_up_height + MARGIN_BETWEEN_VERTICAL, MARGIN_L + box_len + MARGIN_BETWEEN_HORIZONTAL);
	refresh();

    box(windows->lwin, 0, 0);
    box(windows->rwin_up, 0, 0);
    box(windows->rwin_down, 0, 0);
    wrefresh(windows->lwin);
    wrefresh(windows->rwin_up);
    wrefresh(windows->rwin_down);
    refresh();

    return windows;
}

void bluegraph_end_tui(BluegraphWindows windows)
{
    if (!windows) return;

    delwin(windows->lwin);
    delwin(windows->rwin_up);
    delwin(windows->rwin_down);
    
    endwin();
}