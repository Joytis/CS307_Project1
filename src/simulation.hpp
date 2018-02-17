/*******************************************************************
*   simulation.hpp
*   project1_307
*   Author: Clark Chambers
*   Date: 2018-02-16
*   
*   This program is entirely my own work
*******************************************************************/


#pragma once

#include <string>
#include <map>

#include "organism_metadata.hpp"
#include "organism.hpp"
#include "gene_metadata.hpp"

class simulation {

private:

	organism _parent1;				// first parent to be bred
	organism _parent2;				// second parent to be bred
	organism_metadata _org_data;	// storage for organism metadata. 

	std::vector<organism> _children;	
	std::map<organism, size_t> _organism_counts;
	

public:
	simulation();

	// attempts to setup simulation by parsing the given filepath. 
	bool setup(std::string filepath); 

	void print_master_info(std::ostream& os);
	void print_parent_info(std::ostream& os);
	void print_results(std::ostream& os);

	void run(size_t trials);

};