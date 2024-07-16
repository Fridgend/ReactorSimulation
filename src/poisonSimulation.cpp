#include <cmath>
#include <iostream>
#include <vector>

const float URANIUM_235_THERMAL_FISSION_CROSS_SECTION = 585.1f;						// barns
const float URANIUM_235_THERMAL_ABSORPTION_CROSS_SECTION = 585.1f + 98.71f;			// barns
const float URANIUM_235_THERMAL_AVERAGE_NEUTRONS_PER_FISSION = 2.42f + 1.585e-2;	// neutrons

const float URANIUM_238_THERMAL_FISSION_CROSS_SECTION = 585.1f;						// barns
const float URANIUM_238_THERMAL_ABSORPTION_CROSS_SECTION = 585.1f + 98.71f;			// barns
const float URANIUM_238_THERMAL_AVERAGE_NEUTRONS_PER_FISSION = 2.281 + 4.634e-2;	// neutrons

struct Fuel {
	float avg_neutrons_per_fission;
	float fission_cross_section;
	float absorption_cross_section;

	std::vector<float[4]> composition;	// neutrons per fission, fission cross section, absorption cross section, fraction
};

struct UO2 : public Fuel {
	UO2(std::vector<float[4]> composition) {
		update_composition(composition);
	}

	void update_composition(std::vector<float[4]> composition) {
		this->composition = composition;
		this->avg_neutrons_per_fission = 0;
		for (int i = 0; i < this->composition.size(); i++) {
			this->avg_neutrons_per_fission += this->composition[i][0] * this->composition[i][3];
		}

		this->fission_cross_section = 0;
		for (int i = 0; i < this->composition.size(); i++) {
			this->fission_cross_section += this->composition[i][1] * this->composition[i][3];
		}

		this->absorption_cross_section = 0;
		for (int i = 0; i < this->composition.size(); i++) {
			this->absorption_cross_section += this->composition[i][2] * this->composition[i][3];
		}
	}
};

class Simulation {
public:
	Simulation(Fuel fuel) {
		this->fuel = fuel;
	}

	void step() {
		this->eta = calculateEta();
		this->Keff = calculateKeff();
	}

	void set_fuel(Fuel fuel) {
		this->fuel = fuel;
	}

	float calculateKeff() {
		return this->eta * this->epsilon * this->p * this->pth * this->f * this->fth;
	}

	float calculateEta() {
		return (this->fuel.avg_neutrons_per_fission * this->fuel.fission_cross_section) / (this->fuel.absorption_cross_section);
	}

	float calculateEpsilon() {
		return 0.0f;
	}

	float getKeff() { return this->Keff; }
	float getEta() { return this->eta; }
	float getEpsilon() { return this->epsilon; }
	float getP() { return this->p; }
	float getPth() { return this->pth; }
	float getF() { return this->f; }
	float getFth() { return this->fth; }

	float getAvgNeutronsPerFission() { return this->fuel.avg_neutrons_per_fission; }
	float getFissionCrossSectionFuel() { return this->fuel.fission_cross_section; }
	float getAbsorptionCrossSectionFuel() { return this->fuel.absorption_cross_section; }

private:
	float Keff;								// Effective multiplication factor

	float eta;								// Reproduction factor
	float epsilon;							// Fast fission factor	
	float p;								// Resonance escape probability
	float pth;								// Thermal nonleakage probability
	float f;								// Thermal utilization factor
	float fth;								// Fast nonleakage factor

	float avg_neutrons_per_fission;			// Average number of neutrons per fission
	float fission_cross_section_fuel;		// Fission cross section of fuel
	float absorption_cross_section_fuel;	// Absorption cross section of fuel

	Fuel fuel;
};

int main() {
	std::vector<float[4]> composition{
		{URANIUM_235_THERMAL_AVERAGE_NEUTRONS_PER_FISSION, URANIUM_235_THERMAL_FISSION_CROSS_SECTION, URANIUM_235_THERMAL_ABSORPTION_CROSS_SECTION, 0.03f},
		{URANIUM_238_THERMAL_AVERAGE_NEUTRONS_PER_FISSION, URANIUM_238_THERMAL_FISSION_CROSS_SECTION, URANIUM_238_THERMAL_ABSORPTION_CROSS_SECTION, 0.97f}
	};

	Simulation simulation(UO2(composition));
	simulation.step();
	std::cout << "Keff: " << simulation.getKeff() << std::endl;
	std::cout << "Eta: " << simulation.getEta() << std::endl;
	return 0;
}