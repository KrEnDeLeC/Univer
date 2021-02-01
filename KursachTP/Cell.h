#pragma once

enum Сharacters
{
	NONE = -1,
	EMPTY,
	FOX,
	CHICKEN
};

struct Cords
{
	int col;
	int row;
};

using namespace std;

class Cell
{
public:
	Cell(Сharacters _type, int col, int row);
	Cords cords;
	bool isSelected;
	//int range; // Номер кольца
	//void setCoord(int x1, int y2); //Установить координаты
	//bool isMove;
	Сharacters type;
};

