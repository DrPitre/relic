/*
 * raginBasicParser: P a r s e r  H e a d e r 
 *
 * Generated from: ../../../rb/fe/rb.g
 *
 * Terence Parr, Russell Quong, Will Cohen, and Hank Dietz: 1989-2001
 * Parr Research Corporation
 * with Purdue University Electrical Engineering
 * with AHPCRC, University of Minnesota
 * ANTLR Version 1.33MR33
 */

#ifndef raginBasicParser_h
#define raginBasicParser_h

#ifndef ANTLR_VERSION
#define ANTLR_VERSION 13333
#endif

#include "AParser.h"


// any definitions that you need in the generated files
#include "../../common/codegen.h"
class raginBasicParser : public ANTLRParser {
public:
	static  const ANTLRChar *tokenName(int tk);
	enum { SET_SIZE = 82 };
protected:
	static const ANTLRChar *_token_tbl[];
private:

// parser definitions go here
public:


char *typestr[8]; 

	class parseState
{
  public:
  int scope;
  int level;
  int params;
  int index, type, size, negate;
  
			int idCount;
  
		parseState(void)
  {
    scope = level = params = index = type = size = idCount = negate = 0;
  }
};

	CodeGen g;
parseState state;

        void init()
{
  ANTLRParser::init();
  
            // any specific initialization you need
  // (if none, don't override the init() method
}


void showerror(char *string)
{
fprintf(stderr, "**** line %d **** : %s\n", 0, string);
}

		
void checkForwards()
{
for (int i = 0; i < g.s.count(); i++)
{
if (g.s.getType(i) == TYPE_LABEL && g.s.getValue(i) == NULL)
{
printf("%s is an unresolved reference\n", g.s.getName(i));
}
}
}


int isNumber(int type)
{
if (type == TYPE_INTEGER || type == TYPE_REAL || type == TYPE_BYTE)
{
return 1;
}

			return 0;
}

        int isSignedNumber(int type)
{
if (type == TYPE_INTEGER || type == TYPE_REAL)
{
return 1;
}

			return 0;
}

        int validAssignment(int typeTo, int typeFrom)
{
if (typeTo == typeFrom)
{
// same type -- always a valid assignment
return 1;
}

			if (isNumber(typeTo) && isNumber(typeFrom))
{
#if 0
// both types are numbers -- return valid only if going up
switch (typeFrom)
{
case TYPE_BYTE:
if (typeTo == TYPE_INTEGER || typeTo == TYPE_REAL)
{
return 1;
}
break;

					case TYPE_INTEGER:
if (typeTo == TYPE_REAL || typeTo == TYPE_BYTE)
{
return 1;
}
break;
}
#else
return 1;

#endif
}

			// invalid assignment -- return as such
return 0;
}
protected:
	static SetWordType err1[12];
	static SetWordType STRING_LITERAL_set[12];
	static SetWordType STRING_LITERAL_errset[12];
	static SetWordType err4[12];
	static SetWordType setwd1[82];
	static SetWordType err5[12];
	static SetWordType err6[12];
	static SetWordType err7[12];
	static SetWordType err8[12];
	static SetWordType setwd2[82];
	static SetWordType err9[12];
	static SetWordType err10[12];
	static SetWordType err11[12];
	static SetWordType COMMENT_set[12];
	static SetWordType COMMENT_errset[12];
	static SetWordType err14[12];
	static SetWordType err15[12];
	static SetWordType setwd3[82];
	static SetWordType err16[12];
	static SetWordType err17[12];
	static SetWordType setwd4[82];
	static SetWordType err18[12];
	static SetWordType err19[12];
	static SetWordType err20[12];
	static SetWordType err21[12];
	static SetWordType err22[12];
	static SetWordType setwd5[82];
	static SetWordType err23[12];
	static SetWordType err24[12];
	static SetWordType err25[12];
	static SetWordType setwd6[82];
	static SetWordType err26[12];
	static SetWordType err27[12];
	static SetWordType err28[12];
	static SetWordType err29[12];
	static SetWordType err30[12];
	static SetWordType setwd7[82];
	static SetWordType ADD_OP_set[12];
	static SetWordType ADD_OP_errset[12];
	static SetWordType MULT_OP_set[12];
	static SetWordType MULT_OP_errset[12];
	static SetWordType RELATIONAL_OP_set[12];
	static SetWordType RELATIONAL_OP_errset[12];
	static SetWordType setwd8[82];
	static SetWordType BOOLEAN_OP_set[12];
	static SetWordType BOOLEAN_OP_errset[12];
	static SetWordType setwd9[82];
private:
	void zzdflthandlers( int _signal, int *_retsignal );

public:
	raginBasicParser(ANTLRTokenBuffer *input);
	void program(void);
	void procedure(void);
	void procedureBody(parseState & state);
	void varDecl(parseState & state);
	void identList(parseState & state);
	void constantValue(parseState & state);
	void paramDecl(parseState & state);
	void typeDecl(parseState & state);
	void integerConstant(void);
	void recordDecl(parseState & state);
	void typeName(parseState & state);
	void statement(parseState & state);
	void runStatement(parseState & state);
	void statementList(parseState & state);
	void gotoStatement(parseState & state);
	void assignmentStatement(parseState & state);
	void exitStatement(parseState & state);
	void forStatement(parseState & state);
	void ifStatement(parseState & state);
	void ifPart(parseState & state);
	void whileStatement(parseState & state);
	void endStatement(parseState & state);
	void variableReference(parseState & state);
	void declaredVariable(parseState & state);
	void modFunction(parseState & state);
	void notFunction(parseState & state);
	void trueFunction(parseState & state);
	void falseFunction(parseState & state);
	void ioStatement(parseState & state);
	void primitiveElement(parseState & state);
	void signExpression(parseState & state);
	void multiplyingExpression(parseState & stateL);
	void addingExpression(parseState & stateL);
	void relationalExpression(parseState & stateL);
	void expression(parseState & stateL);
};

#endif /* raginBasicParser_h */
