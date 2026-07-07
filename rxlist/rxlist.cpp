/*
 * rxlist - list relax code file in a readable manner
 */

#define	VERMAJOR	0
#define	VERMINOR	1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <codegen.h>

CodeGen g;

int translate(FILE *fp, int *codeSize);


				
int main(int argc, char **argv)
{
 	FILE *fi;
 	
 	if (argc != 2)
 	{
 		fprintf(stderr, "Usage: rxlist <icodefile>\n");
 		exit(1);
 	}
 	
 	fi = fopen(argv[1], "r");

	if (fi != NULL)
	{
		int dataSize;
		
		g.s.read(fi, &dataSize);
		g.read(fi);
		g.s.dump(stdout);
		g.dump(stdout, 1);
		
		fclose(fi);
	}
}
