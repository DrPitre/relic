vpath %.cpp	../../../relic

OBJ		= relic
CC		= g++
CPPFLAGS	+= -I../../../common -g

$(OBJ):	$(OBJ).o

install: $(OBJ)
	cp $(OBJ) ~/bin

clean:
	rm -f $(OBJ) $(OBJ).o
