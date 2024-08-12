#include "fuel.h"

UO2::UO2(std::vector<std::pair<Isotope, float>> composition) {
	update_composition(composition);
	update_molar_mass();
	update_macroscopic_absorption_cross_section();
}

void UO2::update_macroscopic_absorption_cross_section() {
	this->macroscopic_absorption_cross_section = 0.0f;
	for (int i = 0; i < this->composition.size(); i++) {
		this->macroscopic_absorption_cross_section += (
			isotope_properties[this->composition[i].first]["absorption_cross_section"] * 1e-24f			// absorption cross section of isotope (in cm^2)
			* this->composition[i].second * calculate_number_density(this->density, this->molar_mass)	// number density of isotope
		);
	}
}

void UO2::update_molar_mass() {
	this->molar_mass = 0;
	for (int i = 0; i < this->composition.size(); i++) {
		this->molar_mass += (
			(isotope_properties[this->composition[i].first]["molar_mass"] * this->composition[i].second)	// molar mass of isotope
			+ (isotope_properties[O16]["molar_mass"] * 2)													// molar mass of 2 oxygen
		);
	}
}

void UO2::update_composition(std::vector<std::pair<Isotope, float>> composition) {
	this->composition = composition;

	this->avg_neutrons_per_fission = 0;
	for (int i = 0; i < this->composition.size(); i++) {
		this->avg_neutrons_per_fission += isotope_properties[this->composition[i].first]["neutrons_per_fission"] * this->composition[i].second;
	}

	this->fission_cross_section = 0;
	for (int i = 0; i < this->composition.size(); i++) {
		this->fission_cross_section += isotope_properties[this->composition[i].first]["fission_cross_section"] * this->composition[i].second;
	}

	this->absorption_cross_section = 0;
	for (int i = 0; i < this->composition.size(); i++) {
		this->absorption_cross_section += isotope_properties[this->composition[i].first]["absorption_cross_section"] * this->composition[i].second;
	}
}

float UO2::calculate_p_numerator() {
	float sum = 0.0f;

	for (std::pair<Isotope, float> isotopeFraction : this->composition) {
		float isotopeNumberDensity = isotopeFraction.second * calculate_number_density(this->density, this->molar_mass);
		float isotopeResonanceIntegral = integrate_resonance_cross_section(("../../data/cross_sections/total/ISOTOPE" + std::to_string(isotopeFraction.first) + ".txt").c_str());
		sum += isotopeNumberDensity * isotopeResonanceIntegral;
	}

	return sum;
}