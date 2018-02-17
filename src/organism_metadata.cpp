/*******************************************************************
*   organism_metadata.cpp
*   project1_307
*   Author: Clark Chambers
*   Date: 2018-02-16
*   
*   This program is entirely my own work
*******************************************************************/


#include "organism_metadata.hpp"
#include "organism.hpp"

#include <algorithm>

organism_metadata::organism_metadata(std::string&& genus, std::string&& species, 
								     std::string&& common_name, std::string&& sci_name,
								     std::vector<gene_metadata>&& genotype_meta)
	: _genus(genus), _species(species), _common_name(common_name), 
	  _sci_name(sci_name), _genotype_meta(genotype_meta)
{} // do nothing. 

organism_metadata::organism_metadata() {} // do nothing. 

gene_metadata* organism_metadata::find_metadata_for_pair(std::string& str) {
	auto meta = std::find_if(
		_genotype_meta.begin(), // stard
	    _genotype_meta.end(),  //end
	    [&](gene_metadata& m) -> bool { // condition
	    	return m.check_gene_pair(str);
	    }
    );

	if(meta == _genotype_meta.end()) 
		return nullptr; // no matching iterator
	else
		return &(*meta); // found our metadata. 
}

std::string organism_metadata::get_genus() { return _genus; }
std::string organism_metadata::get_species() { return _species; }
std::string organism_metadata::get_common_name() { return _common_name; }
std::string organism_metadata::get_sci_name() { return _sci_name; }

void organism_metadata::reset() {
	for(auto& meta : _genotype_meta) {
		meta.reset();
	}
}