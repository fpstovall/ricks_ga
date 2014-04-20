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

build:	conftest
	@cp -v confreader.h ../include
	@cp -v confreader.o ../obj
	@echo build complete

confreader.o:	confreader.h confreader.cpp Makefile
	@rm -f confreader.o
	g++ -c confreader.cpp -I ../include

conftest:	confreader.o conftest.cpp
	@rm -f conftest
	g++ -c conftest.cpp
	g++ -o conftest conftest.o confreader.o ../obj/common.o

clean:
	@rm -rvf *.o conftest ../include/confreader.h ../obj/confreader.o
	@echo all objects and executables have been erased.
