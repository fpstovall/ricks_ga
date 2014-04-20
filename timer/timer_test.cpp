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


/* hirez_timer test program */

#include <iostream>
#include <unistd.h>
#include "hires_timer.h"

using namespace nrtb;
using namespace std;

int main()
{
	hirez_timer overall;
	hirez_timer interval;
	overall.start();
	interval.start();
	cout << "sleep 1 second" << endl;
	sleep(1);
	cout << overall.interval() << " | " << interval.stop() << " (stop)" << endl;
	cout << "sleep 3 seconds" << endl;
	sleep(3);
	cout << overall.interval() << " | " << interval.stop() << " (start)" << endl;	
	interval.start();
	cout << "sleep 2 seconds" << endl;
	sleep(2);
	cout << overall.interval() << " | " << interval.stop() << " (reset)" << endl;
	interval.reset();
	interval.start();
	cout << "sleep 500000 useconds" << endl;
	usleep(500000);
	cout << overall.interval() << " | " << interval.stop() << endl;
	// test the advanced formationg function
	interval.start(109472.34);
	interval.stop();
	cout << "Extended interval_as_HMS() test: \""
		<< interval.interval_as_HMS(true)
		<< "\" or \"" << interval.interval_as_HMS() << "\"" << endl;
	cout << "Total run time: " << overall.interval() << " seconds." << endl;
	return 0;
};

