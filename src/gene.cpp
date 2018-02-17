#include "gene.hpp"

#include <random> 
#include <chrono>

gene::gene() {} // do nothing
gene::gene(genotype geno, gene_metadata* meta) 
	: _geno(geno), _metadata(meta)
{
	// Increment the current gene type in metadata. 
	_metadata->increment_genotype(_geno);
}

std::pair<allele, allele> gene::genotype_to_pair(genotype geno) {
	static std::unordered_map<genotype, std::pair<allele, allele>> table = {
		{genotype::homozygous_dominant, std::make_pair(allele::dominant, allele::dominant)}, 
		{genotype::heterozygous_dominant, std::make_pair(allele::dominant, allele::recessive)}, 
		{genotype::homozygous_recessive, std::make_pair(allele::recessive, allele::recessive)} 
	};

	// create allales from the genotypes 
    return table[geno];
}
	

std::string gene::get_genotype_string() const {
	// Make sure to order such that greatest comes first. 
	std::string ret_val = "";
	// Homozygus dominant
	switch(_geno) {
		case genotype::homozygous_dominant:
			ret_val += _metadata->get_dominant_char();
			ret_val += _metadata->get_dominant_char();
			break;
		case genotype::heterozygous_dominant:
			ret_val += _metadata->get_dominant_char();
			ret_val += _metadata->get_recessive_char();
			break;
		case genotype::homozygous_recessive:
			ret_val += _metadata->get_recessive_char();
			ret_val += _metadata->get_recessive_char();
			break;
	}

	// Copy prevented due to copy elision :)
	return ret_val;
}

std::string gene::get_trait() const {
	return _metadata->get_trait(); // Not stored locally. Query metadata. 
}

bool gene::can_breed_with(gene& other) {
	return _metadata == other._metadata;
}

gene gene::breed(const gene& gene1, const gene& gene2) {
	// Only this function cares about random generators. So we don't 
	// Muddle other people's code. 
	static auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	static std::mt19937 rd(seed);
    static std::uniform_real_distribution<> dist(0.0, 1.0);

    std::pair<allele, allele> gene1_pair = genotype_to_pair(gene1._geno);
    std::pair<allele, allele> gene2_pair = genotype_to_pair(gene2._geno);
    // pick new gene randomly.
    allele new_first;
    if(dist(rd) < 0.5) 
    	new_first = gene1_pair.first;
    else
    	new_first = gene1_pair.second;

    // pick new gene2 randomly
    allele new_second;
    if(dist(rd) < 0.5) 
    	new_second = gene2_pair.first;
    else
    	new_second = gene2_pair.second;

    // Calculate new genotype. 
	genotype new_geno = gene_metadata::pair_to_genotype(new_first, new_second);

	// Copy prevented due to copy elision :)
    return gene(new_geno, gene1._metadata);
}

bool gene::operator==(const gene& other) const{
	return (_geno == other._geno) &&
		   (_metadata == other._metadata);
}
