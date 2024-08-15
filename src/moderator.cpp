#include "moderator.h"

mH2O::mH2O() {
	this->update_macroscopic_absorption_cross_section();
	this->name = "H2O - Water";
}

void mH2O::set_volume(float volume) {
	this->volume = volume;
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
	float macroscopic_scattering_cross_section_water = microscopic_scattering_cross_section_water * calculate_number_density(this->density, this->molar_mass);

	return logarithmic_energy_decrement_per_collision * macroscopic_scattering_cross_section_water * this->volume;
}
