// mam4xx: Copyright (c) 2022,
// Battelle Memorial Institute and
// National Technology & Engineering Solutions of Sandia, LLC (NTESS)
// SPDX-License-Identifier: BSD-3-Clause

#include <mam4xx/mam4.hpp>

#include <mam4xx/aero_config.hpp>
#include <mam4xx/gas_chem.hpp>
#include <skywalker.hpp>
#include <validation.hpp>

using namespace skywalker;
using namespace mam4;
using namespace gas_chemistry;

void mam_linmat(Ensemble *ensemble) {

  ensemble->process([=](const Input &input, Output &output) {
   
    const Real zero =0;
    std::vector<Real> mat(nzcnt,zero);
    const auto rxt = input.get_array("rxt");
    const auto het_rates= input.get_array("het_rates");

    lin_matrix(mat.data(),rxt.data(),het_rates.data());

    output.set("mat", mat);


    
  });
}
