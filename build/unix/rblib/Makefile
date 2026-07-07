DIRS	= 6309 ppc cell arm64

all:
	$(foreach dir, $(DIRS), (cd $(dir); make);)

clean:
	$(foreach dir, $(DIRS), (cd $(dir); make clean);)

install: all
	$(foreach dir, $(DIRS), (cd $(dir); make install);)
