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
#***********************************************/

build:	bc_test.o
	@cp -v basic_chromosome.h ../include
	@echo build complete

bc_test.o:	basic_chromosome.h bc_test.cpp Makefile
	@rm -f bc_test.o
	g++ -c bc_test.cpp

clean:
	@rm -rvf *.o  ../include/basic_chromosome.h
	@echo all objects and executables have been erased.
