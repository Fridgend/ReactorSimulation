#pragma once

#include <vector>
#include <array>
#include <unordered_map>
#include <utility>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include <chrono>
#include <ctime>

#include "isotopes.h"

struct Fuel {
	virtual ~Fuel() = default;
	Fuel() : avg_neutrons_per_fission(0), fission_cross_section(0), absorption_cross_section(0), macroscopic_absorption_cross_section(0), molar_mass(0) {}

	float avg_neutrons_per_fission;
	float fission_cross_section;
	float absorption_cross_section;
	float macroscopic_absorption_cross_section;
	float molar_mass;

	virtual float calculate_p_numerator(float moderator_scattering) = 0;
	virtual void write_total_macroscopic() = 0;

	std::vector<std::pair<Isotope, float>> composition;	// isotope, fraction

private:
	float density = 0.0f;						// g/cm³
};

struct UO2 : public Fuel {
	UO2(std::vector<std::pair<Isotope, float>> composition);
	void update_composition(std::vector<std::pair<Isotope, float>> composition);
	void update_molar_mass();
	void update_macroscopic_absorption_cross_section();

	float calculate_p_numerator(float moderator_scattering) override;
	void write_total_macroscopic() override;

private:
	float density = 10.97f;					// g/cm³
};
