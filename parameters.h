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
	global constants for the initial traveling salesman test program.
	Should be included before any of the "local" include files.

	NOTE: As of 2002-02-20, these are the default values only. They 
		may be overridden by command line or stored simulation data
		at run time.
*/

#ifndef parameters_h
#define parameters_h

#include <string>

/************************************
	This group defines the run characteristics.
************************************/
// number of chromosomes per generation
unsigned int c_count= 100000;

// number of viable chromosomes required to start run.
// 0 = none; just create c_count chromosomes without 
// 	validation.
unsigned int v_count = 2;

// number of chromosomes to be placed in the breeding pool.
unsigned int b_count = 500;

// odds of any given chromosome mutating spontainiously.
long double mutations = 1e-6;

// number of highest ranking chromosomes to be transfered without
// modification to the next generation.
unsigned int save_count = 0;

// maximum number of generations to be run.
int genlimit = 1000;

// number of consectutive identical best fitness
// scores required to complete the run.
int samelimit = 50;

// Level that entropy in the system must drop to before
// the simulation is allowed to exit. Expressed in 
// integer percentage.
unsigned int e_threshold = 95;

// file to write the generation results out to
std::string outfile = "tourney.out";

// file to read the "city" list from
std::string infile = "input.lst";
// which "breeding" method to use.
bool splice = true;

#endif // parameters_h
