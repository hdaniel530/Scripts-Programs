//Hannah Daniel A2 9/8/17
//hd892, N16745127
//rec01.cpp
//This program implements Conway's Game of Life which is a game with zero players and the result is dependent on the initial input state of alive and dead cells.

//external libaries
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

//constant character variables to signify alive and dead
const char ALIVE = '*';
const char DEAD = ' ';

//main function of program
int main() {
	//open the file
	ifstream initialState("life.txt");

	//raise an error if file could not be opened
	if (!initialState) 
	{
		cerr << "Could not open file" << endl;
		exit(1);
	}
	//copies data from "life.txt" file into a vector
	vector <string> rows;
	string line;
	while (getline(initialState, line)) 
	{
		rows.push_back(line);
	}
	initialState.close();

	
	//creates board with border with 2D vector, starting with all dead cells; created an extra board to retain new information throughout generations
	int row = rows.size() + 2;
	int col = rows[0].length() + 2;
	vector <vector <char>> board(row, vector<char>(col, DEAD));
	vector <vector <char>> boardcopy(row, vector<char>(col, DEAD));

	//inputs data of initial state into the newly created board
	for (size_t i = 1; i < row - 1; i++) 
	{
		for (int j = 1; j < col - 1; j++) 
		{
			if (rows[i - 1][j - 1] == '-')  //replacing dashes with spaces
			{
				board[i][j] = ' ';
			}
			else 
			{
				board[i][j] = rows[i - 1][j - 1];
			}
		}
	}

	//prints initial state board
	cout << "Initial World" << endl;
	for (size_t i = 1; i < board.size() - 1; i++)
	{
		for (int j = 1; j < board[0].size() - 1; j++)
		{
			cout << board[i][j];
		}
		cout << endl;
	}
	cout << "===========================" << endl;
	
	//iterate 10 times for 10 generations, plays the game
	int gen = 1; //keeps track of the generation number
	while (gen <= 10) 
	{
		
		for (int i = 1; i < board.size() - 1; i++)
		{
			for (int j = 1; j < board[0].size() - 1; j++)
			{
				int count = 0; //variable for keeping count of the number of neighbors

				//checks 8 neighbor cells for life
				if (board[i - 1][j - 1] == ALIVE)
				{
					count++;
				}
				if (board[i - 1][j] == ALIVE)
				{
					count++;
				}
				if (board[i - 1][j + 1] == ALIVE)
				{
					count++;
				}
				if (board[i][j - 1] == ALIVE)
				{
					count++;
				}
				if (board[i][j + 1] == ALIVE)
				{
					count++;
				}
				if (board[i + 1][j - 1] == ALIVE)
				{
					count++;
				}
				if (board[i + 1][j] == ALIVE)
				{
					count++;
				}
				if (board[i + 1][j + 1] == ALIVE)
				{
					count++;
				}

				//Rules for alive and dead cells
				//Loneliness
				if ((board[i][j] == ALIVE) && (count == 0 || count == 1))
				{
					boardcopy[i][j] = DEAD;
				}
				//Lives to next gen.
				else if ((board[i][j] == ALIVE) && (count == 2 || count == 3))
				{
					boardcopy[i][j] = ALIVE;
				}
				//Death by overcrowding
				else if (board[i][j] == ALIVE && count > 3)
				{
					boardcopy[i][j] = DEAD;
				}
				//Comes back to life 
				else if (board[i][j] == DEAD && count == 3)
				{
					boardcopy[i][j] = ALIVE;
				}
			}
		}
		board = boardcopy; //copies the information from the board copy into the real board

		//prints current board
		cout << "Generation: " << gen << endl;
		for (size_t i = 1; i < board.size() - 1; i++) 
		{
			for (int j = 1; j < board[0].size() - 1; j++) 
			{
				cout << board[i][j];
			}
			cout << endl;
		}
		cout << "===========================" << endl;
		gen++;  //increments to run while loop

	}
	getchar(); //displays screen
	return 0;
}

		

		
	