#include "LeakyJeans.h"

#include "Scenes/MainMenu.h"
#include "Scenes/Game.h"
#include "Scenes/OptionsMenu.h"

LeakyJeans::LeakyJeans() {
	Name = "Leaky Jeans";
	//AddScene(new MainMenu());
	//AddScene(new Game());
	AddScene(new OptionsMenu());

}


LeakyJeans::~LeakyJeans() {

}
