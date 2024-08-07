#include "isotopes.h"

std::unordered_map<Isotope, std::unordered_map<std::string, float>> isotope_properties {
	{ U235,
		{
			{"neutrons_per_fission", 2.43585f},
			{"fission_cross_section", 585.1f},
			{"absorption_cross_section", 683.81f},
			{"molar_mass", 235.04393f}
	}
	},

	{ U238,
		{
			{"neutrons_per_fission", 2.32734f},
			{"fission_cross_section", 0.0000168f},
			{"absorption_cross_section", 2.6830168f},
			{"molar_mass", 238.02891f}
		}
	},

	{ O16,
		{
			{"neutrons_per_fission", 0.0f},
			{"fission_cross_section", 0.0f},
			{"absorption_cross_section", 0.0001899f},
			{"molar_mass", 15.99491461957f}
		}
	},

	{ H1,
		{
			{"neutrons_per_fission", 0.0f},
			{"fission_cross_section", 0.0f},
			{"absorption_cross_section", 3.32f},
			{"molar_mass", 1.00784f}
		}
	}
};