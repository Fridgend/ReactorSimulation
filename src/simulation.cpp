#include "simulation.h"
#include <iostream>

void Simulation::step() {
	this->eta = calculateEta();
	this->f = calculateF();
	this->Keff = calculateKeff();
	this->p = calculateP();
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
	float numerator = this->fuel->calculate_p_numerator();
	float denominator = this->moderator->calculate_p_denominator();

	float effective_resonance_integral = 1.0f;

	float resonance_escape_probability = std::exp(-(numerator / denominator)*effective_resonance_integral);

	std::cout << "Resonance Escape Probability Numerator: " << numerator << std::endl;
	std::cout << "Resonance Escape Probability Denominator: " << denominator << std::endl;
	std::cout << "Resonance Escape Probability: " << resonance_escape_probability << std::endl;

	return 0.0f;
}
