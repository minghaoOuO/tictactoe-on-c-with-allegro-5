#ifndef board_hpp
#define board_hpp

#include <vector>

// tic tac toe board class definition
class BOARD {
public:
	int board[9];	// record board OXs  
	std::vector<int> losestep;	// record which steps O will win
	
	BOARD();
	int playerset();	// get keyboard input from player
	int getplayer();	// return the player now
	int comset(int mode);	// computer automatically set
	int checkwin();		// check if there is a winner
	int cur_player;		// record current player
	void set(int n, int player);	// set a O / X on the board
	int isValid(int n);		// check if this step is valid
	BOARD(BOARD &a);
	int random();		// random set
	int autoset();		// smarter computer set 
	int getvalue(BOARD b);	// get the value of the board now
	int alphabeta(BOARD b, int alpha, int beta, int depth, bool maximize, bool setenable);	// using alpha beta pruning to search the best step
	void findlosestep();	// find all the lose steps
};

#endif board_hpp
