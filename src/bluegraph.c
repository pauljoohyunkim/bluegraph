#include <ncurses.h>
#include "storage.h"
#include "tui.h"

int main()
{
	BluegraphWindow window = NULL;
	BluegraphStorage storage = NULL;
	window = bluegraph_initialize_tui();
	storage = bluegraph_load_storage();

	// Load to windows
	loadRecipients(window, storage);

	getch();
	freeRecipients(window);
	bluegraph_end_tui(window);
	freeBluegraphStorage(storage);
	return 0;
}