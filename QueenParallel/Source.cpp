#include<iostream>
#include <ppl.h>
#include <Windows.h>

using namespace concurrency; 
using namespace std;

template <class Function>
__int64 time_call(Function&& f)
{
	__int64 begin = GetTickCount();
	f();
	return GetTickCount() - begin;
}

//board size
#define X 8

//prints if board is complete
void print(int board[X][X])
{
	int i;
	int j;

	for (i = 0; i < X; ++i)
	{
		cout << "\t\t";
		for (j = 0; j < X; ++j)
			cout << board[i][j] << " ";
		cout << endl;
	}
	cout << endl;
}

//if placing a queen in [row][col] is correct, returns true.
//otherwise, returns false
bool correct(int board[X][X], int row, int col)
{
	int i;
	int j;

	//check column
	for (i = 0; i < X; ++i)
	{
		if (board[row][i])
			return false;
	}

	//check row
	for (i = 0; i < X; ++i)
	{
		if (board[i][col])
			return false;
	}

	//check upper left diagonal
	for (i = row, j = col; i >= 0 && j >= 0; i--, j--)
	{
		if (board[i][j])
			return false;
	}

	//check lower left diagonal
	for (i = row, j = col; i < X && j >= 0; ++i, --j)
	{
		if (board[i][j])
			return false;
	}

	//check upper right diagonal
	for (i = row, j = col; i >= 0 && j < X; i--, ++j)
	{
		if (board[i][j])
			return false;
	}

	//check lower right diagonal
	for (i = row, j = col; i < X && j < X; ++i, ++j)
	{
		if (board[i][j])
			return false;
	}

	//return true if all tests passed
	return true;
}

//parallel test for queen positions
void par_test(int board[X][X], int row)
{
	//if all queens have been placed at non-conflicting positions
	if (row == X)
	{
		print(board);
		return;
	}

	//place queens on all positions in a given row and
	//check if the board is correct
	//continue the process till all possibilities found
	parallel_for(int(0), X, [&](int i)
	{
		if (correct(board, row, i))
		{
			board[row][i] = 1;
			par_test(board, row + 1);
		}
		board[row][i] = 0;
	});
}

//serial test for queen positions
void test(int board[X][X], int row)
{
	int i;
	
	//if all queens have been placed at non-conflicting positions
	if (row == X)
	{
		print(board);
		return;
	}

	//place queens on all positions in a given row and
	//check if the board is correct
	//continue the process till all possibilities found
	for (i = 0; i < X; ++i)
	{
		if (correct(board, row, i))
		{
			board[row][i] = 1;
			test(board, row + 1);
		}
		board[row][i] = 0;
	}
}

int main()
{
	//create the board
	int board[X][X] = {0};

	int parTest;
	int sinTest;

	//single cpu
	sinTest = time_call([&] {
		test(board, 0);
	});

	//dual cpu
	parTest = time_call([&] {
		par_test(board, 0);
	});

	//print both to the screen
	cout << "\n\n";
	cout << "\t\t";
	cout << "Serial: " << sinTest;
	cout << "\n";
	cout << "\t\t";
	cout << "Parallel: " << parTest;
	cout << "\n\n\n";

	system("pause");
	return 0;
}