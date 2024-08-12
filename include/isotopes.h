#pragma once

#include <vector>
#include <array>
#include <unordered_map>
#include <utility>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

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

// Calculates the resonance integral based on total cross-section data
inline float integrate_resonance_cross_section(const char* cross_section_text_file) {
    std::ifstream infile(cross_section_text_file);
    if (!infile) {
        std::cerr << "Error: Unable to open file '" << cross_section_text_file << "'." << std::endl;
        return 0.0f;
    }

    std::vector<std::pair<float, float>> energy_cross_section_data;
    std::string line;
    int lineNr = 1;

    while (getline(infile, line)) {
        // Skip first two rows (headers)
        if (lineNr < 3) {
            if (lineNr == 2 && line != "Energy(eV) XS(b)") {
                std::cerr << "Error: File '" << cross_section_text_file << "' is corrupted." << std::endl;
            }
            lineNr++;
            continue;
        }

        std::stringstream ss(line);
        float energy, cross_section;
        ss >> energy >> cross_section;

        // Only consider energies above thermal (~0.0253 eV) to avoid skewing results with thermal cross-sections
        if (energy > 1.0f) {
            energy_cross_section_data.push_back(std::make_pair(energy, cross_section));
        }
    }

    infile.close();

    float resonance_integral = 0.0f;
    for (size_t i = 1; i < energy_cross_section_data.size(); ++i) {
        float E1 = energy_cross_section_data[i - 1].first;
        float E2 = energy_cross_section_data[i].first;
        float sigma1 = energy_cross_section_data[i - 1].second;
        float sigma2 = energy_cross_section_data[i].second;

        // Trapezoidal rule for integration on a logarithmic scale
        float logE1 = log(E1);
        float logE2 = log(E2);
        float average_sigma = (sigma1 + sigma2) / 2.0f;

        resonance_integral += average_sigma * (logE2 - logE1);
    }

    std::cout << "( " << cross_section_text_file << " ) Resonance Integral : " << resonance_integral << std::endl;
    return resonance_integral;
}
