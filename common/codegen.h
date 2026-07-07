#ifndef _CODEGEN_H
#define _CODEGEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <symbol.h>


class CodeGen
{
	public:
		struct Quad
		{
			char	opcode[12];
			int		src1, src2, dst;
		};

		SymbolTable s;
		
		
#define MAXQUADS	2000
		Quad QuadArray[MAXQUADS];
		int tempCounter;
		int nextQuad;
		
		/* constructor */
		CodeGen(void)
		{
			nextQuad = 0;
		}

		void removeSymbol(int sn)
		{
			/* Remove the symbol from the symbol table ... */
			s.removeSymbol(sn);

			/* go through instructions in the symbol table and update src1, src2
			 * and dst accordingly
			 */
			for (int l = 0; l < getNextQuad(); l++)
			{
//				if (s.getType(i) == TYPE_PROCEDURE || s.getType(i) == TYPE_LABEL)
				{
					if (QuadArray[l].src1 > sn)
					{
						QuadArray[l].src1--;
					}

					if (QuadArray[l].src2 > sn)
					{
						QuadArray[l].src2--;
					}

					if (QuadArray[l].dst > sn)
					{
						QuadArray[l].dst--;
					}
				}
			}
		}

		/* returns the next available quad */
		int getNextQuad(void)
		{
			return nextQuad;
		}
		
		/* removes a quad from the quad table, also adjusts any procedures
		 * or labels in the symbol table as a result of the remove
		 */
		int removeQuad(int whichQuad)
		{
			/* check that the desired quad to be removed is within the bounds
			 * of the quad table
			 */
			if (whichQuad >= nextQuad)
			{
				return -1;
			}
			
#if 0
			strcpy(QuadArray[whichQuad].opcode, "nop");
			QuadArray[whichQuad].src1 = 0;
			QuadArray[whichQuad].src2 = 0;
			QuadArray[whichQuad].dst = 0;
#else
			for (int i = whichQuad + 1; i < nextQuad; i++)
			{
				QuadArray[i - 1] = QuadArray[i];
			}

			/* go through procedures and labels in the symbol table and
			 * update them accordingly
			 */
			for (int i = 0; i < s.count(); i++)
			{
				if (s.getType(i) == TYPE_PROCEDURE || s.getType(i) == TYPE_LABEL)
				{
					if (atoi(s.getValue(i)) > whichQuad)
					{
						int newloc = atoi(s.getValue(i)) - 1;
						char x[24];
						
						sprintf(x, "%d", newloc);
						s.setValue(i, x);
					}
				}
			}
			
			nextQuad--;
			
			return 0;
#endif
		}
		
		
		/* copy source to dest, making dest lowercase in the process */
		char *strlowcpy(char *dst, const char *src)
		{
			while (*src != '\0')
			{
				*dst = *src;
				if (*dst >= 'A' && *dst <= 'Z')
				{
					*dst += 32;
				}
				src++;
				dst++;
			}
			*dst = *src;
			
			return dst;
		}
	
		
		/* properly disassemble a quad */
		char *disassemble(int quadIndex, char *string, char *fmtstring = "%-20s %-12s %-20s %-20s %-20s")
		{
			char *label, src1n[128], src2n[128], dstn[128];

			s.getScopedName(src1n, QuadArray[quadIndex].src1);
			s.getScopedName(src2n, QuadArray[quadIndex].src2);
//			strcpy(src1n, s.getName(QuadArray[quadIndex].src1));
//			strcpy(src2n, s.getName(QuadArray[quadIndex].src2));


			label = s.getLabel(quadIndex);
			if (label == NULL)
			{
				label = "";
			}
			
			if (strcmp(QuadArray[quadIndex].opcode, "call") == 0)
			{
				strcpy(dstn, s.getName(QuadArray[quadIndex].dst));
			}
			else
			{
//				strcpy(dstn, s.getName(QuadArray[quadIndex].dst));
				s.getScopedName(dstn, QuadArray[quadIndex].dst);
			}
			
			// Class 0 quads: op-code only, uses no operands
			if (strcmp(QuadArray[quadIndex].opcode, "nop") == 0
			|| strcmp(QuadArray[quadIndex].opcode, "ret") == 0
			)
			{
				sprintf(string, fmtstring,
					label,
					QuadArray[quadIndex].opcode,
					"",
					"",
					""
				);
			}
			else
			// Class 1 quads: use only destination (i.e. exit)
			if (strcmp(QuadArray[quadIndex].opcode, "exit") == 0
			|| strcmp(QuadArray[quadIndex].opcode, "neg") == 0
			|| strcmp(QuadArray[quadIndex].opcode, "call") == 0
			)
			{
				sprintf(string, fmtstring,
					label,
					QuadArray[quadIndex].opcode,
					"",
					"",
					dstn
				);
			}
			else
			// Class 2 quads: use source1 and destination (i.e. copy)
			if ((strcmp(QuadArray[quadIndex].opcode, "cp") == 0)
			|| (strcmp(QuadArray[quadIndex].opcode, "not") == 0)
			|| (strcmp(QuadArray[quadIndex].opcode, "neg") == 0))
			{
				sprintf(string, fmtstring,
					label,
					QuadArray[quadIndex].opcode,
					src1n,
					"",
					dstn
				);
			}
			else
			{
				// Class 3 quads: use source1, source2 and destination
				if (strcmp(QuadArray[quadIndex].opcode, "gt") == 0)
				{
					sprintf(string, fmtstring,
						label,
						QuadArray[quadIndex].opcode,
						"",
						"",
						dstn //QuadArray[quadIndex].dst
					);
				}
				else
				if (strncmp(QuadArray[quadIndex].opcode, "gt", 2) == 0)
				{
					sprintf(string, fmtstring,
						label,
						QuadArray[quadIndex].opcode,
						src1n,
						src2n,
						dstn
					);
				}
				else
				{
					sprintf(string, fmtstring,
						label,
						QuadArray[quadIndex].opcode,
						src1n,
						src2n,
						dstn
					);
				}
			}

			return string;
		}
		
		
		/* dump the quad table to a file */
		void dump(FILE *out, int dumpType = 0)
		{
			fprintf(out, "%-20s %-12s %-20s %-20s %-20s\n", "ADDRESS", "OPCODE", "SRC1", "SRC2", "DST");
			fprintf(out, "---------------------------------------------------------------------------------------\n");

			if (dumpType == 1)
			{
				for (int i = 0; i < nextQuad; i++)
				{
					char string[512];
					
					fprintf(out, "%s\n", disassemble(i, string));
				}
			}
			else
			{
				for (int i = 0; i < nextQuad; i++)
				{
					fprintf(out, "%-20d %-12s %-20d %-20d %-20d\n",
						i,
						QuadArray[i].opcode,
						QuadArray[i].src1,
						QuadArray[i].src2,
						QuadArray[i].dst
					);
				}
			}

			fprintf(out, "-EOF\n");
		}


		/* patch a quad with the patch value */
		void backpatch(int quad, int patch)
		{
			QuadArray[quad].dst = patch;
		}
		

		/* generate a quad */
		void gen(char *op)
		{
			// code to generate goto label Quad code goes here
			strlowcpy(QuadArray[nextQuad].opcode, op);
			nextQuad++;
			if (nextQuad == MAXQUADS)
			{
				printf("ERROR! Quad Buffer is full!\n");
				exit(0);
			}
		}


		/* generate a quad */
		void gen(char *op, int location)
		{
			// code to generate goto label Quad code goes here
			strlowcpy(QuadArray[nextQuad].opcode, op);
			QuadArray[nextQuad].dst = location;
			nextQuad++;
			if (nextQuad == MAXQUADS)
			{
				printf("ERROR! Quad Buffer is full!\n");
				exit(0);
			}
		}


		/* generate a quad */
 		void gen(char *op, int A, int B, int C)
		{
			// code to generate Quad code for assignment with binary operators
			//  and conditional branches (gotos) goes here.
			strlowcpy(QuadArray[nextQuad].opcode, op);
			QuadArray[nextQuad].src1 = A;
			QuadArray[nextQuad].src2 = B;
			QuadArray[nextQuad].dst  = C;

			nextQuad++;
			if (nextQuad == MAXQUADS)
			{
				printf("ERROR! Quad Buffer is full!\n");
				exit(0);
			}
		}


		/* read the contents of the file into the quad table */
		void read(FILE *in)
		{
			char *opcode;
			char *t;
			int address, src1, src2, dst;
			int linenum = 0;
			char line[128], *li;
			
			nextQuad = 0;

			/* skip over two lines of header */
			fgets(line, 127, in);
			fgets(line, 127, in);

			while ((li = fgets(line, 127, in)) && strcmp(li, "-EOF\n") != 0)
			{
				linenum++;
				t = strtok(line, " \n");
				address = atoi(t);
				opcode = strtok(NULL, " \n");
				t = strtok(NULL, " \n");
				src1 = atoi(t);
				t = strtok(NULL, " \n");
				src2 = atoi(t);
				t = strtok(NULL, " \n");
				dst = atoi(t);
			
				if (opcode != NULL)
				{
					
					strcpy(QuadArray[nextQuad].opcode, opcode);
					QuadArray[nextQuad].src1 = src1;
					QuadArray[nextQuad].src2 = src2;
					QuadArray[nextQuad].dst = dst;

					nextQuad++;
				}
			}
		}
};

#endif	// _CODEGEN_H