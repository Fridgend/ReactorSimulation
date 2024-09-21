#include "simulation.h"
#include <iostream>

void Simulation::step() {
	std::cout << "Stepping" << std::endl;
	std::cout << "Writing fuel macroscopic..." << std::endl;
	this->fuel->write_total_macroscopic();
	std::cout << "Writing moderator macroscopic..." << std::endl;
	this->moderator->write_total_macroscopic();
	std::cout << "Writing coolant macroscopic..." << std::endl;
	if (this->coolant->name != this->moderator->name) {this->coolant->write_total_macroscopic();}
	std::cout << "Writing control rods macroscopic..." << std::endl;
	this->controlRods->write_total_macroscopic();
	std::cout << "Writing fuel cladding macroscopic..." << std::endl;
	this->fuelCladding->write_total_macroscopic();
	std::cout << "Writing pressure vessel macroscopic..." << std::endl;
	this->pressureVessel->write_total_macroscopic();
	std::cout << "Writing system total macroscopic..." << std::endl;
	this->write_total_macroscopic_cross_section_system();

	this->eta = this->calculateEta();
	this->f = this->calculateF();
	this->p = this->calculateP();
	this->epsilon = this->calculateEpsilon();

	this->Keff = this->calculateKeff();
}

void Simulation::set_fuel(Fuel* fuel) {
	this->fuel = fuel;
}

void Simulation::set_moderator(Moderator* moderator) {
	this->moderator = moderator;
}

void Simulation::set_coolant(Coolant* coolant) {
	this->coolant = coolant;
}

// Calculates the total macroscopic cross-section of the entire system
void Simulation::write_total_macroscopic_cross_section_system() {
	std::map<double, double> data;

	std::vector<std::pair<double, double>> fuelData = get_data("data/session_data/cross_sections/macroscopic/FUEL_TOTAL.txt");
	std::vector<std::pair<double, double>> moderatorData = get_data("data/session_data/cross_sections/macroscopic/MODERATOR_TOTAL.txt");
	std::vector<std::pair<double, double>> fuelCladdingData = get_data("data/session_data/cross_sections/macroscopic/FUEL_CLADDING_TOTAL.txt");
	std::vector<std::pair<double, double>> pressureVesselData = get_data("data/session_data/cross_sections/macroscopic/PRESSURE_VESSEL_TOTAL.txt");
	std::vector<std::pair<double, double>> controlRodsData = get_data("data/session_data/cross_sections/macroscopic/CONTROL_RODS_TOTAL.txt");

	std::vector<std::vector<std::pair<double, double>>> energy_vecs = {
		fuelData, moderatorData, fuelCladdingData, pressureVesselData, controlRodsData
	};

	if (this->coolant->name != this->moderator->name) {
		std::vector<std::pair<double, double>> coolantData = get_data("data/session_data/cross_sections/macroscopic/COOLANT_TOTAL.txt");
		energy_vecs.push_back(coolantData);
	}

	auto full_start = std::chrono::system_clock::now();
	auto start = std::chrono::system_clock::now();
	std::cout << "Reading all energy maps" << std::endl;
	for (const auto& energy_vec : energy_vecs) {
		for (const auto& data_point : energy_vec) {
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

	float number_of_steps = data.size() * energy_vecs.size();
	float step = 0;
	for (const auto& [energy, _] : data) {
		double sum = 0.0;
		for (std::vector<std::pair<double, double>>& energy_vec : energy_vecs) {
			sum += get_microscopic_cross_section(energy_vec, energy);

			if ((int)step % 100000 == 0) {
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
	std::string fileLocation = "data/session_data/cross_sections/macroscopic/SYSTEM_TOTAL.txt";
	std::ofstream file(fileLocation);

	if (file.is_open()) {
		file << "SYSTEM Total Macroscopic Cross-Section" << std::endl;
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

// Calculates Keff
float Simulation::calculateKeff() {
	return this->eta * this->epsilon * this->p * this->pth * this->f * this->fth;
}

// Calculates the neutron reproduction factor
float Simulation::calculateEta() {
	return (this->fuel->avg_neutrons_per_fission * this->fuel->fission_cross_section) / (this->fuel->absorption_cross_section);
}

// Calculates the thermal utilization factor
float Simulation::calculateF() {
	float fuel_macroscopic = this->fuel->macroscopic_absorption_cross_section;
	float moderator_macroscopic = this->moderator->macroscopic_absorption_cross_section;
	float coolant_macroscopic = (this->moderator->name != this->coolant->name) ? this->coolant->macroscopic_absorption_cross_section : 0.0f;

	float structural_material_macroscopic = (
		this->fuelCladding->get_macroscopic_absorption_cross_section() +
		this->pressureVessel->get_macroscopic_absorption_cross_section()
		);

	float control_rod_macroscopic = this->controlRods->get_macroscopic_absorption_cross_section() * this->controlRodsDepth * this->maxControlRodFraction;

	std::cout << "FUEL MACROSCOPIC: " << fuel_macroscopic << std::endl;
	std::cout << "MODERATOR MACROSCOPIC: " << moderator_macroscopic << std::endl;
	std::cout << "COOLANT MACROSCOPIC: " << coolant_macroscopic << std::endl;
	std::cout << "STRUCTURAL MATERIALS MACROSCOPIC: " << structural_material_macroscopic << std::endl;
	std::cout << "CONTROL RODS MACROSCOPIC: " << control_rod_macroscopic << std::endl;
	std::cout << "-----" << std::endl;

	return fuel_macroscopic / (
		fuel_macroscopic +
		moderator_macroscopic +
		coolant_macroscopic +
		structural_material_macroscopic +
		control_rod_macroscopic
	);
}

// Calculates the resonance escape probability
float Simulation::calculateP() {
	float numerator = this->fuel->calculate_p_numerator(this->moderator->macroscopic_scattering_cross_section);
	float denominator = this->moderator->calculate_p_denominator();

	float resonance_escape_probability = std::exp(-numerator/denominator);

	return resonance_escape_probability;
}

// Calculates the fast fission factor
float Simulation::calculateEpsilon() {
	float first_term = (1 - this->p)/this->p;
	float second_term = 0.0f;

	return 1.0f + first_term * second_term;
}
