#include "simulation.h"
#include <iostream>

void Simulation::step() {
	this->eta = calculateEta();
	this->Keff = calculateKeff();
}

void Simulation::set_fuel(Fuel fuel) {
	this->fuel = fuel;
}

void Simulation::set_moderator(Moderator moderator) {
	this->moderator = moderator;
}

void Simulation::set_coolant(Coolant coolant) {
	this->coolant = coolant;
}

float Simulation::calculateKeff() {
	return this->eta * this->epsilon * this->p * this->pth * this->f * this->fth;
}

float Simulation::calculateEta() {
	return (this->fuel.avg_neutrons_per_fission * this->fuel.fission_cross_section) / (this->fuel.absorption_cross_section);
}

float Simulation::calculateF() {
	float fuel_macroscopic = this->fuel.macroscopic_absorption_cross_section;
	float moderator_macroscopic = this->moderator.macroscopic_absorption_cross_section;
	float coolant_macroscopic = (this->moderator.name != this->coolant.name) ? this->coolant.macroscopic_absorption_cross_section : 0.0f;
	//float structural_material_macroscopic;
	//float control_rod_macroscopic;

	return fuel_macroscopic / (
		moderator_macroscopic +
		coolant_macroscopic
	);
}