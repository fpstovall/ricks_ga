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

build:	common.o
	@cp -v common.h ../include
	@cp -v common.o ../obj
	@echo build complete

common.o:	common.h common.cpp Makefile
	@rm -f common.o
	g++ -c -O3 common.cpp

clean:
	@rm -rvf *.o  ../include/common.h ../obj/common.o
	@echo all objects and executables have been erased.
