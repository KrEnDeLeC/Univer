#pragma once

enum �haracters
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
	Cell(�haracters _type, int col, int row);
	Cords cords;
	bool isSelected;
	//int range; // ����� ������
	//void setCoord(int x1, int y2); //���������� ����������
	//bool isMove;
	�haracters type;
};

