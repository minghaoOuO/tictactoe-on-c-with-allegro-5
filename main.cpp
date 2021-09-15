#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include "game.hpp"

int main(int argc, char** argv)
{
	// allegro initialize
	al_init();
	al_init_font_addon();
	// game start!
	game tictactoe;
	tictactoe.start();
	return 0;
}
