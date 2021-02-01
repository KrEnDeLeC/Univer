#include "Interface.h"

void Interface::OpenApp()
{
	int side;
	cout << "Выберите сторону: 1 - Лисы, 2 - Курицы" << endl
		<< "> ";
	while (!(cin >> side))
	{
		cin.clear();
		while (cin.get() != '\n');

		cout << "Ошибка. Введите целое число: ";
	}
	switch (side)
	{
	case 1:
		type = FROM_FOX;
		break;
	case 2:
		type = FROM_CHICKEN;
		break;
	default:
		throw (string)"Ошибка выбора стороны.";
	}
	this->window = new sf::RenderWindow(sf::VideoMode(700, 700), "2 FOX AND 20 CHICKENS");

	sf::Image image;
	image.loadFromFile("zone.png");
	tBackground.loadFromImage(image);
	background.setTexture(tBackground);

	image.loadFromFile("fox.png");
	tFox.loadFromImage(image);
	fox.setTexture(tFox);

	image.loadFromFile("chicken.jpg");
	tChicken.loadFromImage(image);
	chicken.setTexture(tChicken);
}

void Interface::IsReady()
{
	bool isSelect = false;
	bool whoMove = type == FROM_CHICKEN;
	Cell* ptrSelected = nullptr;

	srand(time(NULL));
	/*
		[1] - Сделать пошаговый ход игрков - OK
		[2] - Сделать выделение и логику ходов - OK
		[3] - Сделать логику победы одной из сторон - OK
		[4] - Сделать логику поедание лисами - OK
	*/
	sf::Event event;
	CreateArea();
	render();
	while (window->isOpen())
	{
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window->close();
			if (whoMove)
			{
				if (event.type == sf::Event::MouseButtonPressed)//если нажата клавиша мыши
					if (event.key.code == sf::Mouse::Left)//а именно левая
					{
						sf::Vector2i cordMouse = sf::Mouse::getPosition(*window); //Получаем позицию мышки при нажатии на ЛКМ
						if (!isSelect)
						{
							switch (arr[cordMouse.x / 100 * 7 + cordMouse.y / 100]->type)
							{
							case FOX:
								if (type != FROM_FOX)
									break;
								isSelect = !isSelect;
								break;
							case CHICKEN:
								if (type != FROM_CHICKEN)
									break;
								isSelect = !isSelect;
								break;
							default:
								break;
							}
							if (isSelect)
							{
								arr[cordMouse.x / 100 * 7 + cordMouse.y / 100]->isSelected = true;
								ptrSelected = arr[cordMouse.x / 100 * 7 + cordMouse.y / 100];
							}
						}
						else
						{
							int i, j;
							int step_i, step_j;
							switch (arr[cordMouse.x / 100 * 7 + cordMouse.y / 100]->type)
							{
							case EMPTY:
								i = cordMouse.x / 100;
								j = cordMouse.y / 100;
								step_i = abs(i - (ptrSelected->cords.col / 100));
								step_j = abs(j - (ptrSelected->cords.row / 100));
								if ((step_i == 1 || step_i == 0) && (step_j == 1 || step_j == 0))
								{
									if (!((step_i + step_j) % 2))
										break;
									if (j - (ptrSelected->cords.row / 100) == 1 && ptrSelected->type == CHICKEN)
										break;
									arr[i * 7 + j]->type = ptrSelected->type;
									ptrSelected->type = EMPTY;
									whoMove = false;
								}
								break;
							default:
								break;
							}
							isSelect = !isSelect;
							ptrSelected->isSelected = false;
							ptrSelected = nullptr;
						}
						render();
						CheckWin();
					}
			}
			else
			{
				int col, row;
				int irand, irandstep;
				bool next = false;
				bool flag = false;
				vector<Cell*> tmp;
				switch (type)
				{
				case FROM_CHICKEN:
					for (size_t i = 0; i < POLE; i++)
					{
						if (arr[i]->type == FOX)
						{
							col = i / 7;
							row = i - col * 7;
							tmp.clear();
							CheckFoxMove(col, row, tmp);
							if (tmp.size())
							{
								for (size_t i = 0; i < tmp.size(); i=i+2)
								{
									if (tmp[i]->type = CHICKEN)
										tmp[i]->type = EMPTY;
									if (i == (tmp.size() - 2))
									{
										arr[col * 7 + row]->type = EMPTY;
										tmp[i+1]->type = FOX;
										flag = true;
										render();
										Sleep(1000);
										break;
									}
									if (i != 0) {
										tmp[i + 1]->type = FOX;
										tmp[i - 1]->type = EMPTY;
									}
									render();
									Sleep(1000);
								}
							}
							if (flag)
								break;
						}
					}
					if (flag)
					{
						whoMove = true;
						render();
						Sleep(1000);
						CheckWin();
						break;
					}
					irand = rand() % 2;
					for (size_t i = 0; i < POLE; i++)
					{
						if (arr[i]->type == FOX)
						{
							if (irand && !next)
							{
								next = true;
								continue;
							}
							col = i / 7;
							row = i - col * 7;
							irandstep = rand() % 4 + 1;
							switch (irandstep)
							{
							case 1:
								if (row > 0 && arr[col * 7 + (row - 1)]->type == EMPTY)
								{
									arr[col * 7 + (row - 1)]->type = arr[i]->type;
									arr[i]->type = EMPTY;
									flag = true;
								}
								break;
							case 2:
								if (col < 6 && arr[(col + 1) * 7 + row]->type == EMPTY)
								{
									arr[(col + 1) * 7 + row]->type = arr[i]->type;
									arr[i]->type = EMPTY;
									flag = true;
								}
								break;
							case 3:
								if (col > 0 && arr[(col - 1) * 7 + row]->type == EMPTY)
								{
									arr[(col - 1) * 7 + row]->type = arr[i]->type;
									arr[i]->type = EMPTY;
									flag = true;
								}
								break;
							case 4:
								if (row < 6 && arr[col * 7 + (row + 1)]->type == EMPTY)
								{
									arr[col * 7 + (row + 1)]->type = arr[i]->type;
									arr[i]->type = EMPTY;
									flag = true;
								}
								break;
							default:
								i = 0;
								irand == 1 ? irand = 0 : irand = 1;
								next = false;
								break;
							}
							if (flag)
							{
								whoMove = true;
								render();
								Sleep(1000);
								CheckWin();
								break;
							}
							
						}
					}
					break;
				case FROM_FOX:
					for (size_t i = 0; i < POLE; i++)
					{
						if (arr[i]->type == CHICKEN)
						{
							col = i / 7;
							row = i - col * 7;
							if (row > 0 && arr[col * 7 + (row - 1)]->type == EMPTY)
							{
								arr[col * 7 + (row - 1)]->type = arr[i]->type;
								arr[i]->type = EMPTY;
								break;
							}
							if (col < 6 && arr[(col + 1) * 7 + row]->type == EMPTY && col <= 3)
							{
								arr[(col + 1) * 7 + row]->type = arr[i]->type;
								arr[i]->type = EMPTY;
								break;
							}
							if (col > 0 && arr[(col - 1) * 7 + row]->type == EMPTY && col >= 3)
							{
								arr[(col - 1) * 7 + row]->type = arr[i]->type;
								arr[i]->type = EMPTY;
								break;
							}
						}
					}
					whoMove = true;
					render();
					Sleep(1000);
					CheckWin();
					for (size_t i = 0; i < POLE; i++)
					{
						if (arr[i]->type == FOX)
						{
							col = i / 7;
							row = i - col * 7;
							tmp.clear();
							CheckFoxMove(col, row, tmp);
							if (tmp.size())
							{
								for (size_t i = 0; i < tmp.size(); i=i+2)
								{
									if (tmp[i]->type = CHICKEN)
										tmp[i]->type = EMPTY;
									if (i == (tmp.size() - 2))
									{
										arr[col * 7 + row]->type = EMPTY;
										tmp[i+1]->type = FOX;
										flag = true;
										render();
										Sleep(1000);
										break;
									}
									if (i != 0) {
										tmp[i + 1]->type = FOX;
										tmp[i - 1]->type = EMPTY;
									}
									render();
									Sleep(1000);
								}
							}
							if (flag) {
								whoMove = false;
								CheckWin();
								break;
							}
						}
					}
					break;
				default:
					break;
				}
			}
		}
	}
}

void Interface::CreateArea()
{
	Сharacters startChart[7][7] = { {NONE, NONE, EMPTY, EMPTY, EMPTY, NONE, NONE},
							{NONE, NONE, EMPTY, EMPTY, EMPTY, NONE, NONE},
							{EMPTY, EMPTY, FOX, EMPTY, FOX, EMPTY, EMPTY},
							{CHICKEN, CHICKEN, CHICKEN, CHICKEN, CHICKEN, CHICKEN, CHICKEN},
							{CHICKEN, CHICKEN, CHICKEN, CHICKEN, CHICKEN, CHICKEN, CHICKEN},
							{NONE, NONE, CHICKEN, CHICKEN, CHICKEN, NONE, NONE},
							{NONE, NONE, CHICKEN, CHICKEN, CHICKEN, NONE, NONE} };
	window->clear();
	window->draw(background);
	for (size_t i = 0; i < 7; i++)
	{
		for (size_t j = 0; j < 7; j++)
		{
			switch (startChart[j][i])
			{
			case FOX:
				fox.setPosition(i * 100 + 1, j * 100 + 1);
				window->draw(fox);
				break;
			case CHICKEN:
				chicken.setPosition(i * 100 + 1, j * 100 + 1);
				window->draw(chicken);
				break;
			default:
				break;
			}
			if(arr[i * 7 + j] != nullptr)
				delete arr[i * 7 + j];
			arr[i * 7 + j] = new Cell(startChart[j][i], i * 100, j * 100);
		}
	}
}

int Interface::CheckFoxMove(int col, int row, vector<Cell*>& ptr)
{
	int anyMove[4] = { 0,0,0,0 };
	vector<Cell*> vAnyMove[4];
	bool flag;
	int maxPath = -1;
	int maxPathId = -1;
	if (col > 1 && row > 1 && arr[(col - 1) * 7 + row - 1]->type == CHICKEN && arr[(col - 2) * 7 + row - 2]->type == EMPTY)
	{
		flag = false;
		for (size_t i = 0; i < ptr.size(); i++)
		{
			if (ptr[i] == arr[(col - 1) * 7 + row - 1])
				flag = true;
		}
		if (!flag)
		{
			ptr.push_back(arr[(col - 1) * 7 + row - 1]);
			ptr.push_back(arr[(col - 2) * 7 + row - 2]);
			vAnyMove[0] = ptr;
			ptr.clear();
			anyMove[0] = 1 + CheckFoxMove(col - 2, row - 2, vAnyMove[0]);
			if (anyMove[0] > maxPath)
			{
				maxPath = anyMove[0]; 
				maxPathId = 0;
			}
		}
	}
	if (col > 1 && row < 5 && arr[(col - 1) * 7 + row + 1]->type == CHICKEN && arr[(col - 2) * 7 + row + 2]->type == EMPTY)
	{
		flag = false;
		for (size_t i = 0; i < ptr.size(); i++)
		{
			if (ptr[i] == arr[(col - 1) * 7 + row + 1])
				flag = true;
		}
		if (!flag)
		{
			ptr.push_back(arr[(col - 1) * 7 + row + 1]);
			ptr.push_back(arr[(col - 2) * 7 + row + 2]);
			vAnyMove[1] = ptr;
			ptr.clear();
			anyMove[1] = 1 + CheckFoxMove(col - 2, row + 2, vAnyMove[1]);
			if (anyMove[1] > maxPath)
			{
				maxPath = anyMove[1];
				maxPathId = 1;
			}
		}
	}
	if (col < 5 && row < 5 && arr[(col + 1) * 7 + row + 1]->type == CHICKEN && arr[(col + 2) * 7 + row + 2]->type == EMPTY)
	{
		flag = false;
		for (size_t i = 0; i < ptr.size(); i++)
		{
			if (ptr[i] == arr[(col + 1) * 7 + row + 1])
				flag = true;
		}
		if (!flag)
		{
			ptr.push_back(arr[(col + 1) * 7 + row + 1]);
			ptr.push_back(arr[(col + 2) * 7 + row + 2]);
			vAnyMove[2] = ptr;
			ptr.clear();
			anyMove[2] = 1 + CheckFoxMove(col + 2, row + 2, vAnyMove[2]);
			if (anyMove[2] > maxPath)
			{
				maxPath = anyMove[2];
				maxPathId = 2;
			}
		}
	}
	if (col < 5 && row > 1 && arr[(col + 1) * 7 + row - 1]->type == CHICKEN && arr[(col + 2) * 7 + row - 2]->type == EMPTY)
	{
		flag = false;
		for (size_t i = 0; i < ptr.size(); i++)
		{
			if (ptr[i] == arr[(col + 1) * 7 + row - 1])
				flag = true;
		}
		if (!flag)
		{
			ptr.push_back(arr[(col + 1) * 7 + row - 1]);
			ptr.push_back(arr[(col + 2) * 7 + row - 2]);
			vAnyMove[3] = ptr;
			ptr.clear();
			anyMove[3] = 1 + CheckFoxMove(col + 2, row - 2, vAnyMove[3]);
			if (anyMove[3] > maxPath)
			{
				maxPath = anyMove[3];
				maxPathId = 3;
			}
		}
	}
	if (maxPath == -1)
		return 0;
	ptr.clear();
	for (size_t i = 0; i < vAnyMove[maxPathId].size(); i++)
	{
		ptr.push_back(vAnyMove[maxPathId][i]);
	}
	return anyMove[maxPathId];
}

int Interface::CheckWin()
{
	int countChickens = 0;
	bool flag = true;
	for (size_t i = 2; i < 5; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			if (arr[i * 7 + j]->type != CHICKEN)
				flag = false;
		}
	}
	if (flag) {
		cout << "CHICKEN WIN" << endl;
		system("pause");
		CreateArea();
		render();
		return 1;
	}
	for (size_t i = 0; i < POLE; i++)
	{
		if (arr[i]->type == CHICKEN)
			countChickens++;
	}
	if (countChickens < 9) {
		cout << "FOX WIN" << endl;
		system("pause");
		CreateArea();
		render();
		return 2;
	}
	return 0;
}

void Interface::render()
{
	window->clear();
	window->draw(background);

	sf::Vector2i SelectedBlock;
	bool isSelect = false;

	for (size_t i = 0; i < POLE; i++)
	{
		switch (arr[i]->type)
		{
		case FOX:
			fox.setPosition(arr[i]->cords.col + 1, arr[i]->cords.row + 1);
			window->draw(fox);
			break;
		case CHICKEN:
			chicken.setPosition(arr[i]->cords.col + 1, arr[i]->cords.row + 1);
			window->draw(chicken);
			break;
		default:
			break;
		}
		if (arr[i]->isSelected)
		{
			isSelect = true;
			SelectedBlock.x = arr[i]->cords.col;
			SelectedBlock.y = arr[i]->cords.row;
		}
	}

	if (isSelect)
	{
		drawBlock(SelectedBlock.x + 5, SelectedBlock.y + 5, 90);
	}
	
	window->display();
}

void Interface::drawBlock(int x, int y, int size)
{
	sf::RectangleShape rectangle(sf::Vector2f((float)size, (float)size));
	rectangle.setFillColor(sf::Color(255, 255, 255, 0));
	rectangle.setOutlineThickness(5);
	rectangle.setOutlineColor(sf::Color::Red);
	rectangle.setPosition((float)x, (float)y);
	window->draw(rectangle);
}
