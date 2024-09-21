#pragma once

#include <vector>
#include <utility>
#include <chrono>
#include <ctime>

#include "fuel.h"
#include "moderator.h"
#include "coolant.h"
#include "material.h"
#include "isotopes.h"

class Simulation {
public:
	Simulation(
		Fuel* fuel, Moderator* moderator, Coolant* coolant,
		Material* fuelCladding, Material* pressureVessel,
		Material* controlRods
	) :
		fuel(fuel), moderator(moderator), coolant(coolant),
		Keff(0), eta(0), epsilon(0), p(0), pth(0), f(0), fth(0),
		fuelCladding(fuelCladding), pressureVessel(pressureVessel),
		controlRods(controlRods)
	{}

	void step();
	void set_fuel(Fuel* fuel);
	void set_moderator(Moderator* moderator);
	void set_coolant(Coolant* coolant);

	float calculateKeff();
	float calculateEta();
	float calculateEpsilon();
	float calculateP();
	float calculatePth();
	float calculateF();
	float calculateFth();

	void write_total_macroscopic_cross_section_system();

	float getKeff() { return this->Keff; }
	float getEta() { return this->eta; }
	float getEpsilon() { return this->epsilon; }
	float getP() { return this->p; }
	float getPth() { return this->pth; }
	float getF() { return this->f; }
	float getFth() { return this->fth; }

private:
	float Keff;								// Effective multiplication factor

	float eta;								// Reproduction factor
	float epsilon;								// Fast fission factor
	float p;								// Resonance escape probability
	float pth;								// Thermal nonleakage probability
	float f;								// Thermal utilization factor
	float fth;								// Fast nonleakage factor

	Fuel* fuel;
	Moderator* moderator;
	Coolant* coolant;

	Material* fuelCladding;
	Material* pressureVessel;
	Material* controlRods;

	float controlRodsDepth = 0.0f;
	float maxControlRodFraction = 0.7f;
};
