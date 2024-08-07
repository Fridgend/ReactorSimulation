#include "moderator.h"

mH2O::mH2O() {
	update_macroscopic_absorption_cross_section();
	this->name = "H2O - Water"
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