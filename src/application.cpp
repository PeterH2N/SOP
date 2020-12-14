
#include "projection.hpp"
#include "raymarching.hpp"

int main()
{
	// boolean der fort�ller om programmet skal lukkes, eller om der blot skal skiftes mellem vinduer.
	bool exit = false;
	while (!exit)
	{
		// mainLoopRM er raymarching loopet, som returnerer 0 hvis programmet skal forts�tte, og 1 hvis det skal afsluttes.
		exit = mainLoopRM();
		if (exit)
			break;
		// Det samme g�r loopet for projektions delen.
		exit = mainLoopProj();
	}
	return 0;
}