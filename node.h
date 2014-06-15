#ifndef _NODE_H_
#define _NODE_H_
#include <stdlib.h>
#include <stdio.h>

typedef struct Tnode{
	int type; /*0 undefiend 1 int，2 float 3 array 4 struct 5 var-int 6 var-float 7-24 exp*/
	int exp_type;//7-24 exp
	struct Tnode *child;
	struct Tnode *brother;
	char name[50];
	int line; 
	int node_int;
	float node_float;
	char node_value[20];	
}node;

#endif
