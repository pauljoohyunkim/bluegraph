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
        if (i < window->startIndex) continue;

        if (i + 2 < window->startIndex + window->height)
        {
            if (window->selectedIndex == i)
            {
                wattron(window->win, A_BOLD);
                mvwprintw(window->win, i + 1 - window->startIndex, 1, window->bdaddr[i]);
                wattroff(window->win, A_BOLD);
            }
            else
            {
                mvwprintw(window->win, i + 1 - window->startIndex, 1, window->bdaddr[i]);
            }
        }
        
    }
    wrefresh(window->win);
}

void redrawRecipients(BluegraphWindow window)
{
    wclear(window->win);
    box(window->win, 0, 0);
    for (size_t i = 0; i < window->nBdaddr; i++)
    {
        if (i < window->startIndex) continue;

        if (i + 2 < window->startIndex + window->height)
        {
            if (window->selectedIndex == i)
            {
                wattron(window->win, A_BOLD);
                mvwprintw(window->win, i + 1 - window->startIndex, 1, window->bdaddr[i]);
                wattroff(window->win, A_BOLD);
            }
            else
            {
                mvwprintw(window->win, i + 1 - window->startIndex, 1, window->bdaddr[i]);
            }
        }
    }
    wrefresh(window->win);
}

void navigateRecipients(BluegraphWindow window, BluegraphStorage storage)
{
    int ch;
    char *bdaddr_dir = NULL;
    char compressedBDAddr[12] = { 0 };
    char bdaddr[18] = { 0 };

    while ((ch = getch()) != 'q')
    {
        switch (ch)
        {
            case KEY_DOWN:
                // Moving selection
                if (window->selectedIndex + 1 != window->nBdaddr)
                {
                    window->selectedIndex++;
                }
                // Scrolling
                if (window->selectedIndex > window->startIndex + window->height - 3)
                {
                    window->startIndex++;
                }
                redrawRecipients(window);
                break;
            case KEY_UP:
                // Moving selection
                if (window->selectedIndex != 0)
                {
                    window->selectedIndex--;
                }
                // Scrolling
                if (window->selectedIndex < window->startIndex)
                {
                    window->startIndex--;
                }
                redrawRecipients(window);
                break;
            case '\n':
                // Temporarily exit ncurses
                def_prog_mode();
                endwin();

                // Load chat and interact
                bdaddr_dir = calloc(strlen(storage->dir) + 2 + sizeof(compressedBDAddr), sizeof(char));
                strcpy(bdaddr, window->bdaddr[window->selectedIndex]);
                stringAddress2CompressedBDAddress(compressedBDAddr, bdaddr);

                strcpy(bdaddr_dir, storage->dir);
                strcat(bdaddr_dir, "/");
                strcat(bdaddr_dir, compressedBDAddr);
                dumpChat(bdaddr_dir);
                free(bdaddr_dir);

                // Restore ncurses
                reset_prog_mode();
                refresh();
                break;
            default:
                break;
        }
    }
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