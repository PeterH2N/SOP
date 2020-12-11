
#include "projection.hpp"
#include "raymarching.hpp"

int main()
{
	bool exit = false;
	while (!exit)
	{
		exit = mainLoopRM();
		if (exit)
			break;
		exit = mainLoopProj();
	}
	return 0;
}