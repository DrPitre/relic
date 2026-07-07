vpath %.cpp	../../../rxopt

OBJ		= rxopt
CC		= g++
CPPFLAGS	+= -I../../../common -g

$(OBJ):	$(OBJ).o

install: $(OBJ)
	cp $(OBJ) ~/bin

clean:
	rm -f $(OBJ) $(OBJ).o
