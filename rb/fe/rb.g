#header
<<
// any definitions that you need in the generated files
#include "../../common/codegen.h"
>>


<<
// scanner definitions would go here
#include "PBlackBox.h"
#include "DLexerBase.h"
#include "DLGLexer.h"
#include "AToken.h"
typedef ANTLRCommonToken ANTLRToken;

int main(int argc, char **argv)
{
	FILE *in;
		
		
	if (argv[1] == NULL)
	{
		fprintf(stderr, "input file name needed\n");
		exit(1);
	}
		
	if (argv[2] == NULL)
	{
		fprintf(stderr, "output file name needed\n");
		exit(1);
	}
		
	in = fopen(argv[1], "r");
		
	if (in != NULL)
	{
		FILE *out;		
		ParserBlackBox < DLGLexer, raginBasicParser, ANTLRToken > p(in);
	
		out = fopen(argv[2], "w+");
		if (out != NULL)
		{
			int index, type;
	
			p.parser()->program();
			p.parser()->g.s.dump(out);
			p.parser()->g.dump(out);
		}
		else
		{
			fprintf(stderr, "failed to create %s\n", argv[2]);
			exit(1);
		}
	}
	else
	{
		fprintf(stderr, "failed to open %s\n", argv[1]);
		exit(1);
	}

#if 0
    DLGFileInput in(stdin);
    DLGLexer scanner(&in);
    ANTLRTokenBuffer pipe(&scanner);
    ANTLRToken tok;
	scanner.setToken(&tok);
    raginBasicParser bParser(&pipe);

    bParser.init(); // initialize
    bParser.program(); // start first rule
	bParser.checkForwards(); // check forward references in symbol table
	bParser.g.s.dump("program.st");
	bParser.g.dump("program.ic");
#endif
    return 0;
}
>>


// scanner rules

#lexclass START // Not really necessary, but
                // good commentary nonetheless

// White Space
#token "[\ \t]"       <<skip();>>
#token CR "\n"          <<newline();>>

// Comments
//#token COMMENT1 "\(\* ~[\n@]* [\n@]" <<skip(); newline();>>
//#token COMMENT2 "(R|r)(E|e)(M|m) ~[\n@]* [\n@]" <<skip(); newline();>>
#token COMMENT1 "\(\* ~[\n@]* [\n@]" <<skip(); newline();>>
#token COMMENT2 "(R|r)(E|e)(M|m) ~[\n@]* [\n@]" <<skip(); newline();>>

// Keywords
#token DO        "DO"
#token DIM       "DIM"
#token ELSE      "ELSE"
#token END       "END"
#token ENDWHILE  "ENDWHILE"
#token ENDIF     "ENDIF"
#token PARAM     "PARAM"
#token REM       "REM"
#token TYPE      "TYPE"
#token FOR       "FOR"
#token TO        "TO"
#token NEXT      "NEXT"
#token STEP      "STEP"
#token PROCEDURE "PROCEDURE"
#token PRINT     "PRINT"
#token INPUT     "INPUT"
#token GOTO      "GOTO"
#token CALL      "RUN"
#token IF        "IF"
#token THEN      "THEN"
#token WHILE     "WHILE"
#token LOOP      "LOOP"
#token AND       "AND"
#token OR        "OR"
#token FALSE_  	 "FALSE"
#token INTEGER   "INTEGER"
#token REAL      "REAL"
#token LET       "LET"
#token MOD       "MOD"
#token NOT       "NOT"
#token TRUE_     "TRUE"
#token BOOLEAN   "BOOLEAN" 
#token STR       "STRING"
#token BYTE      "BYTE" 

// Operators
#token DOT        "."
#token POUND      "#"
#token BECOMES    "="
#token COLON      ":"
#token SEMI       ";"
#token COMMA      ","
#token EQUALS     "="
#token LBRACKET   "\["
#token RBRACKET   "\]"
#token DOTDOT     ".."
#token LPAREN     "\("
#token RPAREN     "\)"
#token NOT_EQUALS "<>"
#token LT         "<"
#token LTE        "<="
#token GT         ">"
#token GTE        ">="
#token PLUS       "\+"
#token MINUS      "\-"
#token TIMES      "\*"
#token DIV        "/"
#token DOLLAR     "\$"
#token BSLASH     "\\"

// Identifiers
#token IDENT		"[a-zA-Z] [a-zA-Z0-9_]*"
#token IDENTSTR		"[a-zA-Z] [a-zA-Z0-9_]* \$"

// Literals
#token INTLIT   "[0-9]+ { . } | \$[0-9A-Fa-f]+"
#token REALLIT  "[0-9]+ { . [0-9]+ } {(e | E) { \+ | \- } [0-9]+}"
#token CHARLIT  "\' ~[] \'"
#token         "\"" << // start STRINGLIT
                        skip();
                        mode(STRING);
                    >>

// String Literal Processing
// Separate Scanner class!
#lexclass STRING
#token           "\"\"" <<
                            more();
                            replchar('\"');
                        >>
#token BADSTRING "\n"   <<
                            replchar('\0');
                            newline();
                            mode(START);
                            /* error message */
                        >>
#token STRINGLIT "\""   <<
                            replchar('\0');
                            mode(START);
                        >>
#token           "~[]"  <<more();>>

#tokclass STRING_LITERAL {STRINGLIT BADSTRING}
#tokclass ADD_OP {PLUS MINUS}
#tokclass RELATIONAL_OP {EQUALS NOT_EQUALS GT GTE LT LTE}
#tokclass BOOLEAN_OP {AND OR XOR}
#tokclass MULT_OP {TIMES DIV}
#tokclass COMMENT {COMMENT1 COMMENT2}

class raginBasicParser
{
<<
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
>>


program
	: (procedure)+
	;

procedure
	:
	<<
		int temp;
	>>
	PROCEDURE id:IDENT
  	<<
		temp = g.s.store(id->getText(), TYPE_PROCEDURE, -1);

		state.params = 0;
		g.s.setScope(temp, temp);
		state.scope = temp;
		{
			char str[12];
			
			sprintf(str, "%d", g.getNextQuad());
			g.s.setValue(temp, str);
		}
	>>
	CR
	procedureBody[state]
	<<
		g.gen("ret");
	>>
	;


procedureBody[parseState & state]
	:
	statementList[state]
	;


varDecl[parseState & state]
	:
	<<
		int start = g.s.count();

		state.idCount = 0;
	>>
	DIM identList[state] COLON typeName[state]
	<<
		for (int j = start; j < start + state.idCount; j++)
		{
		  	g.s.setType(j, state.type);
			// Check if the STRING has been declared a specific size
			if (state.type == TYPE_STRING && state.size != g.s.getSize(j))
			{
				g.s.setSize(j, state.size);
			}
			printf("size = %d\n", g.s.getSize(j));
		}
	>>
	( SEMI identList[state] COLON typeName[state]
	<<
		for (int j = start; j < start + state.idCount; j++)
		{
		  	g.s.setType(j, state.type);
			// Check if the STRING has been declared a specific size
			if (state.type == TYPE_STRING && state.size != g.s.getSize(j))
			{
				g.s.setSize(j, state.size);
			}
		}
	>>
	)*
	;


identList[parseState & state]
  : id1:IDENT
	<<
		int i;

		if ((i = g.s.getIndex((char *)id1->getText(), state.scope)) != -1)
		{
			printf("ERROR: redefined symbol %s\n", (char *)id1->getText());
			exit(1);
		}

		g.s.store(id1->getText(), TYPE_UNKNOWN, state.scope);
		state.idCount++;
	>>
	{ LPAREN INTLIT RPAREN } (COMMA id2:IDENT
	<<
		int i;

		if ((i = g.s.getIndex((char *)id2->getText(), state.scope)) != -1)
		{
			printf("ERROR: redefined symbol %s\n", (char *)id2->getText());
			exit(1);
		}

		g.s.store(id2->getText(), TYPE_UNKNOWN, state.scope);
		state.idCount++;
	>>
	)*
  ;


constantValue[parseState & state]
  : i:INTLIT
	<<
		state.type = TYPE_INTEGER;
		state.index = g.s.mkTemp(state.type, state.scope);
		g.s.setValue(state.index, (char *)i->getText());
	>>
  | r:REALLIT
	<<
		state.type = TYPE_REAL;
		state.index = g.s.mkTemp(state.type, state.scope);
		g.s.setValue(state.index, (char *)r->getText());
	>>
  | s:STRING_LITERAL
	<<
		state.type = TYPE_STRING;
		state.index = g.s.mkTemp(state.type, state.scope);
		state.size = strlen(s->getText());
		g.s.setValue(state.index, (char *)s->getText());
		g.s.setSize(state.index, state.size);
	>>
  ;


paramDecl[parseState & state]
	:
	<<
		int start = g.s.count();

		state.idCount = 0;
	>>
	PARAM identList[state] COLON typeName[state]
 	<<
		for (int j = start; j < start + state.idCount; j++)
		{
			g.s.setType(j, state.type);
			g.s.setParam(j, ++state.params);
			// Check if the STRING has been declared a specific size
			if (state.type == TYPE_STRING && state.size != g.s.getSize(j))
			{
				g.s.setSize(j, state.size);
			}
		}
	>>
	;

typeDecl[parseState & state]
  : TYPE IDENT EQUALS
    recordDecl[state]
  ;

integerConstant
  : INTLIT
  | IDENT // again, a constant...
  ;


recordDecl[parseState & state]
  :
	<<
		state.idCount = 0;
	>>
 identList[state] COLON typeName[state] ( SEMI identList[state] COLON typeName[state] )*
  ;


typeName[parseState & state]
	: STR
	<<
		state.type = TYPE_STRING;
		state.size = g.s.typelen[state.type];
	>>
	{ LBRACKET i:INTLIT RBRACKET
	<<
		state.size = atoi(i->getText());
		printf("state.size for this string is %d\n", state.size);
	>>
	}
	| INTEGER
	<<
		state.type = TYPE_INTEGER;
		state.size = g.s.getSize(state.type);
	>>
	| BOOLEAN
	<<
		state.type = TYPE_BOOLEAN;
		state.size = g.s.getSize(state.type);
	>>
	| REAL
	<<
		state.type = TYPE_REAL;
		state.size = g.s.getSize(state.type);
	>>
	| BYTE
	<<
		state.type = TYPE_BYTE;
		state.size = g.s.getSize(state.type);
	>>
	;


statement[parseState & state]
	:
	varDecl[state]
	| typeDecl[state]
	| paramDecl[state]
	| endStatement[state]
	| exitStatement[state]
	| gotoStatement[state]
	| ifStatement[state]
	| forStatement[state]
	| whileStatement[state]
	| ioStatement[state]
	| runStatement[state]
	| assignmentStatement[state]
	;

runStatement[parseState & state]
	:
	<<
		int index, paramCount = 0;
		int fromIndex[24], toIndex[24];
	>>
	CALL id:IDENT
	<<
		index = g.s.getIndex(id->getText(), 0);
	
		if (index == -1)
		{
			showerror("unknown procedure");
			exit(1);
		}
	>>
	LPAREN { expression[state]
	<<
		fromIndex[paramCount] = state.index;
		toIndex[paramCount] = g.s.getParam(g.s.getScope(index), paramCount + 1);
		if (g.s.getType(fromIndex[paramCount]) != state.type)
		{
			printf("ERROR: Parameter mismatch in position %d\n", paramCount + 1);
			exit(1);
		}
		paramCount++;
	>>
	(COMMA expression[state]
	<<
		fromIndex[paramCount] = state.index;
		toIndex[paramCount] = g.s.getParam(g.s.getScope(index), paramCount + 1);
		if (g.s.getType(fromIndex[paramCount]) != state.type)
		{
			printf("ERROR: Parameter mismatch in position %d\n", paramCount + 1);
			exit(1);
		}
		paramCount++;
	>>
	)* } RPAREN
	<<
		// copy into procedure parameters
		for (int i = 0; i < paramCount; i++)
		{
			g.gen("cp", fromIndex[i], 0, toIndex[i]);
		}
		
		g.gen("call", index);
		
		// copy back parameters
		for (int i = 0; i < paramCount; i++)
		{
			g.gen("cp", toIndex[i], 0, fromIndex[i]);
		}
	>>
	;

statementList[parseState & state]
	: { i:INTLIT
	<<
		int index;

		if ((index = g.s.getIndex((char *)i->getText(), state.scope)) != -1)
		{
			// Note: we are now allowing redefinition of line numbers in a procedure.
			// Also, if there is a forward reference on a line number (i.e. GOTO 60 \ [...] \ 60 PRINT)
			// then it will be resolved when line 60 is encountered further down.
//			printf("ERROR: line number %s already used in this procedure\n", (char *)i->getText());
//			exit(1);
		}

		index = g.s.store(i->getText(), TYPE_LABEL, state.scope);

		{
			char str[12];
			
			sprintf(str, "%d", g.getNextQuad());
			g.s.setValue(index, str);
		}
	>>
	} { statement[state] } { COMMENT } ( CR | BSLASH )
	( statementList[state] )*
	;


gotoStatement[parseState & state]
	:
	<<
		int index;
	>>
	GOTO i:INTLIT
	<<
		index = g.s.store(i->getText(), TYPE_LABEL, state.scope);

		g.gen("gt", 0, 0, index);
	>>
	;


assignmentStatement[parseState & state]
	:
	<<
		int vartype, varindex;
	>>
	{ LET } variableReference[state]
	<<
		vartype = state.type;
		varindex = state.index;
	>>
	BECOMES
	expression[state]
	<<
		if (validAssignment(vartype, state.type))
		{
			g.gen("cp", state.index, 0, varindex);
		
			if (vartype == TYPE_STRING && state.type == TYPE_STRING && g.s.getSize(varindex) == 0)
			{
				// when assinging a string to a string, only update the size if the assigned variable has a length of 0
				g.s.setSize(varindex, g.s.getSize(state.index));
			}
		}
		else
		{
			printf("ERROR[%d]: type mismatch (%s = %s)\n", vartype, g.s.getTypeName(vartype), g.s.getTypeName(state.type));
			exit(1);
		}
	>>
	;


exitStatement[parseState & state]
	: EXIT WHEN expression[state]
	;


forStatement[parseState & state]
	:
	<<
		int vartype, varindex;
		int fromtype, fromindex;
		int totype, toindex;
		int steptype, stepindex = -1, stepnegated = 0;
		int nexttype, nextindex;
		int loopTop, loopTopIndex;
		int loc;
	>>
	FOR variableReference[state]
	<<
		vartype = state.type;
		varindex = state.index;

		if (vartype != TYPE_INTEGER && vartype != TYPE_REAL)
		{
			printf("ERROR: FOR loop variable must be INTEGER or REAL\n");
			exit(1);
		}
	>>
	BECOMES expression[state]
	<<
		fromtype = state.type;
		fromindex = state.index;

		if (fromtype != TYPE_BYTE && fromtype != TYPE_INTEGER && fromtype != TYPE_REAL)
		{
			printf("ERROR: FOR loop FROM must be BYTE, INTEGER or REAL\n");
			exit(1);
		}

		if (validAssignment(vartype, fromtype))
		{
			g.gen("cp", fromindex, 0, varindex);
		}
		else
		{
			printf("ERROR[%d]: type mismatch (%s = %s)\n", vartype, g.s.getTypeName(vartype), g.s.getTypeName(state.type));
			exit(1);
		}
	>>
	TO expression[state]
	<<
		totype = state.type;
		toindex = state.index;

		if (totype != TYPE_BYTE && totype != TYPE_INTEGER && totype != TYPE_REAL)
		{
			printf("ERROR: FOR loop TO must be BYTE, INTEGER or REAL\n");
			exit(1);
		}
	>>
	{ STEP expression[state]
	<<
		steptype = state.type;
		stepindex = state.index;
		stepnegated = state.negate;
		state.negate = 0;
		
		if (steptype != TYPE_BYTE && steptype != TYPE_INTEGER && steptype != TYPE_REAL)
		{
			printf("ERROR: FOR loop STEP must be BYTE, INTEGER or REAL\n");
			exit(1);
		}
	>>
	}
	<<
		/* If stepindex = -1 then there was no STEP specified.. set one */
		if (stepindex == -1)
		{
			steptype = TYPE_INTEGER;
			stepindex = g.s.mkTemp(steptype, state.scope);
			g.s.setValue(stepindex, "1");
		}
	>>
	<<
		char t[12];
		
		loopTop = g.s.mkTemp(TYPE_LABEL, state.scope);
		loopTopIndex = g.getNextQuad();
		sprintf(t, "%d", loopTopIndex);
		g.s.setValue(loopTop, t);

		/* Here we test the loop constraint */
		if (stepnegated == 1)
		{
			g.gen("gt<", varindex, toindex, -1);
		}
		else
		{
			g.gen("gt>", varindex, toindex, -1);
		}

	>>
	statementList[state] NEXT variableReference[state]
	<<
		nexttype = state.type;
		nextindex = state.index;

		if (nextindex != varindex)
		{
			printf("ERROR: FOR loop variable must match NEXT variable\n");
			exit(1);
		}
		
		/* Apply STEP value and loop to top of loop */
		g.gen("+", varindex, stepindex, varindex);
		g.gen("gt", 0, 0, loopTop);
		loc = g.s.mkTemp(TYPE_LABEL, state.scope);
		sprintf(t, "%d", g.getNextQuad());
		g.s.setValue(loc, t);
		g.backpatch(loopTopIndex, loc);
	>>
	;


ifStatement[parseState & state]
	: IF ifPart[state]
	;


ifPart[parseState & state]
	: expression[state]
	<<
		int ifTrue, ifFalse, thenLoc, temp;
		
		if (state.type != TYPE_BOOLEAN)
		{
			showerror("IF statement requires BOOLEAN expression");
			exit(1);
		}
		
		temp = g.s.mkTemp(TYPE_BOOLEAN, state.scope);
		g.s.setValue(temp, "1");
		ifFalse = g.getNextQuad();
		g.gen("gt!=", state.index, temp, -1);
	>>
	THEN (i:INTLIT | statementList[state])
	<<
		thenLoc = g.getNextQuad();
		g.gen("gt", 0, 0, -1);
	>>
	{ ELSE
	<<
		char t[24];
		int loc;
			
		loc = g.s.mkTemp(TYPE_LABEL, state.scope);

		g.backpatch(ifFalse, loc);
		sprintf(t, "%d", g.getNextQuad());
		g.s.setValue(loc, t);

		ifFalse = -1;
	>>
	statementList[state]
	} ENDIF
	<<
		char t[24];
		int loc;
			
		if (ifFalse != -1)
		{
			loc = g.s.mkTemp(TYPE_LABEL, state.scope);

			g.backpatch(ifFalse, loc);
			sprintf(t, "%d", g.getNextQuad());
			g.s.setValue(loc, t);
		}
		
		loc = g.s.mkTemp(TYPE_LABEL, state.scope);
		
		g.backpatch(thenLoc, loc);
		sprintf(t, "%d", g.getNextQuad());
		g.s.setValue(loc, t);
	>>
	;


whileStatement[parseState & state]
	:
	<<
		parseState whileState;
		int start;
		int loc;
	>>
	WHILE
	<<
		char t[24];
		
		start = g.s.mkTemp(TYPE_LABEL, state.scope);
		sprintf(t, "%d", g.getNextQuad());
		g.s.setValue(start, t);
	>>
	expression[whileState]
	<<
		int ifTrue, ifFalse, thenLoc, temp;
		
		if (whileState.type != TYPE_BOOLEAN)
		{
			fprintf(stderr, "ERROR: WHILE requires BOOLEAN expression\n");
			exit(1);
		}
		
		temp = g.s.mkTemp(TYPE_BOOLEAN, whileState.scope);
		g.s.setValue(temp, "1");
		ifFalse = g.getNextQuad();
		g.gen("gt!=", whileState.index, temp, -1);
	>>
	DO statementList[state]
	<<
		temp = g.s.mkTemp(TYPE_BOOLEAN, state.scope);
		g.s.setValue(temp, "1");
		g.gen("gt", start);
	>>
	ENDWHILE
	<<
		loc = g.s.mkTemp(TYPE_LABEL, state.scope);

		g.backpatch(ifFalse, loc);
		sprintf(t, "%d", g.getNextQuad());
		g.s.setValue(loc, t);
	>>
	;


endStatement[parseState & state]
	:
	<<
		int newTemp;
	>>
	END
	<<
		newTemp = g.s.mkTemp(TYPE_INTEGER, state.scope);

		g.s.setValue(newTemp, "0");
//		g.gen("exit", newTemp);
		g.gen("ret");
	>>
	;

variableReference[parseState & state]
	:
	ids:IDENTSTR
	<<
		state.index = g.s.getIndex((char *)ids->getText(), state.scope);
		if (state.index == -1)
		{
			state.type = TYPE_STRING;
			state.index = g.s.store((char *)ids->getText(), state.type, state.scope);
		}
	>>
	|
	id:IDENT
	<<
		state.index = g.s.getIndex((char *)id->getText(), state.scope);
		state.type = g.s.getType(state.index);
		if (state.index == -1)
		{
			/* Ragin' Basic assumes undeclared variables are of type REAL. */
			state.type = TYPE_REAL;
			state.index = g.s.store((char *)id->getText(), state.type, state.scope);
//			printf("ERROR: undeclared identifier '%s'\n", (char *)id->getText());
//			exit(1);
		}
	>>
	{ DOLLAR
	|
	{ LPAREN expression[state] RPAREN } ( declaredVariable[state] )* }
	;


declaredVariable[parseState & state]
	: DOT IDENT { LPAREN expression[state] RPAREN }
	;


modFunction[parseState & state]
  : MOD LPAREN expression[state] COMMA expression[state] RPAREN
  ;

notFunction[parseState & state]
  : NOT LPAREN expression[state] RPAREN
  ;


trueFunction[parseState & state]
	:
	TRUE_
	<<
		state.type = TYPE_BOOLEAN;
		state.index = g.s.store("TRUE", state.type, state.scope);
		g.s.setValue(state.index, "1");
	>>
	;

falseFunction[parseState & state]
	:
	FALSE_
	<<
		state.type = TYPE_BOOLEAN;
		state.index = g.s.store("FALSE", state.type, state.scope);
		g.s.setValue(state.index, "0");
	>>
	;

ioStatement[parseState & state]
	:
	<<
		// 0 = nothing, 1 = CR
		int lastPrint = 1;			// assume CR by default
		char *path = "1";			// assume stdout as path
	>>
	PRINT { POUND i:expression[state]
	<<
		path = i->getText();
	>>
	(COMMA | USING)  | USING }
	(
		expression[state]
		<<
			int i, index, pindex;
			
			lastPrint = 1;
			index = g.s.mkTemp(TYPE_BYTE, state.scope);
			g.s.setValue(index, path);
			
			switch (state.type)
			{
				case TYPE_BYTE:
					pindex = g.s.store("printbyte.p0", TYPE_BYTE, -1);
					g.s.setGlobal(pindex, 1);
					g.s.setParam(pindex, 1);
					g.gen("cp", index, 0, pindex);
					pindex = g.s.store("printbyte.p1", state.type, -1);
					g.s.setGlobal(pindex, 1);
					g.s.setParam(pindex, 2);
					g.gen("cp", state.index, 0, pindex);
					g.gen("call", i = g.s.store("printbyte", TYPE_PROCEDURE, -1));
					break;
					
				case TYPE_BOOLEAN:
					pindex = g.s.store("printboolean.p0", TYPE_BYTE, -1);
					g.s.setGlobal(pindex, 1);
					g.s.setParam(pindex, 1);
					g.gen("cp", index, 0, pindex);
					pindex = g.s.store("printboolean.p1", state.type, -1);
					g.s.setGlobal(pindex, 1);
					g.s.setParam(pindex, 2);
					g.gen("cp", state.index, 0, pindex);
					g.s.setGlobal(pindex, 1);
					g.gen("call", i = g.s.store("printboolean", TYPE_PROCEDURE, -1));
					break;
					
				case TYPE_INTEGER:
					pindex = g.s.store("printinteger.p0", TYPE_BYTE, -1);
					g.s.setGlobal(pindex, 1);
					g.s.setParam(pindex, 1);
					g.gen("cp", index, 0, pindex);
					pindex = g.s.store("printinteger.p1", state.type, -1);
					g.s.setGlobal(pindex, 1);
					g.s.setParam(pindex, 2);
					g.gen("cp", state.index, 0, pindex);
					g.s.setGlobal(pindex, 1);
					g.gen("call", i = g.s.store("printinteger", TYPE_PROCEDURE, -1));
					break;
					
				case TYPE_REAL:
					pindex = g.s.store("printreal.p0", TYPE_BYTE, -1);
					g.s.setGlobal(pindex, 1);
					g.s.setParam(pindex, 1);
					g.gen("cp", index, 0, pindex);
					pindex = g.s.store("printreal.p1", state.type, -1);
					g.s.setGlobal(pindex, 1);
					g.gen("cp", state.index, 0, pindex);
					g.s.setGlobal(pindex, 1);
					g.s.setParam(pindex, 2);
					g.gen("call", i = g.s.store("printreal", TYPE_PROCEDURE, -1));
					break;
					
				case TYPE_STRING:
					pindex = g.s.store("printstring.p0", TYPE_BYTE, -1);
					g.s.setGlobal(pindex, 1);
					g.s.setParam(pindex, 1);
					g.gen("cp", index, 0, pindex);
					pindex = g.s.store("printstring.p1", state.type, -1);
					if (g.s.getSize(state.index) > g.s.getSize(pindex))
					{
						g.s.setSize(pindex, g.s.getSize(state.index));
					}
					g.s.setGlobal(pindex, 1);
					g.s.setParam(pindex, 2);
					g.gen("cp", state.index, 0, pindex);
					g.s.setGlobal(pindex, 1);
					g.gen("call", i = g.s.store("printstring", TYPE_PROCEDURE, -1));
					break;
			}
			g.s.setValue(i, "-1");
		>>
		{
			SEMI
			<<
				lastPrint = 0;
			>>
			| COMMA
		}
	)*
	<<
		if (lastPrint == 1)
		{
			int i, newTemp = g.s.mkTemp(TYPE_BYTE, state.scope);
			g.s.setValue(newTemp, path);
			g.gen("cp", newTemp, 0, i = g.s.store("printcr.p0", TYPE_BYTE, -1));
			g.s.setGlobal(i, 1);
			g.s.setParam(i, 1);
			g.gen("call", i = g.s.store("printcr", TYPE_PROCEDURE, -1));
			g.s.setValue(i, "-1");
		}
	>>
	| INPUT { s:STRING_LITERAL
	<<
		int i, index, pindex;
			
		index = g.s.mkTemp(TYPE_BYTE, state.scope);
		g.s.setValue(index, "1");
		pindex = g.s.store("printstring.p0", TYPE_BYTE, -1);
		g.s.setGlobal(pindex, 1);
		g.s.setParam(pindex, 1);
		g.gen("cp", index, 0, pindex);
		pindex = g.s.store("printstring.p1", TYPE_STRING, -1);
		if (g.s.getSize(state.index) > g.s.getSize(pindex))
		{
			g.s.setSize(pindex, g.s.getSize(state.index));
		}
		g.s.setGlobal(pindex, 1);
		g.s.setParam(pindex, 2);
		index = g.s.mkTemp(TYPE_STRING, state.scope);
		g.s.setValue(index, s->getText());
		g.gen("cp", index, 0, pindex);
		g.s.setGlobal(pindex, 1);
		g.gen("call", i = g.s.store("printstring", TYPE_PROCEDURE, -1));
	>>
	 (COMMA | SEMI) }
	variableReference[state]
	<<
		int i, index, pindex, iindex, p0index, p1index, p2index;
		char len[12];
			
		index = g.s.mkTemp(TYPE_BYTE, state.scope);
		g.s.setValue(index, "0");
			
		switch (state.type)
		{
			case TYPE_BYTE:
				p0index = g.s.store("inputbyte.p0", TYPE_BYTE, -1);
				g.s.setGlobal(p0index, 1);
				g.s.setParam(p0index, 1);
				g.gen("cp", index, 0, p0index);
				p1index = g.s.store("inputbyte.p1", state.type, -1);
				g.s.setGlobal(p1index, 1);
				g.s.setParam(p1index, 2);
				g.gen("call", i = g.s.store("inputbyte", TYPE_PROCEDURE, -1));
				g.gen("cp", p1index, 0, state.index);
				break;
					
			case TYPE_BOOLEAN:
				p0index = g.s.store("inputboolean.p0", TYPE_BYTE, -1);
				g.s.setGlobal(p0index, 1);
				g.s.setParam(p0index, 1);
				g.gen("cp", index, 0, p0index);
				p1index = g.s.store("inputboolean.p1", state.type, -1);
				g.s.setGlobal(p1index, 1);
				g.s.setParam(p1index, 2);
				g.gen("call", i = g.s.store("inputboolean", TYPE_PROCEDURE, -1));
				g.gen("cp", p1index, 0, state.index);
				break;
					
			case TYPE_INTEGER:
				p0index = g.s.store("inputinteger.p0", TYPE_BYTE, -1);
				g.s.setGlobal(p0index, 1);
				g.s.setParam(p0index, 1);
				g.gen("cp", index, 0, p0index);
				p1index = g.s.store("inputinteger.p1", state.type, -1);
				g.s.setGlobal(p1index, 1);
				g.s.setParam(p1index, 2);
				g.gen("call", i = g.s.store("inputinteger", TYPE_PROCEDURE, -1));
				g.gen("cp", p1index, 0, state.index);
				break;
					
			case TYPE_REAL:
				p0index = g.s.store("inputreal.p0", TYPE_BYTE, -1);
				g.s.setGlobal(p0index, 1);
				g.s.setParam(p0index, 1);
				g.gen("cp", index, 0, p0index);
				p1index = g.s.store("inputreal.p1", state.type, -1);
				g.s.setGlobal(p1index, 1);
				g.s.setParam(p1index, 2);
				g.gen("call", i = g.s.store("inputreal", TYPE_PROCEDURE, -1));
				g.gen("cp", p1index, 0, state.index);
				break;
					
			case TYPE_STRING:
				pindex = g.s.store("inputstring.p0", TYPE_BYTE, -1);
				g.s.setGlobal(pindex, 1);
				g.s.setParam(pindex, 1);
				g.gen("cp", index, 0, pindex);
				iindex = g.s.store("inputstring.p1", state.type, -1);
				if (g.s.getSize(state.index) > g.s.getSize(iindex))
				{
					g.s.setSize(iindex, g.s.getSize(state.index));
				}
				sprintf(len, "%d", g.s.getSize(iindex));
				g.s.setGlobal(iindex, 1);
//				g.gen("cp", state.index, 0, pindex);
//				g.s.setGlobal(pindex, 1);
				index = g.s.mkTemp(TYPE_INTEGER, state.scope);
				g.s.setValue(index, len);
				pindex = g.s.store("inputstring.p2", TYPE_INTEGER, -1);
				g.s.setGlobal(pindex, 1);
				g.s.setParam(pindex, 2);
				g.gen("cp", index, 0, pindex);
				g.gen("call", i = g.s.store("inputstring", TYPE_PROCEDURE, -1));
				g.gen("cp", iindex, 0, state.index);
				break;
		}
		g.s.setValue(i, "-1");
	>>
	(COMMA variableReference[state])*
	;


primitiveElement[parseState & state]
  :
  trueFunction[state]
  | falseFunction[state]
  | modFunction[state]
  | notFunction[state]
  | variableReference[state]
  | constantValue[state]
  | LPAREN expression[state] RPAREN
  ;


signExpression[parseState & state]
	:
	<<
		int neg = 0, pos = 0;
	>>
	(
	op:ADD_OP
	<<
		// count up number of +'s and -'s
		if (*op->getText() == '-')
		{
			neg++;
		}
		else
		{
			pos++;
		}
	>>
	)* primitiveElement[state]
	<<
		// check types (allow real/integer only to have +/- in front)
		if ((pos > 0 || neg > 0) && !isNumber(state.type) )
		{
			showerror("sign token is only applicable to BYTE, INTEGER and REAL types");
			exit(1);
		}

		// If we have an odd number of negatives, negate element
		if (neg % 2 == 1)
		{
			g.gen("neg", state.index);
			state.negate = 1;
		}
	>>
	;


multiplyingExpression[parseState & stateL]
	:
  	<<
		parseState stateR;
	>>
	signExpression[stateL]
	(op:MULT_OP signExpression[stateR]
	<<
		// check types (allow REAL/INTEGER/BYTE only)
		if ( !(isNumber(stateL.type) && isNumber(stateR.type)) )
		{
			showerror("type must be REAL, INTEGER or BYTE for multiplication or division");
			exit(1);
		}

		// newTemp must be larger of the two types
		int newTemp = g.s.mkTemp(g.s.largerType(g.s.largerType(stateL.type, stateR.type), TYPE_INTEGER), stateR.scope);
		g.gen((char *)op->getText(), stateL.index, g.s.getIndex(g.s.getName(stateR.index), stateR.scope), newTemp);

		stateL.index = newTemp;
	>>
	)*
	;

addingExpression[parseState & stateL]
	:
	<<
		parseState stateR;
	>>
	multiplyingExpression[stateL]
	(op:ADD_OP multiplyingExpression[stateR]
	<<
		// check types (allow REAL/INTEGER/BYTE only)
		if ( !(isNumber(stateL.type) && isNumber(stateR.type) || (stateL.type == TYPE_STRING && stateR.type == TYPE_STRING)) )
		{
			showerror("type must be STRING, REAL, INTEGER or BYTE for addition");
			exit(1);
		}

		// newTemp must be larger of the two types
		int newTemp = g.s.mkTemp(g.s.largerType(g.s.largerType(stateL.type, stateR.type), TYPE_INTEGER), stateR.scope);
		g.gen((char *)op->getText(), stateL.index, g.s.getIndex(g.s.getName(stateR.index), stateR.scope), newTemp);
		
		if (stateL.type == TYPE_STRING)
		{
			if (strcmp(op->getText(), "-") == 0)
			{
				showerror("strings cannot be subtracted");
				exit(1);
			}
			
			// Add two strings together.  Get total size from size of src1 and src2 strings
			int sum = g.s.getSize(stateL.index) + g.s.getSize(stateR.index);
			if (g.s.getSize(newTemp) < sum)
			{
printf("indexL = %d, indexR = %d\n", g.s.getSize(stateL.index), g.s.getSize(stateR.index));
printf("%d + %d = %d\n", g.s.getSize(stateL.index), g.s.getSize(stateR.index), sum);
				g.s.setSize(newTemp, sum);
			}
		}

		stateL.index = newTemp;
	>>
	)*
	;

relationalExpression[parseState & stateL]
	:
	<<
		parseState stateR;
	>>
	addingExpression[stateL]
	(op:RELATIONAL_OP addingExpression[stateR]
	<<
		// newTemp must be a BOOLEAN type
		int newTemp = g.s.mkTemp(TYPE_BOOLEAN, stateR.scope);
		g.gen((char *)op->getText(), stateL.index, g.s.getIndex(g.s.getName(stateR.index), stateR.scope), newTemp);

		stateL.index = newTemp;
		stateL.type = TYPE_BOOLEAN;
	>>
	)*
	;

expression[parseState & stateL]
	:
	<<
		parseState stateR;
	>>
	relationalExpression[stateL]
	(op:BOOLEAN_OP relationalExpression[stateR]
	<<
		// Only allow boolean operations if types are BOOLEAN
		if (g.s.getType(stateL.index) == TYPE_BOOLEAN && g.s.getType(stateR.index) == TYPE_BOOLEAN)
		{
			int newTemp = g.s.mkTemp(TYPE_BOOLEAN, stateR.scope);
			
			g.gen((char *)op->getText(), stateL.index, g.s.getIndex(g.s.getName(stateR.index), stateR.scope), newTemp);

			state.index = newTemp;
			state.type = TYPE_BOOLEAN;
		}
		else
		{
			fprintf(stderr, "BOOLEAN type required\n");
			exit(1);
		}
	>>
	)*
	;

}
