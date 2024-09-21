#pragma once

#include <vector>
#include <array>
#include <unordered_map>
#include <utility>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <map>
#include <limits>

enum Isotope {
	U235,
	U238,
	O16,
	H1,
	Zr90,
	Sn120,
	Fe56,
	Cr52,
	Ni58,
	C12,
	Mn55,
	Mo98,
	V51,
	Si28,
	Al27,
	N14,
	P31,
	S32,
	Cu63,
	As75,
	Co59,
	B10
};

extern std::unordered_map<Isotope, std::unordered_map<std::string, float>> isotope_properties;

// Calculates the number density of the material based on density (g/cm^3) and molar mass (g/mol)
inline float calculate_number_density(float density, float molar_mass) {
	float avogadro = 6.0221408e23f;
	return density * avogadro / molar_mass;
}

// Calculates the macroscopic absorption cross section based on number density (atoms/cm^3) and microscopic absorption cross section (barns)
inline float calculate_macroscopic_absorption_cross_section(float number_density, float microscopic_absorption_cross_section) {
	return number_density * microscopic_absorption_cross_section * 1e-24f; // 1 barn = 1e-24 cm^2
}

// Calculates the average logarithmic energy decrement based on an isotope's atomic number
inline float calculate_xi(int atomic_number) {
	if (atomic_number == 1) { return 0.0f; }
	float a = (float)std::pow((atomic_number - 1.0f) / (atomic_number + 1.0f), 2);
	return 1.0f + (a / (1.0f - a)) * std::log(a);
}

// Temporary function to calculate absorption cross-section dataset
inline void calculate_absorption_cross_section(const char* capture_cross_section, const char* fission_cross_section, const char* out_file_name) {
    std::ifstream capture_xsection_file(capture_cross_section);
    std::ifstream fission_xsection_file(fission_cross_section);

    if (!capture_xsection_file) {
	std::cerr << "Error: Unable to open file '" << capture_cross_section << "'." << std::endl;
    }
    if (!fission_xsection_file) {
	std::cerr << "Error: Unable to open file '" << fission_cross_section << "'." << std::endl;
    }

    std::vector<std::pair<float, float>> capture_xsection_data;
    std::string line;
    int lineNr = 1;

    while (getline(capture_xsection_file, line)) {
	if (lineNr < 3) {
	    if (lineNr == 2 && line != "Energy(eV) XS(b)") {
		std::cerr << "Error: File '" << capture_cross_section << "' is corrupted." << std::endl;
	    }
	    lineNr++;
	    continue;
	}

	std::stringstream ss(line);
	float energy, cross_section;
	ss >> energy >> cross_section;

	capture_xsection_data.emplace_back(energy, cross_section);
    }

    capture_xsection_file.close();

    std::vector<std::pair<float, float>> fission_xsection_data;
    lineNr = 1;

    while (getline(fission_xsection_file, line)) {
	if (lineNr < 3) {
	    if (lineNr == 2 && line != "Energy(eV) XS(b)") {
		std::cerr << "Error: File '" << fission_cross_section << "' is corrupted." << std::endl;
	    }
	    lineNr++;
	    continue;
	}

	std::stringstream ss(line);
	float energy, cross_section;
	ss >> energy >> cross_section;

	fission_xsection_data.emplace_back(energy, cross_section);
    }

    fission_xsection_file.close();

    std::ofstream out_file(out_file_name);
    if (out_file.is_open()) {
	out_file << "ISOTOPE(n, g + total fission) ENDFB-8.0" << std::endl;
	out_file << "Energy(eV) XS(b)" << std::endl;
	for (size_t i = 0; i < capture_xsection_data.size(); i++) {
	    if (i >= fission_xsection_data.size()) { break; }
	    out_file << fission_xsection_data[i].first << " " << fission_xsection_data[i].second + capture_xsection_data[i].second << std::endl;
	}
    } else {
	std::cerr << "Error: Unable to open file '" << out_file_name << "'." << std::endl;
    }
}

inline std::map<double, double> get_data_map(const char* file_name, double range_low=0.0, double range_high=10000000.0) {
	std::map<double, double> energy_map;
	std::ifstream infile(file_name);

	if (!infile) {
		std::cerr << "Error: Unable to open file '" << file_name << "'." << std::endl;
		return energy_map;
	}

	int lineNr = 1;
	std::string line;
	while (getline(infile, line)) {
		if (lineNr < 3) {
			if (lineNr == 2 && line != "Energy(eV) XS(b)") {
				std::cerr << "Error: File '" << file_name << "' is corrupted." << std::endl;
				return energy_map;
			}
			lineNr++;
			continue;
		}

		std::stringstream ss(line);
		double energy, cross_section;
		ss >> energy >> cross_section;

		if (energy >= range_low && energy <= range_high) {
			energy_map[energy] = cross_section;
		}
	}

	infile.close();

	return energy_map;
}

inline std::vector<std::pair<double, double>> get_data(const char* file_name, double range_low=0.0, double range_high=10000000.0) {
	std::ifstream file(file_name);
	std::vector<std::pair<double, double>> data;

	if (!file) {
		std::cerr << "Error: Unable to open file '" << file_name << "'." << std::endl;
		return data;
	}

	int lineNr = 1;
	std::string line;
	while (getline(file, line)) {
		if (lineNr < 3) {
			if (lineNr == 2 && line != "Energy(eV) XS(b)") {
				std::cerr << "Error: File '" << file_name << "' is corrupted." << std::endl;
				return data;
			}
			lineNr++;
			continue;
		}

		std::stringstream ss(line);
		float energy, cross_section;
		ss >> energy >> cross_section;

		if (energy >= range_low && energy <= range_high) {
			data.emplace_back(energy, cross_section);
		}
	}

	file.close();

	return data;
}

// Returns the microscopic cross-section based on the inputs
// PARAMETERS:
// const std::vector<std::pair<double, double>>& data_vec = The vector containing all of the cross-sections
// double requested_energy = The energy to recieve the cross-section from.
inline float get_microscopic_cross_section(const std::vector<std::pair<double, double>>& data_vec, double requested_energy) {
	auto lower = std::lower_bound(
		data_vec.begin(), data_vec.end(), std::pair<double, double>(requested_energy, 0.0)
	);

	if (lower == data_vec.end()) {
		return (lower - 1)->second;
	} else if (lower == data_vec.begin()) {
		return lower->second;
	} else if (lower->first == requested_energy) {
		return lower->second;
	}

	auto upper = lower;
	lower--;
	double slope = (upper->second - lower->second) / (upper->first - lower->first);
	return lower->second + slope * (requested_energy - lower->first);
}

// Calculates the resonance integral for absorption
// PARAMETERS:
// const char* absorption_cross_section_file = The location of the file containing the microscopic absorption cross-section for the nuclide that is being calculated for.
// float moderator_scattering = The macroscopic scattering cross-section of the moderator at thermal energies (around 0.0253 eV)
inline float calculate_resonance_integral(const char* absorption_cross_section_file, float moderator_scattering) {
	std::vector<std::pair<double, double>> data_vec = get_data(absorption_cross_section_file, 0.5, 1e7);

	std::vector<std::pair<double, double>> total_data_vec = get_data("data/session_data/cross_sections/macroscopic/SYSTEM_TOTAL.txt", 0.5, 1e7);
	const int N = data_vec.size()-1;
	double deltaE = std::fabs(get_microscopic_cross_section(data_vec, 0.5) - get_microscopic_cross_section(data_vec, 1e7)) / N;
	double sum = 0.0;

	for (int i = 1; i <= N; i++) {
		double e = i * deltaE;
		double term = (
			(moderator_scattering / get_microscopic_cross_section(total_data_vec, e)) *
			(get_microscopic_cross_section(data_vec, e) / e)
		);

		if (i == 0 || i == N) {
			sum += 0.5 * term;
		} else {
			sum += term;
		}
	}

	return (float)(deltaE * sum);
}
