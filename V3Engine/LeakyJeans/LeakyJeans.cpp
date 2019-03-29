#include "LeakyJeans.h"

#include "MainMenu.h"

LeakyJeans::LeakyJeans() {
	Name = "Leaky Jeans";
	AddScene(new MainMenu());
}


LeakyJeans::~LeakyJeans() {

}
