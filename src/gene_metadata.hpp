#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include "allele.hpp"
#include "genotype.hpp"

class gene_metadata {

private:
	std::string _trait;
	char _dominant_char;
	std::string _dominant_phenotype;
	char _recessive_char;
	std::string _recessive_phenotype;

	bool check_character(char c); // checks if character is either a dominant or recessive char. 
	void reset_genotype_counters();

	// running tallies of all genes created with specific genotypes. 
	std::unordered_map<genotype, size_t> _totals;

public:
	gene_metadata(std::string&& trait, 
				  char dom_char, std::string&& dom_desc,
				  char rec_char, std::string&& rec_desc);

	static genotype pair_to_genotype(allele first, allele second);

	std::string get_trait();
	std::string get_recessive_phenotype();
	std::string get_dominant_phenotype();
	char get_recessive_char();
	char get_dominant_char();
	void reset(); // resets the counters on the dictionary. 

	size_t get_genotype_count(genotype geno);
	void increment_genotype(genotype geno);

	bool check_gene_pair(std::string& std); // check if gene pair is valid
	genotype parse_pair(std::string& str); //Parse into alleles. 

	friend std::ostream& operator<<(std::ostream& os, const gene_metadata& m);
};