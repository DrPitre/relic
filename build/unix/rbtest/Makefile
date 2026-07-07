srcpath = ../../../rb/tests
dsk = ../../../rbtest.dsk

vpath %.rb $(srcpath)

OBJS = add age boolean calc divide for goto if math multiply print quick relation string subtract while

%: %.rb
	relic $< -L=/Users/boisy/relic -xr

all: $(OBJS)

install: dsk
	cp $(dsk) $(HOME)/relic

dsk: all
	os9 format $(dsk)
	os9 makdir $(dsk),CMDS
	os9 makdir $(dsk),SRC
	$(foreach obj, $(OBJS), os9 copy -l $(srcpath)/$(obj).rb $(dsk),SRC;)
	$(foreach obj, $(OBJS), os9 copy $(obj) $(dsk),CMDS;)
	$(foreach obj, $(OBJS), os9 attr -qe $(dsk),CMDS/$(obj);)
	

dskclean:
	-rm $(dsk)

clean: dskclean
	-$(foreach obj, $(OBJS), rm $(obj) $(obj).rx $(obj).ro $(obj).a $(obj).r;)

