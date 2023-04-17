// mam4xx: Copyright (c) 2022,
// Battelle Memorial Institute and
// National Technology & Engineering Solutions of Sandia, LLC (NTESS)
// SPDX-License-Identifier: BSD-3-Clause

#include <iostream>
#include <mam4xx/hetfrz.hpp>
#include <skywalker.hpp>
#include <validation.hpp>

void usage() {
  std::cerr << "aging_driver: a Skywalker driver for validating the "
               "MAM4 aging parameterizations."
            << std::endl;
  std::cerr << "aging_driver: usage:" << std::endl;
  std::cerr << "aging_driver <input.yaml>" << std::endl;
  exit(1);
}

using namespace skywalker;
using namespace mam4;

// Parameterizations used by the hetfzr process.
void get_reynolds_number(Ensemble *ensemble);
void get_temperature_diff(Ensemble *ensemble);
void get_air_viscosity(Ensemble *ensemble);
void get_latent_heat_vapor(Ensemble *ensemble);
void calcualte_collkernel_sub(Ensemble *ensemble);
void collkernel(Ensemble *ensemble);
void get_Aimm(Ensemble *ensemble);
void get_dg0imm(Ensemble *ensemble);
void get_form_factor(Ensemble *ensemble);
void calculate_hetfrz_contact_nucleation(Ensemble *ensemble);

int main(int argc, char **argv) {

  if (argc == 1) {
    usage();
  }

  Kokkos::initialize(argc, argv);
  std::string input_file = argv[1];
  std::string output_file = validation::output_name(input_file);
  std::cout << argv[0] << ": reading " << input_file << std::endl;

  // Load the ensemble. Any error encountered is fatal.
  Ensemble *ensemble = skywalker::load_ensemble(input_file, "mam4xx");

  // the settings.
  Settings settings = ensemble->settings();
  if (!settings.has("function")) {
    std::cerr << "No function specified in mam4xx.settings!" << std::endl;
    exit(1);
  }

  // Dispatch to the requested function.
  auto func_name = settings.get("function");
  try {
    if (func_name == "get_reynolds_number") {
      get_reynolds_number(ensemble);
    }
    if (func_name == "get_temperature_diff") {
      get_temperature_diff(ensemble);
    }
    if (func_name == "get_air_viscosity") {
      get_air_viscosity(ensemble);
    }
    if (func_name == "get_latent_heat_vapor") {
      get_latent_heat_vapor(ensemble);
    }
    if (func_name == "calculate_collkernel_sub") {
      calcualte_collkernel_sub(ensemble);
    }
    if (func_name == "collkernel") {
      collkernel(ensemble);
    }
    if (func_name == "get_Aimm") {
      get_Aimm(ensemble);
    }
    if (func_name == "get_dg0imm") {
      get_dg0imm(ensemble);
    }
    if (func_name == "get_form_factor") {
      get_form_factor(ensemble);
    }
    if (func_name == "calculate_hetfrz_contact_nucleation") {
      calculate_hetfrz_contact_nucleation(ensemble);
    }

  } catch (std::exception &e) {
    std::cerr << argv[0] << ": Error: " << e.what() << std::endl;
  }

  // Write out a Python module.

  std::cout << argv[0] << ": writing " << output_file << std::endl;
  ensemble->write(output_file);

  //
  delete ensemble;
  Kokkos::finalize();
}