CXX=g++
CXXFLAGS=-g -std=c++11 -I. -O3 -Wall -Wextra -Wpedantic -Werror -Wno-error=unused-parameter
USE_ROOT=yes

OBJ = marley_utils.o TMarleyParticle.o TMarleyEvent.o TMarleyEvaporationThreshold.o
OBJ += TMarleyGenerator.o TMarleyReaction.o TMarleyGamma.o TMarleyLevel.o
OBJ += TMarleyDecayScheme.o
OBJ += TMarleyMassTable.o TMarleyStructureDatabase.o TMarleyConfigFile.o
OBJ += TMarleyNuclearPhysics.o TMarleyBackshiftedFermiGasModel.o
OBJ += TMarleyIntegrator.o
OBJ += TMarleyNeutrinoSource.o TMarleyKinematics.o

ifdef USE_ROOT
# Adding the g++ compiler option -DUSE_ROOT to the CXXFLAGS
# variable allows you to use conditional compilation
# via the preprocessor directive #ifdef USE_ROOT.
# Currently none of the core MARLEY classes use such
# preprocessor directives, but the example executable
# react does.
#
# The root_dict.o object file should be added
# to the list of prerequisites for any executable
# that uses TTrees containing TMarleyEvents
# or TMarleyParticles
CXXFLAGS += `root-config --cflags` -DUSE_ROOT
OBJ_DICT = root_dict.o
LDFLAGS=`root-config --libs`
endif

all: parse react validate check

# Don't use our standard warnings for this object file. You need to
# fix the issues in the cwfcomp library that you borrowed or rewrite
# the code.
TMarleySphericalOpticalModel.o: TMarleySphericalOpticalModel.cc
	g++ -g -std=c++11 -I. -O3 -c -o $@ $^

coulomb.o: coulomb.cc
	g++ -g -std=c++11 -I. -O3 -c -o $@ $^

coulomb: coulomb.o
	g++ -g -std=c++11 -I. -O3 -o $@ $^

%.o: %.c
	$(CXX) -c -o $@

parse: $(OBJ) parse.o TMarleySphericalOpticalModel.o
	$(CXX) -o $@ $^

react: $(OBJ) $(OBJ_DICT) react.o TMarleySphericalOpticalModel.o
	$(CXX) -o $@ $^ $(LDFLAGS)

validate: $(OBJ) $(OBJ_DICT) validate.o TMarleySphericalOpticalModel.o
	$(CXX) -o $@ $^ $(LDFLAGS)

check: $(OBJ) $(OBJ_DICT) check.o TMarleySphericalOpticalModel.o
	$(CXX) -o $@ $^ $(LDFLAGS)

check_kinem: $(OBJ) check_kinem.o TMarleySphericalOpticalModel.o
	$(CXX) -o $@ $^ $(LDFLAGS)

plots: $(OBJ) $(OBJ_DICT) plots.o TMarleySphericalOpticalModel.o
	$(CXX) -o $@ $^ $(LDFLAGS)

# Add more header files to the prerequisites for
# root_dict.o if you would like to store other
# MARLEY classes in ROOT trees. All such classes
# currently use a single automatically-generated
# dictionary source file root_dict.cc
# 
# The commands invoked to create root_dict.o
# do the following things:
# 1. Remove old dictionary files
# 2. Create new dictionary files, enabling ROOT
#    i/o by adding the '+' suffix to each prerequisite
#    header file (.hh extension)
# 3. Compile the dictionary source file
root_dict.o: TMarleyParticle.hh TMarleyEvent.hh
	rm -f root_dict.cc root_dict.h
	rootcint root_dict.cc -c $(subst .hh,.hh+,$^)
	$(CXX) $(CXXFLAGS) -c -o root_dict.o root_dict.cc

.PHONY: clean

clean:
	rm -f *.o parse react validate root_dict.cc root_dict.h
