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

void UO2::write_total_macroscopic() {
	std::map<double, double> data;

	std::vector<std::pair<double, double>> microscopic_235 = get_data("data/predefined_cross_sections/total/ISOTOPE0.txt");
	std::vector<std::pair<double, double>> microscopic_238 = get_data("data/predefined_cross_sections/total/ISOTOPE1.txt");
	std::vector<std::pair<double, double>> microscopic_o16 = get_data("data/predefined_cross_sections/total/ISOTOPE2.txt");

	std::vector<std::vector<std::pair<double, double>>> energy_maps = {microscopic_235, microscopic_238, microscopic_o16};

	float fraction_235 = (this->composition[0].first == U235) ? this->composition[0].second : this->composition[1].second;
	float fraction_238 = (this->composition[0].first == U238) ? this->composition[0].second : this->composition[1].second;

	float numberDensity = calculate_number_density(this->density, this->molar_mass);

	auto full_start = std::chrono::system_clock::now();
	auto start = std::chrono::system_clock::now();
	std::cout << "Reading all energy maps" << std::endl;
	for (const auto& energy_map : energy_maps) {
		for (const auto& data_point : energy_map) {
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
	float number_of_steps = data.size() * 3.0f;
	float step = 0.0f;
	for (const auto& [energy, _] : data) {
		double sum = 0.0;
		sum += get_microscopic_cross_section(microscopic_235, energy) * fraction_235 * numberDensity;
		if ((int)step % 10000 == 0) {
			std::cout << "Step: " << step << " (" << (step / number_of_steps * 100.0f) << "%)." << std::endl;
		}
		step++;

		sum += get_microscopic_cross_section(microscopic_238, energy) * fraction_238 * numberDensity;
		if ((int)step % 10000 == 0) {
			std::cout << "Step: " << step << " (" << (step / number_of_steps * 100.0f) << "%)." << std::endl;
		}
		step++;

		sum += get_microscopic_cross_section(microscopic_o16, energy) * 2 * numberDensity;
		if ((int)step % 10000 == 0) {
			std::cout << "Step: " << step << " (" << (step / number_of_steps * 100.0f) << "%)." << std::endl;
		}
		step++;

		data[energy] = sum;
	}
	std::cout << "Step: " << step << " (" << (step / number_of_steps * 100.0f) << "%)." << std::endl;
	end = std::chrono::system_clock::now();
	elapsed_seconds = end-start;

	std::cout << "Data added (time: " << elapsed_seconds.count() << "s). Writing to file..." << std::endl;

	start = std::chrono::system_clock::now();
	std::string fileLocation = "data/session_data/cross_sections/macroscopic/FUEL_TOTAL.txt";
	std::ofstream file(fileLocation);

	if (file.is_open()) {
		file << "UO2 Total Macroscopic Cross-Section" << std::endl;
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

	auto full_end = std::chrono::system_clock::now();
	std::chrono::duration<double> full_elapsed_seconds = full_end-full_start;

	std::cout << "Written to file (time: " << elapsed_seconds.count() << "s)." << std::endl;
	std::cout << "Total time: " << full_elapsed_seconds.count() << "s." << std::endl;
}

float UO2::calculate_p_numerator(float moderator_scattering) {
	float sum = 0.0f;
	for (std::pair<Isotope, float> isotopeFraction : this->composition) {
		std::string isotopeFilePath = "data/predefined_cross_sections/absorption/ISOTOPE" +
					      std::to_string(isotopeFraction.first) +
					      ".txt";

		float isotopeNumberDensity = isotopeFraction.second * calculate_number_density(this->density, this->molar_mass);
		float resonance_integral = calculate_resonance_integral(isotopeFilePath.c_str(), moderator_scattering);

		sum += isotopeNumberDensity * resonance_integral;
	}

	return sum;
}
