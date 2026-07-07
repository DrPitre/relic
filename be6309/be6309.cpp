/*
 * be6309
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
int inAddr = 0;

char verstr[256];

void createInitializedDataSegment(FILE *fp);
void createUninitializedDataSegment(FILE *fp);
void translate(FILE *fp, int *codeSize);
int library(char *filename, FILE *fp, int *codeSize);
void gen(FILE *fp, char *address, char *opcode, char *operand, char *comment);
void gen(FILE *fp, char *address, char *opcode, int operand, char *comment);
char *addrstr(int address, char *labelstr, char preface);
int myatoi(char *num);


				
void gen(FILE *fp, char *address, char *opcode, char *operand, char *comment)
{
	if (comment == NULL || comment == "")
	{
		fprintf(fp, "%-12s  %-7s %s\n", address, opcode, operand);
	}
	else
	{
		fprintf(fp, "%-12s  %-7s %s  * %s\n", address, opcode, operand, comment);	
	}
}


void gen(FILE *fp, char *address, char *opcode, int operand, char *comment)
{
	char b[24];
	
	sprintf(b, "%d", operand);

	gen(fp, address, opcode, b, comment);
}


char *addrstr(int address, char *labelstr, char preface)
{
	sprintf(labelstr, "%c%04X", preface, address);

	return labelstr;
}


void call(FILE *fp, char *labelstr, char *opcode, int src1, int src2, int dst, char *comment)
{
	gen(fp, labelstr, "lbsr", g.s.getName(dst), comment);
}


void ret(FILE *fp, char *labelstr, char *opcode, int src1, int src2, int dst, char *comment)
{
	gen(fp, labelstr, "rts", "", "");
}


void _exit(FILE *fp, char *labelstr, char *opcode, int src1, int src2, int dst, char *comment)
{
	char d[128];
	
	sprintf(d, "#%s", g.s.getValue(dst));
	gen(fp, labelstr, "ldb", d, "");
	gen(fp, "", "os9", "F$Exit", "");
}


void relation(FILE *fp, int address, char *labelstr, char *opcode, int src1, int src2, int dst, char *comment)
{
	char *op;
	char s1[124];
	char s2[124];
	char d[124];
	
	g.s.getScopedName(s1, src1); strcat(s1, ",u");
	g.s.getScopedName(s2, src2); strcat(s2, ",u");
	g.s.getScopedName(d, dst); strcat(d, ",u");

	if (g.s.getType(dst) != TYPE_BOOLEAN)
	{
		fprintf(stderr, "Address %d: relation result can only be assigned to a BOOLEAN\n", inAddr);
		exit(1);
	}
	
	if (strcmp(opcode, ">") == 0)
	{
		op = "bgt";
	} else if (strcmp(opcode, "<") == 0)
	{
		op = "blt";
	} else if (strcmp(opcode, ">=") == 0)
	{
		op = "bge";
	} else if (strcmp(opcode, "<=") == 0)
	{
		op = "ble";
	} else if (strcmp(opcode, "=") == 0)
	{
		op = "beq";
	} else
	{
		op = "bne";
	}

	switch (g.s.getType(src1))
	{
		case TYPE_BOOLEAN:
			if (g.s.getType(src2) != TYPE_BOOLEAN)
			{
				fprintf(stderr, "Address %d: BOOLEAN can only be compared with BOOLEAN\n", inAddr);
				exit(1);
			}
			gen(fp, labelstr, "ldb", s1, comment);
			gen(fp, "", "cmpb", s2, "");
			break;
		
		case TYPE_BYTE:
			gen(fp, labelstr, "ldb", s1, comment);
			switch (g.s.getType(src2))
			{
				case TYPE_BYTE:
					gen(fp, "", "cmpb", s2, "");
					break;

				case TYPE_INTEGER:
					gen(fp, "", "clra", "", "");
					gen(fp, "", "cmpd", s2, "");
					break;
					
				case TYPE_REAL:
					gen(fp, "", "clra", "", "");
					gen(fp, "", "leax", s2, "");
                    gen(fp, "", "lbsr", "cmp_D_to_real_at_X", "");
//					printf("TODO: BYTE relop REAL\n");
					break;
			}
			break;
		
		case TYPE_INTEGER:
			gen(fp, labelstr, "ldd", s1, comment);
			switch (g.s.getType(src2))
			{
				case TYPE_BYTE:
					gen(fp, "", "tfr", "d,x", "");
					gen(fp, "", "ldb", s2, "");
					gen(fp, "", "clra", "", "");
					gen(fp, "", "pshs", "d", "");
					gen(fp, "", "cmpd", ",s++", "");
					break;

				case TYPE_INTEGER:
					gen(fp, "", "cmpd", s2, "");
					break;
					
				case TYPE_REAL:
					gen(fp, "", "clra", "", "");
					gen(fp, "", "leax", s2, "");
                    gen(fp, "", "lbsr", "cmp_D_to_real_at_X", "");
//					printf("TODO: INTEGER relop REAL\n");
					break;
			}
			break;
		
		case TYPE_REAL:
			switch (g.s.getType(src2))
			{
				case TYPE_BYTE:
					gen(fp, labelstr, "ldb", s2, comment);
					gen(fp, "", "clra", "", "");
					gen(fp, "", "leax", s1, "");
					gen(fp, "", "lbsr", "cmp_real_at_X_to_D", "");
					break;

				case TYPE_INTEGER:
					gen(fp, labelstr, "ldd", s2, comment);
					gen(fp, "", "leax", s1, "");
					gen(fp, "", "lbsr", "cmp_real_at_X_to_D", "");
					break;
					
				case TYPE_REAL:
					gen(fp, labelstr, "leay", s2, comment);
					gen(fp, "", "leax", s1, "");
					gen(fp, "", "lbsr", "cmp_real_at_X_to_real_at_Y", "");
					break;
			}
			break;
	}

	char truelabel[24], falselabel[24];
	
	gen(fp, "", op, addrstr(address++, labelstr, 'L'), "");
	strcpy(truelabel, labelstr);
	gen(fp, "", "clrb", "", "");
	strcpy(falselabel, addrstr(address++, labelstr, 'L'));
	gen(fp, "", "bra", falselabel, "");
	gen(fp, truelabel, "ldb", "#$01", "");
	gen(fp, falselabel, "stb", d, "");
}


void gtconditional(FILE *fp, char *labelstr, char *opcode, int src1, int src2, int dst, char *comment)
{
	char s1[124];
	char s2[124];
	char d[124];

	g.s.getScopedName(s1, src1); strcat(s1, ",u");
	g.s.getScopedName(s2, src2); strcat(s2, ",u");
	g.s.getScopedName(d, dst);

	switch (g.s.getType(src1))
	{
		case TYPE_BOOLEAN:
			if (g.s.getType(src2) != TYPE_BOOLEAN)
			{
				fprintf(stderr, "Address %d: BOOLEAN can only be compared with BOOLEAN\n", inAddr);
				exit(1);
			}
			gen(fp, labelstr, "ldb", s1, comment);
			gen(fp, "", "cmpb", s2, "");
			break;

		case TYPE_BYTE:
			switch (g.s.getType(src2))
			{
				case TYPE_BOOLEAN: // BOOLEAN != BYTE
					fprintf(stderr, "Address %d: BOOLEAN can only be compared with BOOLEAN\n", inAddr);
					exit(1);
					
				case TYPE_BYTE:	// BYTE != BYTE
					gen(fp, labelstr, "ldb", s1, comment);
					gen(fp, "", "cmpb", s2, "");
					break;
				
				case TYPE_INTEGER:	// BYTE != INTEGER
					gen(fp, labelstr, "ldb", s1, comment);
					gen(fp, "", "sex", "", "");
					gen(fp, "", "cmpd", s2, "");
					break;
					
				case TYPE_REAL:	// BYTE != REAL
					printf("TODO: BYTE != REAL\n");
					break;
			}
			break;
			
		case TYPE_INTEGER:
			switch (g.s.getType(src2))
			{
				case TYPE_BOOLEAN: // BOOLEAN != BYTE
					fprintf(stderr, "Address %d: BOOLEAN can only be compared with BOOLEAN\n", inAddr);
					exit(1);
					
				case TYPE_BYTE:	// INTEGER != BYTE
					gen(fp, labelstr, "ldb", s2, comment);
					gen(fp, "", "sex", "", "");
					gen(fp, "", "cmpd", s1, "");
					break;
				
				case TYPE_INTEGER:	// INTEGER != INTEGER
					gen(fp, labelstr, "ldd", s1, comment);
					gen(fp, "", "cmpd", s2, "");
					break;
					
				case TYPE_REAL:	// INTEGER != REAL
					printf("TODO: INTEGER != REAL\n");
					break;
			}
			break;
			
		case TYPE_REAL:
			switch (g.s.getType(src2))
			{
				case TYPE_BOOLEAN: // BOOLEAN != BYTE
					fprintf(stderr, "Address %d: BOOLEAN can only be compared with BOOLEAN\n", inAddr);
					exit(1);
					
				case TYPE_BYTE:	// REAL != BYTE
					printf("TODO: REAL != BYTE\n");
					break;
				
				case TYPE_INTEGER:	// REAL != INTEGER
					printf("TODO: REAL != INTEGER\n");
					break;
					
				case TYPE_REAL:	// REAL != REAL
					printf("TODO: REAL != REAL\n");
					break;
			}
			break;

		default:
			printf("errant case!\n");
			exit(1);
	}

	if (strcmp(opcode, "gt>=") == 0)
	{
		gen(fp, "", "lbge", d, "");
	}
	else
	if (strcmp(opcode, "gt<=") == 0)
	{
		gen(fp, "", "lble", d, "");
	}
	else
	if (strcmp(opcode, "gt>") == 0)
	{
		gen(fp, "", "lbgt", d, "");
	}
	else
	if (strcmp(opcode, "gt<") == 0)
	{
		gen(fp, "", "lblt", d, "");
	}
	else
	if (strcmp(opcode, "gt!=") == 0)
	{
		gen(fp, "", "lbne", d, "");
	}
	else
	{
		gen(fp, "", "lbeq", d, "");
	}
}


/* Legal copies:

	Boolean is an 8-bit value (0 = FALSE, 1 = TRUE)
	BOOLEAN = BOOLEAN

	Byte is an Unsigned 8-bit value (0-255)
	BYTE = BYTE
	BYTE = INTEGER % 256
	BYTE = REAL % 256
	
	Integer is a Signed 16-bit value (-32768 to 32767)
	INTEGER = BYTE
	INTEGER = INTEGER
	INTEGER = REAL % 65536
	
	REAL = BYTE
	REAL = INTEGER
	REAL = REAL
	
	STRING = STRING
	
 */
void cp(FILE *fp, char *labelstr, char *opcode, int src1, int src2, int dst, char *comment)
{
	char s1[124];
	char d[124];
	

	switch (g.s.getType(dst))
	{
		case TYPE_BOOLEAN:
			g.s.getScopedName(d, dst); strcat(d, ",u");
		
			switch (g.s.getType(src1))
			{
				case TYPE_BOOLEAN:
					g.s.getScopedName(s1, src1); strcat(s1, ",u");
					gen(fp, labelstr, "lda", s1, comment);
					gen(fp, "", "sta", d, "");
					break;

				default:
					printf("Address %d: BOOLEAN can only be assigned to BOOLEAN\n", inAddr);
					break;
			}
			break;
			
		case TYPE_BYTE:
			g.s.getScopedName(d, dst); strcat(d, ",u");

			switch (g.s.getType(src1))
			{
				case TYPE_BYTE:
					g.s.getScopedName(s1, src1); strcat(s1, ",u");
					gen(fp, labelstr, "lda", s1, comment);
					gen(fp, "", "sta", d, "");
					break;
					
				case TYPE_INTEGER:
					g.s.getScopedName(s1, src1); strcat(s1, "+1,u");
					gen(fp, labelstr, "lda", s1, comment);
					gen(fp, "", "sta", d, "");
					break;

				case TYPE_REAL:
					printf("TODO: BYTE = REAL\n");
					break;
			}
			break;
			
		case TYPE_INTEGER:
			g.s.getScopedName(d, dst); strcat(d, ",u");

			switch (g.s.getType(src1))
			{
				case TYPE_BYTE:
					g.s.getScopedName(s1, src1); strcat(s1, ",u");
					gen(fp, labelstr, "ldb", s1, comment);
					gen(fp, "", "sex", "", "");
					gen(fp, "", "std", d, "");
					break;
					
				case TYPE_INTEGER:
					g.s.getScopedName(s1, src1); strcat(s1, ",u");
					gen(fp, labelstr, "ldd", s1, comment);
					gen(fp, "", "std", d, "");
					break;
				
				case TYPE_REAL:
					printf("TODO: INTEGER = REAL\n");
					break;
			}
			break;
			
		case TYPE_REAL:
			switch (g.s.getType(src1))
			{
				case TYPE_BYTE:
					printf("TODO: REAL = BYTE\n");
					break;
					
				case TYPE_INTEGER:
					printf("TODO: REAL = INTEGER\n");
					break;
				
				case TYPE_REAL:
					g.s.getScopedName(s1, src1); strcat(s1, ",u");
					g.s.getScopedName(d, dst); strcat(d, ",u");
					gen(fp, labelstr, "ldd", s1, comment);
					gen(fp, "", "std", d, "");

					g.s.getScopedName(s1, src1); strcat(s1, "+2,u");
					g.s.getScopedName(d, dst); strcat(d, "+2,u");

					gen(fp, "", "ldd", s1, "");
					gen(fp, "", "std", d, "");

					g.s.getScopedName(s1, src1); strcat(s1, "+3,u");
					g.s.getScopedName(d, dst); strcat(d, "+3,u");

					gen(fp, "", "ldb", s1, "");
					gen(fp, "", "stb", d, "");
					break;
			}
			break;
			
		case TYPE_STRING:
			switch (g.s.getType(src1))
			{
				case TYPE_STRING:
					sprintf(d, "#%d", g.s.getSize(dst));
					gen(fp, labelstr, "ldd", d, comment);
					g.s.getScopedName(d, dst); strcat(d, ",u");
					gen(fp, "", "leax", d, "");
					gen(fp, "", "pshs", "x,d", "");

					sprintf(s1, "#%d", g.s.getSize(src1));
					gen(fp, "", "ldd", s1, "");
					g.s.getScopedName(s1, src1); strcat(s1, ",u");
					gen(fp, "", "leax", s1, "");
					gen(fp, "", "pshs", "x,d", "");
					gen(fp, "", "lbsr", "copystring", "copystring(srcsize, srcaddr, dstsize, dstaddr)");
					gen(fp, "", "leas", "8,s", "");
					break;

				default:
					printf("Address %d: Can only add STRING to STRING\n", inAddr);
					break;
			}
			break;
			
#if 0
		case TYPE_USERTYPE:
			switch (g.s.getType(src1))
			{
			}
			break;
#endif
	}

}


/* Legal additions:

	Booleans cannot be added

	Bytes, Integers and Reals can be added together
	BYTE + BYTE = 0-255
	INTEGER + INTEGER = -32768 to 32767
	REAL + REAL = REAL range
	BYTE + INTEGER = -32768 to 32767
	BYTE + REAL = REAL range
	INTEGER + REAL = REAL range
*/
void add(FILE *fp, char *labelstr, char *opcode, int src1, int src2, int dst, char *comment)
{
	char s1[128];
	char s2[128];
	char d[128];
	int src1Type, src2Type, dstType;
	
	g.s.getScopedName(s1, src1); strcat(s1, ",u");
	g.s.getScopedName(s2, src2); strcat(s2, ",u");
	g.s.getScopedName(d, dst); strcat(d, ",u");

	src1Type = g.s.getType(src1);
	src2Type = g.s.getType(src2);
	dstType = g.s.getType(dst);
	
	switch (src1Type)
	{
		case TYPE_BYTE:
			switch (src2Type)
			{
				case  TYPE_BYTE:	// BYTE + BYTE
					gen(fp, labelstr, "ldb", s1, comment);
					gen(fp, "", "addb", s2, "");

					switch (dstType)
					{
						case TYPE_BYTE: // BYTE = BYTE + BYTE
							gen(fp, "", "stb", d, "");
							break;

						case TYPE_INTEGER: // INTEGER = BYTE + BYTE
							gen(fp, "", "sex", "", "");
							gen(fp, "", "std", d, "");
							break;

						case TYPE_REAL:  // REAL = BYTE + BYTE
							gen(fp, "", "sex", "", "");
							gen(fp, "", "leax", d, "");
							gen(fp, "", "lbsr", "assign_D_to_real_at_X", "");
							break;
					}
					break;

				case  TYPE_INTEGER:	// BYTE + INTEGER
					gen(fp, labelstr, "ldb", s1, comment);
					gen(fp, "", "sex", "", "");
					gen(fp, "", "addd", s2, "");

					switch (dstType)
					{
						case TYPE_BYTE:  // BYTE = BYTE + INTEGER
							gen(fp, "", "stb", d, "");
							break;

						case TYPE_INTEGER:  // INTEGER = BYTE + INTEGER
							gen(fp, "", "std", d, "");
							break;

						case TYPE_REAL:  // REAL = BYTE + INTEGER
							gen(fp, "", "leax", d, "");
							gen(fp, "", "lbsr", "assign_D_to_real_at_X", "");
							break;
					}
					break;

				case  TYPE_REAL: // BYTE + REAL
					gen(fp, labelstr, "ldb", s1, comment);
					gen(fp, "", "sex", "", "");
					gen(fp, "", "leax", s2, "");
					gen(fp, "", "leas", "-5,s", "");
					gen(fp, "", "lbsr", "add_D_to_real_at_X_to_2s", "");

					switch (dstType)
					{
						case TYPE_BYTE:	// BYTE = BYTE + REAL
							gen(fp, "", "tfr", "s,x", "");
							gen(fp, "", "lbsr", "assign_real_at_X_to_D", "");
							gen(fp, "", "stb", d, "");
							gen(fp, "", "leas", "5,s", "");
							break;

						case TYPE_INTEGER: // INTEGER = BYTE + REAL
							gen(fp, "", "tfr", "s,x", "");
							gen(fp, "", "lbsr", "assign_real_at_X_to_D", "");
							gen(fp, "", "std", d, "");
							gen(fp, "", "leas", "5,s", "");
							break;

						case TYPE_REAL: // REAL = BYTE + REAL
							gen(fp, "", "puls", "d", "");
							gen(fp, "", "std", d, "");
							gen(fp, "", "puls", "d", "");
							g.s.getScopedName(d, dst); strcat(d, "+2,u");
							gen(fp, "", "std", d, "");
							gen(fp, "", "puls", "a", "");
							g.s.getScopedName(d, dst); strcat(d, "+4,u");
							gen(fp, "", "sta", d, "");
							break;
					}
					break;
			}
			break;
					
		case TYPE_INTEGER:
			switch (src2Type)
			{
				case  TYPE_BYTE:	// INTEGER + BYTE
					gen(fp, labelstr, "ldd", s1, comment);
					gen(fp, "", "addb", s2, "");
					gen(fp, "", "adca", "#0", "");

					switch (dstType)
					{
						case TYPE_BYTE:	// BYTE = INTEGER + BYTE
							gen(fp, "", "stb", d, "");
							break;

						case TYPE_INTEGER: // INTEGER = INTEGER + BYTE
							gen(fp, "", "std", d, "");
							break;

						case TYPE_REAL:	// REAL = INTEGER + BYTE
							gen(fp, "", "leax", d, "");
							gen(fp, "", "lbsr", "assign_D_to_real_at_X", "");
							break;
					}
					break;

				case  TYPE_INTEGER:	// INTEGER + INTEGER
					gen(fp, labelstr, "ldd", s1, comment);
					gen(fp, "", "addd", s2, "");

					switch (dstType)
					{
						case TYPE_BYTE:	// BYTE = INTEGER + INTEGER
							gen(fp, "", "stb", d, "");
							break;

						case TYPE_INTEGER:	// INTEGER = INTEGER + INTEGER
							gen(fp, "", "std", d, "");
							break;

						case TYPE_REAL:	// REAL = INTEGER + INTEGER
							gen(fp, "", "leax", d, "");
							gen(fp, "", "lbsr", "assign_D_to_real_at_X", "");
							break;
					}
					break;

				case  TYPE_REAL:	// INTEGER + REAL
					gen(fp, labelstr, "ldd", s1, comment);
					gen(fp, "", "leax", s2, "");
					gen(fp, "", "leas", "-5,s", "");
					gen(fp, "", "lbsr", "add_D_to_real_at_X_to_2s", "");


					switch (dstType)
					{
						case TYPE_BYTE:	// BYTE = INTEGER + REAL
							gen(fp, "", "tfr", "s,x", "");
							gen(fp, "", "lbsr", "assign_real_at_X_to_D", "");
							gen(fp, "", "std", d, "");
							gen(fp, "", "leas", "5,s", "");
							break;

						case TYPE_INTEGER: // INTEGER = INTEGER + REAL
							gen(fp, "", "tfr", "s,x", "");
							gen(fp, "", "lbsr", "assign_real_at_X_to_D", "");
							gen(fp, "", "std", d, "");
							gen(fp, "", "leas", "5,s", "");
							break;

						case TYPE_REAL: // REAL = INTEGER + REAL
							gen(fp, "", "puls", "d", "");
							gen(fp, "", "std", d, "");
							gen(fp, "", "puls", "d", "");
							g.s.getScopedName(d, dst); strcat(d, "+2,u");
							gen(fp, "", "std", d, "");
							gen(fp, "", "puls", "a", "");
							g.s.getScopedName(d, dst); strcat(d, "+4,u");
							gen(fp, "", "sta", d, "");
							break;
					}
					break;
			}
			break;
					
		case TYPE_REAL:
			switch (src2Type)
			{
				case  TYPE_BYTE:	// REAL + BYTE
					gen(fp, labelstr, "ldb", s2, comment);
					gen(fp, "", "sex", "", "");
					gen(fp, "", "leax", s1, "");
					gen(fp, "", "leas", "-5,s", "");
					gen(fp, "", "lbsr", "add_D_to_real_at_X_to_2s", "");

					switch (dstType)
					{
						case TYPE_BYTE:	// BYTE = BYTE + REAL
							gen(fp, "", "tfr", "s,x", "");
							gen(fp, "", "lbsr", "assign_real_at_X_to_D", "");
							gen(fp, "", "stb", d, "");
							gen(fp, "", "leas", "5,s", "");
							break;

						case TYPE_INTEGER: // INTEGER = BYTE + REAL
							gen(fp, "", "tfr", "s,x", "");
							gen(fp, "", "lbsr", "assign_real_at_X_to_D", "");
							gen(fp, "", "std", d, "");
							gen(fp, "", "leas", "5,s", "");
							break;

						case TYPE_REAL: // REAL = BYTE + REAL
							gen(fp, "", "puls", "d", "");
							gen(fp, "", "std", d, "");
							gen(fp, "", "puls", "d", "");
							g.s.getScopedName(d, dst); strcat(d, "+2,u");
							gen(fp, "", "std", d, "");
							gen(fp, "", "puls", "a", "");
							g.s.getScopedName(d, dst); strcat(d, "+4,u");
							gen(fp, "", "sta", d, "");
							break;
					}
					break;

				case  TYPE_INTEGER:	// REAL + INTEGER
					gen(fp, labelstr, "ldd", s2, comment);
					gen(fp, "", "leax", s1, "");
					gen(fp, "", "leas", "-5,s", "");
					gen(fp, "", "lbsr", "add_D_to_real_at_X_to_2s", "");


					switch (dstType)
					{
						case TYPE_BYTE:	// BYTE = INTEGER + REAL
							gen(fp, "", "tfr", "s,x", "");
							gen(fp, "", "lbsr", "assign_real_at_X_to_D", "");
							gen(fp, "", "std", d, "");
							gen(fp, "", "leas", "5,s", "");
							break;

						case TYPE_INTEGER: // INTEGER = INTEGER + REAL
							gen(fp, "", "tfr", "s,x", "");
							gen(fp, "", "lbsr", "assign_real_at_X_to_D", "");
							gen(fp, "", "std", d, "");
							gen(fp, "", "leas", "5,s", "");
							break;

						case TYPE_REAL: // REAL = INTEGER + REAL
							gen(fp, "", "puls", "d", "");
							gen(fp, "", "std", d, "");
							gen(fp, "", "puls", "d", "");
							g.s.getScopedName(d, dst); strcat(d, "+2,u");
							gen(fp, "", "std", d, "");
							gen(fp, "", "puls", "a", "");
							g.s.getScopedName(d, dst); strcat(d, "+4,u");
							gen(fp, "", "sta", d, "");
							break;
					}
					break;

				case  TYPE_REAL: // REAL + REAL
					gen(fp, "", "leax", s1, "");
					gen(fp, "", "leay", s2, "");
					gen(fp, "", "leas", "-5,s", "");
					gen(fp, "", "lbsr", "add_real_at_X_to_real_at_Y_to_2s", "");

					switch (dstType)
					{
						case TYPE_BYTE:	// BYTE = REAL + REAL
							gen(fp, "", "tfr", "s,x", "");
							gen(fp, "", "lbsr", "assign_real_at_X_to_D", "");
							gen(fp, "", "stb", d, "");
							gen(fp, "", "leas", "5,s", "");
							break;

						case TYPE_INTEGER: // INTEGER = REAL + REAL
							gen(fp, "", "tfr", "s,x", "");
							gen(fp, "", "lbsr", "assign_real_at_X_to_D", "");
							gen(fp, "", "std", d, "");
							gen(fp, "", "leas", "5,s", "");
							break;

						case TYPE_REAL: // REAL = REAL + REAL
							gen(fp, "", "puls", "d", "");
							gen(fp, "", "std", d, "");
							gen(fp, "", "puls", "d", "");
							g.s.getScopedName(d, dst); strcat(d, "+2,u");
							gen(fp, "", "std", d, "");
							gen(fp, "", "puls", "a", "");
							g.s.getScopedName(d, dst); strcat(d, "+4,u");
							gen(fp, "", "sta", d, "");
							break;
					}
					break;
			}
			break;
			
		case TYPE_STRING:
			switch (src2Type)
			{
				case  TYPE_STRING:	// STRING + STRING
					g.s.getScopedName(d, dst); strcat(d, ",u");
					gen(fp, labelstr, "leax", d, "");
					gen(fp, "", "pshs", "x", "");

					g.s.getScopedName(s2, src2); strcat(s2, ",u");
					gen(fp, "", "leax", s2, "");
					gen(fp, "", "pshs", "x", "");

					g.s.getScopedName(s1, src1); strcat(s1, ",u");
					gen(fp, "", "leax", s1, "");
					gen(fp, "", "pshs", "x", "");

					gen(fp, "", "lbsr", "addstring", "addstring(src1addr, src2addr, dstaddr)");
					gen(fp, "", "leas", "6,s", "");
					break;
				
				default:
					printf("Address %d: Can only assign STRING to STRING\n", inAddr);
					break;
			}
			break;
	}
}


/* Legal subtractions:

	Booleans cannot be subtracted

	Bytes, Integers and Reals can be subtracted from each other
	BYTE - BYTE = 0-255
	INTEGER - INTEGER = -32768 to 32767
	REAL - REAL = REAL range
	BYTE - INTEGER = -32768 to 32767
	BYTE - REAL = REAL range
	INTEGER - REAL = REAL range
*/
void subtract(FILE *fp, char *labelstr, char *opcode, int src1, int src2, int dst, char *comment)
{
	char s1[128];
	char s2[128];
	char d[128];
	int src1Type, src2Type, dstType;
	
	g.s.getScopedName(s1, src1); strcat(s1, ",u");
	g.s.getScopedName(s2, src2); strcat(s2, ",u");
	g.s.getScopedName(d, dst); strcat(d, ",u");

	src1Type = g.s.getType(src1);
	src2Type = g.s.getType(src2);
	dstType = g.s.getType(dst);
	
	switch (src1Type)
	{
		case TYPE_BYTE:
			switch (src2Type)
			{
				case  TYPE_BYTE:	// BYTE - BYTE
					gen(fp, labelstr, "ldb", s1, comment);
					gen(fp, "", "subb", s2, "");

					switch (dstType)
					{
						case TYPE_BYTE:	// BYTE = BYTE - BYTE
							gen(fp, "", "stb", d, "");
							break;

						case TYPE_INTEGER:	// INTEGER = BYTE - BYTE
							gen(fp, "", "sex", "", "");
							gen(fp, "", "std", d, "");
							break;

						case TYPE_REAL:	// REAL = BYTE - BYTE
							gen(fp, "", "sex", "", "");
							gen(fp, "", "leax", d, "");
							gen(fp, "", "lbsr", "assign_D_to_real_at_X", "");
							break;
					}
					break;

				case  TYPE_INTEGER:	// BYTE - INTEGER
					gen(fp, labelstr, "ldb", s1, comment);
					gen(fp, "", "sex", "", "");
					gen(fp, "", "subd", s2, "");

					switch (dstType)
					{
						case TYPE_BYTE:	// BYTE = BYTE - INTEGER
							gen(fp, "", "stb", d, "");
							break;

						case TYPE_INTEGER:	// INTEGER = BYTE - INTEGER
							gen(fp, "", "std", d, "");
							break;

						case TYPE_REAL:		// REAL = BYTE - INTEGER
							gen(fp, "", "leax", d, "");
							gen(fp, "", "lbsr", "assign_D_to_real_at_X", "");
							break;
					}
					break;

				case  TYPE_REAL:	// BYTE - REAL
					gen(fp, labelstr, "ldb", s1, comment);
					gen(fp, "", "sex", "", "");
					gen(fp, "", "leax", s2, "");
					gen(fp, "", "leas", "-5,s", "");
					gen(fp, "", "lbsr", "subtract_real_at_X_from_D_to_2s", "");

					switch (dstType)
					{
						case TYPE_BYTE:	// BYTE = BYTE - REAL
							gen(fp, "", "tfr", "s,x", "");
							gen(fp, "", "lbsr", "assign_real_at_X_to_D", "");
							gen(fp, "", "stb", d, "");
							gen(fp, "", "leas", "5,s", "");
							break;

						case TYPE_INTEGER: // INTEGER = BYTE + REAL
							gen(fp, "", "tfr", "s,x", "");
							gen(fp, "", "lbsr", "assign_real_at_X_to_D", "");
							gen(fp, "", "std", d, "");
							gen(fp, "", "leas", "5,s", "");
							break;

						case TYPE_REAL: // REAL = BYTE + REAL
							gen(fp, "", "puls", "d", "");
							gen(fp, "", "std", d, "");
							gen(fp, "", "puls", "d", "");
							g.s.getScopedName(d, dst); strcat(d, "+2,u");
							gen(fp, "", "std", d, "");
							gen(fp, "", "puls", "a", "");
							g.s.getScopedName(d, dst); strcat(d, "+4,u");
							gen(fp, "", "sta", d, "");
							break;
					}
					break;
			}
			break;
					
		case TYPE_INTEGER:
			switch (src2Type)
			{
				case  TYPE_BYTE:	// INTEGER - BYTE
					gen(fp, labelstr, "ldb", s2, comment);
					gen(fp, "", "sex", "", "");
					gen(fp, "", "pshs", "d", "");
					gen(fp, "", "ldd", s1, "");
					gen(fp, "", "subd", ",s++", "");

					switch (dstType)
					{
						case TYPE_BYTE:	// BYTE = INTEGER - BYTE
							gen(fp, "", "stb", d, "");
							break;

						case TYPE_INTEGER:	// INTEGER = INTEGER - BYTE
							gen(fp, "", "std", d, "");
							break;

						case TYPE_REAL: // REAL = INTEGER - BYTE
							gen(fp, "", "leax", d, "");
							gen(fp, "", "lbsr", "assign_D_to_real_at_X", "");
							break;
					}
					break;

				case  TYPE_INTEGER:	// INTEGER - INTEGER
					gen(fp, labelstr, "ldd", s1, comment);
					gen(fp, "", "subd", s2, "");

					switch (dstType)
					{
						case TYPE_BYTE:	// BYTE = INTEGER - INTEGER
							gen(fp, "", "stb", d, "");
							break;

						case TYPE_INTEGER:	// INTEGER = INTEGER - INTEGER
							gen(fp, "", "std", d, "");
							break;

						case TYPE_REAL:	// REAL = INTEGER - INTEGER
							gen(fp, "", "leax", d, "");
							gen(fp, "", "lbsr", "assign_D_to_real_at_X", "");
							break;
					}
					break;

				case  TYPE_REAL:	// INTEGER - REAL
					gen(fp, labelstr, "ldd", s1, comment);
					gen(fp, "", "leax", s2, "");
					gen(fp, "", "leas", "-5,s", "");
					gen(fp, "", "lbsr", "subtract_real_at_X_from_D_to_2s", "");


					switch (dstType)
					{
						case TYPE_BYTE:	// BYTE = INTEGER - REAL
							gen(fp, "", "tfr", "s,x", "");
							gen(fp, "", "lbsr", "assign_real_at_X_to_D", "");
							gen(fp, "", "std", d, "");
							gen(fp, "", "leas", "5,s", "");
							break;

						case TYPE_INTEGER: // INTEGER = INTEGER - REAL
							gen(fp, "", "tfr", "s,x", "");
							gen(fp, "", "lbsr", "assign_real_at_X_to_D", "");
							gen(fp, "", "std", d, "");
							gen(fp, "", "leas", "5,s", "");
							break;

						case TYPE_REAL: // REAL = INTEGER - REAL
							gen(fp, "", "puls", "d", "");
							gen(fp, "", "std", d, "");
							gen(fp, "", "puls", "d", "");
							g.s.getScopedName(d, dst); strcat(d, "+2,u");
							gen(fp, "", "std", d, "");
							gen(fp, "", "puls", "a", "");
							g.s.getScopedName(d, dst); strcat(d, "+4,u");
							gen(fp, "", "sta", d, "");
							break;
					}
					break;
			}
			break;
					
		case TYPE_REAL:
			switch (src2Type)
			{
				case  TYPE_BYTE:	// REAL - BYTE
					gen(fp, labelstr, "ldb", s2, comment);
					gen(fp, "", "sex", "", "");
					gen(fp, "", "leax", s1, "");
					gen(fp, "", "leas", "-5,s", "");
					gen(fp, "", "lbsr", "subtract_D_from_real_at_X_to_2s", "");

					switch (dstType)
					{
						case TYPE_BYTE:	// BYTE = BYTE - REAL
							gen(fp, "", "tfr", "s,x", "");
							gen(fp, "", "lbsr", "assign_real_at_X_to_D", "");
							gen(fp, "", "stb", d, "");
							gen(fp, "", "leas", "5,s", "");
							break;

						case TYPE_INTEGER: // INTEGER = BYTE - REAL
							gen(fp, "", "tfr", "s,x", "");
							gen(fp, "", "lbsr", "assign_real_at_X_to_D", "");
							gen(fp, "", "std", d, "");
							gen(fp, "", "leas", "5,s", "");
							break;

						case TYPE_REAL: // REAL = BYTE - REAL
							gen(fp, "", "puls", "d", "");
							gen(fp, "", "std", d, "");
							gen(fp, "", "puls", "d", "");
							g.s.getScopedName(d, dst); strcat(d, "+2,u");
							gen(fp, "", "std", d, "");
							gen(fp, "", "puls", "a", "");
							g.s.getScopedName(d, dst); strcat(d, "+4,u");
							gen(fp, "", "sta", d, "");
							break;
					}
					break;

				case  TYPE_INTEGER:	// REAL - INTEGER
					gen(fp, labelstr, "ldd", s2, comment);
					gen(fp, "", "leax", s1, "");
					gen(fp, "", "leas", "-5,s", "");
					gen(fp, "", "lbsr", "subtract_D_from_real_at_X_to_2s", "");


					switch (dstType)
					{
						case TYPE_BYTE:	// BYTE = INTEGER - REAL
							gen(fp, "", "tfr", "s,x", "");
							gen(fp, "", "lbsr", "assign_real_at_X_to_D", "");
							gen(fp, "", "std", d, "");
							gen(fp, "", "leas", "5,s", "");
							break;

						case TYPE_INTEGER: // INTEGER = INTEGER - REAL
							gen(fp, "", "tfr", "s,x", "");
							gen(fp, "", "lbsr", "assign_real_at_X_to_D", "");
							gen(fp, "", "std", d, "");
							gen(fp, "", "leas", "5,s", "");
							break;

						case TYPE_REAL: // REAL = INTEGER - REAL
							gen(fp, "", "puls", "d", "");
							gen(fp, "", "std", d, "");
							gen(fp, "", "puls", "d", "");
							g.s.getScopedName(d, dst); strcat(d, "+2,u");
							gen(fp, "", "std", d, "");
							gen(fp, "", "puls", "a", "");
							g.s.getScopedName(d, dst); strcat(d, "+4,u");
							gen(fp, "", "sta", d, "");
							break;
					}
					break;

				case  TYPE_REAL: // REAL - REAL
					gen(fp, "", "leax", s1, "");
					gen(fp, "", "leay", s2, "");
					gen(fp, "", "leas", "-5,s", "");
					gen(fp, "", "lbsr", "subtract_real_at_Y_from_real_at_X_to_2s", "");

					switch (dstType)
					{
						case TYPE_BYTE:	// BYTE = REAL - REAL
							gen(fp, "", "tfr", "s,x", "");
							gen(fp, "", "lbsr", "assign_real_at_X_to_D", "");
							gen(fp, "", "stb", d, "");
							gen(fp, "", "leas", "5,s", "");
							break;

						case TYPE_INTEGER: // INTEGER = REAL - REAL
							gen(fp, "", "tfr", "s,x", "");
							gen(fp, "", "lbsr", "assign_real_at_X_to_D", "");
							gen(fp, "", "std", d, "");
							gen(fp, "", "leas", "5,s", "");
							break;

						case TYPE_REAL: // REAL = REAL - REAL
							gen(fp, "", "puls", "d", "");
							gen(fp, "", "std", d, "");
							gen(fp, "", "puls", "d", "");
							g.s.getScopedName(d, dst); strcat(d, "+2,u");
							gen(fp, "", "std", d, "");
							gen(fp, "", "puls", "a", "");
							g.s.getScopedName(d, dst); strcat(d, "+4,u");
							gen(fp, "", "sta", d, "");
							break;
					}
					break;
			}
			break;
	}
}


/* Legal multiplications:

	Booleans cannot be multiplied

	Bytes, Integers and Reals can be multiplied together
	BYTE * BYTE = 0-255
	INTEGER * INTEGER = -32768 to 32767
	REAL * REAL = REAL range
	BYTE * INTEGER = -32768 to 32767
	BYTE * REAL = REAL range
	INTEGER * REAL = REAL range
*/
void multiply(FILE *fp, char *labelstr, char *opcode, int src1, int src2, int dst, char *comment)
{
	char s1[128];
	char s2[128];
	char d[128];
	int src1Type, src2Type, dstType;
	
	g.s.getScopedName(s1, src1); strcat(s1, ",u");
	g.s.getScopedName(s2, src2); strcat(s2, ",u");
	g.s.getScopedName(d, dst); strcat(d, ",u");

	src1Type = g.s.getType(src1);
	src2Type = g.s.getType(src2);
	dstType = g.s.getType(dst);

	switch (src1Type)
	{
		case TYPE_BYTE:
			switch (src2Type)
			{
				case  TYPE_BYTE:
					gen(fp, labelstr, "lda", s1, comment);
					gen(fp, "", "ldb", s2, comment);
					gen(fp, "", "mul", "", "");

					switch (dstType)
					{
						case TYPE_BYTE:
							gen(fp, "", "stb", d, "");
							break;

						case TYPE_INTEGER:
							gen(fp, "", "std", d, "");
							break;

						case TYPE_REAL:
							printf("TODO: REAL = BYTE * BYTE\n");
							break;
					}
					break;

				case  TYPE_INTEGER:
					gen(fp, labelstr, "lda", s1, comment);
					gen(fp, "", "ldx", s2, "");
					gen(fp, "", "lbsr", "signed_multiply_integer_byte", "");

					switch (dstType)
					{
						case TYPE_BYTE:
							gen(fp, "", "stb", d, "");
							break;

						case TYPE_INTEGER:
							gen(fp, "", "std", d, "");
							break;

						case TYPE_REAL:
							printf("TODO: REAL = BYTE * INTEGER\n");
							break;
					}
					break;

				case  TYPE_REAL:
					gen(fp, labelstr, "ldb", s1, comment);
					gen(fp, "", "leax", s2, "");
					gen(fp, "", "lbsr", "multiply_real_byte", "");

					switch (dstType)
					{
						case TYPE_BYTE:
							gen(fp, "", "stb", d, "");
							break;

						case TYPE_INTEGER:
							gen(fp, "", "std", d, "");
							break;

						case TYPE_REAL:
							printf("TODO: REAL = BYTE * REAL\n");
							break;
					}
					break;
			}
			break;
					
		case TYPE_INTEGER:
			switch (src2Type)
			{
				case  TYPE_BYTE:
					gen(fp, labelstr, "lda", s2, comment);
					gen(fp, "", "ldx", s1, "");
					gen(fp, "", "lbsr", "signed_multiply_integer_byte", "");

					switch (dstType)
					{
						case TYPE_BYTE:
							gen(fp, "", "stb", d, "");
							break;

						case TYPE_INTEGER:
							gen(fp, "", "std", d, "");
							break;

						case TYPE_REAL:
							printf("TODO: REAL = INTEGER * BYTE\n");
							break;
					}
					break;

				case  TYPE_INTEGER:
					gen(fp, labelstr, "ldd", s1, comment);
					gen(fp, "", "ldx", s2, "");
					gen(fp, "", "lbsr", "signed_multiply_integer_integer", "");

					switch (dstType)
					{
						case TYPE_BYTE:
							gen(fp, "", "stb", d, "");
							break;

						case TYPE_INTEGER:
							gen(fp, "", "std", d, "");
							break;

						case TYPE_REAL:
							printf("TODO: REAL = INTEGER * INTEGER\n");
							break;
					}
					break;

				case  TYPE_REAL:
					gen(fp, labelstr, "ldd", s1, comment);
					gen(fp, "", "leax", s2, "");
					gen(fp, "", "lbsr", "multiply_real_integer", "");

					switch (dstType)
					{
						case TYPE_BYTE:
							gen(fp, "", "stb", d, "");
							break;

						case TYPE_INTEGER:
							gen(fp, "", "std", d, "");
							break;

						case TYPE_REAL:
							printf("TODO: REAL = INTEGER * REAL\n");
							break;
					}
					break;
			}
			break;
					
		case TYPE_REAL:
			switch (src2Type)
			{
				case  TYPE_BYTE:
					switch (dstType)
					{
						case TYPE_BYTE:
							printf("TODO: BYTE = REAL * BYTE\n");
							break;

						case TYPE_INTEGER:
							printf("TODO: INTEGER = REAL * BYTE\n");
							break;

						case TYPE_REAL:
							printf("TODO: REAL = REAL * BYTE\n");
							break;
					}
					break;

				case  TYPE_INTEGER:
					switch (dstType)
					{
						case TYPE_BYTE:
							printf("TODO: BYTE = REAL * INTEGER\n");
							break;

						case TYPE_INTEGER:
							printf("TODO: INTEGER = REAL * INTEGER\n");
							break;

						case TYPE_REAL:
							printf("TODO: REAL = REAL * INTEGER\n");
							break;
					}
					break;

				case  TYPE_REAL:
					switch (dstType)
					{
						case TYPE_BYTE:
							printf("TODO: BYTE = REAL * REAL\n");
							break;

						case TYPE_INTEGER:
							printf("TODO: INTEGER = REAL * REAL\n");
							break;

						case TYPE_REAL:
							printf("TODO: REAL = REAL * REAL\n");
							break;
					}
					break;
			}
			break;					
	}
}


void divide(FILE *fp, char *labelstr, char *opcode, int src1, int src2, int dst, char *comment)
{
	char s1[128];
	char s2[128];
	char d[128];
	int src1Type, src2Type, dstType;
	
	g.s.getScopedName(s1, src1); strcat(s1, ",u");
	g.s.getScopedName(s2, src2); strcat(s2, ",u");
	g.s.getScopedName(d, dst); strcat(d, ",u");

	src1Type = g.s.getType(src1);
	src2Type = g.s.getType(src2);
	dstType = g.s.getType(dst);

	switch (src1Type)
	{
		case TYPE_BYTE:
			switch (src2Type)
			{
				case  TYPE_BYTE:
					gen(fp, labelstr, "lda", s2, comment);
					gen(fp, "", "ldb", s1, "");
					gen(fp, "", "lbsr", "byte_div_byte", "");

					switch (dstType)
					{
						case TYPE_BYTE:
							gen(fp, "", "stb", d, "");
							break;

						case TYPE_INTEGER:
							gen(fp, "", "std", d, "");
							break;

						case TYPE_REAL:
							printf("TODO: REAL = BYTE * BYTE\n");
							break;
					}
					break;

				case  TYPE_INTEGER:
					gen(fp, labelstr, "lda", s2, comment);
					gen(fp, "", "ldx", s1, "");
					gen(fp, "", "lbsr", "byte_div_integer", "");

					switch (dstType)
					{
						case TYPE_BYTE:
							gen(fp, "", "stb", d, "");
							break;

						case TYPE_INTEGER:
							gen(fp, "", "std", d, "");
							break;

						case TYPE_REAL:
							printf("TODO: REAL = BYTE / INTEGER\n");
							break;
					}
					break;

				case  TYPE_REAL:
					gen(fp, labelstr, "ldb", s2, comment);
					gen(fp, "", "leax", s1, "");
					gen(fp, "", "lbsr", "real_div_byte", "");

					switch (dstType)
					{
						case TYPE_BYTE:
							gen(fp, "", "stb", d, "");
							break;

						case TYPE_INTEGER:
							gen(fp, "", "std", d, "");
							break;

						case TYPE_REAL:
							printf("TODO: REAL = BYTE * REAL\n");
							break;
					}
					break;
			}
			break;
					
		case TYPE_INTEGER:
			switch (src2Type)
			{
				case  TYPE_BYTE:
					gen(fp, labelstr, "lda", s2, comment);
					gen(fp, "", "ldx", s1, "");
					gen(fp, "", "lbsr", "integer_div_byte", "");

					switch (dstType)
					{
						case TYPE_BYTE:
							gen(fp, "", "stb", d, "");
							break;

						case TYPE_INTEGER:
							gen(fp, "", "clra", "", "");
							gen(fp, "", "std", d, "");
							break;

						case TYPE_REAL:
							printf("TODO: REAL = INTEGER / BYTE\n");
							break;
					}
					break;

				case  TYPE_INTEGER:
					gen(fp, labelstr, "ldd", s2, comment);
					gen(fp, "", "ldx", s1, "");
					gen(fp, "", "lbsr", "integer_div_integer", "");

					switch (dstType)
					{
						case TYPE_BYTE:
							gen(fp, "", "tfr", "x,d", "");
							gen(fp, "", "stb", d, "");
							break;

						case TYPE_INTEGER:
							gen(fp, "", "stx", d, "");
							break;

						case TYPE_REAL:
							printf("TODO: REAL = INTEGER / INTEGER\n");
							break;
					}
					break;

				case  TYPE_REAL:
					gen(fp, labelstr, "ldd", s2, comment);
					gen(fp, "", "leax", s1, "");
					gen(fp, "", "lbsr", "integer_div_real", "");

					switch (dstType)
					{
						case TYPE_BYTE:
							gen(fp, "", "stb", d, "");
							break;

						case TYPE_INTEGER:
							gen(fp, "", "clra", "", "");
							gen(fp, "", "std", d, "");
							break;

						case TYPE_REAL:
							printf("TODO: REAL = INTEGER / REAL\n");
							break;
					}
					break;
			}
			break;
					
		case TYPE_REAL:
			switch (src2Type)
			{
				case  TYPE_BYTE:
					switch (dstType)
					{
						case TYPE_BYTE:
							printf("TODO: BYTE = REAL / BYTE\n");
							break;

						case TYPE_INTEGER:
							printf("TODO: INTEGER = REAL / BYTE\n");
							break;

						case TYPE_REAL:
							printf("TODO: REAL = REAL / BYTE\n");
							break;
					}
					break;

				case  TYPE_INTEGER:
					switch (dstType)
					{
						case TYPE_BYTE:
							printf("TODO: BYTE = REAL / INTEGER\n");
							break;

						case TYPE_INTEGER:
							printf("TODO: INTEGER = REAL / INTEGER\n");
							break;

						case TYPE_REAL:
							printf("TODO: REAL = REAL / INTEGER\n");
							break;
					}
					break;

				case  TYPE_REAL:
					switch (dstType)
					{
						case TYPE_BYTE:
							printf("TODO: BYTE = REAL / REAL\n");
							break;

						case TYPE_INTEGER:
							printf("TODO: INTEGER = REAL / REAL\n");
							break;

						case TYPE_REAL:
							printf("TODO: REAL = REAL / REAL\n");
							break;
					}
					break;
			}
			break;					
	}
}


void rem(FILE *fp, char *labelstr, char *opcode, int src1, int src2, int dst, char *comment)
{
	char s1[128];
	char s2[128];
	char d[128];
	
	g.s.getScopedName(s1, src1); strcat(s1, ",u");
	g.s.getScopedName(s2, src2); strcat(s2, ",u");
	g.s.getScopedName(d, dst); strcat(d, ",u");
	gen(fp, labelstr, "ldd", s1, comment);
	gen(fp, "", "divd", s2, "");
	gen(fp, "", "std", d, "");
}


void neg(FILE *fp, char *labelstr, char *opcode, int src1, int src2, int dst, char *comment)
{
	char d[128];
	
	g.s.getScopedName(d, dst); strcat(d, ",u");
	gen(fp, labelstr, "ldd", d, comment);
	gen(fp, "", "coma", "", "");
	gen(fp, "", "comb", "", "");
	gen(fp, "", "addd", "#$0001", "");
	gen(fp, "", "std", d, "");
}


void _not(FILE *fp, int address, char *labelstr, char *opcode, int src1, int src2, int dst, char *comment)
{
	char s1[128];
	char s2[128];
	
	g.s.getScopedName(s1, src1); strcat(s1, ",u");
	g.s.getScopedName(s2, src2); strcat(s2, ",u");
	gen(fp, labelstr, "ldd", s1, comment);
	gen(fp, "", "bne", addrstr(address - 1, labelstr, 'T'), "");
	gen(fp, "", "ldd", "#$0001", "");
	gen(fp, "", "std", s2, "");
	gen(fp, "", "bra", addrstr(address, labelstr, 'L'), "");
	gen(fp, addrstr(address - 1, labelstr, 'T'), "ldd", "#$0000", "");
	gen(fp, "", "std", s2, "");
}


/*
 * Only BOOLEAN types can participate in this operation
 */
void _or(FILE *fp, char *labelstr, char *opcode, int src1, int src2, int dst, char *comment)
{
	char s1[128];
	char s2[128];
	char d[128];
	
	g.s.getScopedName(s1, src1); strcat(s1, ",u");
	g.s.getScopedName(s2, src2); strcat(s2, ",u");
	g.s.getScopedName(d, dst); strcat(d, ",u");
	gen(fp, labelstr, "ldb", s1, comment);
	gen(fp, "", "orb", s2, "");
	gen(fp, "", "stb", d, "");
}


/*
 * Only BOOLEAN types can participate in this operation
 */
void _and(FILE *fp, char *labelstr, char *opcode, int src1, int src2, int dst, char *comment)
{
	char s1[128];
	char s2[128];
	char d[128];
	
	g.s.getScopedName(s1, src1); strcat(s1, ",u");
	g.s.getScopedName(s2, src2); strcat(s2, ",u");
	g.s.getScopedName(d, dst); strcat(d, ",u");
	gen(fp, labelstr, "ldb", s1, comment);
	gen(fp, "", "andb", s2, "");
	gen(fp, "", "stb", d, "");
}


void gt(FILE *fp, char *labelstr, char *opcode, int src1, int src2, int dst, char *comment)
{
	char dststr[128];

	gen(fp, labelstr, "lbra", g.s.getScopedName(dststr, dst), comment);
}


void nop(FILE *fp, char *labelstr, char *opcode, int src1, int src2, int dst, char *comment)
{
	gen(fp, labelstr, "nop", "", comment);
}


int myatoi(char *num)
{
	char o, *p;
	int n, e;
	
	p = strchr(num, 'e');
	if (p == NULL)
	{
		p = strchr(num, 'E');
		if (p == NULL)
		{
			return atoi(num);
		}
	}

	// We have found an exponent. 
	o = *p;
	
	*p = '\0';
	n = atoi(num);
	e = atoi(p + 1);
	*p = o;
	
	return n*(int)pow(10, e);
}

char *fname(char *file)
{
	char *p = strrchr(file, '/');
	if (p == NULL)
	{
		p = file;
	}
	else
	{
		p++;
	}

	return p;
}


int main(int argc, char **argv)
{
 	char *ic, *of;
 	int ds, cs;
 	FILE *fi, *fo;
 	
 	sprintf(verstr, "v%d.%d", VERMAJOR, VERMINOR);
 	
 	if (argc != 3)
 	{
 		fprintf(stderr, "Usage: %s <icodefile> <outfile>\n", argv[0]);
 		exit(1);
 	}
 	
 	ic = argv[1];
	of = argv[2];
	
	fi = fopen(ic, "r");
	if (fi == NULL)
	{
		fprintf(stderr, "Address %d: could not open file %s\n", inAddr, ic);
	}
	
	fo = fopen(of, "w+");
	
	if (fo != NULL)
	{
		char dsstr[32];
		static const char *rtlImports[] =
		{
			"add_D_to_real_at_X_to_2s", "add_real_at_X_to_real_at_Y_to_2s", "addstring",
			"assign_D_to_real_at_X", "assign_real_at_X_to_D", "byte_div_byte", "byte_div_integer",
			"cmp_D_to_real_at_X", "cmp_real_at_X_to_D", "cmp_real_at_X_to_real_at_Y", "copystring",
			"integer_div_byte", "integer_div_integer", "integer_div_real", "multiply_real_byte",
			"multiply_real_integer", "real_div_byte", "signed_multiply_integer_byte",
			"signed_multiply_integer_integer", "subtract_D_from_real_at_X_to_2s",
			"subtract_real_at_X_from_D_to_2s", "subtract_real_at_Y_from_real_at_X_to_2s",
			NULL
		};

		fprintf(fo, "* Generated by %s %s\n\n", argv[0], verstr);
		g.s.read(fi, &ds);
		g.read(fi);
		sprintf(dsstr, "%d", ds);

		for (int i = 0; rtlImports[i] != NULL; i++)
		{
			fprintf(fo, "%-12s  import  %s\n", "", rtlImports[i]);
		}
		for (int i = 0; i < g.s.count(); i++)
		{
			if (g.s.getType(i) == TYPE_PROCEDURE)
			{
				char *value = g.s.getValue(i);

				if (value != NULL && atoi(value) >= 0)
				{
					// procedure is defined in this translation unit
					fprintf(fo, "%-12s  export  %s\n", "", g.s.getName(i));
				}
				else
				{
					// procedure is defined elsewhere (runtime library)
					fprintf(fo, "%-12s  import  %s\n", "", g.s.getName(i));
				}
			}
		}
		fprintf(fo, "\n");

		fprintf(fo, "* Uninitialized data\n");
		fprintf(fo, "%-12s  section bss,bss\n", "");
		createUninitializedDataSegment(fo);
		fprintf(fo, "%-12s  endsect\n\n", "");
		fprintf(fo, "* Initialized data\n");
		fprintf(fo, "%-12s  section code\n", "");
		createInitializedDataSegment(fo);
//		gen(fo, "main:", "", "", "");
		translate(fo, &cs);
		fprintf(fo, "\n%-12s  endsect\n", "");

		fclose(fi);
		fclose(fo);
	}
	else
	{
		fprintf(stderr, "Address %d: could not create file %s\n", inAddr, of);
	}
}
 


void createUninitializedDataSegment(FILE *fp)
{
	int i;
	
	for (i = 0; i < g.s.count(); i++)
	{
		char v[24], w[48];
		
		if (g.s.isInitialized(i))
		{
			continue;
		}
		
		sprintf(v, "%d", g.s.getSize(i));
		switch (g.s.getType(i))
		{
			case TYPE_PROCEDURE:
			case TYPE_LABEL:
			case TYPE_UNKNOWN:
				break;
				
			default:
				if (g.s.getGlobal(i) == 1)
				{
					sprintf(w, "%s:", g.s.getName(i));
				}
				else
				{
					g.s.getScopedName(w, i);
//					sprintf(w, "%s", g.s.getName(i));
				}
				gen(fp, w, "rmb", v, g.s.getTypeName(g.s.getType(i)));

				if (g.s.getType(i) == TYPE_STRING)
				{
					// add one additional byte for nul character
					gen(fp, "", "rmb", "1", "");
				}
				break;
		}
	}
}


void createInitializedDataSegment(FILE *fp)
{
	int i;
	
	for (i = 0; i < g.s.count(); i++)
	{
		char name[48];
		
		if (!g.s.isInitialized(i))
		{
			continue;
		}
		
		if (g.s.getGlobal(i) == 1)
		{
			sprintf(name, "%s:", g.s.getName(i));
		}
		else
		{
//			sprintf(name, "%s", g.s.getName(i));
			g.s.getScopedName(name, i);
		}

		switch (g.s.getType(i))
		{
			case TYPE_STRING:
			{
				char s[256];
				char extra[256];
				int l = g.s.getSize(i) - strlen(g.s.getValue(i));
				
				sprintf(s, "\"%s\"", g.s.getValue(i));
				
				gen(fp, name, "fcc", s, g.s.getValue(i));

				strcpy(extra, "");
				while (l--)
				{
					strcat(extra, "0,");
				}
				strcat(extra, "0");
				gen(fp, "", "fcb", extra, "");
			}
			break;
				
			case TYPE_INTEGER:
			{
				char ivalue[32];
				int in;
				
				if (g.s.getValue(i) == NULL)
				{
					g.s.setValue(i, "0");
				}
				in = myatoi(g.s.getValue(i));
				if (in > 65535)
				{
					fprintf(stderr, "WARNING! %d exceeds 65535\n", in);
				}
				sprintf(ivalue, "%d", in);
				gen(fp, name, "fdb", ivalue, g.s.getValue(i));
			}
			break;
				
			case TYPE_BYTE:
			{
				char ivalue[32];
				int in;
				
				if (g.s.getValue(i) == NULL)
				{
					g.s.setValue(i, "0");
				}
				in = myatoi(g.s.getValue(i));
				if (in > 65535)
				{
					fprintf(stderr, "WARNING! %d exceeds 65535\n", in);
				}
				sprintf(ivalue, "%d", in);
				gen(fp, name, "fcb", ivalue, g.s.getValue(i));
			}
			break;
				
			case TYPE_BOOLEAN:
			{
				gen(fp, name, "fcb", g.s.getValue(i), g.s.getValue(i));
			}
			break;
				
#if 0
			case TYPE_PROCEDURE:
			{
				char *avalue;
				
				if (g.s.getValue(i) == NULL)
				{
					g.s.setValue(i, "0");
				}
				avalue = g.s.getValue(i);
				gen(fp, name, "fdb", avalue, g.s.getValue(i));
			}
			break;
#endif
				
			case TYPE_REAL:
			{
				// TODO : determine format for reals
				if (g.s.getValue(i) == NULL)
				{
					g.s.setValue(i, "0");
				}
				gen(fp, name, "fdb", "0,0,0,0,0", g.s.getValue(i));
			}
		}
	}
}



void translate(FILE *fp, int *codeSize)
{
	int i;
	
 	*codeSize = 0;
 	
	/* start reading icode lines */
	for (i = 0; i < g.nextQuad; i++)
	{
		inAddr = i;
		
		char labelstr[128];
		char comment[128];
		char *opcode;
		int address = 0, src1 = 0, src2 = 0, dst = 0;
		
//		addrstr(codeloc++, labelstr, 'L');
		labelstr[0] = '\0';
		
		address = i;
		opcode = g.QuadArray[i].opcode;
		src1 = g.QuadArray[i].src1;
		src2 = g.QuadArray[i].src2;
		dst = g.QuadArray[i].dst;
	
		if (opcode != NULL)
		{
			g.disassemble(address, comment, "%s %s %s %s %s");

			for (int j = 0; j < g.s.count(); j++)
			{
				if (g.s.getType(j) == TYPE_PROCEDURE || g.s.getType(j) == TYPE_LABEL)
				{
					if (g.s.getValue(j) == NULL)
					{
						// only error out if the label is undefined
						// procedures can be external
						if (g.s.getType(j) == TYPE_LABEL)
						{
							fprintf(stderr, "Undefined label '%s'\n", g.s.getName(j));
							exit(1);
						}
					}
					else
					if (address == atoi(g.s.getValue(j)))
					{
						if (g.s.getType(j) == TYPE_LABEL)
						{
							g.s.getScopedName(labelstr, j);
						}
						else
						{
							sprintf(labelstr, "%s:", g.s.getName(j));
						}
						gen(fp, labelstr, "", "", "");
						labelstr[0] = '\0';
					}
				}
			}

			if (strcasecmp(opcode, "nop") == 0)
			{
				nop(fp, labelstr, opcode, src1, src2, dst, comment);
			}
			else if (strcasecmp(opcode, "gt") == 0)
			{
				gt(fp, labelstr, opcode, src1, src2, dst, comment);
			}
			else if (strcasecmp(opcode, "and") == 0)
			{
				_and(fp, labelstr, opcode, src1, src2, dst, comment);
			}
			else if (strcasecmp(opcode, "or") == 0)
			{
				_or(fp, labelstr, opcode, src1, src2, dst, comment);
			}
			else if (strcasecmp(opcode, "not") == 0)
			{
				_not(fp, address, labelstr, opcode, src1, src2, dst, comment);
			}
			else if (strcasecmp(opcode, "neg") == 0)
			{
				neg(fp, labelstr, opcode, src1, src2, dst, comment);
			}
			else if (strcasecmp(opcode, "+") == 0)
			{
				add(fp, labelstr, opcode, src1, src2, dst, comment);
			}
			else if (strcasecmp(opcode, "-") == 0)
			{
				subtract(fp, labelstr, opcode, src1, src2, dst, comment);
			}
			else if (strcasecmp(opcode, "*") == 0)
			{
				multiply(fp, labelstr, opcode, src1, src2, dst, comment);
			}
			else if (strcasecmp(opcode, "/") == 0)
			{
				divide(fp, labelstr, opcode, src1, src2, dst, comment);
			}
			else if (strcasecmp(opcode, "rem") == 0)
			{
				rem(fp, labelstr, opcode, src1, src2, dst, comment);
			}
			else if (strcasecmp(opcode, "cp") == 0)
			{
				cp(fp, labelstr, opcode, src1, src2, dst, comment);
			}
			else
			if (strcasecmp(opcode, "gt!=") == 0
				|| strcasecmp(opcode, "gt=") == 0
				|| strcasecmp(opcode, "gt>=") == 0
				|| strcasecmp(opcode, "gt<=") == 0
				|| strcasecmp(opcode, "gt>") == 0
				|| strcasecmp(opcode, "gt<") == 0
			)
			{
				gtconditional(fp, labelstr, opcode, src1, src2, dst, comment);
			}
			else if (strcmp(opcode, ">") == 0
			|| strcmp(opcode, "<") == 0
			|| strcmp(opcode, ">=") == 0
			|| strcmp(opcode, "<=") == 0
			|| strcmp(opcode, "=") == 0
			|| strcmp(opcode, "<>") == 0
			)
			{
				relation(fp, address, labelstr, opcode, src1, src2, dst, comment);
			}
			else if (strcasecmp(opcode, "ret") == 0)
			{
				ret(fp, labelstr, opcode, src1, src2, dst, comment);
			}
			else if (strcasecmp(opcode, "exit") == 0)
			{
				_exit(fp, labelstr, opcode, src1, src2, dst, comment);
			}
			else if (strcasecmp(opcode, "call") == 0)
			{
				call(fp, labelstr, opcode, src1, src2, dst, comment);
			}
			else
			{
				fprintf(stderr, "unknown instruction '%s'\n", comment);
				exit(1);
			}
		}
	}
}

