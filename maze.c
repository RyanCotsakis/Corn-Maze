#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define SIZE 151//must be odd. (SIZE-1)/2 is actual size...
#define EMPTY 'I'
#define OCC ' '

//fixed constants
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

int countAround(int searchFor, int row, int column);
int directionOfnth(int searchFor, int row, int column, int n);
int countOCC(int row, int column);
char whichChar(int index);

//0 if unoccupied, 1 if occupied.
int maze[SIZE][SIZE];

int main(void){
	int row, column, direction, count, foundOne;
	FILE* file;
	char fileName[FILENAME_MAX]="maze.txt";

	printf("Generating...\n\n");

	for(row=0;row<SIZE;row++){
		for(column=0;column<SIZE;column++)
			maze[row][column]=EMPTY;
	}

	srand((unsigned)time(NULL));

	//this is where you start (bottom left).
	row=SIZE-2;
	column=1;
	maze[row][column]=OCC;

	//this<             >is where you end (top right).
	while(maze[1][SIZE-2]==EMPTY){

		count=countAround(EMPTY,row,column);

		if(count){
			direction=directionOfnth(EMPTY,row,column,rand()%count+1);
			if(direction==UP){
				row--;
				maze[row][column]=OCC;
				row--;
				maze[row][column]=OCC;
			}
			else if(direction==RIGHT){
				column++;
				maze[row][column]=OCC;
				column++;
				maze[row][column]=OCC;
			}
			else if(direction==DOWN){
				row++;
				maze[row][column]=OCC;
				row++;
				maze[row][column]=OCC;
			}
			else{
				column--;
				maze[row][column]=OCC;
				column--;
				maze[row][column]=OCC;
			}
		}

		else{
			foundOne=0;
			row=1;
			while(row<=SIZE-2 && !foundOne){
				column=SIZE-2;
				while(column>=1 && !foundOne){
					count=countAround(OCC,row,column);
					if(count){
						foundOne=1;
						maze[row][column]=OCC;
						direction=directionOfnth(OCC,row,column,rand()%count+1);
						if(direction==DOWN)
							maze[row+1][column]=OCC;
						else
							maze[row][column-1]=OCC;
					}
					else
						column-=2;
				}
				if(!foundOne)
					row+=2;
			}
		}
	}
	//now there's a path from start to finish.
	while(1){
		int temp;

		foundOne=0; //find me some empties
		row=SIZE-2;
		while(row>=1 && !foundOne){
			column=1;
			while(column<=SIZE-2 && !foundOne){
				if(maze[row][column]==EMPTY)
					foundOne=1;
				else
					column+=2;
			}
			if(!foundOne)
				row-=2;
		}

		if(!foundOne) //havent found one? then we're done
			break;

		if(rand()%2){ //toss a coin, look horizontally or vertically for a path?
			temp=row;
			while(row>=1 && maze[row][column]==EMPTY)
				row-=2;
			if(row<1){
				row=temp;
				while(maze[row][column]==EMPTY)
					row+=2;
			}
		}
		else{
			temp=column;
			while(column>=1 && maze[row][column]==EMPTY)
				column-=2;
			if(column<1){
				column=temp;
				while(maze[row][column]==EMPTY)
					column+=2;
			}
		}

		while(1){
			count=countAround(EMPTY,row,column);
			if(count){
				direction=directionOfnth(EMPTY,row,column,rand()%count+1);
				if(direction==UP){
					row--;
					maze[row][column]=OCC;
					row--;
					maze[row][column]=OCC;
				}
				else if(direction==RIGHT){
					column++;
					maze[row][column]=OCC;
					column++;
					maze[row][column]=OCC;
				}
				else if(direction==DOWN){
					row++;
					maze[row][column]=OCC;
					row++;
					maze[row][column]=OCC;
				}
				else{
					column--;
					maze[row][column]=OCC;
					column--;
					maze[row][column]=OCC;
				}
			}
			else
				break;
		}
	}
	maze[0][SIZE-2]=OCC;
	maze[SIZE-1][1]=OCC;
	/*prints the maze on screen as 1s and 0s
	for(row=0;row<SIZE;row++){
		for(column=0;column<SIZE;column++)
			printf("%i",maze[row][column]);
		printf("\n");
	}*/
	

	file=fopen(fileName,"r");
	foundOne=1;
	while(file!=NULL){
		fclose(file);
		sprintf(fileName,"maze_%i.txt",foundOne);
		file=fopen(fileName,"r");
		foundOne++;
	}
	file=fopen(fileName,"w");
	for(row=0;row<SIZE;row++){
		for(column=0;column<SIZE;column++)
			fprintf(file,"%c",maze[row][column]);
		fprintf(file,"\n");
	}	
	fclose(file);

	for(row=0;row<=SIZE-1;row+=2){
		for(column=0;column<=SIZE-1;column+=2)
			printf("%c",whichChar(countOCC(row,column)));
		printf("\n");
	}

	system("PAUSE");
	return 0;
}

//looks at the boxes two away and counts how many searchFors there are out of the 4
int countAround(int searchFor, int row, int column){
	int count=0;

	if(row>=2 && maze[row-2][column]==searchFor)
		count++;
	if(column<SIZE-2 && maze[row][column+2]==searchFor)
		count++;
	if(row<SIZE-2 && maze[row+2][column]==searchFor)
		count++;
	if(column>=2 && maze[row][column-2]==searchFor)
		count++;

	return count;
}

//finds the nth searchFor around that space, and points in the direction of it. starts by looking up then clockwise. -1 if not found
int directionOfnth(int searchFor, int row, int column, int n){
	int lookingFor=1;

	if(row>=2 && maze[row-2][column]==searchFor){
		if(lookingFor==n)
			return UP;
		lookingFor++;
	}
	if(column<SIZE-2 && maze[row][column+2]==searchFor){
		if(lookingFor==n)
			return RIGHT;
		lookingFor++;
	}
	if(row<SIZE-2 && maze[row+2][column]==searchFor){
		if(lookingFor==n)
			return DOWN;
		lookingFor++;
	}
	if(column>=2 && maze[row][column-2]==searchFor){
		if(lookingFor==n)
			return LEFT;
	}

	printf("ERROR: Couldn't find %i %is around row %i, column %i\n",n,searchFor,row,column);
	return -1;
}

int countOCC(int row, int column){
	int yee=0;
	if(row==0 || maze[row-1][column]==OCC)
		yee+=8;
	if(column==SIZE-1 || maze[row][column+1]==OCC)
		yee+=4;
	if(row==SIZE-1 || maze[row+1][column]==OCC)
		yee+=2;
	if(column==0 || maze[row][column-1]==OCC)
		yee++;
	return yee;
}

char whichChar(int index){
	char string[]={197,195,193,192,180,179,217,' ',194,218,196,' ',191,' ',' ','\0'};
		         //0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15
	return string[index];
}