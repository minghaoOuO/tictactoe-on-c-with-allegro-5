#include "board.hpp"
#include <allegro5/allegro.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>

// some useful arrays, for determining board situation
int win[] = { 123, 456, 789, 147, 258, 369, 159, 357 };	// win lines
int player_turn[] = { 1, 2 };	// players record
int neighbor[] = {24, 135, 26, 157, 2468, 359, 48, 579, 68};	// neighbors on boards
int setPoint;	// record the best steps
int inmap[] = { 0, 3, 6, 1, 4, 7, 2, 5, 8 };	// mapping keycode to right number on boards

BOARD::BOARD() {
	for (int i = 0; i < 9; i++) {
		board[i] = 0;
	}
	cur_player = 1;
}

// get keyboard input from player
int BOARD::playerset() {
	// allegro variables
	ALLEGRO_EVENT Event;
	ALLEGRO_EVENT_QUEUE* EventQueue = NULL;

	// allegro initialize
	EventQueue = al_create_event_queue();
	if (EventQueue == NULL)
	{
		return -1;
	}

	if (!al_install_keyboard()) ///setup the keyboard
	{
		return -1;
	}
	///Tell allegro to get events from the keyboard
	al_register_event_source(EventQueue, al_get_keyboard_event_source()); 
	int keycode = -100;
	// get the keyboard input
	while (!isValid(keycode)) {
		al_wait_for_event(EventQueue, &Event);
		if (Event.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if (Event.keyboard.keycode >= 28 && Event.keyboard.keycode <= 36)
			{
				keycode = Event.keyboard.keycode - 28;	// map to 0 - 8
			}
			keycode = inmap[keycode];	// map to right numbers on board
		}
	}

	//al_rest(1.0);
	al_destroy_event_queue(EventQueue);
	cur_player = 1;
	std::cout << "PLAYER turn: " << keycode << std::endl;
	return keycode;
}

// return the player now
int BOARD::getplayer() {
	return cur_player;
}

// computer automatically set
int BOARD::comset(int mode) {
	int computerSet;
	cur_player = 2;
	if (mode == 1) {	// mode 1 : random set
		computerSet = random();
	}
	else {				// mode 2 : smarter automatically set
		computerSet = autoset();
	}
	std::cout << "COM turn: " << computerSet << std::endl;
	return computerSet;
}

// random set
int BOARD::random()
{
	int set = -100;

	while (!isValid(set)) {
		set = rand() % 9;
	}
	return set;
}

// smarter computer set
int BOARD::autoset() {
	findlosestep();
	if (losestep.empty()) {
		setPoint = -1;
		alphabeta(*this, -100000, 100000, 5, 1, 1);
		return setPoint;
	}
	else {
		return losestep[0];
	}
	return random();
}

// calculate the score of the board now
int BOARD::getvalue(BOARD nowstate) {
	int value = 0;
	for (int i = 0; i < 9; i++) {
		if (nowstate.board[i] == 0) continue;
		if (nowstate.board[i] == 1) { // O
			if (i == 4) value -= 100;
			if (nowstate.board[neighbor[i] / 1000] == 1) value -= 5;
			if (nowstate.board[neighbor[i] / 100 % 10] == 1) value -= 5;
			if (nowstate.board[neighbor[i] / 10 % 10] == 1) value -= 5;
			if (nowstate.board[neighbor[i] % 10] == 1) value -= 5;
		}
		else {	// X
			if (i == 4) value += 100;
			if (nowstate.board[neighbor[i] / 1000] == 2) value += 5;
			if (nowstate.board[neighbor[i] / 100 % 10] == 2) value += 5;
			if (nowstate.board[neighbor[i] / 10 % 10] == 2) value += 5;
			if (nowstate.board[neighbor[i] % 10] == 2) value += 5;
		}
	}
	return value;
}

// using alpha beta pruning to search the best step
int BOARD::alphabeta(BOARD nowstate, int alpha, int beta, int depth, bool maximize, bool setenable) {
	if (nowstate.checkwin() == 1) return -90000;
	else if (nowstate.checkwin() == 2) return 90000;
	if (depth == 0) return getvalue(nowstate);
	
	int value;
	if (maximize)
	{
		value = -100000;
		for (int i = 0; i < 9; i++)
		{
			if (!nowstate.isValid(i)) continue;
			BOARD nextstate = nowstate;
			nextstate.set(i, player_turn[maximize]);
			value = std::max(value, alphabeta(nextstate, depth - 1, alpha, beta, false, false));
			if (value >= alpha && setenable && setPoint != 4)
			{
				std::cout << "i: " << i << std::endl;
				setPoint = i;
			}
			alpha = std::max(alpha, value);
			if (alpha >= beta)
			{
				break;
			}
		}
		return value;
	}
	else
	{
		value = 100000;
		for (int i = 0; i < 9; i++) {
			if (!nowstate.isValid(i)) continue;
			BOARD nextstate = nowstate;
			nextstate.set(i, player_turn[maximize]);
			value = std::min(value, alphabeta(nextstate, depth - 1, alpha, beta, true, false));
			beta = std::min(beta, value);
			if (alpha >= beta)
			{
				break;
			}
		}
		return value;
	}
}

// check if there is a winner
int BOARD::checkwin() {
	for (int i = 0; i < 8; i++) {
		int index = win[i];
		int idx1 = index / 100;
		int idx2 = index / 10 % 10;
		int idx3 = index % 10;
		int result = board[idx1 - 1] * board[idx2 - 1] * board[idx3 - 1];
		if (result == 1) return 1;	// player win!!
		else if (result == 8) return 2;	// com win!!
	}
	for (int i = 0; i < 9; i++) {
		if (board[i] == 0) return 0;
	}
	return -1;
}

// set a O / X on the board
void BOARD::set(int n, int player) {
	board[n] = player;
}

// check if this step is valid
int BOARD::isValid(int n) {
	if (n == -100) return 0;
	if (board[n] != 0) {
		return 0;
	}
	else return 1;
}

BOARD::BOARD(BOARD& a)
{
	for (int i = 0; i < 9; i++) {
		board[i] = a.board[i];
		cur_player = a.cur_player;
	}
}

// find all the lose steps
void BOARD::findlosestep() {
	losestep.clear();
	for (int i = 0; i < 9; i++) {
		BOARD temp = *this;
		if (temp.isValid(i)) {
			temp.set(i, 1);
			if (temp.checkwin() == 1) {
				losestep.push_back(i);
			}
		}
	}
}