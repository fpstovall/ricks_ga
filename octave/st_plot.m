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

function st_plot(totals)
	x=size(totals)(1);
	axis ([1,x-1,min(totals(:,2))-100, max(totals(:,4))+100]);
	grid on;
	ylabel "distance";
	xlabel "generations";
	temp = sprintf("title (\"Traveling Salesman Results with %d datasets (best=%f)\")",totals(1,1),min(totals(:,2)));
	eval(temp);
	hold off;
	plot (totals(:,2),"2;Best;",totals(:,5),"3;Average;",totals(:,4),"1;Worst;");
endfunction
