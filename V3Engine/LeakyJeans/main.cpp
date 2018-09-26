#include <stdio.h>
#include <Engine/V3Engine.h>

int main(int argc, char* argv[]) {
	V3Engine::GetInstance()->speak();

	getchar();
	return 0;
}