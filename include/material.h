#pragma once

#include "isotopes.h"
#include <vector>
#include <utility>

struct Material {
	Material() : macroscopic_absorption_cross_section(0), density(0.0f), molar_mass(0.0f) {}

	void update_macroscopic_absorption_cross_section();
	float macroscopic_absorption_cross_section;

	void update_molar_mass();

	std::string name;
	std::vector<std::pair<Isotope, float>> composition;

private:
	float density; // g/cm^3
	float molar_mass; // g/mol
};



struct smZircaloy2 : public Material {
	smZircaloy2();

private:
	float density = 6.55f; // g/cm^3
	float molar_mass = 0.0f; // g/mol
};



struct smSA508_Steel : public Material {
	smSA508_Steel();

private:
	float density = 7.85f; // g/cm^3
	float molar_mass = 0.0f; // g/mol
};



struct BoronCarbide : public Material {
	BoronCarbide();

private:
	float density = 2.52f; // g/cm^3
	float molar_mass = 0.0f; // g/mol
};