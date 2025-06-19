#include <ncurses.h>
#include "tui.h"

void bluegraph_initialize_tui(WINDOW **lwin, WINDOW **rwin_up, WINDOW **rwin_down)
{
    int ch;
    int terminal_x = 0;
    int terminal_y = 0;
    int box_len = 0;
    int box_height = 0;
    int rbox_up_height = 0;
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

    *lwin = newwin(box_height, box_len, MARGIN_T, MARGIN_L);
    *rwin_up = newwin(rbox_up_height, box_len, MARGIN_T, MARGIN_L + box_len + MARGIN_BETWEEN_HORIZONTAL);
    *rwin_down = newwin(RWIN_MESSAGE_BOX_HEIGHT, box_len, MARGIN_T + rbox_up_height + MARGIN_BETWEEN_VERTICAL, MARGIN_L + box_len + MARGIN_BETWEEN_HORIZONTAL);
	refresh();

    box(*lwin, 0, 0);
    box(*rwin_up, 0, 0);
    box(*rwin_down, 0, 0);
    wrefresh(*lwin);
    wrefresh(*rwin_up);
    wrefresh(*rwin_down);
}

void bluegraph_end_tui(WINDOW *lwin, WINDOW *rwin_up, WINDOW *rwin_down)
{
    delwin(lwin);
    delwin(rwin_up);
    delwin(rwin_down);
    
    endwin();
}