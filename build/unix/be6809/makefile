vpath %.cpp	../../../be6309

OBJ		= be6309
CC		= g++
CPPFLAGS	+= -I../../../common -g

$(OBJ):	$(OBJ).o

install: $(OBJ)
	cp $(OBJ) ~/bin
	mv ~/bin/$(OBJ) ~/bin/be6809

clean:
	rm -f $(OBJ) $(OBJ).o
