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

	Fuel* fuel = new UO2(composition);
	Moderator* moderator = new mH2O;
	Coolant* coolant = new cH2O;

	Material* fuelCladding = new smZircaloy2;
	Material* pressureVessel = new smSA508_Steel;
	Material* controlRods = new BoronCarbide;

	Simulation simulation(fuel, moderator, coolant, fuelCladding, pressureVessel, controlRods);
	simulation.step();
	std::cout << "Keff: " << simulation.getKeff() << std::endl;
	std::cout << "Eta: " << simulation.getEta() << std::endl;
	std::cout << "F: " << simulation.getF() << std::endl;
	std::cout << "P: " << simulation.getP() << std::endl;
	std::cout << "Epsilon: " << simulation.getEpsilon() << std::endl;

	return 0;
}
