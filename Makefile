#***********************************************
# This file is part of the Rick's Generic GA Solver project (https://launchpad.net/ricks-ga).
#
#    Rick's Generic GA Solver is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    Rick's Generic GA Solver is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with Rick's Generic GA Solver.  If not, see <http://www.gnu.org/licenses/>.
#
#***********************************************/# Useful directories

MYCODEDIR := .

# Directories to search for header files

SEARCHDIRS := -I${MYCODEDIR} -I include

# makemake variables

#DEPENDFLAGS := -O3 -Wall -Werror ${SEARCHDIRS}
DEPENDFLAGS := -O3 -Werror ${SEARCHDIRS}

# C++ compiler

CXX      := g++
CXXFLAGS  = ${DEPENDFLAGS}

# C/C++/Eiffel/FORTRAN linker

LINKER    := g++
LDFLAGS    = -L ./obj
LOADLIBES := -lm obj/confreader.o obj/hires_timer.o obj/common.o

############################################
### Build rules start here #################
############################################

build: salesman_tourney
	@echo "new salesman build complete"
	
salesman_tourney : libs obj/chromosome.o obj/fitness_tester.o obj/bc_bench.o
	${LINKER} ${LDFLAGS} -o $@ obj/bc_bench.o obj/chromosome.o obj/fitness_tester.o ${LOADLIBES}

libs:	
	@cd common; make
	@cd chromosome; make 
	@cd point; make
	@cd timer; make
	@cd confreader; make

obj/bc_bench.o:	 bc_bench.cpp parameters.h
	${CXX} ${CXXFLAGS} -c bc_bench.cpp -o obj/bc_bench.o

obj/chromosome.o: chromosome.h chromosome.cpp
	${CXX} ${CXXFLAGS} -c chromosome.cpp -o obj/chromosome.o
	
obj/fitness_tester.o: fitness_tester.h fitness_tester.cpp
	${CXX} ${CXXFLAGS} -c fitness_tester.cpp -o obj/fitness_tester.o

clean: 
	@cd common; make clean
	@cd chromosome; make clean 
	@cd point; make clean
	@cd timer; make clean
	@cd confreader; make clean
	@rm -vf obj/*.o salesman_tourney
