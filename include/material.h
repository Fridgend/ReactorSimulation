#pragma once

#include "isotopes.h"
#include <vector>
#include <utility>
#include <iostream>

struct Material {
	Material() : density(0.0f), molar_mass(0.0f), macroscopic_absorption_cross_section(0.0f) {}

	void update_macroscopic_absorption_cross_section();
	void update_molar_mass();

	std::string name;
	std::vector<std::pair<Isotope, float>> composition;

	float get_macroscopic_absorption_cross_section() { return this->macroscopic_absorption_cross_section; }

protected:
	float macroscopic_absorption_cross_section;
	float density; // g/cm^3
	float molar_mass; // g/mol
};



struct smZircaloy2 : public Material {
	smZircaloy2() {
		this->name = "Zircaloy-2 (Grade R60802)";
		this->density = 6.55f;
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
};



struct smSA508_Steel : public Material {
	smSA508_Steel() {
		this->name = "SA508 Gr.3 Steel";
		this->density = 7.85f;
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
};



struct BoronCarbide : public Material {
	BoronCarbide() {
		this->name = "Boron Carbide (B4C)";
		this->density = 2.52f;
		this->composition = {
			{B10, 0.8f},
			{C12, 0.2f}
		};

		this->update_molar_mass();
		this->update_macroscopic_absorption_cross_section();
	}
};