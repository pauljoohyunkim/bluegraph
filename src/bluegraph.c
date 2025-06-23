#include "conn.h"
#include "storage.h"

int main()
{
	BluegraphStorage storage = NULL;
	storage = bluegraph_load_storage();
	startServer(storage);
	
	freeBluegraphStorage(storage);
	return 0;
}