#include "moderator.h"

mH2O::mH2O() {
	this->update_macroscopic_absorption_cross_section();
	this->update_macroscopic_scattering_cross_section();
	this->name = "H2O - Water";
}

void mH2O::update_macroscopic_absorption_cross_section() {
	float waterNumberDensity = calculate_number_density(this->density, this->molar_mass);

	float numberDensityH = waterNumberDensity * 2;
	float numberDensityO = waterNumberDensity;

	this->macroscopic_absorption_cross_section = (
		isotope_properties[H1]["absorption_cross_section"] * 1e-24f * numberDensityH +
		isotope_properties[O16]["absorption_cross_section"] * 1e-24f * numberDensityO
	);
}

void mH2O::update_macroscopic_scattering_cross_section() {
	float waterNumberDensity = calculate_number_density(this->density, this->molar_mass);

	float numberDensityH = waterNumberDensity * 2;
	float numberDensityO = waterNumberDensity;

	this->macroscopic_scattering_cross_section = (
		isotope_properties[H1]["elastic_cross_section"] * 1e-24f * numberDensityH +
		isotope_properties[O16]["elastic_cross_section"] * 1e-24f * numberDensityO
	);
}

float mH2O::calculate_p_denominator() {
	float logarithmic_energy_decrement_per_collision = 0.92f;

	return logarithmic_energy_decrement_per_collision * this->macroscopic_scattering_cross_section;
}

void mH2O::write_total_macroscopic() {
	std::map<double, double> data;

	std::vector<std::pair<double, double>> microscopic_h1 = get_data("data/predefined_cross_sections/total/ISOTOPE3.txt");
	std::vector<std::pair<double, double>> microscopic_o16 = get_data("data/predefined_cross_sections/total/ISOTOPE2.txt");
	std::vector<std::vector<std::pair<double, double>>> energy_maps = {microscopic_h1, microscopic_o16};

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
	float number_of_steps = data.size() * 2.0f;
	float step = 0.0f;
	for (const auto& [energy, _] : data) {
		double sum = 0.0;
		sum += get_microscopic_cross_section(microscopic_h1, energy) * 2 * numberDensity;
		if ((int)step % 10000 == 0) {
			std::cout << "Step: " << step << " (" << (step / number_of_steps * 100.0f) << "%)." << std::endl;
		}
		step++;

		sum += get_microscopic_cross_section(microscopic_o16, energy) * numberDensity;
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
	std::string fileLocation = "data/session_data/cross_sections/macroscopic/MODERATOR_TOTAL.txt";
	std::ofstream file(fileLocation);

	if (file.is_open()) {
		file << "mH2O Total Macroscopic Cross-Section" << std::endl;
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
