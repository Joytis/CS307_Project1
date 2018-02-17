/*******************************************************************
*   simulation.cpp
*   project1_307
*   Author: Clark Chambers
*   Date: 2018-02-16
*   
*   This program is entirely my own work
*******************************************************************/


#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>

#include "simulation.hpp"
#include "GeneticsSimDataParser.hpp"

// Do nothing. 
simulation::simulation() {}

bool simulation::setup(std::string filepath) 
{

	// Spin up a Coleman parser and snag some info from it. 
	GeneticsSimDataParser parser(const_cast<char*>(filepath.c_str()));

	// Parse organism metadata.
	std::string genus(parser.getGenus());
	std::string species(parser.getSpecies());
	std::string sci_name(parser.getScientificName());
	std::string common_name(parser.getCommonName());

	// Screw it. We'll just use a 256k buffers and convert to strings.
	char buffer0[256] = {0};
	char buffer1[256] = {0};
	char buffer2;
	char buffer3[256] = {0};
	char buffer4;

	// hold all the metadata to be moved to organism
	std::vector<gene_metadata> metadata; 

	// Read parent genotypes
	parser.getParentGenotype(buffer0);
	std::string parent_geno_1(buffer0);
	std::memset(buffer0, 0, sizeof(buffer0));
	parser.getParentGenotype(buffer0);
	std::string parent_geno_2(buffer0);
	std::memset(buffer0, 0, sizeof(buffer0));

	// Create metadata for each gene data thing. 
	for(int i = 0; i < parser.getGeneCount(); i++) {
		// Read the metadata from file
		parser.getGeneData(buffer0, buffer1, &buffer2, buffer3, &buffer4);
		std::string trait(buffer0);
		std::string dom_desc(buffer1);
		char domSymbol = buffer2;
		std::string rec_desc(buffer3);
		char recSymbol = buffer4;

		// Stuff it into a new object. 
		// Also, call move on all the buffer data. No need to waste cycles!
		gene_metadata m_data(
			std::move(trait), 
			domSymbol, std::move(dom_desc),
			recSymbol, std::move(rec_desc)
		);

		metadata.push_back(std::move(m_data));
	}

	// Construct a new org metadata. If we care about the speed we're losing on the default 
	//		constructor being called at the beginning of the class, we'll make this a pointer 
	//		or something. 
	_org_data = organism_metadata(std::move(genus), std::move(species),
								  std::move(common_name), std::move(sci_name),
								  std::move(metadata));

	// Setup the parents...
	_parent1.set_metadata(&_org_data); // org data lifetime is same as parent, so it's cool. 
	_parent2.set_metadata(&_org_data); // org data lifetime is same as parent, so it's cool. 
	if(!_parent1.parse_genotype(std::move(parent_geno_1)))
		return false;
	if(!_parent2.parse_genotype(std::move(parent_geno_2)))
		return false;

	return true;
}

void simulation::run(size_t trials) {
	// NO MORE THAN 1000 TRIALS
	if(trials > 1000)
		trials = 1000;

	// Appent a bunch of children. 
	_children.clear();
	_organism_counts.clear();
	_org_data.reset();

	if(_parent1.can_breed_with(_parent2)) {
		for(int i = 0; i < trials; i++) {
			organism child = organism::breed(_parent1, _parent2);
			// Increment table of children by 1. 
			if(_organism_counts.find(child) == _organism_counts.end()) {
				_organism_counts[child] = 0;
			}
			_organism_counts[child] += 1;
			_children.push_back(std::move(child));
		}
	}
	else {
		std::cout << "Parents can't breed!" << std::endl;
	}
}


void simulation::print_master_info(std::ostream& os) {
	os << "Master Genes:" << std::endl;

	// I love you so much, lambdas. 
	_org_data.foreach_gene_metadata([&](gene_metadata& g) {
		os << "\tTrait Name: " << g.get_trait() << std::endl
		   << "\t\tDominant Name: " << g.get_dominant_phenotype() // print name
		   		<< '(' << g.get_dominant_char() << ')' << std::endl // print character
		   << "\t\tRecessive Name: " << g.get_recessive_phenotype()  // print name
		   		<< '(' << g.get_recessive_char() << ')'<< std::endl; // print character
	});	

}
void simulation::print_parent_info(std::ostream& os) {
	static std::string parent_title_1("Sim parent 1");
	static std::string parent_title_2("Sim parent 2");
	_parent1.format_print(os, parent_title_1);
	os << std::endl;
	_parent2.format_print(os, parent_title_2);
	os << std::endl;
}
void simulation::print_results(std::ostream& os) {

	// Gene: Plant Stature
	// 251 homozygous dominant (Tall TT)
	// 498 heterozygous dominant (Tall Tt)
	// 251 homozygous recessive (Dwarf tt)
	os << "======================= Results of this Run =======================" << std::endl << std::endl;
		
	// Print all metadata info. 
	_org_data.foreach_gene_metadata([&](gene_metadata& meta) {

		os << "Gene: " << meta.get_trait() << std::endl;

		// Grab relevant stuff from metadata. 
		char dom_char = meta.get_dominant_char();
		char rec_char = meta.get_recessive_char();
		std::string dom_pheno = meta.get_dominant_phenotype();
		std::string rec_pheno = meta.get_recessive_phenotype();

		// Homo dominant
		os << '\t' << meta.get_genotype_count(genotype::homozygous_dominant) 
		   << " homozygous dominant "<< '(' << dom_pheno << ' ' << dom_char << dom_char << ')' 
		   << std::endl;
		// Hetero dominant
		os << '\t' << meta.get_genotype_count(genotype::heterozygous_dominant) 
		   << " heterozygous dominant "<< '(' << dom_pheno << ' ' << dom_char << rec_char << ')' 
		   << std::endl;
		// Homo recessive. 
		os << '\t' << meta.get_genotype_count(genotype::homozygous_recessive) 
		   << " homozygous recessive "<< '(' << rec_pheno << ' ' << rec_char << rec_char << ')' 
		   << std::endl << std::endl;
	});

	// Print all organism permutations and count 
	os << "------------------ Organism permutations ------------------" << std::endl << std::endl;

	for(auto& kvp : _organism_counts) {
		std::cout << "Genotype = " << kvp.first.get_genotype_string() << "   " 
				  << "Offspring = " << kvp.second << std::endl;
	}
}	