/*
Ex3 - Mine Sweeper

Name:	Shmuel Regev
ID:		303176085
*/
#include <stdio.h>
#include <stdlib.h>
#include <getch.h>
#include <colorPrint.h>
#include <mineSweeper.h>

// get all the settings for the game and call initBoard
// return the pointer to the GameBoard g
GameBoard* startGame();

// free tiles, board and game memory
void freeEverything(GameBoard* g);

int main()
{
	char c = 0;

	int cursorCoords[2];	
	GameBoard *g = startGame();
	
	// starting the game - set cursor to (0,0);
	cursorCoords[0]=0;
	cursorCoords[1]=0;
	
	//clean stdin
	while(getchar()!='\n');
	
	do
	{
		clearScreen();
		switch(c)
		{
		case DOWN:
			if(cursorCoords[0] < (g->rows)-1)
				cursorCoords[0]++;
			break;
		case UP:
			if(cursorCoords[0] > 0)
				cursorCoords[0]--;
			break;
		case LEFT:
			if(cursorCoords[1] > 0)
				cursorCoords[1]--;
			break;
		case RIGHT:
			if(cursorCoords[1] < (g->cols)-1)
				cursorCoords[1]++;
			break;
		case FLAG_TILE:
			flagTile(g, cursorCoords[0], cursorCoords[1]);
			break;
		case CLICK_TILE:
			clickTile(g, cursorCoords[0], cursorCoords[1]);
			break;
		default:
			break;
		}

		printBoard(g, cursorCoords);
		if(g->isMineClicked == TRUE)
		{
			// mine is clicked, oops
			// free memory and good riddance
			colorPrint(FG_Red,BG_Def,ATT_Bright,"\nGame Lost!\n");
			freeEverything(g);
			exit(1);
		}
		else if(g->hiddenTiles == g->totalMines)
		{
			// discover all the tiles and didn't clicked on mine
			// whooho - free memory and give an award
			colorPrint(FG_Blue,BG_Def,ATT_Def,"\nGame Won!\n");
			freeEverything(g);
			exit(1);
		}
		else
		{
			printf("\nPress %c to quit.\n",QUIT);
		}
	}
	while((c = getch()) != QUIT); 

	freeEverything(g);
	return 0;
}

// free tiles, board and game memory
void freeEverything(GameBoard* g)
{
	int i;
	for(i=0 ; i<(g->rows); i++)
		free( *(g->board+i) );
	free(g->board);
	free(g);
}

// get all the settings for the game and call initBoard
// return the pointer to the GameBoard g
GameBoard* startGame()
{
	int rows=0, cols=0, level=0;
	
	// geting dimension and level
	clearScreen();
	colorPrint(FG_Blue,BG_Def,ATT_Def,"Starting a new game.");
	printf("\nEnter x dimension (1-%d): ", MAX_BOARD_DIM);
	scanf("%d",&cols);
	if(cols < 1 || cols > MAX_BOARD_DIM)
	{
		printf("Invalid X dimension.\nError creating board.\n");
		exit(1);
	}
	printf("Enter y dimension (1-%d): ", MAX_BOARD_DIM);
	scanf("%d",&rows);
	if(rows < 1 || rows > MAX_BOARD_DIM)
	{
		printf("Invalid Y dimension.\nError creating board.\n");
		exit(1);
	}
	printf("\n1) Easy\n2) Medium\n3) Hard\n");
	scanf("%d",&level);
	if(level < 1 || level >3)
	{
		printf("Invalid level select.\nError creating board.\n");
		exit(1);
	}
	
	GameBoard *g = (GameBoard*)malloc(sizeof(GameBoard));
	if(g==NULL)
	{
		printf("\nError creating board.\n");
		exit(1);
	}
	if(FALSE == initBoard(g, rows, cols, level))
	{
		printf("\nError creating board.\n");
		free(g);
		exit(1);
	}
	
	return g;
}
