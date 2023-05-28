#pragma once
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <string>
#include "Header.h"

#define RED "\033[31;1m"
#define GREEN "\033[32m"
#define WHITE "\033[37m"
#define YELLOW "\033[33m"
#define BLUE "\033[34;1m"
#define CYAN "\033[36;1m"
#define DEFAULT_COLOR "\033[m"
#define PINK "\033[35;1m"
#define BACKGROUND_GREY "\033[48;5;235m"
#define DEFAULT_BACKGROUND_COLOR "\033[40m"

#define CLEAR_LINE() (cout<<"\033[2K")
#define CLEAR_TO_END() (cout<<"\033[0J")
#define TO_COLUMN(num) (cout<<"\033["+to_string(num)+"G")
#define TO_POSITION(row, column) (cout<<"\033[" + to_string(row) + ";" + to_string(column) +"H")
#define CLEAR_LINE_TO_END() (cout<<"\033[0K")
#define UP_LINE() (cout << "\033[1A")
#define UP_LINES(x) (cout << "\033[" + to_string(x) + "F")
#define DOWN_LINE() (cout << "\033[1E")
#define DOWN_LINES(x) (cout << "\033[" + to_string(x) + "E")
#define CURSOR_FORWARD (cout << "\033[1C")
#define CURSOR_BACK (cout << "\033[1D")
using namespace std;

enum NodeType
{
	Empty,
	Block,
	Player
};

class Node
{
public:
	char symbol = ' ';
	NodeType nodeType = Empty;
	void SetNode(NodeType nodeType) 
	{
		switch (nodeType)
		{
		case Empty:
			symbol = ' ';
			this->nodeType = nodeType;
			break;
		case Block:
			symbol = '#';
			this->nodeType = nodeType;
			break;
		case Player:
			symbol = '@';
			this->nodeType = nodeType;
			break;
		default:
			break;
		}
	}
};

class Grid
{
public:
	string sArr[15] = {
		"        #      ",
		"    ##       ##",
		"               ",
		"       ###     ",
		"       #       ",
		"####           ",
		"            ###",
		"      ###      ",
		" ##            ",
		"          ##  #",
		"  ####         ",
		"          ###  ",
		"    ##         ",
		"           ### ",
		" ###           ",
	};
	int rows=15, columns=15;
	Node** arr = new Node*[rows];
	Grid();
	void PrintGrid();
};

class PlayerC
{
public:
	int x=0, y=0;
	void Control();
	void move(int a);
};


