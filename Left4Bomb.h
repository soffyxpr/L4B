#pragma once
#include <string>

using namespace std;
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
#define TO_POSITION(row, column) (cout<<"\033[" + to_string(row+1) + ";" + to_string(column+1) +"H")
#define CLEAR_LINE_TO_END() (cout<<"\033[0K")
#define UP_LINE() (cout << "\033[1A")
#define UP_LINES(x) (cout << "\033[" + to_string(x) + "F")
#define DOWN_LINE() (cout << "\033[1E")
#define DOWN_LINES(x) (cout << "\033[" + to_string(x) + "E")
#define CURSOR_FORWARD (cout << "\033[1C")
#define CURSOR_BACK (cout << "\033[1D")

enum NodeType
{
	Empty,
	Block,
	Player,
	Bomb,
	ActiveBomb,
	Coin
};

class Node
{
public:
	bool canStepOn = true;
	char symbol = ' ';
	pair<int, int> position;
	string color;
	NodeType nodeType = Empty;

	void SetNode(NodeType nodeType);
	void SetNode(NodeType nodeType, int posX, int posY);
	void Update();
};

class Grid
{
public:

	vector<pair<Node*, int>> activeBombs;

	int rows = 0, columns = 0;
	Node** arr;

	Grid();
	bool CheckGridClear();
	void PrintGrid();
	void SpawnBomb();
	void BombsStatusUpdate();
	void DetonateBomb(Node* n);

};

class PlayerC
{
public:
	int x = 0, y = 0;
	int bombRadius = 1;
	void Control();
	void Move(int a);
	bool PlaceBomb();
};

class GameStat
{
public:
	string name;
	int value = 0;
	pair<int, int> headerPos;
	pair<int, int> valuePos;
	GameStat(string name, int headerX, int headerY);
	void PrintHeader();
	void Update();
};