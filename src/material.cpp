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
		float microscopic_absorption_cross_section = isotope_properties[isotopeFraction.first]["absorption_cross_section"] * 1e-24f; // barns to cm^2
		float numberDensity = calculate_number_density(this->density, isotope_properties[isotopeFraction.first]["molar_mass"]);
		this->macroscopic_absorption_cross_section += microscopic_absorption_cross_section * numberDensity * isotopeFraction.second;
	}
}



smZircaloy2::smZircaloy2() {
	this->name = "Zircaloy-2 (Grade R60802)";
	this->composition = {
		{Zr90, 0.985f},
		{Sn120, 0.015f},
		{Fe56, 0.0015f},
		{Cr52, 0.0005f},
		{Ni58, 0.0005f}
	};

	this->update_molar_mass();
	this->update_macroscopic_absorption_cross_section();
}



smSA508_Steel::smSA508_Steel() {
	this->name = "SA508 Gr.3 Steel";
	this->composition = {
		{C12, 0.0018f},
		{Mn55, 0.014f},
		{Ni58, 0.0079f},
		{Cr52, 0.0014f},
		{Mo98, 0.0051f},
		{V51, 0.00005f},
		{Si28, 0.0022f},
		{Al27, 0.00024f},
		{N14, 0.000123f},
		{P31, 0.00005f},
		{S32, 0.00003f},
		{Cu63, 0.0004f},
		{As75, 0.00004f},
		{Co59, 0.00008f},
		{Fe56, 0.966587f}
	};

	this->update_molar_mass();
	this->update_macroscopic_absorption_cross_section();
}



BoronCarbide::BoronCarbide() {
	this->name = "Boron Carbide (B4C)";
	this->composition = {
		{B10, 0.8f},
		{C12, 0.2f}
	};

	this->update_molar_mass();
	this->update_macroscopic_absorption_cross_section();
}