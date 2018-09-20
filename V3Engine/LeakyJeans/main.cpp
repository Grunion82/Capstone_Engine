#include <stdio.h>
#include <V3Engine.h>


int main(int argc, char* argv[]) {

	printf("Hello World-- Main\n");

	V3Engine::GetInstance()->speak();

	getchar();
	return 0;
}