#include "Jeans.h"

#include <stdio.h>

Jeans::Jeans() : GameObject("Jeans") {

}

Jeans::~Jeans(){

}

void Jeans::Update(float deltaTime) {
	printf("Hello I am Jeans\n");
}
