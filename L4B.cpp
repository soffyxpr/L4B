#include <iostream>
#include <algorithm>
#include <iomanip>
#include <string>
#include <conio.h>
#include "Header.h"
using namespace std;

Grid grid;
PlayerC player;

Grid::Grid()
{
	for (int i = 0; i < rows; i++)
	{
		arr[i] = new Node[columns];
	}

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			NodeType nt;
			switch (sArr[i][j]) {
			case ' ':
				nt = Empty;
				break;
			case '#':
				nt = Block;
				break;
			case '@':
				nt = Player;
				break;
			default:
				break;
			}
			arr[i][j] = Node();
			arr[i][j].SetNode(nt);
		}
	}
}
void Grid::PrintGrid()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			cout << arr[i][j].symbol;
		}
		cout << BLUE << '|' << DEFAULT_COLOR << endl;
	}
	for (int i = 0; i < columns; i++)
	{
		cout << BLUE << '-';
	}
	cout << BLUE << '+' << DEFAULT_COLOR << endl;
}
void PlayerC::Control() // 
{
	int c = _getch();
	switch (c)
	{
	case 'w':
		move(0);
			break;
	case 'a':
		move(1);
		break;
	case 's':
		move(2);
		break;
	case 'd':
		move(3);
		break;
	default:
		break;
	}
}

void PlayerC::move(int a)
{
	bool isMoved = false;
	switch (a)
	{
	case 0:
		if (y - 1 >= 0 && grid.arr[y - 1][x].nodeType != Block)
		{
			TO_POSITION(y - 1, x);
			cout << '@';
			isMoved = true;
			y--;
		}
		break;
	case 1:
		if (x - 1 >= 0 && grid.arr[y][x - 1].nodeType != Block)
		{
			TO_POSITION(y, x - 1);
			cout << '@';
			isMoved = true;
			x--;
		}
		break;
	case 2:
		if (y + 1 < grid.rows && grid.arr[y + 1][x].nodeType != Block)
		{
			TO_POSITION(y + 1, x);
			cout << '@';
			isMoved = true;
			y++;
		}
		break;
	case 3:
		if (x + 1 < grid.rows && grid.arr[y][x + 1].nodeType != Block)
		{
			TO_POSITION(y, x + 1);
			cout << '@';
			isMoved = true;
			x++;
		}
		break;
	}
	if (isMoved)
	{
		TO_POSITION(y, x);
		cout << ' ';
	}
}

int main()
{
	grid.PrintGrid();
	while (true)
	{
		player.move(0);
		player.Control();
	}
    return NULL;
}

