#include <iostream>

#include "simulation.hpp"

int main(int argc, const char** argv) {

	if(argc != 2) {
		std::cout << "USAGE INFORMATION" << std::endl;
		return -1;
	}

	std::string filepath(argv[1]);
	// std::string num_trials(argv[2]);


	simulation sim;
	if(!sim.setup(filepath)) {
		std::cout << "Organism did NOT set up correctly" << std::endl;
		return -1;
	}
	sim.print_master_info(std::cout);
	sim.print_parent_info(std::cout);

	// Query for user input with trials. 
	size_t num_trials;
	std::cout << "How many offspring do you want to generate?"
			  << " (Type the number then press Enter)" << std::endl << "==>";
	std::cin >> num_trials;

	sim.run(num_trials);

	sim.print_results(std::cout);

    return 0;
}
