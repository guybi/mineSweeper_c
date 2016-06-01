/*
Ex3 - Mine Sweeper

Name:	Shmuel Regev
ID:		303176085
*/
#include <stdio.h>
#include <stdlib.h>
#include <colorPrint.h>
#include <mineSweeper.h>
#include <getch.h>
#include <time.h>

// defining my colors for the game
#define HIDDEN 		BG_Blue
#define VISIBLE		BG_White
#define MINE		BG_Red
#define FLAGGED		BG_Green
#define CURSOR		BG_Yellow
#define NUM_COLOR	FG_Black
#define TILE_STR	"  "


bool initBoard(GameBoard *g, int rows, int cols, int level)
{
	int i;
	g->rows = rows;
	g->cols = cols;
	g->isMineClicked = FALSE;
	g->hiddenTiles = rows*cols;

	g->board = (Tile**)malloc(rows * sizeof(Tile*));
	if(g->board == NULL)
		return FALSE;
	for(i=0; i<rows; i++)
	{
		*(g->board+i) = (Tile*)(malloc(cols * sizeof(Tile)));
		if( *(g->board+i) == NULL)
			return FALSE;
	}
	
	populateMines(g, level);
	markNumbers(g);
	
	return TRUE;
} 


void clickTile(GameBoard *g, int row, int col)
{
	int i,j;
	if( !(*(g->board+row)+col)->isVisible &&  !(*(g->board+row)+col)->isFlagged )
	{
		if( (*(g->board+row)+col)->isMine)
		{
			// clicked on mine, oops
			(*(g->board+row)+col)->isVisible = TRUE;
			g->isMineClicked = TRUE;
		}
		else
		{
			if( (*(g->board+row)+col)->numOfMines > 0)
			{
				(*(g->board+row)+col)->isVisible = TRUE;
				g->hiddenTiles--;
			}
			else
			{
				// number of îeighbors mines = 0
				// call this function for all neighbors
				(*(g->board+row)+col)->isVisible = TRUE;
				g->hiddenTiles--;
				for(i=row-1; i<=row+1; i++)
				{
					for(j=col-1; j<=col+1; j++)
					{
						if( i>=0 && i<(g->rows) && j>=0 && j<(g->cols) && !(i==row && j==col) )
							clickTile(g, i, j);
					}
				}
			}
		}		
	}
}


void flagTile(GameBoard *g, int row, int col)
{
	if( !(*(g->board+row)+col)->isVisible )
	{
		if((*(g->board+row)+col)->isFlagged)
			(*(g->board+row)+col)->isFlagged=FALSE;
		else
			(*(g->board+row)+col)->isFlagged=TRUE;
	}
}


void populateMines(GameBoard *g, int level)
{
	int i, j, numOfMines;
	
	
	switch(level)
	{
		case EASY:		level = EASY_LEVEL_PERCENT;
						break;
		case MEDIUM:	level = MEDIUM_LEVEL_PERCENT;
						break;	
		case HARD:		level = HARD_LEVEL_PERCENT;
						break;
	}
	// level is now the percentage of the mines in the game
	
	

	// calculating the number of mines
	numOfMines = (g->rows)*(g->cols)*level/100;
	g->totalMines = numOfMines;
	
	// making sure we are realy random
	srand ( time(NULL) );
	
	// populating the mines
	while(numOfMines > 0)
	{
		i = rand() % (g->rows);
		j = rand() % (g->cols);
		
		if( ( *(g->board+i) +j ) -> isMine == FALSE)
		{
			( *(g->board+i) +j ) -> isMine = TRUE;
			numOfMines--;
		}
	}
}


void markNumbers(GameBoard *g)
{
	int i, j ,counter, i2, j2;
	for(i = 0 ; i < (g->rows) ; i++)
	{
		for(j = 0 ; j < (g->cols) ; j++)
		{
			if( (*(g->board+i)+j)->isMine == FALSE )
			{
				// this tile is not mine
				counter = 0;
				// count mines around (i,j)
				for(i2=i-1; i2<=i+1; i2++)
				{
					if(i2>=0 && i2<(g->rows))
					{
						for(j2=j-1; j2<=j+1; j2++)
						{
							if( j2>=0 && j2<(g->cols)&& !(i==i2 && j==j2) )
							{
								if((*(g->board+i2)+j2)->isMine)
									counter++;
							}
						}
					}
				}
				// end of counting
				
				( *(g->board+i)+j )-> numOfMines = counter;
			}
			else
			{
				// this tile is mine
				( *(g->board+i)+j )-> numOfMines = -1;
			}
		}
	}
}


void printBoard(GameBoard *g, int cursorCoords[2])
{
	int i,j;
	for(i = 0 ; i < (g->rows) ; i++)
	{
		for(j = 0 ; j < (g->cols) ; j++)
		{
			if( (*(g->board+i)+j)->isMine && (*(g->board+i)+j)->isVisible )
			{
				colorPrint(FG_Def,MINE,ATT_Def,TILE_STR);
			}
			else if(cursorCoords[0] == i  && cursorCoords[1] == j)
			{
				colorPrint(FG_Def,CURSOR,ATT_Def,TILE_STR);
			}
			else if( (*(g->board+i)+j)->isVisible )
			{
				if( (*(g->board+i)+j)->numOfMines > 0 )
					colorPrint(NUM_COLOR,VISIBLE,ATT_Def,"%2d",(*(g->board+i)+j)->numOfMines);
				else
					colorPrint(NUM_COLOR,VISIBLE,ATT_Def,TILE_STR);
			}
			else if( (*(g->board+i)+j)->isFlagged )
			{
				colorPrint(FG_Def,FLAGGED,ATT_Def,TILE_STR);
			}
			else
				colorPrint(NUM_COLOR,HIDDEN,ATT_Def,TILE_STR);
		}
		printf("\n");
	}
}
