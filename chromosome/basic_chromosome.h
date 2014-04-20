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
/* basic_chromosome.h - presents the basic chromosome template.
*/

#ifndef basic_chromosome_h
#define basic_chromosome_h

#include <stdio.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include <iostream>

namespace ricks_ga
{

/** Simple chromosome template. Provides mutation and simple cross-over
 ** "breeding" methods, as well as support for enstreaming and destreaming
 ** (putting into and retreaving from a character storable format. Limited 
 ** acccess to the individual genes and method to return a valid but random
 ** gene index are also provided.
 ** 
 ** basic_chromosome should work with any integer (char, short, long, etc.) 
 ** type without modification. By overiding the *stream and mutate functions
 ** other data types can be supported as needed.
 **/
template <class G>
class basic_chromosome
{
	private:
		std::vector<G> genlist;
		int mutation_index;
		int mutation_value;
	protected:
	public:
	
		/// Parent for all basic_chromosome exceptions.
		class general_exception: public std::exception {};
		/** Thrown if there is an attempt to access a gene that does not 
		 ** exist in this chromosome.
		 ** 
		 ** May be thrown by the operator [] if the supplied index is 
		 ** < 0 or greater than length()-1.
		 **/
		class index_error: public general_exception {};
		/** Thrown if there is an unexpected error in one of the 
		 ** recombine() methods.
		 ** 
		 ** Most like cause would be running out of memory during the operation.
		 **/
		class recombine_error: public general_exception {};
		/** Thrown by rand_index in the case of an unexpected error.
		 ** 
		 ** Likely causes include failure to seed the random number generator 
		 ** (via the srand48() or seed48() calls) prior to it's first call.
		 **/
		class rand_index_error: public general_exception {};
		/** Thrown if there is an unexpected error in one of the mutate() 
		 ** methods.
		 ** 
		 ** Most likely cause would be an invalid gene index provided.
		 **/
		class mutate_error: public general_exception {};
		/** Thrown by enstream in the case of an unexpected error.
		 ** 
		 ** Possible causes include lack of memory or the use of a datatype
		 ** for the template that does not work well with the snprintf() "%x"
		 ** operator.
		 **/
		class enstream_error: public general_exception {};
		/** Thrown if an error occurs while destreaming a stored chromosome.
		 ** 
		 ** Most likely causes include a data type mismatch between the streamed
		 ** chromosome and the destreaming one, input data not in the same format
		 ** created by enstream(), or lack of memory.
		 **/
		class destream_error: public general_exception {};
		/** Thrown if an error occurs in the splice() methods.
		 ** 
		 ** Most likely causes are index out of bounds or memory starvation.
		 **/
		class splice_error: public general_exception{};

		/** Simple constructor. Creates a chromosome with 0 genes in it.
		 **/			
		basic_chromosome();
		/** Creates a chromosome with length genes with randomly assigned values.
		 ** 
		 ** Behavor is undefined if the srand random number generator is not 
		 ** initialized (see man srand for more information).
		 **/
		basic_chromosome(unsigned int length);
		/** Deallocates memory and destructs object.
		 **/
		~basic_chromosome();
		/** Returns the number of genes in this chromosome.
		 **/
		void reload(unsigned int length);
		/** Loads this gene with new values from two parents using a random
		 ** crossover point.
		 ** 
		 ** Recombination is a simple way to create a new chromosome with 
		 ** traits from two parents. Given two chromosomes, a and b, the new gene
		 ** is created using the following steps:
		 ** 
		 ** 1. A crossover point is selected. This should be an integer between 0
		 ** and the length of the first chromosome. In this method, the crossover
		 ** is selected randomly.
		 ** 
		 ** 2. All the genes from beginning of the a chromosome to the crossover
		 ** point are copied to the new gene.
		 ** 
		 ** 3. All the genes from the crossover point on the b chromosome to the 
		 ** end are copied onto the end of the new gene.
		 ** 
		 ** In this case, the new gene is the object whose method is called. Any 
		 ** previously exiting genes in this object are replaced with the new 
		 ** information.
		 **/
		void recombine(basic_chromosome<G> &a, basic_chromosome<G> &b);
		/** Loads this gene with new values from two parents using a supplied
		 ** crossover point.
		 ** 
		 ** See recombine(a,b) for a detailed description of how recombination 
		 ** works. The only difference between this function and the other is that
		 ** the crossover point is supplied by the caller instead of being 
		 ** assigned randomly.
		 ** 
		 ** If crossover is out of bounds for the a or b chromosome a 
		 ** recombine_error will be thrown.
		 **/
		void recombine(basic_chromosome<G> &a, basic_chromosome<G> &b,
			unsigned int crossover);
		/** Loads this chromosome with the result of splicing parent chromosomes.
		 ** 
		 ** Requires that both chromosomes be the same length.
		 ** 
		 ** Loads this chromosome with the results of splicing a section of 
		 ** chromosome b into chromosome a. Splicing builds a child from two
		 ** parents in the following fashion: a start point and an end point 
		 ** selected and the section of chromosome a between the start and end
		 ** points is replaced by genes from chromosome b.
		 ** 
		 ** This version of splice() determines the section spliced randomly
		 ** and preserves the length of the a chromosome in the child.
		 **/
		void splice(basic_chromosome<G> &a, basic_chromosome<G> &b);
		/** Loads this chromosome with the result of a user defined splice 
		 ** of two parents.
		 ** 
		 ** Requires both chromosomes be the same length.
		 ** 
		 ** See splice(a,b) for a description of the process of chromosome
		 ** splicing. This method differs from splice(a,b) in that the caller
		 ** gets to define the start and the end of the spliced section. If 
		 ** end is actually less than start, the spliced section from 
		 ** chromosome b in inserted into a in reverse order. In all cases the 
		 ** length of the a chromosome is preserved in the resulting child.
		 ** 
		 ** This method does the actual work for all the splicing methods.
		 **/
		void splice(basic_chromosome<G> &a, basic_chromosome<G> &b, 
			unsigned int begin, unsigned int end);
		/** Mutates one randomly selected gene randomly.
		 ** 
		 ** A randomly selected gene in this chromosome is changed to new randomly
		 ** selected value. Note: it is possible, though unlikely, that the gene
		 ** will be randomly set to it's previously existing value, leading to no
		 ** effect change in the chromosome. It's okay.. that's why they call them
		 ** random mutations.... ;)
		 **/
		int mutate();
		/** Randomly mutates the selected gene.
		 ** 
		 ** The gene at index which is loaded with a new, randomly selected value.
		 ** Of course, it's possible that the new randomly selected value will be 
		 ** the same as the orignal one, particularly in the case of single byte 
		 ** genes (odds are 1 in 256 in that case). Get over it.
		 ** 
		 ** If which is out of bounds for this chromosome, a mutate_error is 
		 ** thrown.
		 **/
		int mutate(unsigned int which);
		/** Sets the selected gene to the selected  value.
		 ** 
		 ** The gene at index which is loaded with the value supplied. This method 
		 ** does the actual work for the other mutate() methods. 
		 ** 
		 ** If which is out of bounds for this chromosome, a mutate_error is 
		 ** thrown.
		 **/
		int mutate(unsigned int which, G value);
		/** Encodes the chromosome into a std::string.
		 ** 
		 ** Very useful for storing or displaying the chromosome. The format is  
		 ** simply a series of hex encoded gene values, each gene prefixed by 
		 ** "0x" with no seperating spaces. For example a byte chromosome with 
		 ** two entries could be enstreamed as "0xab0xf2"; a short int chromosome
		 ** with three entries might be "0xa3000x00710x5cd8". No so easy for humans 
		 ** to read, but easy for a computer. 
		 ** 
		 ** See destream() for a way to load a chromosome from a string generated
		 ** by this method.
		 **/
		std::string enstream();
		/** Load from an enstreamed chromosome.
		 ** 
		 ** The source string is assumed to have been created by the enstream 
		 ** method of a chromosome with the same gene type (char, int, etc.)
		 ** as the destreaming one. Things can get pretty weird if the types are 
		 ** not the same.
		 ** 
		 ** See enstream() for a discussion of the expected format for the input
		 ** string.
		 **/
		void destream(std::string source);
		/** Returns the number of genes in this chromosome.
		 ** 
		 ** This is _not_ the storage requiremented for the chromosome; genes may 
		 ** take up any number of bytes as determined by the datatype used to 
		 ** instanciate the template.
		 **/
		unsigned int length();
		/** Array-like gene access operator.
		 ** 
		 ** This does not allow the caller to change the value of the gene, 
		 ** just to see it. Use the mutate() methods to change the value.
		 **/
		const G& operator [](unsigned int index);	
		/** Returns a valid random gene index.
		 ** 
		 ** The returned value will be between 0 and length()-1, inclusive.
		 **/
		unsigned int rand_index();
};

// definition starts below

template <class G> unsigned int basic_chromosome<G>::rand_index()
{
	try
	{
		long double working = drand48();
		working *= genlist.size();
		return (int) floor(working);
	}
	catch (...)
	{
		std::cerr << "\n" << __FILE__ << ":" << __FUNCTION__
			<< ": Unexpected error." << std::endl;
		throw rand_index_error();
	};
};

template <class G> basic_chromosome<G>::basic_chromosome()
{
	genlist.clear();
	mutation_index = -1;
	mutation_value = 0;
};

template <class G> basic_chromosome<G>::basic_chromosome(unsigned int length)
{
	reload(length);
};

template <class G> basic_chromosome<G>::~basic_chromosome()
{
	genlist.clear();
};

template <class G> void basic_chromosome<G>::reload(unsigned int length)
{
	mutation_index = -1;
	mutation_value = 0;	
	for (unsigned int i=0; i < length; i++)
	{
		genlist.push_back(lrand48());
	};	
};

template <class G> void basic_chromosome<G>::recombine(basic_chromosome<G> &a, 
	basic_chromosome<G> &b)
{
	// get a random location for the cross-over
	recombine(a,b,a.rand_index());
};

template <class G> void basic_chromosome<G>::recombine(basic_chromosome<G> &a, 
	basic_chromosome<G> &b, unsigned int crossover)
{
	try
	{
		if ((a.length() > crossover) && (b.length() > crossover))
		{
			// clear our the current genlist contents
			genlist.clear();
			// move the first part of a to our genlist
			for (unsigned int i = 0; i < crossover; i++)
			{
				genlist.push_back(a[i]);
			};
			// move the second part of b to our genlist
			for (unsigned int i = crossover; i < b.length(); i++)
			{
				genlist.push_back(b[i]);
			};
		}
		else 
		{
			throw recombine_error();
		};
	}
	catch (...)
	{
		std::cerr << "\n" << __FILE__ << ":" << __FUNCTION__ 
			<< ": Errored\n\t(a.length=" << a.length() 
			<< ", b.length=" << b.length()
			<< ", crossover=" << crossover 
			<< ")" << std::endl; 
		throw recombine_error();
	};
};

// new section

template <class G> void basic_chromosome<G>::splice(basic_chromosome<G> &a, 
	basic_chromosome<G> &b)
{
	// get a random location for the start and end.
	splice(a,b,a.rand_index(),a.rand_index());
};

template <class G> void basic_chromosome<G>::splice(basic_chromosome<G> &a, 
	basic_chromosome<G> &b, unsigned int start, unsigned int end)
{
	// only proceed if both start and end are in bounds...
	if ((start >= 0) && (start < a.length())
	  && (end >= 0) && (end < a.length())
	  && (a.length() == b.length()))
	{
		// do the splice.
		try
		{
			genlist.clear();
			// which way are we going?
			bool reverse = start > end;
			unsigned int astart = std::min(start,end);
			unsigned int aend = std::max(start,end);
			unsigned int diff = aend - astart;
			// load the first part of our chromosome.
			for (unsigned int i=0; i < astart; i++)
			{
				genlist.push_back(a[i]);
			};
			// load the second.
			for (unsigned int i=0; i <= diff; i++)
			{
				int work_i = reverse ? (aend - i) : (astart+i);
				genlist.push_back(b[work_i]);
			};
			// load the last section.
			for (unsigned int i = aend+1; i < a.length(); i++)
			{
				genlist.push_back(a[i]);
			};
		}
		catch (...)
		{
			std::cerr << "\n" << __FILE__ << ":" << __FUNCTION__ 
				<< ": Errored\n\t(a.length=" << a.length() 
				<< ", b.length=" << b.length()
				<< ", start=" << start
				<< ", end=" << end 
				<< ")" << std::endl; 
			throw splice_error();
		};
		if (a.length() != length())
		{
			std::cerr << "\n" << __FILE__ << ":" << __FUNCTION__
				<< ": child/parent length mismatch\n\t"
				<< "(parent length=" << a.length()
				<< " child length=" << length()
				<< ")" << std::endl;
			throw splice_error();
		};
	}
	else
	{
		// start or end was out of bounds.
		std::cerr << "\n" << __FILE__ << ":" << __FUNCTION__ 
			<< ": Bounds Error\n\t(a.length=" << a.length() 
			<< ", b.length=" << b.length()
			<< ", start=" << start
			<< ", end=" << end 
			<< ")" << std::endl; 
		throw splice_error();
	};
};

// end new section

template <class G> int basic_chromosome<G>::mutate()
{
	G value = lrand48();
	return mutate(rand_index(),value);
};

template <class G> int basic_chromosome<G>::mutate(unsigned int which)
{
	G value = lrand48();
	return mutate(which, value);
};

template <class G> int basic_chromosome<G>::mutate(unsigned int which, G value)
{
	if (which < genlist.size())
	{
		genlist[which] = value;
		mutation_index = which;
		mutation_value = value;
	}
	else
	{
		std::cerr << "\n" << __FILE__ << ":" << __FUNCTION__
			<< ":which out of range.\n\tRecieved " 
			<< which << ", allowable range is 0 to " << genlist.size()-1 << "."
			<< std::endl;
		throw mutate_error();
	};
	return which;
};

template <class G> std::string basic_chromosome<G>::enstream()
{
	try
	{
		std::string returnme;
		char workingstring[100];
		returnme = "";
		for (unsigned int i=0; i< genlist.size(); i++)
		{
			snprintf(workingstring,100,"0x%x",genlist[i]);
			returnme += workingstring;
		};
		return returnme;
	}
	catch (...)
	{
		std::cerr << "\n" << __FILE__ << ":" << __FUNCTION__ 
			<< ": Unexpected error." << std::endl;
		throw enstream_error();
	};
};

template <class G> void basic_chromosome<G>::destream(std::string source)
{
	try
	{
		genlist.clear();
		while (source.length())
		{
			// break off the next piece
			std::string temp;
			int loc = source.find("0x",2);
			if (loc != std::string::npos)
			{
				temp = source.substr(0,loc);
				source.erase(0,loc);
			}
			else
			{
				temp = source;
				source = "";
			};
			G value = strtoul(temp.c_str(),NULL,16);
			genlist.push_back(value);
		};
	}
	catch (...)
	{
		std::cerr << "\n" << __FILE__ << ":" << __FUNCTION__
			<< ": Unexpected error." << std::endl;
		throw destream_error();
	};
};

template <class G> unsigned int basic_chromosome<G>::length()
{
	return genlist.size();
};

template <class G> const G& basic_chromosome<G>::operator [](unsigned int index) 
{
	if (index < genlist.size())
	{
		return genlist[index];
	}
	else
	{
		std::cerr << "\n" << __FILE__ << ":[]:Index out of range.\n"
			<< "Recieved " << index << ", allowable range is 0 to "
			<< genlist.size()-1 << "."
			<< std::endl;
		throw index_error();
	};
};

}// namespace ricklib

#endif
