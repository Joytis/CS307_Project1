#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "gene_metadata.hpp"
#include "organism.hpp"

class organism; // forward declaration

class organism_metadata {

private:
	std::string _genus;
	std::string _species;
	std::string _common_name;
	std::string _sci_name;
	std::vector<gene_metadata> _genotype_meta;

public:
	// only accept moves for this constructor, because this should only be
	//		made during parsing of a file.
	organism_metadata();
	organism_metadata(std::string&& _genus, std::string&& _species, 
				      std::string&& _common_name, std::string&& _sci_name,
				      std::vector<gene_metadata>&& _genotype_meta);

	std::string get_genus();
	std::string get_species();
	std::string get_common_name();
	std::string get_sci_name();

	void reset(); // Doesn't reset anything, but tells the gene metadata to reset. 

	gene_metadata* find_metadata_for_pair(std::string& str);

	// Templates here must be defined in header file. 
	template<typename F>
	void foreach_gene_metadata(F func) {
		// apply function to all things in metadata
		std::for_each(_genotype_meta.begin(), _genotype_meta.end(), func);
	}
};