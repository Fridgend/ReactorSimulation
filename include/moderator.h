#pragma once

#include <vector>
#include <utility>
#include <map>
#include <sstream>
#include <fstream>
#include <chrono>
#include <ctime>

#include "isotopes.h"

struct Moderator {
	virtual ~Moderator() = default;
	Moderator() : macroscopic_absorption_cross_section(0.0f), macroscopic_scattering_cross_section(0.0f), name("None") {}
	float macroscopic_absorption_cross_section;
	float macroscopic_scattering_cross_section;

	virtual void update_macroscopic_absorption_cross_section() = 0;
	virtual void update_macroscopic_scattering_cross_section() = 0;
	virtual float calculate_p_denominator() = 0;
	virtual void write_total_macroscopic() = 0;

	std::string name;
};

struct mH2O : public Moderator {
	mH2O();
	void update_macroscopic_absorption_cross_section() override;
	void update_macroscopic_scattering_cross_section() override;
	float calculate_p_denominator() override;
	void write_total_macroscopic() override;

private:
	float density = 1.0f; // g/cm³
	float molar_mass = 18.01528f; // g/mol
};
