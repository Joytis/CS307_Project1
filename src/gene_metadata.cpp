#include "gene_metadata.hpp"


gene_metadata::gene_metadata(std::string&& trait, 
						     char dom_char, std::string&& dom_desc,
						     char rec_char, std::string&& rec_desc) :
	_trait(trait),
	_dominant_char(dom_char), _dominant_phenotype(dom_desc),
	_recessive_char(rec_char), _recessive_phenotype(rec_desc)
{
	reset_genotype_counters();
}

void gene_metadata::increment_genotype(genotype geno) {
	_totals[geno] += 1;
}

void gene_metadata::reset_genotype_counters() {
	_totals.clear();
	_totals[genotype::homozygous_dominant] = 0;
	_totals[genotype::homozygous_recessive] = 0;
	_totals[genotype::heterozygous_dominant] = 0;
}

std::string gene_metadata::get_trait() {
	return _trait;
}
std::string gene_metadata::get_recessive_phenotype() {
	return _recessive_phenotype;
}
std::string gene_metadata::get_dominant_phenotype() {
	return _dominant_phenotype;
}
char gene_metadata::get_recessive_char() {
	return _recessive_char;
}
char gene_metadata::get_dominant_char() {
	return _dominant_char;
}

bool gene_metadata::check_character(char c) {
	return (c == _recessive_char || c == _dominant_char);
}

size_t gene_metadata::get_genotype_count(genotype geno) {
	return _totals[geno];
}

void gene_metadata::reset() {
	reset_genotype_counters();
}

bool gene_metadata::check_gene_pair(std::string& str) {
	// can't be a pair if it isn't size 2!
	if(str.size() != 2) 
		return false;

	// return validity. 
	return check_character(str[0]) && check_character(str[1]);
}

// Small data, so just return a copy.
// Requires input string to be valid and checked by gene pair. 
genotype gene_metadata::parse_pair(std::string& str) {
	allele first;
	allele second;

	// Parse first allele
	if(str[0] == _recessive_char) 
		first = allele::recessive;
	else 
		first = allele::dominant;

	// parse second allele
	if(str[1] == _recessive_char) 
		second = allele::recessive;
	else 
		second = allele::dominant;

	return pair_to_genotype(first, second);

}


genotype gene_metadata::pair_to_genotype(allele first, allele second) {
    if(first == allele::dominant && second == allele::dominant)  {
    	return genotype::homozygous_dominant;
    }
    else if((first == allele::dominant && second == allele::recessive) ||
    	    (first == allele::recessive && second == allele::dominant)) {
    	return genotype::heterozygous_dominant;
    }
	else {
		return genotype::homozygous_recessive;
	}
}

std::ostream& operator<<(std::ostream& os, const gene_metadata& md) {
	os << "Gene Metadata: " << std::endl 
	   << "Trait: " << md._trait << std::endl  
	   << "DominantChar: " << md._dominant_char << std::endl  
	   << "DominantPhenotype: " << md._dominant_phenotype << std::endl  
	   << "RecessiveChar: " << md._recessive_char << std::endl  
	   << "RecessivePhenotype: " << md._recessive_phenotype << std::endl;
	return os;
}
