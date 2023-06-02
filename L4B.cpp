#include <iostream>
#include <windows.h>
#include <conio.h>
#include "Header.h"
using namespace std;

Grid grid;
PlayerC player;
GameStat turnStat("turn", 0, grid.rows + 1);
GameStat scoreStat("score", 0, grid.rows + 2);
GameStat bombsStat("bombs", 0, grid.rows + 3);
int main()
{
    CONSOLE_CURSOR_INFO cursor;
    cursor.bVisible = false;
    cursor.dwSize = 1;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
    grid.PrintGrid();
    turnStat.PrintHeader();
    scoreStat.PrintHeader();
    bombsStat.PrintHeader();
    while (true)
    {
        turnStat.Update();
        scoreStat.Update();
        bombsStat.Update();
        player.Control();
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
        Sleep(50);
    }

}

void Node::SetNode(NodeType nodeType)
{
    switch (nodeType) {
    case Empty:
        symbol = ' ';
        this->nodeType = nodeType;
        color = DEFAULT_COLOR;
        break;
    case Block:
        symbol = '#';
        this->nodeType = nodeType;
        color = YELLOW;
        break;
    case Player:
        symbol = '@';
        color = GREEN;
        this->nodeType = nodeType;
        break;
    case Bomb:
        symbol = '*';
        color = CYAN;
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
    TO_POSITION(position.first, position.second);
    cout << color << symbol << DEFAULT_COLOR;
}

Grid::Grid()
{
    for (int i = 0; i < rows; i++)
    {
        arr[i] = new Node[columns];
        for (int j = 0; j < columns; j++)
        {
            NodeType nt = Empty;
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
            }
            arr[i][j] = Node();
            arr[i][j].SetNode(nt, i, j);
        }
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

}

void PlayerC::Control()
{
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
    }
}

void PlayerC::Move(int a)
{
    bool isMoved = false;
    int prewX = x;
    int prewY = y;
    cout << GREEN;
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
        if (x + 1 < grid.columns && grid.arr[y][x + 1].nodeType != Block)
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

        grid.arr[y][x].SetNode(Player);
        grid.arr[y][x].Update();
        turnStat.value++;
        TO_POSITION(prewY, prewX);
        grid.arr[prewY][prewX].SetNode(Empty);
        //grid.arr[prewY][prewX].Update();
        cout << ' ';
    }
    cout << DEFAULT_COLOR;
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