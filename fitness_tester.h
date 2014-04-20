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
	fitness tests
*/

#ifndef fitness_test_h
#define fitness_test_h

#include <map>
#include "chromosome.h"
#include <triad.h>

/* Singleton class
 */
class world
{
	private:
		struct city {
			std::string name;
			nrtb::triad<float> loc;
		};
		typedef std::vector<city> city_vector;
		typedef std::multimap<long long int,city> rank_map;
		city_vector cities;
		float calc_distance(rank_map::iterator d, rank_map::iterator a);
		static world * me;
	protected:
		world();
		~world();
		world(const world &) {};
	public:
		static world & get_instance();
		void load(const std::string filename);
		float check_fitness(chromosome &a);	
		void dump();
		int length();
		std::string show_route(chromosome &a);
};

class fitness_updater:
	public std::unary_function<chromosome,void>
{
	private: 
		world * w;
	public: 
		fitness_updater()
		{
			w = &(world::get_instance());
		};
		void operator() (chromosome & c)
		{
			w->check_fitness(c);
		};
};

struct dead_chromosome:
	public std::unary_function<chromosome,bool>
{
	bool operator() (chromosome & c)
	{
		return c.fitness < 0;
	};
};
#endif // fitness_test_h

