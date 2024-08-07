#pragma once

#include <vector>
#include <array>
#include <unordered_map>
#include <utility>
#include <string>

enum Isotope {
	U235,
	U238,
	O16,
	H1
};

extern std::unordered_map<Isotope, std::unordered_map<std::string, float>> isotope_properties;

// Calculates the number density of the material based on density (g/cm^3) and molar mass (g/mol)
inline float calculate_number_density(float density, float molar_mass) {
	float avogadro = 6.0221408e23f;
	return density * avogadro / molar_mass;
}

// Calculates the macroscopic absorption cross section based on number density (atoms/cm^3) and microscopic absorption cross section (barns)
inline float calculate_macroscopic_absorption_cross_section(float number_density, float microscopic_absorption_cross_section) {
	return number_density * microscopic_absorption_cross_section * 1e-24f; // 1 barn = 1e-24 cm^2
}