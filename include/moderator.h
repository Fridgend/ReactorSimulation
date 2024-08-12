#pragma once

#include "isotopes.h"

struct Moderator {
	virtual ~Moderator() = default;
	Moderator() : macroscopic_absorption_cross_section(0), name("None") {}
	float macroscopic_absorption_cross_section;

	virtual void update_macroscopic_absorption_cross_section() = 0;
	virtual float calculate_p_denominator() = 0;

	std::string name;
};

struct mH2O : public Moderator {
	mH2O();
	void update_macroscopic_absorption_cross_section() override;

	float calculate_p_denominator() override;

private:
	float density = 1.0f; // g/cm^3
	float molar_mass = 18.01528f; // g/mol
};