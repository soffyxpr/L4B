#include <iostream>
#include <windows.h>
#include <vector>
#include <conio.h>
#include <time.h>
#include "Left4Bomb.h"
#include <thread>
#include <fstream>
using namespace std;

vector<vector<string>> levels;

Grid grid;
PlayerC player;
GameStat* turnStat;
GameStat* scoreStat;
GameStat* bombsStat;

void LoadAllLevels();

void GlobalUpdate()
{
	turnStat->Update();
	scoreStat->Update();
	bombsStat->Update();
	grid.BombsStatusUpdate();
	player.Control();
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));

}

int main()
{
	srand(time(NULL));
	LoadAllLevels();
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = false;
	cursor.dwSize = 1;

	int prewScore = 0;
	int prewTurn = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
	while (true)
	{
		bool isGameOver = false;
		grid = Grid();
		turnStat = new GameStat("turn", 0, grid.rows + 1);
		scoreStat = new GameStat("score", 0, grid.rows + 2);
		bombsStat = new GameStat("bombs", 0, grid.rows + 3);
		turnStat->value = prewTurn;
		scoreStat->value = prewScore;

		grid.PrintGrid();
		turnStat->PrintHeader();
		scoreStat->PrintHeader();
		bombsStat->PrintHeader();

		grid.SpawnBomb();
		while (!isGameOver)
		{
			GlobalUpdate();
			Sleep(50);
			isGameOver = grid.CheckGridClear();
		}
		player.bombRadius++;
		system("cls");
		player.x = 0;
		player.y = 0;
		prewScore = scoreStat->value;
		prewTurn = turnStat->value;
	}
}

void Node::SetNode(NodeType nodeType)
{
	switch (nodeType) {
	case Empty:
		symbol = ' ';
		canStepOn = true;
		this->nodeType = nodeType;
		color = DEFAULT_COLOR;
		break;
	case Block:
		symbol = '#';
		canStepOn = false;
		this->nodeType = nodeType;
		color = YELLOW;
		break;
	case Player:
		symbol = '@';
		canStepOn = true;
		color = GREEN;
		this->nodeType = nodeType;
		break;
	case Bomb:
		symbol = '*';
		canStepOn = true;
		color = CYAN;
		this->nodeType = nodeType;
		break;
	case ActiveBomb:
		symbol = 'O';
		canStepOn = false;
		color = RED;
		this->nodeType = nodeType;
		break;
	case Coin:
		symbol = 'o';
		canStepOn = true;
		color = YELLOW;
		this->nodeType = nodeType;
		break;
	}
}

void Node::SetNode(NodeType nodeType, int posX, int posY)
{
	SetNode(nodeType);
	position.first = posX;
	position.second = posY;
}

void Node::Update()
{
	TO_POSITION(position.second, position.first);
	cout << color << symbol << DEFAULT_COLOR;
}

Grid::Grid()
{
	if (levels.size() == 0)
		return;
	int randomI = rand() % levels.size();
	rows = levels[randomI].size();
	columns = levels[randomI][0].size();
	arr = new Node * [rows];
	for (int i = 0; i < rows; i++)
	{
		arr[i] = new Node[columns];
		for (int j = 0; j < columns; j++)
		{
			NodeType nt = Empty;
			switch (levels[randomI][i][j]) {
			case ' ':
				nt = Empty;
				break;
			case '#':
				nt = Block;
				break;
			case '@':
				nt = Player;
				break;
			}
			arr[i][j] = Node();
			arr[i][j].SetNode(nt, j, i);
		}
	}

}

bool Grid::CheckGridClear()
{
	for (int i = 0; i < grid.rows; i++)
	{
		for (int j = 0; j < grid.columns; j++)
		{
			if (grid.arr[i][j].nodeType == Block || grid.arr[i][j].nodeType == Coin)
			{
				return false;
			}
		}
	}
	return true;
}

void LoadAllLevels()
{
	ifstream in("levels.txt");
	string s;
	int i = 0, j = 0;

	vector<string> onelvl;

	while (!in.eof())
	{

		getline(in, s);
		if (s[0] == '-')
		{
			levels.push_back(onelvl);
			onelvl.clear();
			continue;
		}
		onelvl.push_back(s);
	}
}

void Grid::PrintGrid()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			cout << arr[i][j].color << arr[i][j].symbol << DEFAULT_COLOR;
		}
		cout << RED << '|' << DEFAULT_COLOR << endl;
	}
	for (int i = 0; i < columns; i++)
	{
		cout << RED << '-';
	}
	cout << RED << '+' << DEFAULT_COLOR << endl;
}

void Grid::SpawnBomb()
{
	vector<Node*> v;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			if (arr[i][j].nodeType == Empty)
			{
				v.push_back(&(arr[i][j]));
			}
		}
	}
	int x = rand() % v.size();
	v[x]->SetNode(Bomb);
	v[x]->Update();
}

void Grid::BombsStatusUpdate()
{
	for (int i = 0; i < activeBombs.size(); i++)
	{
		pair<Node*, int> p = activeBombs[i];
		if (p.second == 0)
		{
			p.first->SetNode(Empty);
			p.first->Update();
			grid.DetonateBomb(p.first);
			activeBombs.erase(activeBombs.begin() + i);
		}
		else
		{
			p.second--;
			activeBombs[i] = p;
		}
	}
}

bool isInGrid(int i, int j)
{
	if (j < grid.columns && i < grid.rows && j >= 0 && i >= 0)
	{
		return true;
	}
	return false;
}

void MakeExplosionEffect(Node* n)
{
	pair<char, string> eff[4] = {
		{'*', WHITE}, {'o', "\033[93m"},
		{'O', YELLOW}, {'0', RED}
	};
	int x = n->position.first;
	int y = n->position.second;
	for (int g = 0; g < 4; g++)
	{
		for (int i = -player.bombRadius; i <= player.bombRadius; i++)
		{
			for (int j = -player.bombRadius; j <= player.bombRadius; j++)
			{
				if (isInGrid(y + i, x + j))
				{
					Node* n = &grid.arr[y + i][x + j];
					n->symbol = eff[g].first;
					n->color = eff[g].second;
					n->Update();
				}
			}
		}
		Sleep(400);
	}

	/*for (int i = 0; i < 8; i++)
	{
		try {
			Node* n = &grid.arr[y + ar[i].first][x + ar[i].second];
			n->symbol = '*';
			n->color = WHITE;
			n->Update();
		}
		catch (...) {}
	}
	this_thread::sleep_for(chrono::milliseconds(200));
	for (int i = 0; i < 8; i++)
	{
		try {
			Node* n = &grid.arr[y + ar[i].first][x + ar[i].second];
			n->symbol = 'o';
			n->color = "\033[93m";
			n->Update();
		}
		catch (...) {}
	}
	this_thread::sleep_for(chrono::milliseconds(200));
	for (int i = 0; i < 8; i++)
	{
		try {
			Node* n = &grid.arr[y + ar[i].first][x + ar[i].second];
			n->symbol = 'O';
			n->color = YELLOW;
			n->Update();
		}
		catch (...) {}
	}
	this_thread::sleep_for(chrono::milliseconds(200));
	for (int i = 0; i < 8; i++)
	{
		try {
			Node* n = &grid.arr[y + ar[i].first][x + ar[i].second];
			n->symbol = '0';
			n->color = RED;
			n->Update();
		}
		catch (...) {}
	}
	this_thread::sleep_for(chrono::milliseconds(200));*/
}

void Grid::DetonateBomb(Node* n)
{
	int x = n->position.first;
	int y = n->position.second;
	bool b;
	MakeExplosionEffect(n);
	for (int i = -player.bombRadius; i <= player.bombRadius; i++)
	{
		for (int j = -player.bombRadius; j <= player.bombRadius; j++)
		{
			try {
				if (isInGrid(y + i, x + j))
				{
					Node* n = &grid.arr[y + i][x + j];
					b = rand() % 2;
					n->SetNode(n->nodeType);
					if (n->nodeType == Block)
					{
						if (b)
							n->SetNode(Empty);
						else
							n->SetNode(Coin);
					}
					//thread th1(MakeExplosionEffect, n);
					n->Update();
				}
			}
			catch (...) {}
		}
	}
}

void PlayerC::Control()
{
	bool isTurn;
	do
	{
		isTurn = true;
		int c = _getch();
		switch (c)
		{
		case 'w':
			Move(0);
			break;
		case 'a':
			Move(1);
			break;
		case 's':
			Move(2);
			break;
		case 'd':
			Move(3);
			break;
		case 'e':
			if (!PlaceBomb())
			{
				isTurn = false;
			}
			break;
		}
	} while (!isTurn);

}

void PlayerC::Move(int a)
{
	bool isMoved = false;
	int prewX = x;
	int prewY = y;
	switch (a)
	{
	case 0:
		if (y - 1 >= 0 && grid.arr[y - 1][x].canStepOn)
		{
			isMoved = true;
			y--;
		}
		break;
	case 1:
		if (x - 1 >= 0 && grid.arr[y][x - 1].canStepOn)
		{
			isMoved = true;
			x--;
		}
		break;
	case 2:
		if (y + 1 < grid.rows && grid.arr[y + 1][x].canStepOn)
		{
			isMoved = true;
			y++;
		}
		break;
	case 3:
		if (x + 1 < grid.columns && grid.arr[y][x + 1].canStepOn)
		{
			isMoved = true;
			x++;
		}
		break;
	}
	if (isMoved)
	{
		if (grid.arr[y][x].nodeType == Bomb)
		{
			bombsStat->value++;
			//scoreStat.value++;
			grid.SpawnBomb();
		}
		else if (grid.arr[y][x].nodeType == Coin)
		{
			scoreStat->value++;
		}

		grid.arr[y][x].SetNode(Player);
		grid.arr[y][x].Update();
		turnStat->value++;

		if (grid.arr[prewY][prewX].nodeType == Player)
		{
			grid.arr[prewY][prewX].SetNode(Empty);
		}
		grid.arr[prewY][prewX].Update();

	}
	cout << DEFAULT_COLOR;
}

bool PlayerC::PlaceBomb()
{
	if (bombsStat->value > 0)
	{
		Node* n = &grid.arr[player.y][player.x];
		n->SetNode(ActiveBomb);
		bombsStat->value--;
		grid.activeBombs.push_back(make_pair(n, 5));
		return true;
	}
	else
		return false;
}

GameStat::GameStat(string name, int headerX, int headerY)
{
	this->name = name;
	headerPos = make_pair(headerX, headerY);
	valuePos = make_pair(headerX + name.length() + 2, headerY);
}

void GameStat::PrintHeader()
{
	TO_POSITION(headerPos.second, headerPos.first);
	cout << name << ": ";
}

void GameStat::Update()
{
	TO_POSITION(valuePos.second, valuePos.first);
	cout << value;
	CLEAR_LINE_TO_END();
}
