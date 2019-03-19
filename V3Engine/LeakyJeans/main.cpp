#include <stdio.h>
#include <Engine/V3Engine.h>

#include "LeakyJeans.h"

int main(int argc, char* argv[]) {
	GameInterface* LJ = new LeakyJeans();

	V3Engine::GetInstance()->speak();

	V3Engine::GetInstance()->NewGame(LJ);
	V3Engine::GetInstance()->Run();

	getchar();
	return 0;
}