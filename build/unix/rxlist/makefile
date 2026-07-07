vpath %.cpp	../../../rxlist

OBJ		= rxlist
CC		= g++
CPPFLAGS	+= -I../../../common -g

$(OBJ):	$(OBJ).o #../common/codegen.h ../common/symbol.h

install: $(OBJ)
	cp $(OBJ) ~/bin

clean:
	rm -f $(OBJ) $(OBJ).o
