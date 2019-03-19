#include "MainMenu.h"

#include "Jeans.h"

MainMenu::MainMenu() {
	Name = "Main Menu";
	AddGameObject(new Jeans());
}


MainMenu::~MainMenu() {

}
