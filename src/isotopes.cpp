#include "isotopes.h"

std::unordered_map<Isotope, std::unordered_map<std::string, float>> isotope_properties {
	{ U235,
		{
			{"neutrons_per_fission", 2.43585f},
			{"fission_cross_section", 585.1f},
			{"absorption_cross_section", 683.81f},
			{"elastic_cross_section", 14.1f},
			{"molar_mass", 235.04393f}
		}
	},

	{ U238,
		{
			{"neutrons_per_fission", 2.32734f},
			{"fission_cross_section", 0.0000168f},
			{"absorption_cross_section", 2.6830168f},
			{"elastic_cross_section", 9.24f},
			{"molar_mass", 238.02891f}
		}
	},

	{ O16,
		{
			{"neutrons_per_fission", 0.0f},
			{"fission_cross_section", 0.0f},
			{"absorption_cross_section", 0.0001899f},
			{"elastic_cross_section", 3.92f},
			{"molar_mass", 15.99491461957f}
		}
	},

	{ H1,
		{
			{"neutrons_per_fission", 0.0f},
			{"fission_cross_section", 0.0f},
			{"absorption_cross_section", 3.32f},
			{"elastic_cross_section", 30.3f},
			{"molar_mass", 1.00784f}
		}
	},

	{ Zr90,
		{
			{"neutrons_per_fission", 0.0f},
			{"fission_cross_section", 0.0f},
			{"absorption_cross_section", 0.01066f},
			{"elastic_cross_section", 5.3f},
			{"molar_mass", 89.904698755f}
		}
	},

	{ Sn120,
		{
			{"neutrons_per_fission", 0.0f},
			{"fission_cross_section", 0.0f},
			{"absorption_cross_section", 0.1392f},
			{"elastic_cross_section", 5.64f},
			{"molar_mass", 119.902202557f}
		}
	},

	{ Fe56,
		{
			{"neutrons_per_fission", 0.0f},
			{"fission_cross_section", 0.0f},
			{"absorption_cross_section", 2.591f},
			{"elastic_cross_section", 12.2f},
			{"molar_mass", 55.934935537f}
		}
	},

	{ Cr52,
		{
			{"neutrons_per_fission", 0.0f},
			{"fission_cross_section", 0.0f},
			{"absorption_cross_section", 0.858f},
			{"elastic_cross_section", 3.08f},
			{"molar_mass", 51.940504714f}
		}
	},

	{ Ni58,
		{
			{"neutrons_per_fission", 0.0f},
			{"fission_cross_section", 0.0f},
			{"absorption_cross_section", 4.22f},
			{"elastic_cross_section", 25.0f},
			{"molar_mass", 57.93534165f}
		}
	},

	{ C12,
		{
			{"neutrons_per_fission", 0.0f},
			{"fission_cross_section", 0.0f},
			{"absorption_cross_section", 0.003530f},
			{"elastic_cross_section", 4.95f},
			{"molar_mass", 12.0f}
		}
	},

	{ Mn55,
		{
			{"neutrons_per_fission", 0.0f},
			{"fission_cross_section", 0.0f},
			{"absorption_cross_section", 13.28f},
			{"elastic_cross_section", 2.12f},
			{"molar_mass", 54.93804304f}
		}
	},

	{ Mo98,
		{
			{"neutrons_per_fission", 0.0f},
			{"fission_cross_section", 0.0f},
			{"absorption_cross_section", 0.13f},
			{"elastic_cross_section", 5.67f},
			{"molar_mass", 97.905403609f}
		}
	},

	{ V51,
		{
			{"neutrons_per_fission", 0.0f},
			{"fission_cross_section", 0.0f},
			{"absorption_cross_section", 4.92f},
			{"elastic_cross_section", 4.97f},
			{"molar_mass", 50.943957664f}
		}
	},

	{ Si28,
		{
			{"neutrons_per_fission", 0.0f},
			{"fission_cross_section", 0.0f},
			{"absorption_cross_section", 0.1768f},
			{"elastic_cross_section", 1.99f},
			{"molar_mass", 27.97692653442f}
		}
	},

	{ Al27,
		{
			{"neutrons_per_fission", 0.0f},
			{"fission_cross_section", 0.0f},
			{"absorption_cross_section", 0.2303f},
			{"elastic_cross_section", 1.45f},
			{"molar_mass", 26.981538408f}
		}
	},

	{ N14,
		{
			{"neutrons_per_fission", 0.0f},
			{"fission_cross_section", 0.0f},
			{"absorption_cross_section", 0.075f},
			{"elastic_cross_section", 10.3f},
			{"molar_mass", 14.00307400425f}
		}
	},

	{ P31,
		{
			{"neutrons_per_fission", 0.0f},
			{"fission_cross_section", 0.0f},
			{"absorption_cross_section", 0.169f},
			{"elastic_cross_section", 4.17f},
			{"molar_mass", 30.97376199768f}
		}
	},

	{ S32,
		{
			{"neutrons_per_fission", 0.0f},
			{"fission_cross_section", 0.0f},
			{"absorption_cross_section", 0.5282f},
			{"elastic_cross_section", 0.979f},
			{"molar_mass", 31.97207117354f}
		}
	},

	{ Cu63,
		{
			{"neutrons_per_fission", 0.0f},
			{"fission_cross_section", 0.0f},
			{"absorption_cross_section", 4.507f},
			{"elastic_cross_section", 5.14f},
			{"molar_mass", 62.929597119f}
		}
	},

	{ As75,
		{
			{"neutrons_per_fission", 0.0f},
			{"fission_cross_section", 0.0f},
			{"absorption_cross_section", 4.153f},
			{"elastic_cross_section", 5.47f},
			{"molar_mass", 74.921594562f}
		}
	},

	{ Co59,
		{
			{"neutrons_per_fission", 0.0f},
			{"fission_cross_section", 0.0f},
			{"absorption_cross_section", 37.21f},
			{"elastic_cross_section", 6.04f},
			{"molar_mass", 58.933193524f}
		}
	},

	{ B10,
		{
			{"neutrons_per_fission", 0.0f},
			{"fission_cross_section", 0.0f},
			{"absorption_cross_section", 3837.0f},
			{"elastic_cross_section", 2.2f},
			{"molar_mass", 	10.012936862f}
		}
	}
};
