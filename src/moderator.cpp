#include "moderator.h"

mH2O::mH2O() {
	this->update_macroscopic_absorption_cross_section();
	this->name = "H2O - Water";
}

void mH2O::update_macroscopic_absorption_cross_section() {
	float waterNumberDensity = calculate_number_density(this->density, this->molar_mass);

	float numberDensityH = waterNumberDensity * 2;
	float numberDensityO = waterNumberDensity;

	this->macroscopic_absorption_cross_section = 0.0f;
	this->macroscopic_absorption_cross_section += (
		isotope_properties[H1]["absorption_cross_section"] * 1e-24f * numberDensityH +
		isotope_properties[O16]["absorption_cross_section"] * 1e-24f * numberDensityO
	);
}

float mH2O::calculate_p_denominator() {
	float logarithmic_energy_decrement_per_collision = 0.92f;
	float microscopic_scattering_cross_section_water = isotope_properties[H1]["elastic_cross_section"] * 2 + isotope_properties[O16]["elastic_cross_section"];

	float slowing_down_power = microscopic_scattering_cross_section_water * logarithmic_energy_decrement_per_collision;
	float macroscopic_slowing_down_power = calculate_number_density(this->density, this->molar_mass) * slowing_down_power;

	return macroscopic_slowing_down_power * logarithmic_energy_decrement_per_collision;
}