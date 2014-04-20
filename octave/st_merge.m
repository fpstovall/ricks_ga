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

function out = st_merge(ag,new)
## out and ag are assumed to have the same row format:
## count,best,sum of avg,worst,avg 
## new is the data from salesman_tourney
	## make the needed space
	rows = size(new)(1);
	out = zeros(max(rows,size(ag)(1)),5);
	## add'm up
	for i=[1:rows]
		## skip summing ag if necessary
		if size(ag)(1) <= i
			out(i,1) = 1;
			out(i,2) = new(i,2);
			out(i,3) = new(i,2);
			out(i,4) = new(i,2);
		else
			## count
			out(i,1) = ++ag(i,1);
			## best score
			out(i,2) = min(ag(i,2),new(i,2));
			## sum of average scores
			out(i,3) = ag(i,3)+new(i,2);
			## worst score
			out(i,4) = max(ag(i,4),new(i,2));
		end
	end
	## adjust the average 
	for i=[1:size(out)(1)]
		out(i,5) = out(i,3)/out(i,1);
	end;
endfunction
