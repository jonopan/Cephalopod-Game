// Cephalopod Game.cpp : Defines the entry point for the console application.

#include<iostream>

using namespace std;

#include "Neighbour.h"
#include "Board.h"

int main()
{
	int option;
	int row, col;
	srand(time(NULL));
	bool valid;

	cout << "Welcome to Cephalopod Game" << endl;
	do {
		cout << "input the Board size, say (3,3) (3,5) (5,5)" << endl;
		cin >> row >> col;

		cout << "Select 1: Human vs Human " << endl;
		cout << "Select 2: Human vs Random Player" << endl;
		cin >> option;

		Board board(row, col);

		switch (option) {
		case 1:
			cout << "Human vs Human" << endl;
			board.play();
			break;
		case 2:
			cout << "Human vs Random Player" << endl;
			board.randomPlay();
			break;
		default:
			cout << "You have entered an invalid inputs, Try again!" << endl;
			break;
		}
	} while (!valid);
	return 0;
}
