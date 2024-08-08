#include "coolant.h"

cH2O::cH2O() {
	this->update_macroscopic_absorption_cross_section();
	this->name = "H2O - Water";
}

void cH2O::update_macroscopic_absorption_cross_section() {
	float waterNumberDensity = calculate_number_density(this->density, this->molar_mass);

	float numberDensityH = waterNumberDensity * 2;
	float numberDensityO = waterNumberDensity;

	this->macroscopic_absorption_cross_section = 0.0f;
	this->macroscopic_absorption_cross_section += (
		isotope_properties[H1]["absorption_cross_section"] * 1e-24f * numberDensityH +
		isotope_properties[O16]["absorption_cross_section"] * 1e-24f * numberDensityO
		);
}