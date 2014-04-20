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
	benchmarker
*/

// library includes.
#include <vector>
#include <algorithm>
#include <fstream>
#include <time.h>
#include <confreader.h>
#include <hires_timer.h>
#include <boost/random.hpp>
// local includes.
#include "parameters.h"
#include "chromosome.h"
#include "fitness_tester.h"

using namespace std;

typedef vector<chromosome> c_vector;
typedef map<float,chromosome> c_sorted;

int main(int argc, char* argv[])
{
	/* Obsolete arguments replaced by percentage args
	b_count = config.get<unsigned int>("b_count",b_count);
	unsigned int d_count = config.get<unsigned int>("d_count",b_count);
	save_count = config.get<unsigned int>("save_count",save_count);
	*/

	// set up our run-time variables.
	ricks_ga::conf_reader config;
	config.read(argc,argv,"salesman_tourney.config");
	//-- Run control options
	c_count = config.get<unsigned int>("c_count",c_count);
	v_count = config.get<unsigned int>("v_count",v_count);
	splice = !config.exists("--cross") || config.exists("--splice");
	float b_percent = config.get<float>("b_percent",0.0)/100.0;
	float d_percent = config.get<float>("d_percent",b_percent)/100.0;
	float s_percent = config.get<float>("save_percent",0.0)/100.0;
	mutations = config.get<long double>("mutations",mutations);
	//-- Run termination options
	samelimit = config.get<int>("samelimit",samelimit);
	genlimit = config.get<int>("genlimit",genlimit);	
	e_threshold = config.get<unsigned int>("e_threshold",e_threshold);
	//-- IO options
	outfile = config.get<string>("outfile",outfile);
	infile = config.get<string>("infile",infile);
	bool silent = config.exists("--silent");
	bool world_silent = config.exists("--world-silent");
	int gdispmod = config.get<int>("g_mod",1);
	bool mute = config.exists("--mute");
	bool file_headers = 
		config.exists("--file-headers") && !config.exists("--no-file-headers");
	
	// Handle "flexable" parameters
	if (b_percent <= 0.0)
	{
		cerr << "b_percent can not be 0 or less!" << endl;
		exit(1);
	};
	// adjust d_percent.
	d_percent = 1.0 - d_percent;
	
	// enforce muteness if so ordered.
	if (mute)
	{
		silent = true;
	};
	// simularly enforce silence
	if (silent)
	{
		world_silent = true;
	};
	
	// report the configuration read.
	if (!world_silent)
	{
		ricks_ga::conf_reader::iterator c = config.begin();
		ricks_ga::conf_reader::iterator e = config.end();
		cout << "Read these settings from the config file and command line:" 
			<< endl;
	 	while (c != e)
		{
			cout << "\t" << c->first << "=" << c->second << endl;
			c++;
		};
	};
	// -- generation data.
	c_vector gen_list;
	gen_list.clear();
	gen_list.reserve(c_count);
	c_sorted breeding_list;
	breeding_list.clear();
//	breeding_list.reserve(b_count);
	c_sorted sorted;
	c_vector::iterator gc;
	c_vector::iterator ge;
	long int generation = 0;
	// -- fitness testing.
	world & environment = world::get_instance();
	environment.load(infile);
	if (!silent && !world_silent) environment.dump();
	// -- set chromosome length.
	int gensize = environment.length();
	// -- sameness is used to detect run end.
	int sameness = samelimit;
	// -- performance tracking data.
	long double current_best = 0.0;
	long double absolute_best = 1.0e30;
	chromosome winner;
	winner.fitness = absolute_best;
	int first_best = 0;
	// -- time mark for run time determination.
	nrtb::hirez_timer runtime;
	// -- seed the random number generator.
	boost::mt19937 rng(time(NULL));
	boost::uniform_01<boost::mt19937,float> probability(rng);

	ofstream output(outfile.c_str());
	if (file_headers)
	{
		output << "generation"
			<< "\t" << "best" 
			<< "\t" << "worst" 
			<< "\t" << "average"
			<< "\t" << "sec"
			<< "\t" << "viable" 
			<< "\t" << "bred"
			<< "\t" << "entropy"
			<< "\t" << "mutated"
			<< endl;
	};

	// create a random first generation
	bool v_test = true;
	if (v_count == 0)
	{
		v_count = c_count;
		v_test = false;
	};
	if (!silent)
	{
		cout << "\nCreating " << v_count
			<< (!v_test ? " random " : " viable ")
			<< "chromosomes... "  
			<< flush;
	};
	nrtb::hirez_timer gen_time;
	while (gen_list.size() < v_count)
	{
		chromosome loader;
		loader.reload(gensize);
		if (v_test)
		{
			environment.check_fitness(loader);
			if (loader.fitness >= 0)
			{
				gen_list.push_back(loader);
			};
		}
		else
		{
			gen_list.push_back(loader);
		};
	};
	// calculate each chromosome's fitness
	for_each(gen_list.begin(),gen_list.end(),fitness_updater());
	// clear out the deadwood
	gen_list.erase(
		remove_if(gen_list.begin(),gen_list.end(),dead_chromosome()),		
		gen_list.end() );
	if (!silent)
	{
		cout << "done. (" 
			<< gen_time.stop() << " seconds)." << endl;
	};
	
	// make the sorted list.
	gc = gen_list.begin();
	ge = gen_list.end();
	sorted.clear();
	while (gc != ge)
	{
		sorted[gc->fitness] = *gc;
		gc++;
	};
	gen_time.reset();
	gen_time.start();
	// generation processing loop.
	while ((sameness--) && (genlimit--))
	{

		// cull off the lowest performers
		gen_list.clear();

		c_sorted::iterator glc = sorted.begin();
		int mv_count = (int) ceil(sorted.size() * d_percent);
		while (mv_count > 0)
		{
			gen_list.push_back(glc->second);
			glc++; 
			mv_count--;
		};

		// select the breading group
		breeding_list.clear();

		// save the first unique genes without modification.
		save_count = (unsigned int) ceil(s_percent * gen_list.size());
		if (save_count > 0)
		{
			c_sorted::iterator b_curr = sorted.begin();
			c_sorted::iterator b_end = sorted.end();
			int count = save_count;
			while ((count--) && (b_curr != b_end))
			{
				breeding_list[b_curr->first] = b_curr->second;
				b_curr++;
			};
		};
		// -- build the rest of the breeding list.
		b_count = (unsigned int) round(b_percent * gen_list.size());
		if (b_count < 2)
		{
			b_count = 2;
		};
		unsigned int parentsize = gen_list.size();
		unsigned int bailout = c_count * 200;
		while ((breeding_list.size() < b_count) && (bailout-- > 0))
		{
			// get two competetors at random.
			unsigned int a = rng() % parentsize;
			unsigned int b = rng() % parentsize;
			chromosome & parent_a = gen_list[a];
			chromosome & parent_b = gen_list[b];
			// determine the winner
			/*
				FPS 2005-03-19.. removed all the viability checks here.
				They are not needed because of the checks made on the 
				population before we get to this point in the cycle.
			*/
			chromosome * best;
			if (parent_a > parent_b) { best = &parent_b; }
			else best = &parent_a;
			// store unconditionally.
				breeding_list[best->fitness] = *best;
		}; // build the breeding list.

		// breed the next generation
		chromosome child;
		c_sorted::iterator blb = breeding_list.begin();
		c_sorted::iterator ble = breeding_list.end();
		c_sorted::iterator oc = blb;
		c_sorted::iterator ic =  blb;
		while (gen_list.size() < c_count)
		{
			// iterate though deterministicly to build the next generation.
			if (++ic == ble) { oc++; ic = oc; ic++; };
			if (ic == ble) { oc = blb; ic = blb; ic++; };
			try
			{
				if (splice)
				{
					child.splice(oc->second,ic->second,rng() % gensize, rng() % gensize);
				}
				else
				{
					child.recombine(oc->second,ic->second,rng() % gensize);
				};
				gen_list.push_back(child);
			}
			catch (exception & e)
			{
				cerr << "\nError \"" << e.what()
					<< "\" building generation #" << generation+1
					<< endl;
				exit(1);
			};
		};
			
		// introduce random mutations
		unsigned int mutated = 0;
		unsigned int m_count = gen_list.size();
		for (unsigned int i=0; i < m_count; i++)
		{
			if (probability() <= mutations)
			{
				gen_list[i].mutate(rng() % gensize, rng() );
				mutated++;
			};
		};

		// calculate each chromosome's fitness
		for_each(gen_list.begin(),gen_list.end(),fitness_updater());
		
		// clear out the deadwood
		gen_list.erase(
			remove_if(gen_list.begin(),gen_list.end(),dead_chromosome()),		
			gen_list.end() );
			
		// make the sorted list.
		sorted.clear();
		gc = gen_list.begin();
		ge = gen_list.end();
		while (gc != ge)
		{
			sorted[gc->fitness] = *gc;
			gc++;
		};

		generation++;
		gen_time.stop();
		double entropy = sorted.size()*100.0/gen_list.size();

		// report status
		if (generation % gdispmod == 0)
		{
			if (!silent)
			{
				cout << "#" << setw(6) << generation << ": " 
					<< "best " <<  sorted.begin()->second.fitness 
					<< ", worst " << sorted.rbegin()->second.fitness 
					<< ", average "
					<< (sorted.begin()->second.fitness + sorted.rbegin()->second.fitness) /2
					<< " (" << gen_time.interval_as_HMS() << ")."
					<< endl;
				cout << "\tcount: " << gen_list.size()
					<< ", bred: " << breeding_list.size()
					<< ", Mutated: " << mutated 
					<< ", Entropy: " << entropy << "%"
					<< endl;
			}
			else if (!mute)
			{
				cout << "." << flush;
			};
		};

		output << generation 
			<< "\t" << sorted.begin()->second.fitness
			<< "\t" << sorted.rbegin()->second.fitness 
			<< "\t"
			<< (sorted.begin()->second.fitness + sorted.rbegin()->second.fitness) / 2
			<< "\t" << gen_time.interval()
			<< "\t" << gen_list.size() 
			<< "\t" << breeding_list.size()
			<< "\t" << entropy
			<< "\t" << mutated
			<< endl;

		// adjust exit counter.
		if (current_best !=  sorted.begin()->second.fitness)
		{
			current_best = sorted.begin()->second.fitness;
			if (winner.fitness > current_best) 
			{
				winner = sorted.begin()->second;
				first_best = generation;
			};
			sameness = samelimit;
		};
		if (entropy > e_threshold)
		{
			sameness = samelimit;
		};

		// start the clock for the next generation.
		gen_time.reset();
		gen_time.start();
	};

	runtime.stop();
	if (!silent)
	{
		// output the final results.
		cout << "==========================\n\nFinal Best: "
			<< sorted.begin()->second.fitness 
			<< "\n\t" << environment.show_route(sorted.begin()->second)
			<< "\n" << sorted.begin()->second.enstream()
			<< "\n\nAbsolute best: " << winner.fitness 
			<< "\n\t" << environment.show_route(winner) 
			<< "\n" << winner.enstream() 
			<< "\n\n" << generation << " generations run, " 
			<< first_best << " is where the best score was first found."
			<< "\n\nTotal run time was " 
			<< runtime.interval_as_HMS(true) << ".\n"
			<< endl;
	}
	else if (!mute)
	{
		cout << "\nFinal Best = " << sorted.begin()->second.fitness
			<< " (" << runtime.interval_as_HMS(true) << ")"
			<< endl;
	};
	return 0;
};

