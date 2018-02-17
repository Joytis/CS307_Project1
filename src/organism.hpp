#pragma once

#include <string>
#include <unordered_map>
#include <algorithm>

#include "gene.hpp"

class organism_metadata; // forward declaration

class organism {
	// We control this. 
	friend struct std::hash<organism>;

private:
	organism_metadata* _metadata; // hold reference to associated metadata. 
	std::unordered_map<gene_metadata*, gene> _genes; 

public:
	organism(); // default constructor. 
	organism(organism_metadata* metdata); // constructor. 

	bool can_breed_with(organism& other);
	void set_metadata(organism_metadata* meta);

	std::string get_genotype_string() const;

	bool parse_genotype(std::string&& genotype); // Useful if we decide to parse genotype after constructor. 
	void format_print(std::ostream& os, std::string& title);

	// Apply lambda to every gene. 
	template<typename F>
	void foreach_gene_kvp(F func) const {
		// apply function to all things in metadata
		std::for_each(_genes.begin(), _genes.end(), func);
	}

	static organism breed(organism& first, organism& second); // friendly breeding method. 

	// operatorsp
	bool operator==(const organism &a) const;
	bool operator<(const organism &a) const;

};

// Hash function for organism
namespace std {
    template <>
    struct hash<organism> {
        size_t operator()( const organism& k ) const {
            // Compute individual hash values for first, second and third
            // http://stackoverflow.com/a/1646913/126995
            size_t res = 17;
            res = res * 31 + hash<organism_metadata*>()( k._metadata );
            for(auto& kvp : k._genes) {
            	res = res * 31 + hash<gene>()( kvp.second );
            }
            return res;
        }
    };
}


