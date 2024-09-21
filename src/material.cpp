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
		float microscopic_absorption_cross_section_isotope = isotope_properties[isotopeFraction.first]["absorption_cross_section"]; // barns to cm^2
		float isotopeNumberDensity = calculate_number_density(this->density, isotope_properties[isotopeFraction.first]["molar_mass"]);
		this->macroscopic_absorption_cross_section += calculate_macroscopic_absorption_cross_section(isotopeNumberDensity, microscopic_absorption_cross_section_isotope) * isotopeFraction.second;
	}
}

void Material::write_total_macroscopic() {
	std::map<double, double> data;
	std::vector<std::pair<std::vector<std::pair<double, double>>, float>> energy_maps;
	for (std::pair<Isotope, float> isotopeFraction : this->composition) {
		if (isotopeFraction.second > 0.01f) {
			std::string data_map_location = "data/predefined_cross_sections/total/ISOTOPE" +
							std::to_string(isotopeFraction.first) +
							".txt";

			energy_maps.emplace_back(get_data(data_map_location.c_str()), isotopeFraction.second);
		}
	}

	float numberDensity = calculate_number_density(this->density, this->molar_mass);

	auto total_start = std::chrono::system_clock::now();
	auto start = std::chrono::system_clock::now();
	std::cout << "Reading all energy maps" << std::endl;
	for (const auto& energy_map : energy_maps) {
		for (const auto& data_point : energy_map.first) {
			if (data.find(data_point.first) == data.end()) {
				data[data_point.first] = 0.0;
			}
		}
	}
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end-start;

	std::cout << "Data map size fixed (time: " << elapsed_seconds.count() << "s): " << data.size() << std::endl;
	std::cout << "Adding data..." << std::endl;

	start = std::chrono::system_clock::now();

	float number_of_steps = data.size() * energy_maps.size();
	float step = 0;
	for (const auto& [energy, _] : data) {
		double sum = 0.0;
		for (std::pair<std::vector<std::pair<double, double>>, float>& energy_map : energy_maps) {
			sum += get_microscopic_cross_section(energy_map.first, energy) * energy_map.second * numberDensity;
			if ((int)step % 10000 == 0) {
				std::cout << "Step: " << step << " (" << (step / number_of_steps * 100.0f) << "%)." << std::endl;
			}
			step++;
		}
		data[energy] = sum;
	}
	std::cout << "Step: " << step << " (" << (step / number_of_steps * 100.0f) << "%)." << std::endl;
	end = std::chrono::system_clock::now();
	elapsed_seconds = end-start;

	std::cout << "Data added (time: " << elapsed_seconds.count() << "s). Writing to file..." << std::endl;

	start = std::chrono::system_clock::now();
	std::string fileLocation = "data/session_data/cross_sections/macroscopic/" + this->type + "_TOTAL.txt";
	std::ofstream file(fileLocation);

	if (file.is_open()) {
		file << this->name << " Total Macroscopic Cross-Section" << std::endl;
		file << "Energy(eV) XS(b)" << std::endl;
		for (const auto& [energy, cross_section] : data) {
			file << energy << " " << cross_section << std::endl;
		}

		file.close();
	} else {
		std::cerr << "Error: Unable to open file '" << fileLocation << "'." << std::endl;
	}
	end = std::chrono::system_clock::now();
	elapsed_seconds = end-start;

	auto total_end = std::chrono::system_clock::now();
	std::chrono::duration<double> total_elapsed_seconds = total_end-total_start;

	std::cout << "Written to file (time: " << elapsed_seconds.count() << "s)." << std::endl;
	std::cout << "Total time: " << total_elapsed_seconds.count() << "s." << std::endl;
}
