#ifndef BOARD_H_
#define BOARD_H_
// standard template library 
#include<iostream>
#include <ctime>
#include<list>
#include<vector>
#include <stack>
#include <algorithm>
#include <time.h>
#include "Neighbour.h"



//#include<unordered_map>
using namespace std;

class Board {
private:
	int row, col;
	int **grid; // 1 = black; -1 = white; 0 = empty
	list<int> left;

public:
	//based on the board size number input, creat a empty dynamic array
	Board(int r, int c) {
		row = r;
		col = c;
		//dynamically allocate a 2D grid
		grid = new int*[row];	// assign grid as arrays of an arrays -> 2D arrays
		for (int i = 0; i < row; i++)
			grid[i] = new int[col];

		for (int i = 0; i < row; i++)
			for (int j = 0; j < col; j++)
				grid[i][j] = 0;

		for (int i = 0; i < row*col; i++) {
			left.push_back(i);	// add an elements to the end of the list container.
		}

	}
	//Board destructors, 
	// deallocate memory of a class object,
	// destructor is called when the object goes out of scope
	~Board() {
		for (int i = 0; i < row; i++)
			delete[] grid[i];

		delete[] grid; // delete operator
	}

	Board(Board& cboard) {
		row = cboard.row;
		col = cboard.col;

		grid = new int*[row];
		for (int i = 0; i < row; i++)
			grid[i] = new int[col];

		for (int i = 0; i < row; i++)
			for (int j = 0; j < col; j++)
				grid[i][j] = cboard.grid[i][j];
	}

	int player = 1;
	void addMove(int player, int x, int y);
	void capture(int player, int x, int y, vector<NeighbourCell> neighbors);
	vector<vector<int>> getCombinations(int N, int K);
	bool isValidMove(int x, int y);
	void randMove(int& x, int& y);
	int checkWin();
	vector<NeighbourCell> checkNeighbors(int, int);
	void printBoard();
	bool boardFull();
	void play();
	void randomPlay();
};
// checks the winner
int Board::checkWin() {
	// declare player 1 and player 2
	// currently there's the board is empty, no player moves has been placed
	int p1 = 0;
	int p2 = 0;
	for (int i = 0; i<row; i++) {
		for (int j = 0; j<col; j++) {
			// if player 1 place a stone on the board
			if (grid[i][j] > 0) {
				p1++;	// player 1 increment by 1
			}
			// if player 2 place a stone on the board
			if (grid[i][j]< 0) {
				p2++;	// player 2 increment by -1
			}
		}
	}
	// checks total number of a die
	if (p1 > p2) {
		return 1;	// if player 1 has more die on the board return player 1
	}
	else {
		return 2;	// if player 2 has more die on the board return player 2
	}

}

bool Board::isValidMove(int x, int y) {
	// check the cell is occupied or not
	// return false if cell is not empty
	if (x<0 || y<0 || x >= row || y >= col || grid[x][y] != 0)
	{
		return false;
	}
	else {
		// return true if cell is empty
		return true;
	}
}
// this function is simply return a vector of neighbour cells which contains dice in it
vector<NeighbourCell> Board::checkNeighbors(int x, int y) {
	vector<NeighbourCell> neighbors;

	// checking cell below
	if (x + 1 < row && grid[x + 1][y] != 0) {
		NeighbourCell n;
		n.first = "Down";
		n.x = x + 1;
		n.y = y;
		n.pipCount = grid[x + 1][y];
		neighbors.push_back(n);
	}
	// checking cell above
	if (x - 1 >= 0 && grid[x - 1][y] != 0) {
		NeighbourCell n;
		n.first = "Up";
		n.x = x - 1;
		n.y = y;
		n.pipCount = grid[x - 1][y];
		neighbors.push_back(n);
	}
	// checks cell to the right
	if (y + 1 < col && grid[x][y + 1] != 0) {
		NeighbourCell n;
		n.first = "Right";
		n.x = x;
		n.y = y + 1;
		n.pipCount = grid[x][y + 1];
		neighbors.push_back(n);
	}
	// checking cell  to the left
	if (y - 1 >= 0 && grid[x][y - 1] != 0) {
		NeighbourCell n;
		n.first = "Left";
		n.x = x;
		n.y = y - 1;
		n.pipCount = grid[x][y - 1];
		neighbors.push_back(n);
	}

	int total = 0;
	cout << "Neighbor information: " << endl;
	for (int i = 0; i < neighbors.size(); i++) {
		cout << neighbors[i].first << endl;
		cout << neighbors[i].pipCount << endl;
		total = total + abs(neighbors[i].pipCount);	// add the absolute value to tatal pipCount
	}
	if (total>0) {

		cout << "total pip counts = " << total << endl;
	}
	return neighbors;
}



// this function is responsible to call other functions to get the neighbours
// of this move and also capture the adjacent neighbours if possible
void Board::addMove(int player, int x, int y) {
	vector<NeighbourCell> neighbors;

	neighbors = checkNeighbors(x, y);

	capture(player, x, y, neighbors);
}

// this function places the new dice in the corresponding cell 
// and to also test if the adjacent die can also be captured
void Board::capture(int player, int x, int y, vector<NeighbourCell> neighbors) {
	int k = neighbors.size();
	// if there are less than 2 neighbors
	if (k<2) {
		grid[x][y] = player;	// switch player
		return;
	}
	// starting from all the numbers of the dice, and get the combination possible
	// if not found, deduct one number and continue finding combinations as long as
	// the number of die is larger than 2
	do {
		// get all the combinations of the die surrounding this new dice, where k is 
		// the number of die in the combination
		vector<vector<int>> combinations = getCombinations(neighbors.size(), k);
		int total = 0;
		// loop through all the combination
		for (int i = 0; i < combinations.size(); i++) {
			// loop through the die in the combination and get the total
			for (int m = 0; m < combinations[i].size(); m++) {
				cout << combinations[i][m] << endl;
				total = total + abs(neighbors[combinations[i][m]].pipCount);
			}
			// if the total of the adjacent die is smaller or equal to 6
			if (total <= 6) {
				grid[x][y] = total * player;	// capturing the adjacent die
				for (int m = 0; m < combinations[i].size(); m++) {
					// reset the captured die to 0
					grid[neighbors[combinations[i][m]].x][neighbors[combinations[i][m]].y] = 0;
				}
				return;
			}
		}
		k--;
	} while (k >= 2);

	// can't capture any neighbor
	grid[x][y] = player;	// switch player
}

// this is a function to get combinations without repetiton by providing it n and k
//  where n is the total number and k is the number of items to selected.
vector<vector<int>> Board::getCombinations(int n, int k)
{
	vector<vector<int>> combinations;
	// creates a string call bitmask
	//initialises it with K instances of the char 1
	string bitmask(k, 1); // K leading 1's

						  //then resizes the string to be of size N, and fills any gaps with the char '0'
	bitmask.resize(n, 0); // N-K trailing 0's

						  // print integers and permute bitmask
	do {
		vector<int> combination;
		int o = 0;
		for (int i = 0; i < n; ++i) // [0..n-1] integers
		{
			if (bitmask[i]) {
				combination.push_back(i);
				o++;
			}
		}
		combinations.push_back(combination);
	} while (prev_permutation(bitmask.begin(), bitmask.end())); //Rearranges the elements in the range [first,last)
																// this is looping over all the permutation
	return combinations;
}

// this function generates an random move from the randomPlayer
void Board::randMove(int& x, int& y) {
	cout << "Random Player:\n";
	vector<int> emptyCells;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (grid[i][j] == 0)
				emptyCells.push_back(i * col + j);
		}
	}
	int index = rand() % emptyCells.size();
	x = emptyCells[index] / col;
	y = emptyCells[index] % col;
}

bool Board::boardFull() {
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (grid[i][j] == 0) {
				return false;
			}
		}
	}
	return true;
}
// human vs random player
void Board::randomPlay() {
	int x, y;
	int current = player;

	do {
		cout << "Game board:" << endl;
		// Player 1
		do {
			cout << "Input your move with row and column numbers:" << endl;
			cout << "Human Moves:" << endl;
			cin >> x >> y;
		} while (isValidMove(x - 1, y - 1) == false);
		addMove(current, x - 1, y - 1);
		cout << current << " makes a move (" << x << "," << y << ")\n" << endl;
		if (current == 1) {
			current = -1;
		}
		else {
			current = 1;
		}

		printBoard();

		if (boardFull()) {
			this->printBoard();
			int winner = checkWin();
			// if the last player moves is equals to 1
			if (winner == 1) {
				cout << "Congratulation Player 1 Wins" << endl;
			}
			else {
				cout << "Congratulation Random Player Wins" << endl;
			}
			break;
		}
		//Computer Random Move
		randMove(x, y);
		cout << "Random Player move (" << (x + 1) << "," << (y + 1) << ")" << endl;
		addMove(current, x, y);
		if (current == 1) {
			current = -1;
		}
		else {
			current = 1;
		}
		this->printBoard();

		if (boardFull()) {
			this->printBoard();
			int winner = checkWin();
			if (winner == 1) {
				cout << "Congratulation Player 1 Wins" << endl;
			}
			else {
				cout << "Congratulation Random Player Wins" << endl;
			}
			break;
		}

	} while (true);
}

// human vs human 
void Board::play() {
	int x, y;
	int current = player;

	do {
		cout << "Game board:" << endl;
		// Player 1
		do {
			cout << "Input your move with row and column numbers:" << endl;
			cin >> x >> y;
		} while (isValidMove(x - 1, y - 1) == false);

		cout << current << " makes a move (" << x << "," << y << ")\n" << endl;

		addMove(current, x - 1, y - 1);
		// switch player
		if (current == 1) {

			current = -1;
		}
		else {
			current = 1;
		}
		this->printBoard();

		if (boardFull()) {
			this->printBoard();
			int winner = checkWin();
			if (winner == 1) {
				cout << "Congratulation Player 1 Wins" << endl;
			}
			else {
				cout << "Congratulation Player 2 Wins" << endl;
			}
			//printStack();
			//cout << "Congratulation Player White Wins" <<endl;
			break;
		}
	} while (true);
}
// draws the board including the coordinates for both rows and columns
void Board::printBoard() {
	// labelling the coordinates for rows
	cout << "    ";
	for (int j = 0; j < col; j++) {
		cout << j + 1 << "   ";
	}
	cout << endl;

	for (int i = 0; i < row; i++) {
		cout << "  ";
		for (int j = 0; j < col; j++) {
			cout << " ---";
		}
		cout << endl;
		// labelling the coordinates for columns
		cout << (i + 1) << " |";

		for (int j = 0; j < col; j++) {
			if (grid[i][j] == 0) {
				cout << "   |";
			}
			else if (grid[i][j] > 0) {
				cout << " " << grid[i][j] << " |";
			}
			else if (grid[i][j] < 0) {
				cout << "" << grid[i][j] << " |";
			}
		}
		cout << endl;
	}
	cout << "  ";

	for (int j = 0; j < col; j++) {
		cout << " ---";
	}
	cout << endl << endl;
}

#endif /* BOARD_H_ */
