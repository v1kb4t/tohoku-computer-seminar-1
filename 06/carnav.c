#include<stdio.h>
#include<stdlib.h>
#include<math.h>

typedef struct {
	double x,y;
} Position;

typedef struct {
	int id;
	char name[100];
	Position pos;
	double wait;
	int points;
	int next[5];
} Crossing;


