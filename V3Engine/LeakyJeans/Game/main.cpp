#include <stdio.h>
#include <Engine/V3Engine.h>

#include <Engine/Rendering/2D/Image.h>
#include "LeakyJeans.h"

int main(int argc, char* argv[]) {
	V3Engine::GetInstance()->speak();
	GameInterface* LJ = new LeakyJeans();

	V3Engine::GetInstance()->NewGame(LJ);
	V3Engine::GetInstance()->Run();

	getchar();
	return 0;
}