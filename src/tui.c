#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include "tui.h"

BluegraphWindow bluegraph_initialize_tui()
{
    int ch;
    int terminal_x = 0;
    int terminal_y = 0;
    int box_len = 0;
    int box_height = 0;
    BluegraphWindow windows = NULL;
    
    windows = calloc(1, sizeof(BluegraphWindow_st));
    if (!windows)
    {
        fprintf(stderr, "Could not allocate memory for creating windows...\n");
        return NULL;
    }

    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);

    // Get terminal size.
    getmaxyx(stdscr, terminal_y, terminal_x);
    windows->height = terminal_y - MARGIN_T - MARGIN_B - 2 - MARGIN_BETWEEN_VERTICAL - HELPBOX_HEIGHT;
    windows->width = terminal_x - MARGIN_L - MARGIN_R;

    // Create windows
    windows->win = newwin(windows->height, windows->width, MARGIN_T, MARGIN_L);
	refresh();

    box(windows->win, 0, 0);
    wrefresh(windows->win);
    refresh();

    // Create help bar
    windows->helpbar = newwin(HELPBOX_HEIGHT, windows->width, MARGIN_T + windows->height + MARGIN_BETWEEN_VERTICAL, MARGIN_L);
    refresh();

    box(windows->helpbar, 0, 0);
    //mvwprintw(windows->helpbar, 1, 1, "Help: ");
    wrefresh(windows->helpbar);

    return windows;
}

void loadRecipients(BluegraphWindow window, BluegraphStorage storage)
{
    window->nBdaddr = storage->chatdirs->size;
    window->bdaddr = calloc(window->nBdaddr, sizeof(char *));
    for (size_t i = 0; i < window->nBdaddr; i++)
    {
        window->bdaddr[i] = calloc(18, sizeof(char));
        compressedBDAddress2StringAddress(window->bdaddr[i], storage->chatdirs->filenames[i]);
        if (window->startIndex + i + 2 < window->height)
            mvwprintw(window->win, i + 1, 1, window->bdaddr[i + window->startIndex]);
    }
    wrefresh(window->win);
}

void redrawRecipients(BluegraphWindow window)
{
    wclear(window->win);
    box(window->win, 0, 0);
    for (size_t i = 0; i < window->nBdaddr; i++)
    {
        if (window->startIndex + i + 2 < window->height)
            mvwprintw(window->win, i + 1, 1, window->bdaddr[i + window->startIndex]);
    }
    wrefresh(window->win);
}

void freeRecipients(BluegraphWindow window)
{
    for (size_t i = 0; i < window->nBdaddr; i++)
    {
        free(window->bdaddr[i]);
    }
    free(window->bdaddr);
}

void bluegraph_end_tui(BluegraphWindow windows)
{
    if (!windows) return;

    delwin(windows->win);
    
    endwin();
}