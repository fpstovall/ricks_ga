/***********************************************
This file is part of the Rick's Generic GA Solver project (https://launchpad.net/ricks-ga).

    Rick's Generic GA Solver is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Rick's Generic GA Solver is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Rick's Generic GA Solver.  If not, see <http://www.gnu.org/licenses/>.

***********************************************/
/*	
	Chromosome for traveling salesman test cases.
*/

#include "chromosome.h"

void chromosome::recombine(chromosome & a, chromosome & b, unsigned int w)
{
	ricks_ga::basic_chromosome<genetype>::recombine(a,b,w);
	fitness = 0;
};

void chromosome::splice(chromosome & a, chromosome & b, unsigned int s, unsigned int e)
{
	ricks_ga::basic_chromosome<genetype>::splice(a,b,s,e);
	fitness = 0;
};

int operator <(const chromosome &a,const chromosome &b)
{
	return (a.fitness < b.fitness);
};

int operator >(const chromosome &a,const chromosome &b)
{
	return (a.fitness > b.fitness);
};

int operator ==(const chromosome &a,const chromosome &b)
{
	return (a.fitness == b.fitness);
};
