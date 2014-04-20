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

#ifndef chromosome_h
#define chromosome_h

#include <basic_chromosome.h>

/*************************************
	This group is used to define the 
	chromosome used.
*************************************/
// type of genes in the chromosomes
typedef unsigned char genetype;

class chromosome : public ricks_ga::basic_chromosome<genetype>
{
	public: 
		float fitness;
		chromosome() { fitness = 0.0; };
		void recombine(chromosome & a, chromosome & b, unsigned int w);
		void splice(chromosome & a, chromosome & b, unsigned int s, unsigned int e);
};

int operator <(const chromosome &a,const chromosome &b);
int operator >(const chromosome &a,const chromosome &b);
int operator ==(const chromosome &a,const chromosome &b);

#endif // chromosome_h

