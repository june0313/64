#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
#include "64.h"

void startGame()
// 게임을 시작한다.
{
	char ch;
	int state = PLAYABLE;

	currentScore = 0;
	initializeBoard();
	displayGameScreen();

	// 문자 입력 대기
	// 대소문자를 모두 처리할 수 있어야 한다.
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

		if (ch == 'x' || ch == 'X') /*대소문자를 모두 처리할 수 있어야 한다.*/
		{
			exit(1);
			break;
		}

		// 게임의 상태를 체크한다.
		state = getGameState();

		// 게임을 더이상 진행할 수 있는 상태가 아니면 반복문을 빠져나온다.
		if (state != PLAYABLE)
			break;
	}

	if (state == CLEAR)
	{
		printf("\n%d를 만들었습니다!\n", MAX);
		if (currentScore > bestScore)
		{
			bestScore = currentScore;
			printf("%d점으로 BEST Score를 갱신하였습니다!\n", currentScore);
		}
	}
	else
	{
		printf("\n%d를 만들지 못했습니다.\n", MAX);
	}
}

void setRandonNumber(unsigned int seed)
// 보드의 임의의 위치에 2 또는 4를 설정한다.
{
	int rNum;	// 2 또는 4
	int rRow;	// 0 ~ 3의 인덱스
	int rCol;	// 0 ~ 3의 인덱스

	// 랜덤 수 생성
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
// 보드를 초기화 한다.
// 2 또는 4를 보드의 랜덤한 위치 두 곳에 생성시킨다.
{
	clearBoard();
	setRandonNumber((unsigned)time(NULL));
	setRandonNumber((unsigned)time(NULL) * (unsigned)time(NULL));
}

void clearBoard()
// 보드의 모든 숫자를 0으로 설정한다.
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
// 게임의 상태를 리턴한다.
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
			// 최대 값이 발견되면 바로 빠져나온다.
			if (board[i][j] == MAX)
			{
				state = CLEAR;
				break;
			}

			// 빈칸이 하나라도 있으면 플레이 가능한 상태가 된다.
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
// 전체 게임 화면을 출력한다.
{
	printBoard();
	puts("");
	printScore();
	puts("");
	printHelp();
}

void printBoard()
// 게임 판을 화면에 출력한다.
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
// 화면에 점수를 출력한다.
{
	printf("%-5s : %d\n", "BEST", bestScore);
	printf("%-5s : %d\n", "SCORE", currentScore);
}

void printHelp()
// 도움말을 출력한다.
{
	puts("W : 모든 숫자 위로");
	puts("A : 모든 숫자 왼쪽으로");
	puts("S : 모든 숫자 아래로");
	puts("D : 모든 숫자 오른쪽으로");
	puts("X : 게임 종료");
}

void mergeUp()
// 위쪽으로 병합한다.
{
	int col, i, j;
	int isMoved = 0, isMerged = 0;

	// 모든 열에 대해 병합을 먼저 진행한다.
	for (col = 0; col < LENGTH ; col++)
	{
		for (i = 0; i < LENGTH - 1; i++)
		{
			// 빈칸은 그냥 지나간다.
			if (board[i][col] == 0)
				continue;

			// 다음칸에 다른 숫자(0 제외)가 들어있으면 그냥 지나간다. 
			if (board[i + 1][col] != 0 && board[i + 1][col] != board[i][col])
				continue;

			// 다음칸에 0이거나 같은 숫자가 있는경우 병합한다.
			for (j = i + 1; j < LENGTH; j++)
			{
				if (board[i][col] == board[j][col])
				{
					board[i][col] *= 2;
					board[j][col] = 0;

					isMerged = 1;

					// 병합에 성공하면 점수를 증가시킨다.
					currentScore += board[i][col];
					break;
				}
			}

		}
	}

	// 위로 밀어낸다.
	for (col = 0; col < LENGTH; col++)
	{
		for (i = 1; i < LENGTH; i++)
		{
			// 빈칸은 그냥 지나간다.
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
	
	// 병합이 되었다면 랜덤 숫자를 생성한다.
	if (isMoved || isMerged)
		setRandonNumber((unsigned)time(NULL));

	// 병합 결과 출력
	displayGameScreen();
}

void mergeDown()
// 아래쪽으로 병합한다.
{
	int col, i, j;
	int isMoved = 0, isMerged = 0;

	// 모든 열에 대해 병합을 먼저 진행한다.
	for (col = 0; col < LENGTH; col++)
	{
		for (i = LENGTH - 1; i > 0; i--)
		{
			// 빈칸은 그냥 지나간다.
			if (board[i][col] == 0)
				continue;

			// 다음칸에 다른 숫자(0 제외)가 들어있으면 그냥 지나간다. 
			if (board[i - 1][col] != 0 && board[i - 1][col] != board[i][col])
				continue;

			// 다음칸에 0이거나 같은 숫자가 있는경우 병합한다.
			for (j = i - 1; j >= 0 ; j--)
			{
				if (board[i][col] == board[j][col])
				{
					board[i][col] *= 2;
					board[j][col] = 0;

					isMerged = 1;

					// 병합에 성공하면 점수를 증가시킨다.
					currentScore += board[i][col];

					break;
				}
			}
		}
	}

	// 모든 열에 대해 아래로 모은다.
	for (col = 0; col < LENGTH; col++)
	{
		for (i = LENGTH - 1; i >= 0; i--)
		{
			// 빈칸은 그냥 지나간다.
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

	// 병합이 되었다면 랜덤 숫자를 생성한다.
	if (isMoved || isMerged)
		setRandonNumber((unsigned)time(NULL));

	// 병합 결과 출력
	displayGameScreen();
}

void mergeLeft()
// 왼쪽으로 병합한다.
{
	int row, i, j;
	int isMoved = 0, isMerged = 0;

	// 모든 행에 대해 병합을 먼저 진행한다.
	for (row = 0; row < LENGTH; row++)
	{
		for (i = 0; i < LENGTH - 1; i++)
		{
			// 빈칸은 그냥 지나간다.
			if (board[row][i] == 0)
				continue;

			// 다음칸에 다른 숫자(0 제외)가 들어있으면 그냥 지나간다. 
			if (board[row][i + 1] != 0 && board[row][i + 1] != board[row][i])
				continue;

			// 다음칸에 0이거나 같은 숫자가 있는경우 병합한다.
			for (j = i + 1; j < LENGTH; j++)
			{
				if (board[row][i] == board[row][j])
				{
					board[row][i] *= 2;
					board[row][j] = 0;

					isMerged = 1;

					// 병합에 성공하면 점수를 증가시킨다.
					currentScore += board[row][i];

					break;
				}
			}
		}
	}


	// 모든 행에 대해서 반복한다.
	for (row = 0; row < LENGTH; row++)
	{
		for (i = 1; i < LENGTH; i++)
		{
			// 빈칸은 그냥 지나간다.
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

	// 병합이 되었다면 랜덤 숫자를 생성한다.
	if (isMoved || isMerged)
		setRandonNumber((unsigned)time(NULL));

	// 병합 결과 출력
	displayGameScreen();
}

void mergeRight()
// 오른쪽으로 병합한다.
{
	int row, i, j;
	int isMoved = 0, isMerged = 0;

	// 모든 행에 대해 병합을 먼저 진행한다.
	for (row = 0; row < LENGTH; row++)
	{
		for (i = LENGTH - 1; i > 0; i--)
		{
			// 빈칸은 그냥 지나간다.
			if (board[row][i] == 0)
				continue;

			// 다음칸에 다른 숫자(0 제외)가 들어있으면 그냥 지나간다. 
			if (board[row][i - 1] != 0 && board[row][i - 1] != board[row][i])
				continue;

			// 다음칸에 0이거나 같은 숫자가 있는경우 병합한다.
			for (j = i - 1; j >= 0; j--)
			{
				if (board[row][i] == board[row][j])
				{
					board[row][i] *= 2;
					board[row][j] = 0;

					isMerged = 0;

					// 병합에 성공하면 점수를 증가시킨다.
					currentScore += board[row][i];

					break;
				}
			}
		}
	}

	// 모든 행에 대해서 반복한다.
	for (row = 0; row < LENGTH; row++)
	{
		for (i = LENGTH - 1; i >= 0; i--)
		{
			// 빈칸은 그냥 지나간다.
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

	// 병합이 되었다면 랜덤 숫자를 생성한다.
	if (isMoved || isMerged)
		setRandonNumber((unsigned)time(NULL));

	// 병합 결과 출력
	displayGameScreen();
}