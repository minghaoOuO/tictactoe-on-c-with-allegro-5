#ifndef game_hpp
#define game_hpp

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include "board.hpp"


class game {
public:
	game();
	void start();	// start scene
	void initial();	// game initialize
	void terminate();	// close the game
	void play();	// the play scene
	ALLEGRO_DISPLAY* display;	// allegro variables
	ALLEGRO_BITMAP* background;
	int curplayer; // 1: player 2: computer
	BOARD curgame; // now game state
	int mode;	// 1 : random , 2 : smartet automatically choose
};
#endif
