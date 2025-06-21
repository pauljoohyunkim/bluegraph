#ifndef __TUI_H__
#define __TUI_H__

#include <ncurses.h>

// TUI : Recipient Selection Window, so a single window with a list of bdaddr,
// and a single-row box for help.
// Rest of the messaging will be done in console.

#define MARGIN_L 5
#define MARGIN_R 5
#define MARGIN_T 5
#define MARGIN_B 5
#define MARGIN_BETWEEN_VERTICAL 1
#define HELPBOX_HEIGHT 3

typedef struct
{
    WINDOW *win;
    WINDOW *helpbar;
    int height;
    int width;
    int y;
    int x;
    
    char *bdaddr[18];
    size_t nBdaddr;
    size_t capacity;
    int startIndex;
} BluegraphWindow_st;

typedef BluegraphWindow_st *BluegraphWindow;

BluegraphWindow bluegraph_initialize_tui();
void bluegraph_end_tui(BluegraphWindow windows);


#endif