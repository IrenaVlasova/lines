 #define _CRT_SECURE_NO_WARNINGS

#include <stdbool.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define FIELD_SIZE 9
#define ALL_COLORS 5

#define N 11
#define M 11

typedef enum {
	EMPTY,
	GREEN_BEAD,
	YELLOW_BEAD,
	BLUE_BEAD,
	RED_BEAD,
	PINK_BEAD,
	FIELD_INFO_END
}Field_Info;

typedef struct {
	int startX;
	int startY;
	int targetX;
	int targetY;
}Position;


int field_info[FIELD_SIZE][FIELD_SIZE] = { EMPTY };

bool RemoveLines()  //delete lines
{
	for (int i = 0; i < 5; i++)
	{
		field_info[i][i] = GREEN_BEAD;
	}

	int count = 1;
	for (int i = 0; i<FIELD_SIZE; i++)
	{
		for (int j = 0; j<FIELD_SIZE; j++)
		{
			while (field_info[i][j] == field_info[i][j + count])
			{
				count++;
			}
			if (count >= 5)
			{
				for (int k = 0; k <= count; k++)
				{
					  field_info[i][j + k] = EMPTY;
				}
				return true;
			}
			while (field_info[i][j] == field_info[i + count][j])
			{
				count++;
			}
			if (count >= 5)
			{
				for (int k = 0; k <= count; k++)
				{
					 field_info[i + k][j] = EMPTY;
				}
				return true;
			}
			while (field_info[i][j] == field_info[i + count][j + count])
			{
				count++;
			}
			if (count >= 5)
			{
				for (int k = 0; k <= count; k++)
				{
					 field_info[i + k][j + k] = EMPTY;
				}
				return true;
			}
		}
	}
}

void RandomFillingOfArray()
{
	for (int i = 0; i < 3; i++)
	{
		srand(time(NULL));
		int color;
		int x, y;

		do
		{
			color = rand() % ALL_COLORS + 1;
			x = rand() % FIELD_SIZE;
			y = rand() % FIELD_SIZE;
		} while (field_info[x][y] != EMPTY);

		field_info[x][y] = color;
	}
	 RemoveLines();
	

}

bool CheckIfMovementPossible(int startX, int startY, int targetX, int targetY)
{
	bool result = false;
	bool add = true;
	int cMap[FIELD_SIZE][FIELD_SIZE] = { EMPTY };
	int x, y, step = 0;

	for (x = 0; x < FIELD_SIZE; x++)
	{
		for (y = 0; y < FIELD_SIZE; y++)
		{
			if (field_info[x][y] > 0)
				cMap[x][y] = -2;//индикатор стены
			else
				cMap[x][y] = -1;//индикатор еще не ступали сюда
		}
	}
	cMap[startX][startY] = 0;//Начинаем с финиша

	while (add == true)
	{
		add = false;
		for (x = 0; x < FIELD_SIZE; x++)
		{
			for (y = 0; y < FIELD_SIZE; y++)
			{
				if (cMap[y][x] == step) //!!!
				{
					//Ставим значение шага+1 в соседние ячейки (если они проходимы)
					if (x - 1 >= 0 && cMap[y - 1][x] != -2 && cMap[y - 1][x] == -1)
						cMap[y - 1][x] = step + 1;
					if (y - 1 >= 0 && cMap[y][x - 1] != -2 && cMap[y][x - 1] == -1)
						cMap[y][x - 1] = step + 1;
					if (x + 1 <= FIELD_SIZE && cMap[y + 1][x] != -2 && cMap[y + 1][x] == -1)
						cMap[y + 1][x] = step + 1;
					if (y + 1 <= FIELD_SIZE && cMap[y][x + 1] != -2 && cMap[y][x + 1] == -1)
						cMap[y][x + 1] = step + 1;
				}
			}
		}

		step++;
		add = true;
		if (cMap[targetY][targetX] != -1)//решение найдено
		{
			result = true;
			add = false;
		}
		if (step > FIELD_SIZE * FIELD_SIZE)//решение не найдено
		{
			result = false;
			add = false;
		}
	}
	return result;
}

void PrintBoard()
{
	for (int i = 0; i<FIELD_SIZE; i++)
	{
		for (int j = 0; j<FIELD_SIZE; j++)
		{
			printf("%i ", field_info[i][j]);
			if (j == FIELD_SIZE - 1) printf("\r\n");
		}
	}

	printf("\r\n");
}

void InitBoard()
{
	for (int i = 0; i < FIELD_SIZE; i++)
	{
		for (int j = 0; j < FIELD_SIZE; j++)
		{
			field_info[i][j] = 0;
		}
	}
}

bool CheckIfHaveMoves()
{
	for (int i = 0; i < FIELD_SIZE; i++)
	{
		for (int j = 0; j < FIELD_SIZE; j++)
		{
			if (field_info[i][j] == EMPTY) return true;
		}
	}

	return false;
}

void MakeMove()
{
	int x_start, y_start, x_end, y_end;
	Position position;
	printf("\nEnter coordinates");

	do
	{
		printf("\nEnter x_start: ");
		scanf("%i", &x_start);
		printf("\nEnter y_start: ");
		scanf("%i", &y_start);
		//position.startY = x_start;
		if (field_info[x_start][y_start] == EMPTY)
			printf("\nPlease select cell with bead!");
	} while (field_info[x_start][y_start] == EMPTY);

	do
	{
		printf("\nEnter x_end: ");
		scanf("%i", &x_end);
		printf("\nEnter y_end: ");
		scanf("%i", &y_end);
		if (field_info[x_end][y_end] != EMPTY) printf("\nPlease select empty cell!");
	} while (field_info[x_end][y_end] != EMPTY);

	if (CheckIfMovementPossible(x_start, y_start, x_end, y_end))
	{
		field_info[x_end][y_end] = field_info[x_start][y_start];
		field_info[x_start][y_start] = EMPTY;
		if (RemoveLines())
		{
			MakeMove();
		}
		else
		{
			RandomFillingOfArray();
		}
	}
}

void main()
{
	RemoveLines();

	//RandomFillingOfArray();
	//do {
	//	PrintBoard();
	//	MakeMove();
	//}
 //   while (CheckIfHaveMoves());

	_getch();


}