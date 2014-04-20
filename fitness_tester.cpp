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

#include "fitness_tester.h"
#include <fstream>
#include <math.h>

using namespace std;

world * world::me = 0;

world::world()
{
	if (!me)
	{
		cities.clear();
	}
	else
	{
		throw std::exception();
	};
};

world::~world()
{
	cities.clear();
	me = 0;
};

world & world::get_instance()
{
	if (me == 0)
	{
		me = new world;
	};
	return *me;
};

void world::load(const string filename)
{
	// read a file and store it in cities.
	ifstream infile(filename.c_str());
	// set up some working storage
	city where;
	cities.clear();
	// load the cities list.
	while (infile >> where.name >> where.loc)
	{
		cities.push_back(where);
	};
	// close the file
	infile.close();
};

float world::calc_distance(rank_map::iterator d, rank_map::iterator a)
{
	// calculate the xy distance
	return d->second.loc.range(a->second.loc);
};

float world::check_fitness(chromosome &a)
{
	// computes the distance traveled by the submitted chromosome.
	// first, get a list of the cities in the desired order of travel.
	rank_map ranked;
	pair<int,city> new_score;
	unsigned int length = a.length();
	for (unsigned int i=0; i < length; i++)
	{
		//ranked[a[i]] = cities[i];
		new_score.first = a[i];
		new_score.second = cities[i];
		ranked.insert(new_score);
	};	
	// sanity check
	if (ranked.size() != cities.size())
	{
		cerr << __FILE__ << "|" << __FUNCTION__ << "|" << __LINE__ 
		<< ": ranked.size() [" << ranked.size()
		<< "] != cities.size() [" << cities.size()
		<< "]\n" << "Dumping chromosome (" << a.length() << " genes):\n" 
		<< a.enstream() 
		<< endl;
		exit(1); 
	};
	// iterate through the ranked list, calculating the travel distance.
	rank_map::iterator departing = ranked.begin();
	rank_map::iterator arriving = ++ranked.begin();
	// save the starting point for the return calculation.
	rank_map::iterator start_point = departing;
	// sanity check
	if (arriving == departing)
	{
		cerr << __FILE__ << "|" << __FUNCTION__ << "|" << __LINE__ 
		<< ": arriving == departing" << endl;
		exit(1); 
	};
	float total_dist = 0;
	while (arriving != ranked.end())
	{
		total_dist += calc_distance(departing,arriving);
		arriving++;
		departing++;
	};
	// add the return to starting point.
	total_dist += calc_distance(departing,start_point);
	// kill it if the first city is not the first in the list
	if (start_point->second.name != cities[0].name)
	{
			total_dist = -1;
	};
	// store the result in the chromosome and return it.
	a.fitness = total_dist;
	return total_dist;
};

void world::dump()
{
	cout << "\n===============" << endl;
	for(unsigned int i=0; i < cities.size(); i++)
	{
		cout << cities[i].name << "\t" << cities[i].loc << endl;
	};
	cout << "===============" << endl;
};

int world::length()
{
	return cities.size();
};

string world::show_route(chromosome &a)
{
	rank_map ranked;
	pair<long long int,city> new_score;
	for (unsigned int i=0; i < a.length(); i++)
	{
		//ranked[a[i]] = cities[i];
		new_score.first = a[i];
		new_score.second = cities[i];
		ranked.insert(new_score);
	};	
	// sanity check
	if (ranked.size() != cities.size())
	{
		cerr << __FILE__ << "|" << __FUNCTION__ << "|" << __LINE__ 
		<< ": ranked.size() [" << ranked.size()
		<< "] != cities.size() [" << cities.size()
		<< "]\n" << "Dumping chromosome (" << a.length() << " genes):\n" 
		<< a.enstream() 
		<< endl;
		exit(1); 
	};
	rank_map::iterator current = ranked.begin();
	string returnme = current->second.name;
	current++;
	while (current != ranked.end())
	{
		returnme += "->" + current->second.name;
		current++;
	};
	return returnme;
};
