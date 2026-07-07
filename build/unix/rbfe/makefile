#
# PCCTS makefile for: ../../../rb/fe/rb.g
#
# Created from: genmk -CC -class raginBasicParser -project rbfe -pccts_path ../../../pccts ../../../rb/fe/rb.g
#
# PCCTS release 1.33MR23
# Project: rbfe
# C++ output
# DLG scanner
# ANTLR-defined token types
#
TOKENS = tokens.h
#
# The following filenames must be consistent with ANTLR/DLG flags
DLG_FILE = parser.dlg
ERR = err
HDR_FILE =
SCAN = DLGLexer
PCCTS = ../../../pccts
ANTLR_H = $(PCCTS)/h
ANTLR = antlr
DLG = dlg
COTHER = -I../../../common
CFLAGS = -I. -I$(ANTLR_H) $(COTHER) -DPCCTS_USE_NAMESPACE_STD
AFLAGS = -CC -mrhoist off $(AOTHER)
DFLAGS = -C2 -i -CC $(DOTHER)
GRM = ../../../rb/fe/rb.g
SRC = ../../../rb/fe/rb.cpp \
	raginBasicParser.cpp \
	$(ANTLR_H)/AParser.cpp $(ANTLR_H)/DLexerBase.cpp $(ANTLR_H)/ATokenBuffer.cpp $(SCAN).cpp

OBJ = ../../../rb/fe/rb.o \
	raginBasicParser.o \
	AParser.o DLexerBase.o ATokenBuffer.o $(SCAN).o

ANTLR_SPAWN = ../../../rb/fe/rb.cpp raginBasicParser.cpp \
		raginBasicParser.h $(DLG_FILE) $(TOKENS)
DLG_SPAWN = $(SCAN).cpp $(SCAN).h
ifdef CXX
CCC = $(CXX)
endif

ifndef CCC
CCC = CC
endif


rbfe : $(SRC) $(OBJ)
	$(CCC) -o rbfe $(CFLAGS) $(OBJ)

../../../rb/fe/rb.o : $(TOKENS) $(SCAN).h ../../../rb/fe/rb.cpp
	$(CCC) -c $(CFLAGS) -o ../../../rb/fe/rb.o ../../../rb/fe/rb.cpp

raginBasicParser.o : $(TOKENS) $(SCAN).h raginBasicParser.cpp raginBasicParser.h
	$(CCC) -c $(CFLAGS) -o raginBasicParser.o raginBasicParser.cpp

$(SCAN).o : $(SCAN).cpp $(TOKENS)
	$(CCC) -c $(CFLAGS) -o $(SCAN).o $(SCAN).cpp

$(ANTLR_SPAWN) : $(GRM)
	$(ANTLR) $(AFLAGS) $(GRM)

$(DLG_SPAWN) : $(DLG_FILE)
	$(DLG) $(DFLAGS) $(DLG_FILE)

AParser.o : $(ANTLR_H)/AParser.cpp
	$(CCC) -c $(CFLAGS) -o AParser.o $(ANTLR_H)/AParser.cpp

ATokenBuffer.o : $(ANTLR_H)/ATokenBuffer.cpp
	$(CCC) -c $(CFLAGS) -o ATokenBuffer.o $(ANTLR_H)/ATokenBuffer.cpp

DLexerBase.o : $(ANTLR_H)/DLexerBase.cpp
	$(CCC) -c $(CFLAGS) -o DLexerBase.o $(ANTLR_H)/DLexerBase.cpp

clean:
	rm -f *.o core rbfe

scrub: clean
	rm -f $(ANTLR_SPAWN) $(DLG_SPAWN)

install:	rbfe
	cp rbfe /Users/boisy/bin
