/*******************************************************************
*   gene.hpp
*   project1_307
*   Author: Clark Chambers
*   Date: 2018-02-16
*   
*   This program is entirely my own work
*******************************************************************/

#pragma once

#include <unordered_map>

#include "gene_metadata.hpp"

class gene {
	// We control this. 
	friend struct std::hash<gene>;

private:
	// allele _first;
	// allele _second;
	genotype _geno;
	gene_metadata* _metadata;

	static std::pair<allele, allele> genotype_to_pair(genotype geno);

public:

	gene();
	gene(genotype geno, gene_metadata* meta);

	std::string get_genotype_string() const;
	std::string get_trait() const;

	bool can_breed_with(gene& other);

	static gene breed(const gene& gene1, const gene& gene2);

	bool operator==(const gene& other) const;
	bool operator<(const gene& other) const;
	bool operator>=(const gene& other) const;
};

// Hash function for organism
namespace std {
    template <>
    struct hash<gene> {
        size_t operator()( const gene& k ) const {
            // Compute individual hash values for first, second and third
            // http://stackoverflow.com/a/1646913/126995
            size_t res = 17;
            res = res * 31 + hash<genotype>()( k._geno );
            res = res * 31 + hash<gene_metadata*>()( k._metadata );
            return res;
        }
    };
}
