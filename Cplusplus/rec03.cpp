//Hannah Daniel 9/22/17
//hd892, N16745127
//rec03.cpp
//This program implements the game known as Minesweeper  

#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <ctime>
using namespace std;


//Cell structure, takes in position, its random number generated, if it is a bomb or not, if it has been chosen/visible, number of neighborimg bombs
struct Cell {
	int randomNum;
	int numBombs;
	bool isBomb;
	bool chosen;
	int pos1, pos2;
};


class Minesweeper {
public:
	//The constructor to initialize the game
	Minesweeper() {
		srand(time(NULL));
		//pushes Cell objects into the 2D vector
		for (size_t i = 0; i < rownum + 2; i++) {
			vector <Cell> cell;
			board.push_back(cell);
			for (size_t j = 0; j < colnum + 2; j++) {
				int random = rand() % 100;

				Cell cell;
				cell.randomNum = random;
				cell.numBombs = 0;
				cell.isBomb = false;
				cell.chosen = false;
				cell.pos1 = i;
				cell.pos2 = j;
				board[i].push_back(cell);
			}
		}

		//determines which are bombs
		for (size_t index = 1; index < board.size()-1; index++) {
			for (size_t jindex = 1; jindex < board[1].size() - 1; jindex++) {
				if (board[index][jindex].randomNum > 90)  //replacing dashes with spaces
				{
					board[index][jindex].isBomb = true;
					numBombs += 1;
					board[index - 1][jindex - 1].numBombs += 1;
					board[index - 1][jindex].numBombs += 1;
					board[index - 1][jindex + 1].numBombs += 1;
					board[index][jindex - 1].numBombs += 1;
					board[index][jindex + 1].numBombs += 1;
					board[index + 1][jindex - 1].numBombs += 1;
					board[index + 1][jindex].numBombs += 1;
					board[index + 1][jindex + 1].numBombs += 1;
					
				}
				else
				{
					board[index][jindex].isBomb = false;
				}
			}
		}
	}
	//To display the board.Take a boolean argument specifying whether to display the bombs or not.We won't want to for normal turns, but will want to at the end.
	void display(bool dis) {
		if (dis == true) {
			for (size_t rindex = 1; rindex < board.size() - 1; rindex++) {
				for (size_t cindex = 1; cindex < board[0].size() - 1; cindex++) {
					if (board[rindex][cindex].isBomb == true) {
						cout << bomb;
					}
					else if (board[rindex][cindex].chosen == true) {
						cout << board[rindex][cindex].numBombs;
					}
					else {
						cout << reg;
					}	
				}
				cout << endl;
			}
		}
		else {
			for (size_t rindex = 1; rindex < board.size() - 1; rindex++) {
				for (size_t cindex = 1; cindex < board[0].size() - 1; cindex++) {
					if (board[rindex][cindex].chosen == true) {
						cout << board[rindex][cindex].numBombs;
					}
					else {
						cout << reg;
					}
				}
				cout << endl;
			}
		}
	}
	//function to count the number of visible cells
	int numVisible() {
		visible = 0;
		for (size_t rindex = 1; rindex < board.size() - 1; rindex++) {
			for (size_t cindex = 1; cindex < board[0].size() - 1; cindex++) {
				if (board[rindex][cindex].chosen == true) {
					visible += 1;
				}
			}
		}
		return visible;
	}
	//Takes a row number.Returns true if the row number is valid, and false otherwise.	
	bool validRow(int row) {
		if ((row < 1) || (row > rownum)) {
			return false;
		}
		return true;

	}
	//Takes a column number.Returns true if the column number is valid, and false otherwise.
	bool validCol(int col) {
		if ((col < 1) || (col > colnum)) {
			return false;
		}
		return true;
	}
	//Takes a row number and a column number.Returns true if the corresponding cell is visible and false otherwise.
	bool isVisible(int row, int col) {
		if (board[row][col].chosen == true) {
			return true;
		}
		return false;
	}
	//Takes a row number and a column number, changing the state of the board as appropriate for this move.Returns false if the move results in an explosion.
	bool play(int row, int col) {
		if (board[row][col].isBomb == true) {
			game = false;
			return false;
		}
		else {
			game = true;
			if (board[row][col].numBombs == 0){
				vector <Cell> bfs;
				bfs.push_back(board[row][col]);
				//conducts a breadth first search for adjacents
				while (!bfs.empty()) {
					int s = bfs.back().numBombs;
					size_t pos1 = bfs.back().pos1;
					size_t pos2 = bfs.back().pos2;
					bfs.pop_back();
					if ((s == 0)) {
						if (board[pos1][pos2].chosen != true) {
							board[pos1][pos2].chosen = true;
							if (!(pos1 == 0 || pos1 == board.size() - 1 || pos2 == 0 || pos2 == board[0].size() - 1)) {
								for (size_t rows = pos1 - 1; rows < pos1 + 2; rows++) {
									for (size_t cols = pos2 - 1; cols < pos2 + 2; cols++) {
										bfs.push_back(board[rows][cols]);
									}
								}
							}
						}

					} 
					else {
						board[pos1][pos2].chosen = true;
					}
				
				}
			}
			else {
				board[row][col].chosen = true;
			}
			
			return true;

		}

	}
	//Returns true if the game is over, false otherwise.	
	bool done() {
		int num = numVisible();
		if (game == false) {
			return true;
		}
		else if ((rownum*colnum - num) == numBombs) {
			return true;
		}
		else {
			return false;
		}
		

	}
//data hiding
private:
	int rownum = 5;
	int colnum = 5;
	char reg = '-';
	char bomb = '*';
	int numBombs = 0;
	int visible = 0;
	bool game = true;
	vector <vector <Cell>> board;



};


int main() {
	Minesweeper sweeper;
	//boolean if the player lost or not 
	bool lost = false;
	// Continue until the only invisible cells are bombs
	while (!sweeper.done()) {
		sweeper.display(false); // display the board without bombs

		int row_sel = -1, col_sel = -1;
		while (row_sel == -1) {
			// Get a valid move
			int r, c;
			cout << "row? ";
			cin >> r;
			if (!sweeper.validRow(r)) {
				cout << "Row out of bounds\n";
				continue;
			}
			cout << "col? ";
			cin >> c;
			if (!sweeper.validCol(c)) {
				cout << "Column out of bounds\n";
				continue;
			}
			if (sweeper.isVisible(r, c)) {
				cout << "Square already visible\n";
				continue;
			}
			row_sel = r;
			col_sel = c;
		}
		// Set selected square to be visible. May effect other cells.
		if (!sweeper.play(row_sel, col_sel)) {
			cout << "BOOM!!!\n";
			sweeper.display(true);
			getchar();
			lost = true;
			// We're done! Should consider ending more "cleanly",
			// eg. Ask user to play another game.
			//exit(0);
		}
	}
	// Ah! All invisible cells are bombs, so you won!
	if (lost != true) {
		cout << "You won!!!!\n";
		sweeper.display(true); // Final board with bombs shown
		getchar();
	}
	getchar();

	
}

