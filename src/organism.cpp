/*******************************************************************
*   organism.cpp
*   project1_307
*   Author: Clark Chambers
*   Date: 2018-02-16
*   
*   This program is entirely my own work
*******************************************************************/


#include "organism.hpp"
#include "organism_metadata.hpp"


#include <sstream>
#include <iterator>
#include <algorithm>
#include <iostream>	
#include <functional>

organism::organism() {} // do nothing. 

organism::organism(organism_metadata* meta)
	: _metadata(meta)
{} // do nothing;

// explicit move constructor. 
// organism::organism(organism&& org) 
// 	: _metadata(org._metadata), _genes(std::move(org._genes))
// {}

void organism::set_metadata(organism_metadata* meta) { _metadata = meta; }

std::string organism::get_genotype_string() const {
	std::string res = "";
	for(auto& kvp : _genes) {
		res += kvp.second.get_genotype_string() + " ";
	}
	// remove trailing whitespace. 
	if(res.size() > 1) {
		res.pop_back();
	}
	return res;
}

bool organism::parse_genotype(std::string&& genotype) {
	// Split string into gene pairs. 
	std::istringstream iss(genotype);
	std::vector<std::string> gene_strings;
	std::copy(std::istream_iterator<std::string>(iss), //start iterator for string stream
	     	  std::istream_iterator<std::string>(),	 // end iterator for string stream. 
	     	  std::back_inserter(gene_strings));  // Back inserter for vector. 

	// start off with a vector. If we have any bad data, just drop it. 
	std::unordered_map<gene_metadata*, gene> genes;
	for(auto& str : gene_strings) {
		// CAN ONLY PARSE GENE PAIRS.
		if(str.size() != 2) {
			return false;
		}
			
		// Check if a valid metadata exists for pair. 
		gene_metadata* g_meta = _metadata->find_metadata_for_pair(str);
		if(g_meta == nullptr) {  // we didn't find a pair. 
			std::cout << "Did not find a matching gene pair" << std::endl;
			return false;
		}
		auto allele_pair = g_meta->parse_pair(str);

		// Add gene to gene list. 
		if(genes.find(g_meta) == genes.end()) {
			genes[g_meta] = gene(allele_pair, g_meta);
		}
		else { // ... If it isn't there already, that is 
			std::cout << "Organism has two gene pairs of the same type." << std::endl;
			return false;
		}
	}

	// if we got all the way here, we successfully parsed things!
	_genes = std::unordered_map<gene_metadata*, gene>(std::move(genes)); // move into private _genes

	return true;
}

bool organism::can_breed_with(organism& other) {
	if(_metadata != other._metadata) 
		return false;

	// kvp.first is typeof gene_metadata*
	// kvp.second is typeof gene
	for(auto& kvp : _genes) {
 		auto found = other._genes.find(kvp.first);

		// If not in other map...
		if(found == other._genes.end())
			return false; // ... then we can't breed

		// If the matching genes can't breed. 
		if(!found->second.can_breed_with(kvp.second)) {
			return false; // ... then we can't breed :c
		}
	}

	// .. otherwise, we're all good. 
	return true;
}

organism organism::breed(organism& first, organism& second) {

	std::unordered_map<gene_metadata*, gene> new_genes;

	for(auto& kvp : first._genes) {
		// Get relevant data 
		auto meta = kvp.first;
		auto other_gene = second._genes.find(meta);

		// Breed two genes together and add into new map. 
		new_genes[meta] = gene::breed(kvp.second, other_gene->second);
	}

	// They can breed, so they have the same metadata. Thus child has same metadata. 
	organism org(first._metadata); 
	org._genes = std::move(new_genes); // move values into new organism genes. 

	// Copy prevented due to copy elision :)
	return org;
}

void organism::format_print(std::ostream& os, std::string& title) {
	os << title << std::endl
	   << "\tOrganism genus-species: " << std::endl
	   << "\tCommon name: " << std::endl
	   << "\tGenes: " << std::endl;
	for(auto& kvp : _genes) {
		gene& g = kvp.second;
		os << "\t\tGene Type = " << g.get_trait() << std::endl 
		   << "\t\t\tGenotype = " << g.get_genotype_string() << std::endl;
	}
}

bool organism::operator==(const organism& other) const {
	return (_metadata == other._metadata) && (_genes == other._genes);
}

// We're going to sort by string only. That's how the data will be output.
bool organism::operator<(const organism& other) const {
	return get_genotype_string() < other.get_genotype_string();
}