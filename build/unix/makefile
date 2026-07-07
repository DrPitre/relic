DIRS	= relic rbfe rxopt be6809 rxlist rblib rbtest rbtestopt

all:
	$(foreach dir, $(DIRS), (cd $(dir); make);)

clean:
	$(foreach dir, $(DIRS), (cd $(dir); make clean);)
	
install: all
	$(foreach dir, $(DIRS), (cd $(dir); make install);)
	
