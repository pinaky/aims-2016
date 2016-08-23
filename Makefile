
AR=ar
CC=g++
MKDIR_P=mkdir -p

IDIR=./include/
LDIR=./lib
ODIR=./obj
BDIR=./bin
BASEDIR=./src
TESTDIR=$(BASEDIR)/tests
EXAMPLEDIR=$(BASEDIR)/example


CFLAGS=-std=c++11 -c -I${IDIR}
LDFLAGS=-L${LDIR} -laims

ARFLAGS=-rvs

LIBSOURCES=acceptanceprobabilityservice_regular.cpp \
	betascheduler_gausskronrod.cpp \
	chain.cpp \
	chainprinter_json.cpp \
	forwardmodel_gaussian.cpp \
	numsamplesscheduler_constant.cpp \
	prior_generaluniform.cpp \
	proposal_rwmhisotropicnormal.cpp \
	proposalscheduler_constantrwmhisotropicnormal.cpp \
	utilities.cpp

LIBOBJECTS=$(LIBSOURCES:.cpp=.o)

TESTSOURCES=tests.cpp \
	test_utilities_betafunctionandderivative.cpp \
	test_utilities.cpp \
	test_utilities_l2misc.cpp \
	test_utilities_logsumexpmisc.cpp \
	test_utilities_simplemoment_samples.cpp \
	test_utilities_simplemoment_template.cpp

TESTOBJECTS=$(TESTSOURCES:.cpp=.o)

EXAMPLESOURCES=main.cpp

EXAMPLEOBJECTS=$(EXAMPLESOURCES:.cpp=.o)

.PHONY: directories

all: directories libaims.a example tests

example: $(EXAMPLEOBJECTS) libaims.a directories
	$(CC) $(addprefix $(ODIR)/, $(EXAMPLEOBJECTS)) -o $(BDIR)/$@ $(LDFLAGS)

%.o: $(EXAMPLEDIR)/%.cpp directories
	$(CC) $(CFLAGS) $(EXAMPLEDIR)/$(patsubst %.o,%.cpp,$@) -o $(ODIR)/$@

tests: $(TESTOBJECTS) libaims.a directories
	$(CC) $(addprefix $(ODIR)/, $(TESTOBJECTS)) -o $(BDIR)/$@ $(LDFLAGS)

%.o: $(TESTDIR)/%.cpp directories
	$(CC) $(CFLAGS) $(TESTDIR)/$(patsubst %.o,%.cpp,$@) -o $(ODIR)/$@

libaims.a: $(LIBOBJECTS) directories
	$(AR) $(ARFLAGS) $(LDIR)/$@ $(addprefix $(ODIR)/, $(LIBOBJECTS))

%.o: $(BASEDIR)/%.cpp directories
	$(CC) $(CFLAGS) $(BASEDIR)/$(patsubst %.o,%.cpp,$@) -o $(ODIR)/$@

directories: $(ODIR) $(BDIR) $(LDIR)

$(ODIR): 
	$(MKDIR_P) $(ODIR)

$(BDIR):
	$(MKDIR_P) $(BDIR)

$(LDIR):
	$(MKDIR_P) $(LDIR)

clean:
	rm $(ODIR)/* $(BDIR)/* $(LDIR)/*

