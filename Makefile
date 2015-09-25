GCC = gcc
GCCFLAGS = -std=gnu99 -Wall -O3
#NOLIBS = -lm
HEADERS= -I./include
LIBSCAIRO = `pkg-config --cflags --libs cairo`
LIBS = `pkg-config --cflags --libs gsl`
SRCDIR=src
OBJDIR=obj
BINDIR=bin


ITERMAX=--itermax 100 
SEED=--seed 1 
XMAX=--xmax 250
YMAX=--ymax 250 
NEIGHBOURRADIUS=--neighbourradius 5
#PINIT=--pinit 0.9999 0.0001 
PINITBOSQUET=--pinit 0.9 0.1
PINITBAUDENA=--pinit 0 0.4 0.6 0

PR1=--pr1 0.0 
PR2=--pr2 0.1
NTR1=--ntr1 0 
NTR2=--ntr2 1 
PM1=--pm1 0.0 
PM2=--pm2 0.0 
NTM1=--ntm1 0 
NTM2=--ntm2 8

IMGFILEPREFIX=--imgfileprefix "frames/"

G1=--g1 0.2
MU1=--mu1 0.02
MU2=--mu2 0.5
MU3=--mu3 0.5
C2=--c2 6
C3=--c3 2

base=main init caengine stochasticity grid gridtorus initparams parseoptions
initpinit = initgridcells initcellintpinit
initclump = initgridcells initcellintpinitclump
initgaussian = initgridcells initcellarraydoubleclampedgaussian
initseed = initgridoneseed initcellno
bosquet=transitionmarkov transitionsbosquettype1 cellint speciesoncellint neighbourhoodcomputesum
baudena=transitionode transitionsbaudena cellarraydouble speciesoncellarraydouble neighbourhoodcomputeavg
spatial=neighbourhoodindividual neighbourhoodcircular
circularaverage=neighbourhoodindividualaverage neighbourhoodcircular
bosquetmeanfield=neighbourhoodglobal neighbourhoodnoshape
baudenameanfield=neighbourhoodglobalaverage neighbourhoodnoshape
anim=outputanimation
timeseriesavg=outputdensitiestimeseries
timeseriesavgerror=outputodetimeseries
finalstate=outputfinaldensities

all:  $(BINDIR)/bosquetspatialinitclumpanim $(BINDIR)/bosquetspatialinitclumptimeseries $(BINDIR)/bosquetspatialinitseedtimeseries $(BINDIR)/baudenacircularaveragetimeseries $(BINDIR)/bosquetcircularaveragetimeseries $(BINDIR)/bosquetspatialtimeseries $(BINDIR)/bosquetspatialanim $(BINDIR)/bosquetmeanfieldtimeseries $(BINDIR)/bosquetmeanfieldanim $(BINDIR)/baudenameanfieldanim $(BINDIR)/baudenaspatialanim $(BINDIR)/baudenameanfieldfinal $(BINDIR)/baudenaspatialfinal $(BINDIR)/baudenameanfieldtimeseries $(BINDIR)/baudenaspatialtimeseries $(BINDIR)/baudenaspatialinitseedtimeseries $(BINDIR)/baudenaspatialinitseedanim

.SUFFIXES:
.SUFFIXES: .o .c .h

# output.o: output.c output.h
# 	$(GCC) $(GCCFLAGS) -c $< -o $@ `pkg-config --cflags cairo gsl`

$(OBJDIR)/outputanimation.o: $(SRCDIR)/outputanimation.c
	$(GCC) $(GCCFLAGS) -c $< -o $@ $(HEADERS) `pkg-config --cflags cairo`

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(GCC) $(GCCFLAGS) -c $< -o $@ $(HEADERS) $(LIBS)

## BOSQUET
$(BINDIR)/bosquetmeanfieldtimeseries: $(base:%=$(OBJDIR)/%.o) $(bosquet:%=$(OBJDIR)/%.o) $(initpinit:%=$(OBJDIR)/%.o) $(bosquetmeanfield:%=$(OBJDIR)/%.o) $(timeseriesavg:%=$(OBJDIR)/%.o)
	$(GCC) $(GCCFLAGS) -o $@ $^ $(LIBS)

$(BINDIR)/bosquetmeanfieldanim: $(base:%=$(OBJDIR)/%.o) $(bosquet:%=$(OBJDIR)/%.o) $(initpinit:%=$(OBJDIR)/%.o) $(bosquetmeanfield:%=$(OBJDIR)/%.o) $(anim:%=$(OBJDIR)/%.o)
	$(GCC) $(GCCFLAGS) -o $@ $^ $(LIBS) $(LIBSCAIRO)

$(BINDIR)/bosquetcircularaveragetimeseries: $(base:%=$(OBJDIR)/%.o) $(bosquet:%=$(OBJDIR)/%.o) $(initpinit:%=$(OBJDIR)/%.o) $(circularaverage:%=$(OBJDIR)/%.o) $(timeseriesavg:%=$(OBJDIR)/%.o)
	$(GCC) $(GCCFLAGS) -o $@ $^ $(LIBS)

$(BINDIR)/bosquetspatialtimeseries: $(base:%=$(OBJDIR)/%.o) $(bosquet:%=$(OBJDIR)/%.o) $(initpinit:%=$(OBJDIR)/%.o) $(spatial:%=$(OBJDIR)/%.o) $(timeseriesavg:%=$(OBJDIR)/%.o)
	$(GCC) $(GCCFLAGS) -o $@ $^ $(LIBS)

$(BINDIR)/bosquetspatialinitseedtimeseries: $(base:%=$(OBJDIR)/%.o) $(bosquet:%=$(OBJDIR)/%.o) $(initseed:%=$(OBJDIR)/%.o) $(spatial:%=$(OBJDIR)/%.o) $(timeseriesavg:%=$(OBJDIR)/%.o)
	$(GCC) $(GCCFLAGS) -o $@ $^ $(LIBS)

$(BINDIR)/bosquetspatialinitclumptimeseries: $(base:%=$(OBJDIR)/%.o) $(bosquet:%=$(OBJDIR)/%.o) $(initclump:%=$(OBJDIR)/%.o) $(spatial:%=$(OBJDIR)/%.o) $(timeseriesavg:%=$(OBJDIR)/%.o)
	$(GCC) $(GCCFLAGS) -o $@ $^ $(LIBS)

$(BINDIR)/bosquetspatialinitclumpanim: $(base:%=$(OBJDIR)/%.o) $(bosquet:%=$(OBJDIR)/%.o) $(initclump:%=$(OBJDIR)/%.o) $(spatial:%=$(OBJDIR)/%.o) $(anim:%=$(OBJDIR)/%.o)
	$(GCC) $(GCCFLAGS) -o $@ $^ $(LIBS) $(LIBSCAIRO)

$(BINDIR)/bosquetspatialanim: $(base:%=$(OBJDIR)/%.o) $(bosquet:%=$(OBJDIR)/%.o) $(initpinit:%=$(OBJDIR)/%.o) $(spatial:%=$(OBJDIR)/%.o) $(anim:%=$(OBJDIR)/%.o)
	$(GCC) $(GCCFLAGS) -o $@ $^ $(LIBS) $(LIBSCAIRO)

## BAUDENA
$(BINDIR)/baudenameanfieldanim: $(base:%=$(OBJDIR)/%.o) $(baudena:%=$(OBJDIR)/%.o) $(initgaussian:%=$(OBJDIR)/%.o) $(baudenameanfield:%=$(OBJDIR)/%.o) $(anim:%=$(OBJDIR)/%.o)
	$(GCC) $(GCCFLAGS) -o $@ $^ $(LIBS) $(LIBSCAIRO)

$(BINDIR)/baudenameanfieldtimeseries: $(base:%=$(OBJDIR)/%.o) $(baudena:%=$(OBJDIR)/%.o) $(initgaussian:%=$(OBJDIR)/%.o) $(baudenameanfield:%=$(OBJDIR)/%.o) $(timeseriesavg:%=$(OBJDIR)/%.o)
	$(GCC) $(GCCFLAGS) -o $@ $^ $(LIBS) 

$(BINDIR)/baudenameanfieldfinal: $(base:%=$(OBJDIR)/%.o) $(baudena:%=$(OBJDIR)/%.o) $(initgaussian:%=$(OBJDIR)/%.o) $(baudenameanfield:%=$(OBJDIR)/%.o) $(finalstate:%=$(OBJDIR)/%.o)
	$(GCC) $(GCCFLAGS) -o $@ $^ $(LIBS) 

$(BINDIR)/baudenaspatialanim: $(base:%=$(OBJDIR)/%.o) $(baudena:%=$(OBJDIR)/%.o) $(initgaussian:%=$(OBJDIR)/%.o) $(spatial:%=$(OBJDIR)/%.o) $(anim:%=$(OBJDIR)/%.o)
	$(GCC) $(GCCFLAGS) -o $@ $^ $(LIBS) $(LIBSCAIRO)

$(BINDIR)/baudenaspatialtimeseries: $(base:%=$(OBJDIR)/%.o) $(baudena:%=$(OBJDIR)/%.o) $(initgaussian:%=$(OBJDIR)/%.o) $(spatial:%=$(OBJDIR)/%.o) $(timeseriesavg:%=$(OBJDIR)/%.o)
	$(GCC) $(GCCFLAGS) -o $@ $^ $(LIBS) 

$(BINDIR)/baudenaspatialfinal: $(base:%=$(OBJDIR)/%.o) $(baudena:%=$(OBJDIR)/%.o) $(initgaussian:%=$(OBJDIR)/%.o) $(spatial:%=$(OBJDIR)/%.o) $(finalstate:%=$(OBJDIR)/%.o)
	$(GCC) $(GCCFLAGS) -o $@ $^ $(LIBS) 

$(BINDIR)/baudenaspatialinitseedtimeseries: $(base:%=$(OBJDIR)/%.o) $(baudena:%=$(OBJDIR)/%.o) $(initseed:%=$(OBJDIR)/%.o) $(spatial:%=$(OBJDIR)/%.o) $(timeseriesavg:%=$(OBJDIR)/%.o)
	$(GCC) $(GCCFLAGS) -o $@ $^ $(LIBS) 

$(BINDIR)/baudenaspatialinitseedanim: $(base:%=$(OBJDIR)/%.o) $(baudena:%=$(OBJDIR)/%.o) $(initseed:%=$(OBJDIR)/%.o) $(spatial:%=$(OBJDIR)/%.o) $(anim:%=$(OBJDIR)/%.o)
	$(GCC) $(GCCFLAGS) -o $@ $^ $(LIBS) $(LIBSCAIRO)

$(BINDIR)/baudenacircularaveragetimeseries: $(base:%=$(OBJDIR)/%.o) $(baudena:%=$(OBJDIR)/%.o) $(initgaussian:%=$(OBJDIR)/%.o) $(circularaverage:%=$(OBJDIR)/%.o) $(timeseriesavg:%=$(OBJDIR)/%.o)
	$(GCC) $(GCCFLAGS) -o $@ $^ $(LIBS) 



runbosquetspatial: $(BINDIR)/bosquetspatialtimeseries
	bin/bosquetspatialtimeseries --itermax 1000 --xmax 250 --ymax 250 --neighbourradius 7.25 --ntr1 0.5206594 --ntr2 5 --pr1 0.0001942439 --pr2 0.0042149 --ntm1 1.027454 --ntm2 4 --pm1 0.08288770 --pm2 0.250569 --pinit 0.999 0.001

runbaudenaspatial: $(BINDIR)/baudenaspatialtimeseries
	bin/baudenaspatialtimeseries --itermax 1000 --xmax 100 --ymax 100 --neighbourradius 2 --g1 0.2 --mu1 0.02 --mu2 0.5 --mu3 0.5 --c2 3.5 --c3 2 --pinit 0 0.4 0.6 0

runbaudenameanfield: $(BINDIR)/baudenameanfieldtimeseries
	bin/baudenameanfieldtimeseries --itermax 1000 --xmax 100 --ymax 100 --neighbourradius 2 --g1 0.2 --mu1 0.02 --mu2 0.5 --mu3 0.5 --c2 3.5 --c3 2 --pinit 0 0.4 0.6 0


# profile: bosquet
# 	./bosquet
# 	gprof bosquet gmon.out > prof.txt
# 	cat prof.txt

clean:
	rm -f $(OBJDIR)/*.o
	rm -f $(BINDIR)/*

.PHONY: all clean build run
