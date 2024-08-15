#pragma once

#include <vector>
#include <array>
#include <unordered_map>
#include <utility>
#include <string>

#include "isotopes.h"

struct Fuel {
	virtual ~Fuel() = default;
	Fuel() : avg_neutrons_per_fission(0), fission_cross_section(0), absorption_cross_section(0), macroscopic_absorption_cross_section(0), molar_mass(0) {}

	float avg_neutrons_per_fission;
	float fission_cross_section;
	float absorption_cross_section;
	float macroscopic_absorption_cross_section;
	float molar_mass;

	virtual float calculate_p_numerator() = 0;

	std::vector<std::pair<Isotope, float>> composition;	// isotope, fraction

private:
	float density = 0.0f;						// g/cm³
	float volume = 0.0f;
};

struct UO2 : public Fuel {
	UO2(std::vector<std::pair<Isotope, float>> composition, float volume);
	void update_composition(std::vector<std::pair<Isotope, float>> composition);
	void update_molar_mass();
	void update_macroscopic_absorption_cross_section();
	void set_volume(float volume);

	float calculate_p_numerator() override;

private:
	float density = 10.97f;					// g/cm³
	float volume = 0.0f;
};
