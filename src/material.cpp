#include "material.h"

void Material::update_molar_mass() {
	this->molar_mass = 0.0f;
	for (std::pair<Isotope, float> isotopeFraction : this->composition) {
		this->molar_mass += isotope_properties[isotopeFraction.first]["molar_mass"] * isotopeFraction.second;
	}
}

void Material::update_macroscopic_absorption_cross_section() {
	this->macroscopic_absorption_cross_section = 0.0f;
	float materialNumberDensity = calculate_number_density(this->density, this->molar_mass);

	for (std::pair<Isotope, float> isotopeFraction : this->composition) {
		float microscopic_absorption_cross_section_isotope = isotope_properties[isotopeFraction.first]["absorption_cross_section"]; // barns to cm^2
		float isotopeNumberDensity = calculate_number_density(this->density, isotope_properties[isotopeFraction.first]["molar_mass"]);
		this->macroscopic_absorption_cross_section += calculate_macroscopic_absorption_cross_section(isotopeNumberDensity, microscopic_absorption_cross_section_isotope) * isotopeFraction.second;
	}
}