#include <cmath>
#include <iostream>
#include <vector>
#include <array>

#include "fuel.h"
#include "simulation.h"
#include "isotopes.h"

int main() {
	std::vector<std::pair<Isotope, float>> composition{
		{ U235, 0.03f },
		{ U238, 0.97f }
	};

	UO2 fuel(composition);
	mH2O moderator;
	cH2O coolant;

	smZircaloy2 fuelCladding;
	smSA508_Steel pressureVessel;
	BoronCarbide controlRods;

	Simulation simulation(fuel, moderator, coolant, fuelCladding, pressureVessel, controlRods);
	simulation.step();
	std::cout << "Keff: " << simulation.getKeff() << std::endl;
	std::cout << "Eta: " << simulation.getEta() << std::endl;
	std::cout << "F: " << simulation.getF() << std::endl;

	return 0;
}