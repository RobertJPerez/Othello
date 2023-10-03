/* Luke Fanizzi, Angelina Gargiulo, Robert Perez
 * Lauren Olson
 * 3/21/2020
 * This file contains the functions for lab 3. This lab is a
 * fully recursive implementation of the Othello board game.
 */

#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;
struct GameBoard {
	char **board; // the othello board
	int size; //for the size of the board (it's a square, so size is one dimension)
	char p; //for current player ('X' or 'O')
	int totalct; // for number of used squares (so that when the board is full, the game stops)
};

int getMin(int arr[], int n);
void SelectionSort(int arr[], int num, int ind);
void makeBoard(GameBoard *game, int row, int col, int ct, bool flag);
void AllocateBoard(char ** matrix, int ct, int size);
void getSize(int &size);
void printBoard(GameBoard *game, int row, int col, int ct);
int ckwrow(char boardrow[], int size, int itr, char p);
void ckwin(GameBoard *game);
int ckhelper(GameBoard *game, int row, int size, char p);
int shouldweflip(int x, int y, int xd, int yd, GameBoard *game, int ct);
void flipping(int x, int y, int xd, int yd, GameBoard *game);
int figureoutflipping(int x, int y,GameBoard *game);
int findbestspoty(int currx, int curry, int &bestx, int &besty, GameBoard *game, int oldct);
int findbestspot(int currx, int curry,int &bestx,int &besty,GameBoard *game,int oldct);
bool placepieceperson(GameBoard *game);
bool compplacepiece(GameBoard *game);
void startGame(GameBoard *game);

int main() {
	srand(time(NULL));
	int size = 0;
	getSize(size); // gets the size for the game board. Note we only do this once for this game.
	GameBoard *game = new GameBoard; // places a gameBoard struct object on the heap.
	game->size = size; //sets the gameboard's size
	game->totalct = 4; // the 4 middle squares are taken
	cout << "Size is " << game->size << endl;
	makeBoard(game, 0, 0, size, true);
	printBoard(game, 0, 0, size); // for testing purposes
	startGame(game);
}

void playGame(bool fp1,bool fp2,GameBoard *game,bool whoplaysfirstflag) {
	bool s1 = true;
	bool s2 = true;
	if (fp1 && fp2 && (s1 || s2)) {
		if (whoplaysfirstflag) {
			game->p = 'X';
			s1 = placepieceperson(game);
			printBoard(game, 0, 0, game->size);
			game->p = 'O';
			s2 = placepieceperson(game);
			printBoard(game, 0, 0, game->size);
		} else {
			game->p = 'O';
			s1 = placepieceperson(game);
			printBoard(game, 0, 0, game->size);
			game->p = 'X';
			s2 = placepieceperson(game);
			printBoard(game, 0, 0, game->size);
		}
	} else if (fp1 == false && fp2 == false && (s1 || s2)) {
		game->p = 'X';
		s1 = compplacepiece(game);
		printBoard(game, 0, 0, game->size);
		game->p = 'O';
		s2 = compplacepiece(game);
		printBoard(game, 0, 0, game->size);
	} else {
		if (whoplaysfirstflag) {
			game->p = 'X';
			s1 = placepieceperson(game);
			printBoard(game, 0, 0, game->size);
			game->p = 'O';
			s2 = compplacepiece(game);
			printBoard(game, 0, 0, game->size);
		} else {
			game->p = 'X';
			s1 = compplacepiece(game);
			printBoard(game, 0, 0, game->size);
			game->p = 'O';
			s2 = placepieceperson(game);
			printBoard(game, 0, 0, game->size);
		}
	} if (s1 == false && s2 == false || game->totalct == game->size *game->size) {
		cout << "Game over" << endl;
		printBoard(game, 0, 0, game->size);
		return;
	}
	playGame(fp1,fp2,game,whoplaysfirstflag);
}

void startGame(GameBoard *game) {
	cout << "How many players? (0,1, or 2)" << endl;
	int numplayers;
	cin >> numplayers;
	if (numplayers == 0) {
		playGame(false,false,game, true);
	} else if (numplayers == 1) {
		bool whoplaysfirstflag;
		rand()%2 == 0? whoplaysfirstflag = true:whoplaysfirstflag = false;
		playGame(true,false,game,whoplaysfirstflag);
	} else playGame(true,true,game,true);
//	char w = ckwin(game);
//	if (w != 'T') {
//		cout << w << " WON!!!! " << endl;
//	} else {
//		cout << "Tie game. " << endl;
//	}
	ckwin(game);
	string s;
	cout << "Play again? (Y or N)" << endl;
	cin >> s;
	if (s == "Y") {
		game->totalct = 4;
		makeBoard(game, 0, 0, game->size, false);
		cout << "STARTING OVER" << endl;
		printBoard(game, 0, 0, game->size);
		startGame(game);
	} else {
		cout << "THANKS FOR PLAYING!" << endl;
	}
}


//Problem 1:
/* This function is a helper function for the selection sort.
 * It takes in an array of integers and the number of integers in
 * that array, and finds the minimum value.
 */
int getMin(int arr[], int n){
    if (n <= 1){
        return 0;
    }
    n--;
    int x = getMin(arr,n);
    if(arr[n]<arr[x]){
    	return n;
    } else {
    	return x;
    }
}

/* This is the selection sort algorithm using recursion. It sorts
 * the integers in an array in order by the value of their
 * numbers.
 */
void SelectionSort(int arr[], int num, int ind){
    if(num == ind){
        return;
    }
    int min = getMin(arr+ind,num-ind);
    int temp = arr[min+ind];
    arr[min+ind] = arr[ind];
    arr[ind] = temp;
    SelectionSort(arr, num, ind+1);
}

//part 2: Othello
void makeBoard(GameBoard *game, int row, int col, int ct, bool flag){
	int max = game->size;
	if (flag){
		game->board = new char *[game->size];
		AllocateBoard(game->board,0,max);
	}
	if(row==max)
		return;
	if(col==max){
		col = 0;
		row+=1;
	}
	if(row != max){
		game->board[row][col] = '-';
		col+=1;
		makeBoard(game, row, col, max, false);
	}
	game->board[(max/2)-1][(max/2)-1] = 'X';
	game->board[(max/2)-1][max/2] = 'O';
	game->board[max/2][(max/2)-1] = 'O';
	game->board[max/2][max/2] = 'X';
}

void AllocateBoard(char ** matrix, int ct, int size){
	if(ct==size)
		return;
	if (ct < size){
		matrix[ct] = new char[size];
		ct ++;
		return AllocateBoard(matrix, ct, size);
	}
}

//getsize
void getSize(int &size){
	size = rand() % (20 - 7) + 7; // randomizes values from 7  to 20
	if(size%2 != 0){  // logic gate that checks if the size is even
		size += 1;  // adds one to the size
	}
}

/* This function returns nothing and takes in game, which is of the type "Gameboard",
 * the rows, as an integer, the columns, as an integer and a counter, as an integer.
 * The purpose of this function is to make the game board visible to the user.
 */
void printBoard(GameBoard *game, int row, int col, int ct){
	int max = game->size; // initiates an integer, max, as the size of the gamboard
	if(row==max)
		return; // returns if the row ius at the maximum of the gamboard size
	if(col==max){
		col = 0;
		row+=1; // increments the row
		cout << endl;  // changes lines, goes down one line
	}
	if(row != max){
		cout << game->board[row][col]; // prints out the value of the value at the position row, column
		col+=1; // increments columns by 1
		printBoard(game, row, col, max); // recursive line that reiterates the whole function to successfully
	}
}

/* This is a helper function to ckwin that returns an integer and  takes in the character at the current row
 * the size of the board, a counter and the current player.
 */
int ckwrow(char gamerow[], int size, int itr, char p){
	int found = 0; // initiates a counter at 0
	if (itr >= size)
		return 0; // returns 0 if the number found is above the size of the board
	else {
		if (gamerow[itr] == p){ // logic gate to help in counting the number found
			++found; // increments found by one at a time
		}
		return found + ckwrow(gamerow,size,itr+1,p);  // recursive statement t that goes through the whole board until the whole board is checked
	}
}

/* This function returns an integer and takes in game, of type Gameboard, the current row,
 * the current column and the current character, which is a character.
 */
int ckhelper(GameBoard *game, int row, int size, char p){
	if(row==size) // if the row is equal to the size...
		return 0; // returns 0 if size is equal to rows
	int x = ckwrow(game->board[row], size, 0, p); //initializes an integer x as the value returned in ckwrow
	return x + ckhelper(game,row+1,size,p);  //recursive statement that helps us go through all the rows
}

/* This function returns nothing and takes in a parameter, game, which is the type GameBoard.
 * This function is responsible for checking if a player wins and prints out which player has won.
 */
void ckwin(GameBoard *game){
	int x = ckhelper(game,0,game->size,'X'); //Initializes an integer, x, used to store the amount of X characters
	int o = ckhelper(game,0,game->size,'O');
	if(x>o)// logic gate, if more characters on the board are x, player X wins
		cout << "Player X Wins!" << endl;
	else if(o>x) // prints out result to console for players to see
		cout << "Player O Wins!" << endl;
	else cout << "It's a Tie!" << endl; // if neither condition is satisfied, it must be a tie
}


// This function returns an integer and takes in five integers, x,y,xd,yd and ct. game, which is the type GameBoard, is also an input parameter. This function determines what piece(s) are flipped when a player chooses a spot on the board. This code mimics checking North, South, East , West and diagonals like the previous lab

int shouldweflip(int x, int y, int xd, int yd, GameBoard *game, int ct){
	if(xd == 0){ // logic gate to check a specific x (horizontal) direction
		if(yd == 0){ //logic gate to check a specific y (vertical) direction
			return ct; //returns unchanged count because no pieces were flipped
		} else if(yd < 0) { //logic gate to check a different y direction
			y--; // decrements y by one
			yd++; // increments yd by one, changing the y direction
			if(game->board[x][y] != game->p) { // logic gate to check if a piece can be flipped
				ct++; //increments count because a piece can be flipped
			}
			return shouldweflip(x,y,xd,yd,game,ct); // recursive statement to check further in the same direction
		} else if(yd >0) { // logic gate to check a different y direction
			y ++; //increments y by one
			yd --; //decrements yd by one, changing the y direction
			if(game->board[x][y] != game->p) { // logic gate that checks if a piece and be flipped
				ct++; // increments count by one, because a piece can be flipped
			}
			return shouldweflip(x, y, xd, yd, game, ct); // recursive statement to check further in the same direction
		}
	}
	if(xd < 0){ // logic gate to check a specific x (horizontal) direction
		x--; //decrements x by one
		xd++; // increments xd by one, changing the direction
		if(yd == 0){ // logic gate to check a specific direction
			if(game->board[x][y] != game->p){ // logic gate to check if a piece can be flipped
				ct++; //increments count because a piece can be flipped
			}
			return shouldweflip(x, y, xd, yd, game, ct); // recursive statement to check further in the same direction
		} else if(yd < 0){ // logic gate to check a different direction
			y--; //decrements y by one
			yd++; // increments yd by one, changing direction
			if(game->board[x][y] != game->p){ // logic gate to check if a piece can be flipped
				ct++; // increments count by one because a piece can be flipped
			}
			return shouldweflip(x, y, xd, yd, game, ct); // recursuve statement to check further in the same direction
		} else if(yd > 0){ // logic gate to check a different y direction
			y++; // increments y by one
			yd--; //deceements yd by one, changing direction
			if(game->board[x][y] != game->p) { // logic gate to check if a piece can be flipped
				ct++; // count incremented by one because a piece can be flipped
			}
			return shouldweflip(x, y, xd, yd, game, ct); //recursive statement to check further in the same direction
		}
	}
	return 0; // returns an integer because the function is supposed to return an integer to run, like the main function
}

/* This function returns nothing and takes integers x,y,xd,yd, and the GameBoard object game and is
 * responsible for actually flipping the pieces.
*/
void flipping(int x, int y, int xd, int yd, GameBoard *game){
	if (xd == 0){
		if (yd == 0){
			return;
		} else if (yd < 0){
			yd++;
			y--;
			game->board[x][y+1] = game->p;
			return flipping(x, y, xd, yd, game);
		} else if (yd > 0){
			yd--;
			y++;
			game->board[x][y-1] = game->p;
			return flipping(x, y, xd, yd, game);
		}
	}
	if (xd < 0 ){
		xd++;
		x--;
		if (yd == 0){
			game->board[x+1][y] = game->p;
			return flipping(x, y, xd, yd, game);
		} else if (yd < 0) {
			yd++;
			y--;
		game->board[x][y+1] = game->p;
		return flipping(x, y, xd, yd, game);
		} else if (yd > 0) {
			yd--;
			y++;
			game->board[x][y-1] = game->p;
			return flipping(x, y, xd, yd, game);
		}
	}
	if (xd > 0 ) {
		xd--;
		x++;
		if (yd == 0){
			if (game->board[x][y] != game->p){
					game->board[x-1][y] = game->p;
			}
			return flipping(x, y, xd, yd, game);
		} else if (yd < 0){
			yd++;
			y--;
			game->board[x][y+1] = game->p;
			return flipping(x, y, xd, yd, game);
		} else if (yd < 0){
			yd++;
			y--;
			game->board[x][y+1] = game->p;
			return flipping(x, y, xd, yd, game);
			}
	}
}

bool placepieceperson(GameBoard *game){
	int row_cord, col_cord;
	cout<<"Choose an X coordinate: "<<endl;
	cin>> row_cord;
	cout<<"Choose an Y coordinate: "<<endl;
	cin>> col_cord;
	if(game -> board[row_cord][col_cord]!='-'){
		cout<<"There is already a piece here. You forfeit this turn!"<<endl;
		return false;
	}
	char temp = game ->board[row_cord][col_cord];
	game -> board [row_cord][col_cord] = game -> p;
	//choosing an invalid space (off board)
	if(row_cord >= game -> size || row_cord < 0){
		return false;
	} else if(col_cord >= game -> size || col_cord < 0){
		cout<<"Space is off the board. You forfeit your turn!"<< endl;
		return false;
	} else if(figureoutflipping(row_cord, col_cord,game)!= 0){
		game -> board [row_cord][col_cord] = game -> p;
		return true;
	} else {
		cout<<"The selected spot resulted in no flipping" << endl;
		game -> board [row_cord][col_cord] = temp;
		return false;
	}

}

int findbestspoty(int currx, int curry, int &bestx, int &besty, GameBoard *game, int oldct){
	int spt = 0; // int to be used to randomize later
	if (curry < game->size){
		oldct = figureoutflipping(bestx, besty, game); //oldct is set to the best possible move
		int currct = figureoutflipping(currx, curry, game); // currct is set to current move
		if (currct >= oldct){
			if ( currct == oldct){
				spt = rand() % 2; // determines random selection between two moves
			}
			if (spt == 0){
				bestx = currx;
				besty = curry;
				oldct = figureoutflipping(bestx, besty, game); //oldct is set to the best possible move
			}
		}
		curry++;
		return findbestspoty(currx, curry, bestx, besty, game, oldct); //recursively calls the function again
	}
	return besty;
}

int figureoutflipping(int x, int y,GameBoard *game){
	// 1stspt 2ndspt 3rdspt
	// 4thspt thespt 6thspt
	// 7thspt 8thspt 9thspt
//Checks around the current spt
	int firstspt = shouldweflip(x,y,-1,-1,game,0);
	int secondspt = shouldweflip(x,y,-1,0,game,0);
	int thirdspt = shouldweflip(x,y,-1,1,game,0);
	int fourthspt = shouldweflip(x,y,0,-1,game,0);
	int sixthspt = shouldweflip(x,y,0,1,game,0);
	int seventhspt = shouldweflip(x,y,1,-1,game,0);
	int eighthspt = shouldweflip(x,y,1,0,game,0);
	int ninthspt = shouldweflip(x,y,1,1,game,0);
// Flips each piece
	flipping(x,y,-1,-1,game);
	flipping(x,y,-1,0,game);
	flipping(x,y,-1,1,game);
	flipping(x,y,0,-1,game);
	flipping(x,y,0,1,game);
	flipping(x,y,1,-1,game);
	flipping(x,y,1,0,game);
	flipping(x,y,1,1,game);
///Sets the total amount flipped to an int then adds all the values to get the totals
	int totalflped = firstspt+secondspt+thirdspt+fourthspt+sixthspt+seventhspt+eighthspt+ninthspt;
	return totalflped;
}


int findbestspot(int currx, int curry,int &bestx,int &besty,GameBoard *game,int oldct){
	if (currx == game->size){
		return oldct;
	} else {
		int tmp = oldct;
		oldct = findbestspoty(currx,0, bestx,besty,game,-1);
		if (oldct == 0){
			return findbestspot(currx++,curry,bestx,besty,game,oldct);
		}
		if (oldct > tmp){
			bestx = currx;
			return findbestspot(currx++,curry,bestx,besty,game,oldct);
		} else if (oldct < tmp){
			return findbestspot(currx++,curry,bestx,besty,game,tmp);
		} else{
			int whichspt = rand() % 2;
			if (whichspt == 1){
				return findbestspot(currx++,curry,bestx,besty,game,oldct);
			}else{
				bestx = currx;
				return findbestspot(currx++,curry,bestx,besty,game,oldct);
			}
		}
	}
}

bool compplacepiece(GameBoard *game){
	int ct; // counter
	//ct = findbestspot(0, 0, 0, 0, game, ct); sets the ct to findbestspot
	bool flag;
	if(ct == 0){
		cout << "There are no spots that flip pieces. You forfeited your turn." << endl; //prints that the user forfeited their turn
		flag = false; // sets flag to false
	} else {
		flag = true;
	}
	return flag;
}








