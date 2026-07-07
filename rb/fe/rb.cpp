/*
 * A n t l r  T r a n s l a t i o n  H e a d e r
 *
 * Terence Parr, Will Cohen, and Hank Dietz: 1989-2001
 * Purdue University Electrical Engineering
 * With AHPCRC, University of Minnesota
 * ANTLR Version 1.33MR33
 *
 *   antlr -CC -mrhoist off ../../../rb/fe/rb.g
 *
 */

#define ANTLR_VERSION	13333
#include "pcctscfg.h"
#include "pccts_stdio.h"
#include "tokens.h"

// any definitions that you need in the generated files
#include "../../common/codegen.h"
#include "AParser.h"
#include "raginBasicParser.h"
#include "DLexerBase.h"
#include "ATokPtr.h"

/* MR23 In order to remove calls to PURIFY use the antlr -nopurify option */

#ifndef PCCTS_PURIFY
#define PCCTS_PURIFY(r,s) memset((char *) &(r),'\0',(s));
#endif


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

void
raginBasicParser::program(void)
{
  zzRULE;
  {
    int zzcnt=1;
    do {
      procedure();
    } while ( (LA(1)==PROCEDURE) );
  }
  return;
fail:
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd1, 0x1);
}

void
raginBasicParser::procedure(void)
{
  zzRULE;
  ANTLRTokenPtr id=NULL;
  
  int temp;
  zzmatch(PROCEDURE); consume();
  zzmatch(IDENT);
  id = (ANTLRTokenPtr)LT(1);

  
  temp = g.s.store(id->getText(), TYPE_PROCEDURE, -1);
  
		state.params = 0;
  g.s.setScope(temp, temp);
  state.scope = temp;
  {
    char str[12];
    
			sprintf(str, "%d", g.getNextQuad());
    g.s.setValue(temp, str);
  }
 consume();
  zzmatch(CR); consume();
  procedureBody( state );
  
  g.gen("ret");
  return;
fail:
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd1, 0x2);
}

void
raginBasicParser::procedureBody(parseState & state)
{
  zzRULE;
  statementList( state );
  return;
fail:
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd1, 0x4);
}

void
raginBasicParser::varDecl(parseState & state)
{
  zzRULE;
  
  int start = g.s.count();
  
		state.idCount = 0;
  zzmatch(DIM); consume();
  identList( state );
  zzmatch(COLON); consume();
  typeName( state );
  
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
  {
    while ( (LA(1)==SEMI) ) {
      zzmatch(SEMI); consume();
      identList( state );
      zzmatch(COLON); consume();
      typeName( state );
      
      for (int j = start; j < start + state.idCount; j++)
      {
        g.s.setType(j, state.type);
        // Check if the STRING has been declared a specific size
        if (state.type == TYPE_STRING && state.size != g.s.getSize(j))
        {
          g.s.setSize(j, state.size);
        }
      }
    }
  }
  return;
fail:
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd1, 0x8);
}

void
raginBasicParser::identList(parseState & state)
{
  zzRULE;
  ANTLRTokenPtr id1=NULL, id2=NULL;
  zzmatch(IDENT);
  id1 = (ANTLRTokenPtr)LT(1);

  
  int i;
  
		if ((i = g.s.getIndex((char *)id1->getText(), state.scope)) != -1)
  {
    printf("ERROR: redefined symbol %s\n", (char *)id1->getText());
    exit(1);
  }
  
		g.s.store(id1->getText(), TYPE_UNKNOWN, state.scope);
  state.idCount++;
 consume();
  {
    if ( (LA(1)==LPAREN) ) {
      zzmatch(LPAREN); consume();
      zzmatch(INTLIT); consume();
      zzmatch(RPAREN); consume();
    }
    else {
      if ( (setwd1[LA(1)]&0x10) ) {
      }
      else {FAIL(1,err1,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  {
    while ( (LA(1)==COMMA) ) {
      zzmatch(COMMA); consume();
      zzmatch(IDENT);
      id2 = (ANTLRTokenPtr)LT(1);

      
      int i;
      
		if ((i = g.s.getIndex((char *)id2->getText(), state.scope)) != -1)
      {
        printf("ERROR: redefined symbol %s\n", (char *)id2->getText());
        exit(1);
      }
      
		g.s.store(id2->getText(), TYPE_UNKNOWN, state.scope);
      state.idCount++;
 consume();
    }
  }
  return;
fail:
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd1, 0x20);
}

void
raginBasicParser::constantValue(parseState & state)
{
  zzRULE;
  ANTLRTokenPtr i=NULL, r=NULL, s=NULL;
  if ( (LA(1)==INTLIT) ) {
    zzmatch(INTLIT);
    i = (ANTLRTokenPtr)LT(1);

    
    state.type = TYPE_INTEGER;
    state.index = g.s.mkTemp(state.type, state.scope);
    g.s.setValue(state.index, (char *)i->getText());
 consume();
  }
  else {
    if ( (LA(1)==REALLIT) ) {
      zzmatch(REALLIT);
      r = (ANTLRTokenPtr)LT(1);

      
      state.type = TYPE_REAL;
      state.index = g.s.mkTemp(state.type, state.scope);
      g.s.setValue(state.index, (char *)r->getText());
 consume();
    }
    else {
      if ( (setwd1[LA(1)]&0x40) ) {
        zzsetmatch(STRING_LITERAL_set, STRING_LITERAL_errset);
        s = (ANTLRTokenPtr)LT(1);

        
        state.type = TYPE_STRING;
        state.index = g.s.mkTemp(state.type, state.scope);
        state.size = strlen(s->getText());
        g.s.setValue(state.index, (char *)s->getText());
        g.s.setSize(state.index, state.size);
 consume();
      }
      else {FAIL(1,err4,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  return;
fail:
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd1, 0x80);
}

void
raginBasicParser::paramDecl(parseState & state)
{
  zzRULE;
  
  int start = g.s.count();
  
		state.idCount = 0;
  zzmatch(PARAM); consume();
  identList( state );
  zzmatch(COLON); consume();
  typeName( state );
  
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
  return;
fail:
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd2, 0x1);
}

void
raginBasicParser::typeDecl(parseState & state)
{
  zzRULE;
  zzmatch(TYPE); consume();
  zzmatch(IDENT); consume();
  zzmatch(EQUALS); consume();
  recordDecl( state );
  return;
fail:
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd2, 0x2);
}

void
raginBasicParser::integerConstant(void)
{
  zzRULE;
  if ( (LA(1)==INTLIT) ) {
    zzmatch(INTLIT); consume();
  }
  else {
    if ( (LA(1)==IDENT) ) {
      zzmatch(IDENT); consume();
    }
    else {FAIL(1,err5,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
  }
  return;
fail:
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd2, 0x4);
}

void
raginBasicParser::recordDecl(parseState & state)
{
  zzRULE;
  
  state.idCount = 0;
  identList( state );
  zzmatch(COLON); consume();
  typeName( state );
  {
    while ( (LA(1)==SEMI) ) {
      zzmatch(SEMI); consume();
      identList( state );
      zzmatch(COLON); consume();
      typeName( state );
    }
  }
  return;
fail:
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd2, 0x8);
}

void
raginBasicParser::typeName(parseState & state)
{
  zzRULE;
  ANTLRTokenPtr i=NULL;
  if ( (LA(1)==STR) ) {
    zzmatch(STR);
    
    state.type = TYPE_STRING;
    state.size = g.s.typelen[state.type];
 consume();
    {
      if ( (LA(1)==LBRACKET) ) {
        zzmatch(LBRACKET); consume();
        zzmatch(INTLIT);
        i = (ANTLRTokenPtr)LT(1);
 consume();
        zzmatch(RBRACKET);
        
        state.size = atoi(i->getText());
        printf("state.size for this string is %d\n", state.size);
 consume();
      }
      else {
        if ( (setwd2[LA(1)]&0x10) ) {
        }
        else {FAIL(1,err6,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
      }
    }
  }
  else {
    if ( (LA(1)==INTEGER) ) {
      zzmatch(INTEGER);
      
      state.type = TYPE_INTEGER;
      state.size = g.s.getSize(state.type);
 consume();
    }
    else {
      if ( (LA(1)==BOOLEAN) ) {
        zzmatch(BOOLEAN);
        
        state.type = TYPE_BOOLEAN;
        state.size = g.s.getSize(state.type);
 consume();
      }
      else {
        if ( (LA(1)==REAL) ) {
          zzmatch(REAL);
          
          state.type = TYPE_REAL;
          state.size = g.s.getSize(state.type);
 consume();
        }
        else {
          if ( (LA(1)==BYTE) ) {
            zzmatch(BYTE);
            
            state.type = TYPE_BYTE;
            state.size = g.s.getSize(state.type);
 consume();
          }
          else {FAIL(1,err7,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
        }
      }
    }
  }
  return;
fail:
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd2, 0x20);
}

void
raginBasicParser::statement(parseState & state)
{
  zzRULE;
  if ( (LA(1)==DIM) ) {
    varDecl( state );
  }
  else {
    if ( (LA(1)==TYPE) ) {
      typeDecl( state );
    }
    else {
      if ( (LA(1)==PARAM) ) {
        paramDecl( state );
      }
      else {
        if ( (LA(1)==END) ) {
          endStatement( state );
        }
        else {
          if ( (LA(1)==EXIT) ) {
            exitStatement( state );
          }
          else {
            if ( (LA(1)==GOTO) ) {
              gotoStatement( state );
            }
            else {
              if ( (LA(1)==IF) ) {
                ifStatement( state );
              }
              else {
                if ( (LA(1)==FOR) ) {
                  forStatement( state );
                }
                else {
                  if ( (LA(1)==WHILE) ) {
                    whileStatement( state );
                  }
                  else {
                    if ( (setwd2[LA(1)]&0x40) ) {
                      ioStatement( state );
                    }
                    else {
                      if ( (LA(1)==CALL) ) {
                        runStatement( state );
                      }
                      else {
                        if ( (setwd2[LA(1)]&0x80) ) {
                          assignmentStatement( state );
                        }
                        else {FAIL(1,err8,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return;
fail:
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd3, 0x1);
}

void
raginBasicParser::runStatement(parseState & state)
{
  zzRULE;
  ANTLRTokenPtr id=NULL;
  
  int index, paramCount = 0;
  int fromIndex[24], toIndex[24];
  zzmatch(CALL); consume();
  zzmatch(IDENT);
  id = (ANTLRTokenPtr)LT(1);

  
  index = g.s.getIndex(id->getText(), 0);
  
		if (index == -1)
  {
    showerror("unknown procedure");
    exit(1);
  }
 consume();
  zzmatch(LPAREN); consume();
  {
    if ( (setwd3[LA(1)]&0x2) ) {
      expression( state );
      
      fromIndex[paramCount] = state.index;
      toIndex[paramCount] = g.s.getParam(g.s.getScope(index), paramCount + 1);
      if (g.s.getType(fromIndex[paramCount]) != state.type)
      {
        printf("ERROR: Parameter mismatch in position %d\n", paramCount + 1);
        exit(1);
      }
      paramCount++;
      {
        while ( (LA(1)==COMMA) ) {
          zzmatch(COMMA); consume();
          expression( state );
          
          fromIndex[paramCount] = state.index;
          toIndex[paramCount] = g.s.getParam(g.s.getScope(index), paramCount + 1);
          if (g.s.getType(fromIndex[paramCount]) != state.type)
          {
            printf("ERROR: Parameter mismatch in position %d\n", paramCount + 1);
            exit(1);
          }
          paramCount++;
        }
      }
    }
    else {
      if ( (LA(1)==RPAREN) ) {
      }
      else {FAIL(1,err9,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  zzmatch(RPAREN);
  
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
 consume();
  return;
fail:
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd3, 0x4);
}

void
raginBasicParser::statementList(parseState & state)
{
  zzRULE;
  ANTLRTokenPtr i=NULL;
  {
    if ( (LA(1)==INTLIT) ) {
      zzmatch(INTLIT);
      i = (ANTLRTokenPtr)LT(1);

      
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
 consume();
    }
    else {
      if ( (setwd3[LA(1)]&0x8) ) {
      }
      else {FAIL(1,err10,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  {
    if ( (setwd3[LA(1)]&0x10) ) {
      statement( state );
    }
    else {
      if ( (setwd3[LA(1)]&0x20) ) {
      }
      else {FAIL(1,err11,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  {
    if ( (setwd3[LA(1)]&0x40) ) {
      zzsetmatch(COMMENT_set, COMMENT_errset); consume();
    }
    else {
      if ( (setwd3[LA(1)]&0x80) ) {
      }
      else {FAIL(1,err14,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  {
    if ( (LA(1)==CR) ) {
      zzmatch(CR); consume();
    }
    else {
      if ( (LA(1)==BSLASH) ) {
        zzmatch(BSLASH); consume();
      }
      else {FAIL(1,err15,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  {
    while ( (setwd4[LA(1)]&0x1) ) {
      statementList( state );
    }
  }
  return;
fail:
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd4, 0x2);
}

void
raginBasicParser::gotoStatement(parseState & state)
{
  zzRULE;
  ANTLRTokenPtr i=NULL;
  
  int index;
  zzmatch(GOTO); consume();
  zzmatch(INTLIT);
  i = (ANTLRTokenPtr)LT(1);

  
  index = g.s.store(i->getText(), TYPE_LABEL, state.scope);
  
		g.gen("gt", 0, 0, index);
 consume();
  return;
fail:
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd4, 0x4);
}

void
raginBasicParser::assignmentStatement(parseState & state)
{
  zzRULE;
  
  int vartype, varindex;
  {
    if ( (LA(1)==LET) ) {
      zzmatch(LET); consume();
    }
    else {
      if ( (setwd4[LA(1)]&0x8) ) {
      }
      else {FAIL(1,err16,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  variableReference( state );
  
  vartype = state.type;
  varindex = state.index;
  zzmatch(EQUALS); consume();
  expression( state );
  
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
  return;
fail:
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd4, 0x10);
}

void
raginBasicParser::exitStatement(parseState & state)
{
  zzRULE;
  zzmatch(EXIT); consume();
  zzmatch(WHEN); consume();
  expression( state );
  return;
fail:
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd4, 0x20);
}

void
raginBasicParser::forStatement(parseState & state)
{
  zzRULE;
  
  int vartype, varindex;
  int fromtype, fromindex;
  int totype, toindex;
  int steptype, stepindex = -1, stepnegated = 0;
  int nexttype, nextindex;
  int loopTop, loopTopIndex;
  int loc;
  zzmatch(FOR); consume();
  variableReference( state );
  
  vartype = state.type;
  varindex = state.index;
  
		if (vartype != TYPE_INTEGER && vartype != TYPE_REAL)
  {
    printf("ERROR: FOR loop variable must be INTEGER or REAL\n");
    exit(1);
  }
  zzmatch(EQUALS); consume();
  expression( state );
  
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
  zzmatch(TO); consume();
  expression( state );
  
  totype = state.type;
  toindex = state.index;
  
		if (totype != TYPE_BYTE && totype != TYPE_INTEGER && totype != TYPE_REAL)
  {
    printf("ERROR: FOR loop TO must be BYTE, INTEGER or REAL\n");
    exit(1);
  }
  {
    if ( (LA(1)==STEP) ) {
      zzmatch(STEP); consume();
      expression( state );
      
      steptype = state.type;
      stepindex = state.index;
      stepnegated = state.negate;
      state.negate = 0;
      
		if (steptype != TYPE_BYTE && steptype != TYPE_INTEGER && steptype != TYPE_REAL)
      {
        printf("ERROR: FOR loop STEP must be BYTE, INTEGER or REAL\n");
        exit(1);
      }
    }
    else {
      if ( (setwd4[LA(1)]&0x40) ) {
      }
      else {FAIL(1,err17,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  
  /* If stepindex = -1 then there was no STEP specified.. set one */
  if (stepindex == -1)
  {
    steptype = TYPE_INTEGER;
    stepindex = g.s.mkTemp(steptype, state.scope);
    g.s.setValue(stepindex, "1");
  }
  
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
  statementList( state );
  zzmatch(NEXT); consume();
  variableReference( state );
  
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
  return;
fail:
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd4, 0x80);
}

void
raginBasicParser::ifStatement(parseState & state)
{
  zzRULE;
  zzmatch(IF); consume();
  ifPart( state );
  return;
fail:
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd5, 0x1);
}

void
raginBasicParser::ifPart(parseState & state)
{
  zzRULE;
  ANTLRTokenPtr i=NULL;
  expression( state );
  
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
  zzmatch(THEN); consume();
  {
    if ( (LA(1)==INTLIT) ) {
      zzmatch(INTLIT);
      i = (ANTLRTokenPtr)LT(1);
 consume();
    }
    else {
      if ( (setwd5[LA(1)]&0x2) ) {
        statementList( state );
      }
      else {FAIL(1,err18,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  
  thenLoc = g.getNextQuad();
  g.gen("gt", 0, 0, -1);
  {
    if ( (LA(1)==ELSE) ) {
      zzmatch(ELSE);
      
      char t[24];
      int loc;
      
		loc = g.s.mkTemp(TYPE_LABEL, state.scope);
      
		g.backpatch(ifFalse, loc);
      sprintf(t, "%d", g.getNextQuad());
      g.s.setValue(loc, t);
      
		ifFalse = -1;
 consume();
      statementList( state );
    }
    else {
      if ( (LA(1)==ENDIF) ) {
      }
      else {FAIL(1,err19,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  zzmatch(ENDIF);
  
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
 consume();
  return;
fail:
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd5, 0x4);
}

void
raginBasicParser::whileStatement(parseState & state)
{
  zzRULE;
  
  parseState whileState;
  int start;
  int loc;
  zzmatch(WHILE);
  
  char t[24];
  
		start = g.s.mkTemp(TYPE_LABEL, state.scope);
  sprintf(t, "%d", g.getNextQuad());
  g.s.setValue(start, t);
 consume();
  expression( whileState );
  
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
  zzmatch(DO); consume();
  statementList( state );
  
  temp = g.s.mkTemp(TYPE_BOOLEAN, state.scope);
  g.s.setValue(temp, "1");
  g.gen("gt", start);
  zzmatch(ENDWHILE);
  
  loc = g.s.mkTemp(TYPE_LABEL, state.scope);
  
		g.backpatch(ifFalse, loc);
  sprintf(t, "%d", g.getNextQuad());
  g.s.setValue(loc, t);
 consume();
  return;
fail:
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd5, 0x8);
}

void
raginBasicParser::endStatement(parseState & state)
{
  zzRULE;
  
  int newTemp;
  zzmatch(END);
  
  newTemp = g.s.mkTemp(TYPE_INTEGER, state.scope);
  
		g.s.setValue(newTemp, "0");
  //		g.gen("exit", newTemp);
  g.gen("ret");
 consume();
  return;
fail:
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd5, 0x10);
}

void
raginBasicParser::variableReference(parseState & state)
{
  zzRULE;
  ANTLRTokenPtr ids=NULL, id=NULL;
  if ( (LA(1)==IDENTSTR) ) {
    zzmatch(IDENTSTR);
    ids = (ANTLRTokenPtr)LT(1);

    
    state.index = g.s.getIndex((char *)ids->getText(), state.scope);
    if (state.index == -1)
    {
      state.type = TYPE_STRING;
      state.index = g.s.store((char *)ids->getText(), state.type, state.scope);
    }
 consume();
  }
  else {
    if ( (LA(1)==IDENT) ) {
      zzmatch(IDENT);
      id = (ANTLRTokenPtr)LT(1);

      
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
 consume();
      {
        if ( (LA(1)==DOLLAR) ) {
          zzmatch(DOLLAR); consume();
        }
        else {
          if ( (setwd5[LA(1)]&0x20) ) {
            {
              if ( (LA(1)==LPAREN) ) {
                zzmatch(LPAREN); consume();
                expression( state );
                zzmatch(RPAREN); consume();
              }
              else {
                if ( (setwd5[LA(1)]&0x40) ) {
                }
                else {FAIL(1,err20,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
              }
            }
            {
              while ( (LA(1)==DOT) ) {
                declaredVariable( state );
              }
            }
          }
          else {
            if ( (setwd5[LA(1)]&0x80) ) {
            }
            else {FAIL(1,err21,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
          }
        }
      }
    }
    else {FAIL(1,err22,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
  }
  return;
fail:
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd6, 0x1);
}

void
raginBasicParser::declaredVariable(parseState & state)
{
  zzRULE;
  zzmatch(DOT); consume();
  zzmatch(IDENT); consume();
  {
    if ( (LA(1)==LPAREN) ) {
      zzmatch(LPAREN); consume();
      expression( state );
      zzmatch(RPAREN); consume();
    }
    else {
      if ( (setwd6[LA(1)]&0x2) ) {
      }
      else {FAIL(1,err23,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
  }
  return;
fail:
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd6, 0x4);
}

void
raginBasicParser::modFunction(parseState & state)
{
  zzRULE;
  zzmatch(MOD); consume();
  zzmatch(LPAREN); consume();
  expression( state );
  zzmatch(COMMA); consume();
  expression( state );
  zzmatch(RPAREN); consume();
  return;
fail:
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd6, 0x8);
}

void
raginBasicParser::notFunction(parseState & state)
{
  zzRULE;
  zzmatch(NOT); consume();
  zzmatch(LPAREN); consume();
  expression( state );
  zzmatch(RPAREN); consume();
  return;
fail:
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd6, 0x10);
}

void
raginBasicParser::trueFunction(parseState & state)
{
  zzRULE;
  zzmatch(TRUE_);
  
  state.type = TYPE_BOOLEAN;
  state.index = g.s.store("TRUE", state.type, state.scope);
  g.s.setValue(state.index, "1");
 consume();
  return;
fail:
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd6, 0x20);
}

void
raginBasicParser::falseFunction(parseState & state)
{
  zzRULE;
  zzmatch(FALSE_);
  
  state.type = TYPE_BOOLEAN;
  state.index = g.s.store("FALSE", state.type, state.scope);
  g.s.setValue(state.index, "0");
 consume();
  return;
fail:
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd6, 0x40);
}

void
raginBasicParser::ioStatement(parseState & state)
{
  zzRULE;
  ANTLRTokenPtr i=NULL, s=NULL;
  
  // 0 = nothing, 1 = CR
  int lastPrint = 1;			// assume CR by default
  char *path = "1";			// assume stdout as path
  if ( (LA(1)==PRINT) ) {
    zzmatch(PRINT); consume();
    {
      if ( (LA(1)==POUND) ) {
        zzmatch(POUND); consume();
        expression( state );
        
        path = i->getText();
        {
          if ( (LA(1)==COMMA) ) {
            zzmatch(COMMA); consume();
          }
          else {
            if ( (LA(1)==USING) ) {
              zzmatch(USING); consume();
            }
            else {FAIL(1,err24,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
          }
        }
      }
      else {
        if ( (LA(1)==USING) ) {
          zzmatch(USING); consume();
        }
        else {
          if ( (setwd6[LA(1)]&0x80) ) {
          }
          else {FAIL(1,err25,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
        }
      }
    }
    {
      while ( (setwd7[LA(1)]&0x1) ) {
        expression( state );
        
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
        {
          if ( (LA(1)==SEMI) ) {
            zzmatch(SEMI);
            
            lastPrint = 0;
 consume();
          }
          else {
            if ( (LA(1)==COMMA) ) {
              zzmatch(COMMA); consume();
            }
            else {
              if ( (setwd7[LA(1)]&0x2) ) {
              }
              else {FAIL(1,err26,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
            }
          }
        }
      }
    }
    
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
  }
  else {
    if ( (LA(1)==INPUT) ) {
      zzmatch(INPUT); consume();
      {
        if ( (setwd7[LA(1)]&0x4) ) {
          zzsetmatch(STRING_LITERAL_set, STRING_LITERAL_errset);
          s = (ANTLRTokenPtr)LT(1);

          
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
 consume();
          {
            if ( (LA(1)==COMMA) ) {
              zzmatch(COMMA); consume();
            }
            else {
              if ( (LA(1)==SEMI) ) {
                zzmatch(SEMI); consume();
              }
              else {FAIL(1,err27,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
            }
          }
        }
        else {
          if ( (setwd7[LA(1)]&0x8) ) {
          }
          else {FAIL(1,err28,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
        }
      }
      variableReference( state );
      
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
      {
        while ( (LA(1)==COMMA) ) {
          zzmatch(COMMA); consume();
          variableReference( state );
        }
      }
    }
    else {FAIL(1,err29,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
  }
  return;
fail:
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd7, 0x10);
}

void
raginBasicParser::primitiveElement(parseState & state)
{
  zzRULE;
  if ( (LA(1)==TRUE_) ) {
    trueFunction( state );
  }
  else {
    if ( (LA(1)==FALSE_) ) {
      falseFunction( state );
    }
    else {
      if ( (LA(1)==MOD) ) {
        modFunction( state );
      }
      else {
        if ( (LA(1)==NOT) ) {
          notFunction( state );
        }
        else {
          if ( (setwd7[LA(1)]&0x20) ) {
            variableReference( state );
          }
          else {
            if ( (setwd7[LA(1)]&0x40) ) {
              constantValue( state );
            }
            else {
              if ( (LA(1)==LPAREN) ) {
                zzmatch(LPAREN); consume();
                expression( state );
                zzmatch(RPAREN); consume();
              }
              else {FAIL(1,err30,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
            }
          }
        }
      }
    }
  }
  return;
fail:
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd7, 0x80);
}

void
raginBasicParser::signExpression(parseState & state)
{
  zzRULE;
  ANTLRTokenPtr op=NULL;
  
  int neg = 0, pos = 0;
  {
    while ( (setwd8[LA(1)]&0x1) ) {
      zzsetmatch(ADD_OP_set, ADD_OP_errset);
      op = (ANTLRTokenPtr)LT(1);

      
      // count up number of +'s and -'s
      if (*op->getText() == '-')
      {
        neg++;
      }
      else
      {
        pos++;
      }
 consume();
    }
  }
  primitiveElement( state );
  
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
  return;
fail:
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd8, 0x2);
}

void
raginBasicParser::multiplyingExpression(parseState & stateL)
{
  zzRULE;
  ANTLRTokenPtr op=NULL;
  
  parseState stateR;
  signExpression( stateL );
  {
    while ( (setwd8[LA(1)]&0x4) ) {
      zzsetmatch(MULT_OP_set, MULT_OP_errset);
      op = (ANTLRTokenPtr)LT(1);
 consume();
      signExpression( stateR );
      
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
    }
  }
  return;
fail:
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd8, 0x8);
}

void
raginBasicParser::addingExpression(parseState & stateL)
{
  zzRULE;
  ANTLRTokenPtr op=NULL;
  
  parseState stateR;
  multiplyingExpression( stateL );
  {
    while ( (setwd8[LA(1)]&0x10) ) {
      zzsetmatch(ADD_OP_set, ADD_OP_errset);
      op = (ANTLRTokenPtr)LT(1);
 consume();
      multiplyingExpression( stateR );
      
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
    }
  }
  return;
fail:
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd8, 0x20);
}

void
raginBasicParser::relationalExpression(parseState & stateL)
{
  zzRULE;
  ANTLRTokenPtr op=NULL;
  
  parseState stateR;
  addingExpression( stateL );
  {
    while ( (setwd8[LA(1)]&0x40) ) {
      zzsetmatch(RELATIONAL_OP_set, RELATIONAL_OP_errset);
      op = (ANTLRTokenPtr)LT(1);
 consume();
      addingExpression( stateR );
      
      // newTemp must be a BOOLEAN type
      int newTemp = g.s.mkTemp(TYPE_BOOLEAN, stateR.scope);
      g.gen((char *)op->getText(), stateL.index, g.s.getIndex(g.s.getName(stateR.index), stateR.scope), newTemp);
      
		stateL.index = newTemp;
      stateL.type = TYPE_BOOLEAN;
    }
  }
  return;
fail:
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd8, 0x80);
}

void
raginBasicParser::expression(parseState & stateL)
{
  zzRULE;
  ANTLRTokenPtr op=NULL;
  
  parseState stateR;
  relationalExpression( stateL );
  {
    while ( (setwd9[LA(1)]&0x1) ) {
      zzsetmatch(BOOLEAN_OP_set, BOOLEAN_OP_errset);
      op = (ANTLRTokenPtr)LT(1);
 consume();
      relationalExpression( stateR );
      
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
    }
  }
  return;
fail:
  syn(zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk);
  resynch(setwd9, 0x2);
}
