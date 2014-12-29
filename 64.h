#ifndef _64_H_
#define _64_H_

#define LENGTH 4
#define MAX 64

#define PLAYABLE 0
#define CLEAR 1
#define FAIL 2

int board[LENGTH][LENGTH];
int bestScore;
int currentScore;

void startGame();
int getGameState();

void setRandonNumber(unsigned int seed);
void initializeBoard();
void clearBoard();

void displayGameScreen();
void printBoard();
void printScore();
void printHelp();

void mergeUp();
void mergeDown();
void mergeLeft();
void mergeRight();

#endif