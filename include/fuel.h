#pragma once

#include <vector>
#include <array>
#include <unordered_map>
#include <utility>
#include <string>

#include "isotopes.h"

struct Fuel {
	Fuel() : avg_neutrons_per_fission(0), fission_cross_section(0), absorption_cross_section(0), macroscopic_absorption_cross_section(0), molar_mass(0) {}

	float avg_neutrons_per_fission;
	float fission_cross_section;
	float absorption_cross_section;
	float macroscopic_absorption_cross_section;
	float molar_mass;

	std::vector<std::pair<Isotope, float>> composition;	// neutrons per fission, fission cross section, absorption cross section, fraction

private:
	float density = 0.0f;						// g/cm^3
};

struct UO2 : public Fuel {
	UO2(std::vector<std::pair<Isotope, float>> composition);
	void update_composition(std::vector<std::pair<Isotope, float>> composition);
	void update_molar_mass();
	void update_macroscopic_absorption_cross_section();

private:
	float density = 10.97f;					// g/cm^3
};