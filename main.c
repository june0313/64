#include <stdio.h>
#include "64.h"

int main()
{
	char onceAgain;
	
	do
	{
		startGame();

		printf("\n������ �� �� �� �Ͻðڽ��ϱ�? (y/n) : ");
		onceAgain = getchar();
		getchar();

	} while (onceAgain == 'y' || onceAgain == 'Y'); /*��ҹ��ڸ� ��� ó���� �� �־�� �Ѵ�.*/
	
	return 0;
}