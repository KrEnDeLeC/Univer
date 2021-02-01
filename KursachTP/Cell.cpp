#include "Cell.h"

Cell::Cell(Ñharacters _type, int col, int row)
{
	type = _type;
	cords.col = col;
	cords.row = row;
	isSelected = false;
}
