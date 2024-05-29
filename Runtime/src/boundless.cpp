#include "BoundlessEditor.h"

#include "Profiler.h"

int main() {
	BoundlessEditor app;
	int res =  app.Run();

	if(res == 0)
		Profiler::printResults();
	
	return res;
}