#pragma once

#include "isotopes.h"
#include <string>

struct Coolant {
	Coolant() : macroscopic_absorption_cross_section(0), name("None") {}
	float macroscopic_absorption_cross_section;

	std::string name;
};

struct cH2O : public Coolant {
	cH2O();
	void update_macroscopic_absorption_cross_section();

private:
	float density = 1.0f; // g/cm^3
	float molar_mass = 18.01528f; // g/mol
};