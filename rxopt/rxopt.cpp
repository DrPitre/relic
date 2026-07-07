/*
 * rxopt
 */

#define	VERMAJOR	0
#define	VERMINOR	1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <codegen.h>

/* prototypes */
void consecutive_ret_collapsing(CodeGen *g);
void strip_constant_negation(CodeGen *g);
int single_reference_copy_symbol_stripping(CodeGen *g);
int unused_symbol_stripping(CodeGen *g);
int constant_collapsing(CodeGen *g);
int symbolIsDestinationOfAnyInstruction(CodeGen *g, int s);
int symbolIsDestinationOfAnyOtherInstruction(CodeGen *g, int s, int i);

CodeGen g;
int verbose = 1;

char verstr[256];

char *help[] =
{
		"rxopt - RELAX code optimizer\n",
 		"Usage: rxopt <inputfile> <outputfile>\n",
 		"Options:\n",
		NULL
};


void showHelp(void)
{
	char **p = help;
	
	while (*p != NULL)
	{
		fprintf(stderr, *(p++));
	}
}


int main(int argc, char **argv)
{
 	char *ic, *of;
 	int ds;
 	FILE *fi, *fo;
 	
 	sprintf(verstr, "v%d.%d", VERMAJOR, VERMINOR);
 	
 	if (argc != 3)
 	{
		showHelp();
 		exit(1);
 	}
 	
 	ic = argv[1];
	of = argv[2];
	
	/* open input RELAX code file */
	fi = fopen(ic, "r");
	
	if (fi == NULL)
	{
		fprintf(stderr, "ERROR: could not open file %s\n", ic);
		exit(1);
	}
	
	/* open output optimized RELAX code file */
	fo = fopen(of, "w+");
	
	if (fo == NULL)
	{
		fclose(fi);
		fprintf(stderr, "ERROR: could not create file %s\n", of);
		exit(1);
	}
	
	/* read symbol table and i-code */
	g.s.read(fi, &ds);
	g.read(fi);
	
	int numSymbols = g.s.count();
	int numInstructions = g.nextQuad;
	int symbolsStripped;
	
	/* perform the optimization processes */
	consecutive_ret_collapsing(&g);
	strip_constant_negation(&g);
	symbolsStripped = single_reference_copy_symbol_stripping(&g);
	if (verbose)
	{
		printf("A total of %d unused symbols have been stripped from single_reference_copy_symbol_stripping()\n", symbolsStripped);
	}
	
	symbolsStripped = unused_symbol_stripping(&g);
	if (verbose)
	{
		printf("A total of %d unused symbols have been stripped from unused_symbol_stripping()\n", symbolsStripped);
	}
	
//	for (int ii = 1; ii <= 1; ii++)
	{
		symbolsStripped = constant_collapsing(&g);
		if (verbose)
		{
			printf("A total of %d duplicate symbols have been collapsed from constant_collapsing()\n", symbolsStripped);
		}
	}
	
	
	int optNumSymbols = g.s.count();
	int optNumInstructions = g.nextQuad;

	/* dump the resulting symbol and code tables */
	g.s.dump(fo);
	g.dump(fo);
	
	/* close the input and output files */
	fclose(fo);
	fclose(fi);

	/* report */
	printf("Pre-opt symbols       : %5d   Post-opt symbols      : %5d\n", numSymbols, optNumSymbols);
	printf("Symbol reduction      : %5.1f%%\n", (1 - ((float)optNumSymbols / (float)numSymbols)) * 100);
	printf("Pre-opt instructions  : %5d   Post-opt instructions : %5d\n", numInstructions, optNumInstructions);
	printf("Instruction reduction : %5.1f%%\n", (1 - ((float)optNumInstructions / (float)numInstructions)) * 100);
	
	return 0;
}


/* this optimization routine strips redundant ret opcodes
 *
 * i.e.
 *
 * ret
 * ret
 *
 * becomes:
 *
 * ret
 */
void consecutive_ret_collapsing(CodeGen *g)
{
	int i;
	
	for (i = 0; i < g->getNextQuad() - 1; i++)
	{
		if (strcmp(g->QuadArray[i].opcode, "ret") == 0
			&& strcmp(g->QuadArray[i + 1].opcode, "ret") == 0)
		{
			g->removeQuad(i--);
		}
	}
}


/* this optimization routine removes programmatic negation of constants
 *
 * i.e.
 *
 * Symbol:  _10 = 3
 *
 * Code:    neg _10
 *
 * If no other access to _10 occurs before the negation, we can remove the
 * neg opcode and negate the constant in the symbol table.
 */
void strip_constant_negation(CodeGen *g)
{
	
}


/* this optimization routine removes one-time use symbols
 */
int single_reference_copy_symbol_stripping(CodeGen *g)
{
	int s, ip, symbolsStripped = 0;
	
	/* 1. For every "cp" in the code...
	 */
	for (int i = 0; i < g->getNextQuad(); i++)
	{
		if (strcmp(g->QuadArray[i].opcode, "cp") == 0)
		{
			int s1 = g->QuadArray[i].src1;
			
			/* We cannot optimize this variable if the source is a global or a
			 * parameter, since we do not know if it will be changed externally
			 * from this program.
			 */
			
			if (g->s.getGlobal(s1) != 0 || g->s.getParam(s1) != 0)
			{
				break;
			}
			
			s = g->QuadArray[i].dst;

			/* ...that the destination as SCOPE >=0 AND GLOBAL = 0 ... */
			if (g->s.getScope(s) >= 0 && g->s.getGlobal(s) == 0)
			{
				int canOptimize = 1;
				
				/* ... if that symbol is the destination in any other instruction ... */
				if (symbolIsDestinationOfAnyOtherInstruction(g, s, i) == 1)
				{
					canOptimize = 0;
					break;
				}
				
				if (canOptimize == 1 && ip == g->getNextQuad())
				{
					/* If here, we can optimize the symbol 's' and the instruction 'i'
					 * out of the code.
					 */
					printf("optimization candidate: symbol %d, instruction %d\n", s, i);

					int newsrc = g->QuadArray[i].src1;
					
					/* 1. Remove the instruction from the code.
					 */
					g->removeQuad(i);
					  
					/* 2. Replace all references in the code of src1/src2 with src1 of the cp
					 */
					for (ip = 0; ip < g->getNextQuad(); ip++)
					{
						if (g->QuadArray[ip].src1 == s)
						{
							g->QuadArray[ip].src1 = newsrc;
						}
						if (g->QuadArray[ip].src2 == s)
						{
							g->QuadArray[ip].src2 = newsrc;
						}
					}
					  
					/* 3. Remove the temporary from the symbol table.
					 */
					g->removeSymbol(s);

					symbolsStripped++;
					
//					i = -1;
				}
			}
		}
	}

	return symbolsStripped;
}



/* This optimization routine removes unused symbols from the symbol table
 *
 * Cases which fall under this optimization catagory are declared variables
 * that are not used in a program.
 */
int unused_symbol_stripping(CodeGen *g)
{
	int s, ip, symbolsStripped = 0;
	
	/* 1. For every symbol in the symbol table...
	 */
	for (int s = 0; s < g->s.count(); s++)
	{
		int used = 0;

		/* If the type is a label or procedure, just skip it */
		if (g->s.getType(s) == TYPE_PROCEDURE || g->s.getType(s) == TYPE_LABEL)
		{
			continue;
		}
		
		/* ... then for every instruction in code ... */
		for (ip = 0; ip < g->getNextQuad(); ip++)
		{
			/* ... if the current symbol is used in src1, src2 or dst ... */
			if (g->QuadArray[ip].src1 == s || g->QuadArray[ip].src2 == s
				|| g->QuadArray[ip].dst == s)
			{
					/* ... then break out of the inner loop and go to the next symbol */
					used = 1;
			}
		}

		if (used == 0)
		{
			/* ... else remove the symbol from the symbol table ... */
			g->removeSymbol(s);

			symbolsStripped++;
		}
	}

	return symbolsStripped;
}


/* This optimization routine collapses immutable initialized symbols of the same value
 */
int constant_collapsing(CodeGen *g)
{
	int s, ip, symbolsStripped = 0;
	
	/* 1. For every symbol in the symbol table...
	 */
	for (int s = 0; s < g->s.count(); s++)
	{
		/* If the symbol is the destination of any instruction, skip it */
		if (symbolIsDestinationOfAnyInstruction(g, s) == 1)
		{
			continue;
		}
				
		/* If the type is a label or procedure, skip it */
		if (g->s.getType(s) == TYPE_PROCEDURE || g->s.getType(s) == TYPE_LABEL)
		{
			continue;
		}
		
		/* If the symbol is uninitialized, a global or a parameter, skip it */
		if (g->s.isInitialized(s) == 0 || g->s.getParam(s) > 0 || g->s.getGlobal(s) > 0)
		{
			continue;
		}
		
		/* Look for a second symbol of the same value */
		for (int s2 = s + 1; s2 < g->s.count(); s2++)
		{
			/* If the type doesn't match the type of the candidate symbol, skip it */
			if (g->s.getType(s2) != g->s.getType(s))
			{
				continue;
			}

			/* If the symbol is the destination of any instruction, skip it */
			if (symbolIsDestinationOfAnyInstruction(g, s2) == 1)
			{
				continue;
			}
				
			/* If the symbol is uninitialized, a global or a parameter, skip it */
			if (g->s.isInitialized(s2) == 0 || g->s.getParam(s2) > 0 || g->s.getGlobal(s2) > 0)
			{
				continue;
			}

			/* Skip if the values are different */
			{
				char *v1 = g->s.getValue(s);
				char *v2 = g->s.getValue(s2);
				
				if (!v1 || !v2)
				{
					continue;
				}
				
				if (strcmp(v1, v2) == 0)
				{
#ifdef DEBUG
					printf("Candidate symbols %d (%s) and %d (%s) are the same\n", s, g->s.getName(s), s2, g->s.getName(s2));
#endif

					// Change all references in instructions from s2 to s
					for (int ip = 0; ip < g->nextQuad; ip++)
					{
						if (g->QuadArray[ip].src1 == s2)
						{
							g->QuadArray[ip].src1 = s;
						}
						if (g->QuadArray[ip].src2 == s2)
						{
							g->QuadArray[ip].src2 = s;
						}
						if (g->QuadArray[ip].dst == s2)
						{
							g->QuadArray[ip].dst = s;
						}
					}
					

					// Remove the symbol s2
					g->removeSymbol(s2);

					// Decrement s2 in to catch adjacent symbols
					s2--;
					
					symbolsStripped++;
				}
			}
		}
	}

	return symbolsStripped;
}


int symbolIsDestinationOfAnyInstruction(CodeGen *g, int s)
{
	for (int ip = 0; ip < g->nextQuad; ip++)
	{
		if (g->QuadArray[ip].dst == s)
		{
			return 1;
		}
	}

	return 0;
}


int symbolIsDestinationOfAnyOtherInstruction(CodeGen *g, int s, int i)
{
	for (int ip = 0; ip < g->nextQuad; ip++)
	{
		if (i != ip && g->QuadArray[ip].dst == s)
		{
			return 1;
		}
	}

	return 0;
}
