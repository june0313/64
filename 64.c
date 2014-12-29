#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
#include "64.h"

void startGame()
// ������ �����Ѵ�.
{
	char ch;
	int state = PLAYABLE;

	currentScore = 0;
	initializeBoard();
	displayGameScreen();

	// ���� �Է� ���
	// ��ҹ��ڸ� ��� ó���� �� �־�� �Ѵ�.
	while (ch = _getch())
	{
		switch (ch)
		{
		case 'w':
		case 'W':
			mergeUp(); break;
		case 'a':
		case 'A':
			mergeLeft(); break;
		case 's':
		case 'S':
			mergeDown(); break;
		case 'd':
		case 'D':
			mergeRight(); break;
		default:
			break;
		}

		if (ch == 'x' || ch == 'X') /*��ҹ��ڸ� ��� ó���� �� �־�� �Ѵ�.*/
		{
			exit(1);
			break;
		}

		// ������ ���¸� üũ�Ѵ�.
		state = getGameState();

		// ������ ���̻� ������ �� �ִ� ���°� �ƴϸ� �ݺ����� �������´�.
		if (state != PLAYABLE)
			break;
	}

	if (state == CLEAR)
	{
		printf("\n%d�� ��������ϴ�!\n", MAX);
		if (currentScore > bestScore)
		{
			bestScore = currentScore;
			printf("%d������ BEST Score�� �����Ͽ����ϴ�!\n", currentScore);
		}
	}
	else
	{
		printf("\n%d�� ������ ���߽��ϴ�.\n", MAX);
	}
}

void setRandonNumber(unsigned int seed)
// ������ ������ ��ġ�� 2 �Ǵ� 4�� �����Ѵ�.
{
	int rNum;	// 2 �Ǵ� 4
	int rRow;	// 0 ~ 3�� �ε���
	int rCol;	// 0 ~ 3�� �ε���

	// ���� �� ����
	srand(seed);
	rNum = (rand() % 2 + 1) * 2;

	do
	{
		rRow = rand() % 4;
		rCol = rand() % 4;
	} while (board[rRow][rCol] != 0);

	board[rRow][rCol] = rNum;
}

void initializeBoard()
// ���带 �ʱ�ȭ �Ѵ�.
// 2 �Ǵ� 4�� ������ ������ ��ġ �� ���� ������Ų��.
{
	clearBoard();
	setRandonNumber((unsigned)time(NULL));
	setRandonNumber((unsigned)time(NULL) * (unsigned)time(NULL));
}

void clearBoard()
// ������ ��� ���ڸ� 0���� �����Ѵ�.
{
	int row, col;
	for (row = 0; row < LENGTH; row++)
	{
		for (col = 0; col < LENGTH; col++)
		{
			board[row][col] = 0;
		}
	}
}

int getGameState()
// ������ ���¸� �����Ѵ�.
{
	int i, j;
	int state = FAIL;
	
	for (i = 0; i < LENGTH; i++)
	{
		for (j = 0; j < LENGTH - 1; j++)
		{
			if (board[i][j] == board[i][j + 1])
			{
				state = PLAYABLE;
				break;
			}
		}

		if (state == PLAYABLE)
			break;
	}

	for (i = 0; i < LENGTH; i++)
	{
		for (j = 0; j < LENGTH - 1; j++)
		{
			if (board[j][i] == board[j+1][i])
			{
				state = PLAYABLE;
				break;
			}
		}
		if (state == PLAYABLE)
			break;
	}

	for (i = 0; i < LENGTH; i++)
	{
		for (j = 0; j < LENGTH; j++)
		{
			// �ִ� ���� �߰ߵǸ� �ٷ� �������´�.
			if (board[i][j] == MAX)
			{
				state = CLEAR;
				break;
			}

			// ��ĭ�� �ϳ��� ������ �÷��� ������ ���°� �ȴ�.
			if (board[i][j] == 0)
			{
				state = PLAYABLE;
			}
		}

		if (state == CLEAR)
			break;
	}

	return state;
}

void displayGameScreen()
// ��ü ���� ȭ���� ����Ѵ�.
{
	printBoard();
	puts("");
	printScore();
	puts("");
	printHelp();
}

void printBoard()
// ���� ���� ȭ�鿡 ����Ѵ�.
{
	int row, col;
	
	system("cls");
	puts("+-------+-------+-------+-------+");
	for (row = 0; row < LENGTH; row++)
	{
		puts("|       |       |       |       |");
		printf("|");
		for (col = 0; col < LENGTH; col++)
		{
			if (board[row][col] != 0)
				printf("  %2d   |", board[row][col]);
			else
				printf("       |", board[row][col]);
		}
		puts("");
		puts("|       |       |       |       |");
		puts("+-------+-------+-------+-------+");
	}
}

void printScore()
// ȭ�鿡 ������ ����Ѵ�.
{
	printf("%-5s : %d\n", "BEST", bestScore);
	printf("%-5s : %d\n", "SCORE", currentScore);
}

void printHelp()
// ������ ����Ѵ�.
{
	puts("W : ��� ���� ����");
	puts("A : ��� ���� ��������");
	puts("S : ��� ���� �Ʒ���");
	puts("D : ��� ���� ����������");
	puts("X : ���� ����");
}

void mergeUp()
// �������� �����Ѵ�.
{
	int col, i, j;
	int isMoved = 0, isMerged = 0;

	// ��� ���� ���� ������ ���� �����Ѵ�.
	for (col = 0; col < LENGTH ; col++)
	{
		for (i = 0; i < LENGTH - 1; i++)
		{
			// ��ĭ�� �׳� ��������.
			if (board[i][col] == 0)
				continue;

			// ����ĭ�� �ٸ� ����(0 ����)�� ��������� �׳� ��������. 
			if (board[i + 1][col] != 0 && board[i + 1][col] != board[i][col])
				continue;

			// ����ĭ�� 0�̰ų� ���� ���ڰ� �ִ°�� �����Ѵ�.
			for (j = i + 1; j < LENGTH; j++)
			{
				if (board[i][col] == board[j][col])
				{
					board[i][col] *= 2;
					board[j][col] = 0;

					isMerged = 1;

					// ���տ� �����ϸ� ������ ������Ų��.
					currentScore += board[i][col];
					break;
				}
			}

		}
	}

	// ���� �о��.
	for (col = 0; col < LENGTH; col++)
	{
		for (i = 1; i < LENGTH; i++)
		{
			// ��ĭ�� �׳� ��������.
			if (board[i][col] == 0)
				continue;

			for (j = i - 1; j >= 0; j--)
			{
				if (board[j][col] == 0)
				{
					board[j][col] = board[j + 1][col];
					board[j + 1][col] = 0;

					isMoved = 1;
				}
			}
		}
	}
	
	// ������ �Ǿ��ٸ� ���� ���ڸ� �����Ѵ�.
	if (isMoved || isMerged)
		setRandonNumber((unsigned)time(NULL));

	// ���� ��� ���
	displayGameScreen();
}

void mergeDown()
// �Ʒ������� �����Ѵ�.
{
	int col, i, j;
	int isMoved = 0, isMerged = 0;

	// ��� ���� ���� ������ ���� �����Ѵ�.
	for (col = 0; col < LENGTH; col++)
	{
		for (i = LENGTH - 1; i > 0; i--)
		{
			// ��ĭ�� �׳� ��������.
			if (board[i][col] == 0)
				continue;

			// ����ĭ�� �ٸ� ����(0 ����)�� ��������� �׳� ��������. 
			if (board[i - 1][col] != 0 && board[i - 1][col] != board[i][col])
				continue;

			// ����ĭ�� 0�̰ų� ���� ���ڰ� �ִ°�� �����Ѵ�.
			for (j = i - 1; j >= 0 ; j--)
			{
				if (board[i][col] == board[j][col])
				{
					board[i][col] *= 2;
					board[j][col] = 0;

					isMerged = 1;

					// ���տ� �����ϸ� ������ ������Ų��.
					currentScore += board[i][col];

					break;
				}
			}
		}
	}

	// ��� ���� ���� �Ʒ��� ������.
	for (col = 0; col < LENGTH; col++)
	{
		for (i = LENGTH - 1; i >= 0; i--)
		{
			// ��ĭ�� �׳� ��������.
			if (board[i][col] == 0)
				continue;

			for (j = i + 1; j < LENGTH; j++)
			{
				if (board[j][col] == 0)
				{
					board[j][col] = board[j - 1][col];
					board[j - 1][col] = 0;

					isMoved = 1;
				}
			}
		}
	}

	// ������ �Ǿ��ٸ� ���� ���ڸ� �����Ѵ�.
	if (isMoved || isMerged)
		setRandonNumber((unsigned)time(NULL));

	// ���� ��� ���
	displayGameScreen();
}

void mergeLeft()
// �������� �����Ѵ�.
{
	int row, i, j;
	int isMoved = 0, isMerged = 0;

	// ��� �࿡ ���� ������ ���� �����Ѵ�.
	for (row = 0; row < LENGTH; row++)
	{
		for (i = 0; i < LENGTH - 1; i++)
		{
			// ��ĭ�� �׳� ��������.
			if (board[row][i] == 0)
				continue;

			// ����ĭ�� �ٸ� ����(0 ����)�� ��������� �׳� ��������. 
			if (board[row][i + 1] != 0 && board[row][i + 1] != board[row][i])
				continue;

			// ����ĭ�� 0�̰ų� ���� ���ڰ� �ִ°�� �����Ѵ�.
			for (j = i + 1; j < LENGTH; j++)
			{
				if (board[row][i] == board[row][j])
				{
					board[row][i] *= 2;
					board[row][j] = 0;

					isMerged = 1;

					// ���տ� �����ϸ� ������ ������Ų��.
					currentScore += board[row][i];

					break;
				}
			}
		}
	}


	// ��� �࿡ ���ؼ� �ݺ��Ѵ�.
	for (row = 0; row < LENGTH; row++)
	{
		for (i = 1; i < LENGTH; i++)
		{
			// ��ĭ�� �׳� ��������.
			if (board[row][i] == 0)
				continue;

			for (j = i - 1; j >= 0; j--)
			{
				if (board[row][j] == 0)
				{
					board[row][j] = board[row][j + 1];
					board[row][j + 1] = 0;

					isMoved = 1;
				}
			}
		}
	}

	// ������ �Ǿ��ٸ� ���� ���ڸ� �����Ѵ�.
	if (isMoved || isMerged)
		setRandonNumber((unsigned)time(NULL));

	// ���� ��� ���
	displayGameScreen();
}

void mergeRight()
// ���������� �����Ѵ�.
{
	int row, i, j;
	int isMoved = 0, isMerged = 0;

	// ��� �࿡ ���� ������ ���� �����Ѵ�.
	for (row = 0; row < LENGTH; row++)
	{
		for (i = LENGTH - 1; i > 0; i--)
		{
			// ��ĭ�� �׳� ��������.
			if (board[row][i] == 0)
				continue;

			// ����ĭ�� �ٸ� ����(0 ����)�� ��������� �׳� ��������. 
			if (board[row][i - 1] != 0 && board[row][i - 1] != board[row][i])
				continue;

			// ����ĭ�� 0�̰ų� ���� ���ڰ� �ִ°�� �����Ѵ�.
			for (j = i - 1; j >= 0; j--)
			{
				if (board[row][i] == board[row][j])
				{
					board[row][i] *= 2;
					board[row][j] = 0;

					isMerged = 0;

					// ���տ� �����ϸ� ������ ������Ų��.
					currentScore += board[row][i];

					break;
				}
			}
		}
	}

	// ��� �࿡ ���ؼ� �ݺ��Ѵ�.
	for (row = 0; row < LENGTH; row++)
	{
		for (i = LENGTH - 1; i >= 0; i--)
		{
			// ��ĭ�� �׳� ��������.
			if (board[row][i] == 0)
				continue;

			for (j = i + 1; j < LENGTH; j++)
			{
				if (board[row][j] == 0)
				{
					board[row][j] = board[row][j - 1];
					board[row][j - 1] = 0;

					isMoved = 1;
				}
			}
		}
	}

	// ������ �Ǿ��ٸ� ���� ���ڸ� �����Ѵ�.
	if (isMoved || isMerged)
		setRandonNumber((unsigned)time(NULL));

	// ���� ��� ���
	displayGameScreen();
}