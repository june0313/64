#include <stdio.h>
#include "64.h"

int main()
{
	char onceAgain;
	
	do
	{
		startGame();

		printf("\n게임을 한 번 더 하시겠습니까? (y/n) : ");
		onceAgain = getchar();
		getchar();

	} while (onceAgain == 'y' || onceAgain == 'Y'); /*대소문자를 모두 처리할 수 있어야 한다.*/
	
	return 0;
}