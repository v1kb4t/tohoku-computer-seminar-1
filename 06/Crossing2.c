/* Crossing2.c --- Store and display a structure */
#include <stdio.h>
#include <string.h>

#define CrossingNumber 5 /* Number of crossings=5 */
#define MaxName 50 /* Maximum number of characters=50 characters (single-byte characters) */

typedef struct {
	double x, y; /* Position x, y */
} Position; /* Structure representing a position */

typedef struct {
	int id; /* Crossing ID */
	Position pos; /* Structure representing a position */ 
	double wait; /* Average wait time */
	char name[MaxName]; /* Crossing name */
	int points; /* Number of converging roads */
	int next[5]; /* Nearest crossing ID */ 
} Crossing;

Crossing cross[CrossingNumber];

int main(void)
{
	int i,j;
	int crossing_number=0; /* Number of input crossings */
	char YesNo[16]; /* Used to ask whether the user continues to input data */
	
	for (i = 0; i < CrossingNumber; i++) { /* Input data */
		
		/* Create a program to assign data to ?cross? (structure) */

		crossing_number++; /* Increase the number of crossings by 1 */
		
		printf("Crossing ID=");
		scanf("%d", &(cross[i].id)); /* Input crossing ID */
		printf("Name=");
		scanf("%s", cross[i].name); /* Input crossing name */
		printf("x coordinate=");
		scanf("%lf", &(cross[i].pos.x)); /*Input e x-axis */
		printf("y coordinate=");
		scanf("%lf", &(cross[i].pos.y)); /* Input y-axis */
		printf("Wait time=");
		scanf("%lf", &(cross[i].wait)); /* Input wait time */
		printf("Number of converging roads=");
		scanf("%d", &(cross[i].points)); /* Input the number of converging roads */
		for (j = 0; j < cross[i].points; j++) { /* Repeat the operation by the number of converging roads */
			printf("Nearest crossing ID (No.%d)=", j);
			scanf("%d", &(cross[i].next[j])); /* Input nearest crossing ID */
		}

		/* Conditional judgment statement */

		printf("Do you input the next data? (Yes:\"y\")>"); 
		scanf("%s", YesNo);
		if (strcmp(YesNo, "y") != 0) break; /* If the input data is other than "y," end the loop */ 
	}

	for (i = 0; i < crossing_number; i++)  /* Display data */
	{
		printf("Array[%d] ID(%d), x(%lf), y(%lf), Name(%s), Wait(%lf)\n",
				i, cross[i].id, cross[i].pos.x, cross[i].pos.y, 
				cross[i].name, cross[i].wait);

		printf("\t Number of converging roads(%d)\t", cross[i].points);
	
		for(j=0; j<cross[i].points; j++) /* Repeat the operation by the number of converging roads */
		{
			printf("No.%d(%d) ", j, cross[i].next[j]); 
		}
		
		puts(""); /* New line */
	}

	return 0; 
}