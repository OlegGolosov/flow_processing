//
// Created by eugene on 4/15/19.
//

#ifdef __CLING__

R__LOAD_LIBRARY(libQnCorrections.so)
R__LOAD_LIBRARY(libBase.so)

#endif

#include "include/Utils.h"

using namespace std;
using namespace fmt;

Result Combine(vector<Result> resolutionMHParticles, const vector<string> &) {
  assert(resolutionMHParticles.size() > 1);
  Result result{resolutionMHParticles.front()};

  int n{1};
  for (auto it = begin(resolutionMHParticles) + 1; it != end(resolutionMHParticles); ++it) {
    result = result.Apply(*it, [](const Qn::Stats &lhs, const Qn::Stats &rhs) { return Qn::Merge(lhs, rhs); });
    ++n;
  }

  return result;
}

Result Resolution3Sub(vector<Result> args, const vector<string> &) {
  assert(args.size() == 3);

  auto &nom1 = args[0];
  auto &nom2 = args[1];
  auto &denom = args[2];
  return Qn::Sqrt(nom1 * nom2 / denom);
}

struct ResolutionTrack {
  float yMin{0.4};
  float yMax{1.2};

  ResolutionTrack () = default;
  ResolutionTrack (float _yMin, float _yMax) :
	  yMin(_yMin), yMax(_yMax) {};

  Result operator()(std::vector<Result> args, const vector<string> &argNames) const {
    assert(args.size() == 3);

    // multiplication of the two observables is undefined
    // -> convert to REFERENCE
    Result nom1 = args[0];
    nom1 = SetReference(nom1);
    nom1 = RebinRapidity(nom1, {yMin, yMax}).Projection({"Centrality"});
    Result nom2 = args[1];
    nom2 = SetReference(nom2);
    nom2 = RebinRapidity(nom2, {yMin, yMax}).Projection({"Centrality"});
    Result denom = args[2];
    return Qn::Sqrt(nom1 * nom2 / denom);
  }
};

struct Resolution4S {
  float yMin{0.4};
  float yMax{1.2};

  Resolution4S () = default;
  Resolution4S (float _yMin, float _yMax) :
	  yMin(_yMin), yMax(_yMax) {};

  Result operator()(vector<Result> args, const vector<string> &) {

    if (args.size() == 3) {
      Result nom = args[0];
      Result RT = args[1];
      Result denom = args[2];
      denom = SetReference(denom);
      denom = RebinRapidity(denom, {yMin, yMax}).Projection({"Centrality"});
      return nom * RT / denom;
    } else if (args.size() == 2) {
      Result nom = args[0];
      Result RT = args[1];
      nom = SetReference(nom);
      nom = RebinRapidity(nom, {yMin, yMax}).Projection({"Centrality"});
      return nom / RT;
    }

    assert(0);
    return {};
  }

};

struct ResolutionMH {
  float yMin{-0.6};
  float yMax{0.6};

  ResolutionMH () = default;
  ResolutionMH (float _yMin, float _yMax) :
	  yMin(_yMin), yMax(_yMax) {};

  Result operator()(std::vector<Result> args, const vector<string> &argNames) const {
    assert(args.size() == 3);

    auto nom1 = args[0];
    auto nom2 = args[1];
    auto denom = args[2];

    // flip sign
    if (argNames[0].find("X2YY") < argNames[0].size()) {
      nom1 = nom1 * (-1);
    }

    // flip sign
    if (argNames[2].find("X2YY") < argNames[2].size()) {
      denom = denom * (-1);
    }

    nom1 = RebinRapidity(nom1, {yMin, yMax}).Projection({"Centrality"});
    denom = RebinRapidity(denom, {yMin, yMax}).Projection({"Centrality"});

    return Qn::Sqrt(nom1 * nom2 / denom);
  }
};

Result ResolutionMC(std::vector<Result> args, const vector<string> &) {
  assert(args.size() == 1);

  auto nom1 = args[0];
  return nom1;
}

Result FlowV1(std::vector<Result> args, const vector<string> &) {
  assert(args.size() == 2);

  auto uQ = args[0];
  auto R = args[1];
  return uQ / R;
}

Result FlowV1MC(std::vector<Result> args, const vector<string> &) {
  assert(args.size() == 1);

  auto nom1 = args[0];
  return nom1;
}

Result FlowV2Opt1(std::vector<Result> args, const vector<string> &argNames) {
  assert(args.size() == 3);

  auto uQQ = args[0];
  // flip sign
  if (argNames[0].find("X2YY") < argNames[0].size()) {
    uQQ = uQQ * (-1);
  }

  auto R1 = args[1];
  auto R2 = args[2];
  return uQQ * 4 / (R1 * R2);

}

Result FlowV2Opt2(std::vector<Result> args, const vector<string> &argNames) {
  assert(args.size() == 4);

  auto uQQ1 = args[0];
  auto uQQ2 = args[1];
  // no need to flip sign since Opt2 is irrelevant for
  // X2YY
  auto QQ1 = args[2];
  auto QQ2 = args[3];
  return Qn::Sqrt(uQQ1 * uQQ2 * 2 * 2 / (QQ1 * QQ2)) * 4;
}

Result FlowV2Opt3(std::vector<Result> args, const vector<string> &argNames) {
  assert(args.size() == 2);

  auto uQQ = args[0];
  // flip sign
  if (argNames[0].find("X2YY") < argNames[0].size()) {
    uQQ = uQQ * (-1);
  }

  auto QQ = args[1];
  return uQQ * 4 * 2 / QQ;
}

int process(
    std::string input_name = "/home/eugene/analysis/flow/na61/pbpb/13agev/16_011/corr.root",
    std::string output_name = "/home/eugene/analysis/flow/na61/pbpb/13agev/16_011/corr_processed.root"
) {

  using std::vector;
  using std::string;

  ResourceManager rm;
  rm.LoadFile(input_name);

  /** Resolution PSD **/
  {
    /** 3S **/
    rm.Define("RES_psd1_3S_X", {"psd1_psd2_XX", "psd3_psd1_XX", "psd2_psd3_XX"}, Resolution3Sub);
    rm.Define("RES_psd2_3S_X", {"psd1_psd2_XX", "psd2_psd3_XX", "psd3_psd1_XX"}, Resolution3Sub);
    rm.Define("RES_psd3_3S_X", {"psd3_psd1_XX", "psd2_psd3_XX", "psd1_psd2_XX"}, Resolution3Sub);

    rm.Define("RES_psd1_3S_Y", {"psd1_psd2_YY", "psd3_psd1_YY", "psd2_psd3_YY"}, Resolution3Sub);
    rm.Define("RES_psd2_3S_Y", {"psd1_psd2_YY", "psd2_psd3_YY", "psd3_psd1_YY"}, Resolution3Sub);
    rm.Define("RES_psd3_3S_Y", {"psd3_psd1_YY", "psd2_psd3_YY", "psd1_psd2_YY"}, Resolution3Sub);

    /** 4S **/
    rm.Define("RES_psd1_psd3_T_X", {"proton_y_psd1_XX", "proton_y_psd3_XX", "psd3_psd1_XX"}, ResolutionTrack{0.8, 1.2});
    rm.Define("RES_psd1_psd3_T_Y", {"proton_y_psd1_YY", "proton_y_psd3_YY", "psd3_psd1_YY"}, ResolutionTrack{0.8, 1.2});

    rm.Define("RES_psd1_4S_X", {"psd3_psd1_XX", "RES_psd1_psd3_T_X", "proton_y_psd3_XX"}, Resolution4S{0.8, 1.2});
    rm.Define("RES_psd2_4S_X", {"proton_y_psd2_XX", "RES_psd1_psd3_T_X"}, Resolution4S{0.8, 1.2});
    rm.Define("RES_psd3_4S_X", {"psd3_psd1_XX", "RES_psd1_psd3_T_X", "proton_y_psd1_XX"}, Resolution4S{0.8, 1.2});

    rm.Define("RES_psd1_4S_Y", {"psd3_psd1_YY", "RES_psd1_psd3_T_Y", "proton_y_psd3_YY"}, Resolution4S{0.8, 1.2});
    rm.Define("RES_psd2_4S_Y", {"proton_y_psd2_YY", "RES_psd1_psd3_T_Y"}, Resolution4S{0.8, 1.2});
    rm.Define("RES_psd3_4S_Y", {"psd3_psd1_YY", "RES_psd1_psd3_T_Y", "proton_y_psd1_YY"}, Resolution4S{0.8, 1.2});

    /** Mixed-harmonics **/
    {
      ResolutionMH resolutionMH{-0.4, 0.8};
      vector<string> ref_particles{"proton", "pion", "pion_neg"};

      rm.Define("RES_psd1_MH_X1_proton",
                {"proton_y_psd1_psd2_X2XX", "psd3_psd1_XX", "proton_y_psd2_psd3_X2XX"},
                resolutionMH);
      rm.Define("RES_psd1_MH_X2_proton",
                {"proton_y_psd1_psd2_Y2XY", "psd3_psd1_XX", "proton_y_psd2_psd3_Y2YX"},
                resolutionMH);
      rm.Define("RES_psd1_MH_X_proton", {"RES_psd1_MH_X1_proton", "RES_psd1_MH_X2_proton",}, Combine);
      rm.Define("RES_psd1_MH_Y1_proton",
                {"proton_y_psd1_psd2_X2YY", "psd3_psd1_YY", "proton_y_psd2_psd3_Y2XY"},
                resolutionMH);
      rm.Define("RES_psd1_MH_Y2_proton",
                {"proton_y_psd1_psd2_Y2YX", "psd3_psd1_YY", "proton_y_psd2_psd3_Y2XY"},
                resolutionMH);
      rm.Define("RES_psd1_MH_Y_proton", {"RES_psd1_MH_Y1_proton", "RES_psd1_MH_Y2_proton",}, Combine);

      rm.Define("RES_psd3_MH_X1_proton",
                {"proton_y_psd2_psd3_X2XX", "psd3_psd1_XX", "proton_y_psd1_psd2_X2XX"},
                resolutionMH);
      rm.Define("RES_psd3_MH_X2_proton",
                {"proton_y_psd2_psd3_Y2YX", "psd3_psd1_XX", "proton_y_psd1_psd2_Y2XY"},
                resolutionMH);
      rm.Define("RES_psd3_MH_X_proton", {"RES_psd3_MH_X1_proton", "RES_psd3_MH_X2_proton",}, Combine);
      rm.Define("RES_psd3_MH_Y1_proton",
                {"proton_y_psd2_psd3_Y2YX", "psd3_psd1_YY", "proton_y_psd1_psd2_Y2YX"},
                resolutionMH);
      rm.Define("RES_psd3_MH_Y2_proton",
                {"proton_y_psd2_psd3_X2YY", "psd3_psd1_YY", "proton_y_psd1_psd2_X2YY"},
                resolutionMH);
      rm.Define("RES_psd3_MH_Y_proton", {"RES_psd3_MH_Y1_proton", "RES_psd3_MH_Y2_proton",}, Combine);

      rm.Define("RES_psd1_MH_X1_pion",
                {"pion_y_psd1_psd2_X2XX", "psd3_psd1_XX", "pion_y_psd2_psd3_X2XX"},
                resolutionMH);
      rm.Define("RES_psd1_MH_X2_pion",
                {"pion_y_psd1_psd2_Y2XY", "psd3_psd1_XX", "pion_y_psd2_psd3_Y2YX"},
                resolutionMH);
      rm.Define("RES_psd1_MH_X_pion", {"RES_psd1_MH_X1_pion", "RES_psd1_MH_X2_pion",}, Combine);
      rm.Define("RES_psd1_MH_Y1_pion",
                {"pion_y_psd1_psd2_X2YY", "psd3_psd1_YY", "pion_y_psd2_psd3_Y2XY"},
                resolutionMH);
      rm.Define("RES_psd1_MH_Y2_pion",
                {"pion_y_psd1_psd2_Y2YX", "psd3_psd1_YY", "pion_y_psd2_psd3_Y2XY"},
                resolutionMH);
      rm.Define("RES_psd1_MH_Y_pion", {"RES_psd1_MH_Y1_pion", "RES_psd1_MH_Y2_pion",}, Combine);

      rm.Define("RES_psd3_MH_X1_pion",
                {"pion_y_psd2_psd3_X2XX", "psd3_psd1_XX", "pion_y_psd1_psd2_X2XX"},
                resolutionMH);
      rm.Define("RES_psd3_MH_X2_pion",
                {"pion_y_psd2_psd3_Y2YX", "psd3_psd1_XX", "pion_y_psd1_psd2_Y2XY"},
                resolutionMH);
      rm.Define("RES_psd3_MH_X_pion", {"RES_psd3_MH_X1_pion", "RES_psd3_MH_X2_pion",}, Combine);
      rm.Define("RES_psd3_MH_Y1_pion",
                {"pion_y_psd2_psd3_Y2YX", "psd3_psd1_YY", "pion_y_psd1_psd2_Y2YX"},
                resolutionMH);
      rm.Define("RES_psd3_MH_Y2_pion",
                {"pion_y_psd2_psd3_X2YY", "psd3_psd1_YY", "pion_y_psd1_psd2_X2YY"},
                resolutionMH);
      rm.Define("RES_psd3_MH_Y_pion", {"RES_psd3_MH_Y1_pion", "RES_psd3_MH_Y2_pion",}, Combine);

      rm.Define("RES_psd1_MH_X1_pion_neg",
                {"pion_neg_y_psd1_psd2_X2XX", "psd3_psd1_XX", "pion_neg_y_psd2_psd3_X2XX"},
                resolutionMH);
      rm.Define("RES_psd1_MH_X2_pion_neg",
                {"pion_neg_y_psd1_psd2_Y2XY", "psd3_psd1_XX", "pion_neg_y_psd2_psd3_Y2YX"},
                resolutionMH);
      rm.Define("RES_psd1_MH_X_pion_neg", {"RES_psd1_MH_X1_pion_neg", "RES_psd1_MH_X2_pion_neg",}, Combine);
      rm.Define("RES_psd1_MH_Y1_pion_neg",
                {"pion_neg_y_psd1_psd2_X2YY", "psd3_psd1_YY", "pion_neg_y_psd2_psd3_Y2XY"},
                resolutionMH);
      rm.Define("RES_psd1_MH_Y2_pion_neg",
                {"pion_neg_y_psd1_psd2_Y2YX", "psd3_psd1_YY", "pion_neg_y_psd2_psd3_Y2XY"},
                resolutionMH);
      rm.Define("RES_psd1_MH_Y_pion_neg", {"RES_psd1_MH_Y1_pion_neg", "RES_psd1_MH_Y2_pion_neg",}, Combine);

      rm.Define("RES_psd3_MH_X1_pion_neg",
                {"pion_neg_y_psd2_psd3_X2XX", "psd3_psd1_XX", "pion_neg_y_psd1_psd2_X2XX"},
                resolutionMH);
      rm.Define("RES_psd3_MH_X2_pion_neg",
                {"pion_neg_y_psd2_psd3_Y2YX", "psd3_psd1_XX", "pion_neg_y_psd1_psd2_Y2XY"},
                resolutionMH);
      rm.Define("RES_psd3_MH_X_pion_neg", {"RES_psd3_MH_X1_pion_neg", "RES_psd3_MH_X2_pion_neg",}, Combine);
      rm.Define("RES_psd3_MH_Y1_pion_neg",
                {"pion_neg_y_psd2_psd3_Y2YX", "psd3_psd1_YY", "pion_neg_y_psd1_psd2_Y2YX"},
                resolutionMH);
      rm.Define("RES_psd3_MH_Y2_pion_neg",
                {"pion_neg_y_psd2_psd3_X2YY", "psd3_psd1_YY", "pion_neg_y_psd1_psd2_X2YY"},
                resolutionMH);
      rm.Define("RES_psd3_MH_Y_pion_neg", {"RES_psd3_MH_Y1_pion_neg", "RES_psd3_MH_Y2_pion_neg",}, Combine);

    }

    /** MC **/
    rm.Define("RES_psd1_MC_X", {"psd1_psi_XX"}, ResolutionMC);
    rm.Define("RES_psd2_MC_X", {"psd1_psi_XX"}, ResolutionMC);
    rm.Define("RES_psd3_MC_X", {"psd3_psi_XX"}, ResolutionMC);
    rm.Define("RES_psd1_MC_Y", {"psd1_psi_YY"}, ResolutionMC);
    rm.Define("RES_psd2_MC_Y", {"psd1_psi_YY"}, ResolutionMC);
    rm.Define("RES_psd3_MC_Y", {"psd3_psi_YY"}, ResolutionMC);

  }

  /** V1 **/
  {
    std::vector<std::string> particles{
        "proton",
        "pion",
        "pion_neg",
        "mc_proton",
        "mc_pion",
        "mc_pion_neg"
    };
    std::vector<std::string> axes{"pT", "y"};
    std::vector<std::string> psd_references{"psd1", "psd2", "psd3"};
    std::vector<std::string> components{"XX", "YY"};
    std::vector<std::string> resolution_methods{"3S", "4S", "MC"};

    // proton_y_psd1_XX
    // particle:axis:psd_reference:component
    const char *uQ_pattern = "%s_%s_%s_%s";

    // RES_psd1_3S_X
    const char *R_pattern = "RES_%s_%s_%s";

    // v1_proton_y_psd1_3S_X
    const char *v1_pattern = "v1_%s_%s_%s_%s_%s";

    for (const auto &particle : particles) {
      for (const auto &axis : axes) {
        for (const auto &psd_reference : psd_references) {
          for (const auto &component_2 : components) {
            for (const auto &res_method : resolution_methods) {
              auto component_1 = component_2.substr(0, 1);
              std::string uQ_name{sprintf(uQ_pattern, particle, axis, psd_reference, component_2)};
              std::string R_name{sprintf(R_pattern, psd_reference, res_method, component_1)};
              std::string v1_name{sprintf(v1_pattern, particle, axis, psd_reference, res_method, component_1)};
              rm.Define(v1_name, {uQ_name, R_name}, FlowV1);
            } // methods
          } // components
        } // psd reference

        for (const string &res_method : resolution_methods) {
          // combine XY
          for (const string &psd_reference : psd_references) {
            std::string
                v1_name{sprintf("v1_%s_%s_%s_%s_CC", particle, axis, psd_reference, res_method)};
            std::string
                v1_pattern{sprintf("v1_%s_%s_%s_%s_(X|Y)", particle, axis, psd_reference, res_method)};
            rm.Define(v1_name, rm.GetMatchingName(v1_pattern), Combine);
          }

          // combine PSD
          for (const string &component_2 : components) {
            std::string v1_name
                {sprintf("v1_%s_%s_%s_%s_CR", particle, axis, res_method, component_2.substr(0, 1))};
            std::string v1_pattern
                {sprintf("v1_%s_%s_psd(1|2|3)_%s_%s", particle, axis, res_method, component_2.substr(0, 1))};
            rm.Define(v1_name, rm.GetMatchingName(v1_pattern), Combine);
          }
        }

        // Good combination for 3S method resolution: PSD1 and 3, X component only
        // PSD2 is contaminated by shower leakage
        // y is believed biased due to spectator spot
        {
          const std::string v1_name{sprintf("v1_%s_%s_3S_CA", particle, axis)};
          const std::string v1_pattern{sprintf("v1_%s_%s_psd(1|3)_3S_(X)", particle, axis)};
          rm.Define(v1_name, rm.GetMatchingName(v1_pattern), Combine);
        }

        // Good combination for 4S method resolution: PSD1,2,3, X component only
        // y is believed biased due to spectator spot
        {
          const std::string v1_name{sprintf("v1_%s_%s_4S_CA", particle, axis)};
          const std::string v1_pattern{sprintf("v1_%s_%s_psd(1|3)_4S_(X)", particle, axis)};
          rm.Define(v1_name, rm.GetMatchingName(v1_pattern), Combine);
        }
      } // axes
    } // particles

    // mc_proton_y_psi_XX
    // particle:axis:component
    const char *uQ_MC_pattern = "%s_%s_psi_%s";

    // v1_mc_proton_y_psi_X
    const char *v1_MC_pattern = "v1_%s_%s_psi_%s";

    for (const auto &particle : particles) {
      for (const auto &axis : axes) {
        for (const auto &component_2 : components) {
          auto component_1 = component_2.substr(0, 1);
          std::string uQ_name{sprintf(uQ_MC_pattern, particle, axis, component_2)};
          std::string v1_name{sprintf(v1_MC_pattern, particle, axis, component_1)};
          rm.Define(v1_name, {uQ_name}, FlowV1MC);
        } // components
      } // axes
    } // particles
  }

  /*** V2 ***/
  {
    std::vector<std::string> particles{
        "proton",
        "pion",
        "pion_neg",
        "mc_proton",
        "mc_pion"};
    std::vector<std::string> axes{"pT", "y"};
    std::vector<std::string> components{"X", "Y"};
    std::vector<std::string> refPsd{"psd1", "psd2", "psd3"};
    std::vector<std::string> refComponents{"XX", "YY", "XY", "YX"};

    for (const auto &particle : particles) {
      for (const auto &axis : axes) {
        for (const auto &refPSD1 : refPsd) {
          for (const auto &refPSD2 : refPsd) {
            if (refPSD1 == refPSD2) continue;

            for (const auto &component : components) {

              for (const auto &ref_component : refComponents) {
                /*********** opt 1 ***********/
                // this option is relevant for all combinations
                // let this to be default
                if (ref_component == "XY" || ref_component == "YX") {
                  string
                      v2_name{sprintf("v2_%s_%s_%s_%s_%s2%s", particle, axis, refPSD1, refPSD2, component, ref_component
                  )};

                  string
                      uQQ_name{sprintf("%s_%s_%s_%s_%s2%s", particle, axis, refPSD1, refPSD2, component, ref_component
                  )};

                  string
                      R1_name{sprintf("RES_%s_3S_%s", refPSD1, string(ref_component.begin(), ref_component.begin() + 1)
                  )};
                  string R2_name
                      {sprintf("RES_%s_3S_%s", refPSD2, string(ref_component.begin() + 1, ref_component.begin() + 2)
                      )};

                  rm.Define(v2_name, {uQQ_name, R1_name, R2_name}, FlowV2Opt1);
                }
                /*********** opt 2 ***********/
                if (ref_component == "XY" || ref_component == "YX") {
                  string
                      v2_name(sprintf("v2_%s_%s_%s_%s_%s2%s", particle, axis, refPSD1, refPSD2, component, ref_component
                  ));

                  string
                      uQQ1_name{sprintf("%s_%s_%s_%s_%s2%s", particle, axis, refPSD1, refPSD2, component, ref_component
                  )};
                  string uQQ2_name{sprintf("%s_%s_%s_%s_%s2%s",
                                           particle,
                                           axis,
                                           refPSD1,
                                           refPSD2,
                                           component,
                                           string(ref_component.rbegin(), ref_component.rend())
                  )};
                  string QQ1_name{sprintf("%s_%s_XX", refPSD1, refPSD2)};
                  string QQ2_name{sprintf("%s_%s_YY", refPSD1, refPSD2)};
//                  rm.Define(v2_name, {uQQ1_name, uQQ2_name, QQ1_name, QQ2_name}, FlowV2Opt2);
                }
                /*********** opt 3 ***********/
                if (ref_component == "XX" || ref_component == "YY") {
                  string
                      v2_name(sprintf("v2_%s_%s_%s_%s_%s2%s", particle, axis, refPSD1, refPSD2, component, ref_component
                  ));

                  string
                      uQQ_name{sprintf("%s_%s_%s_%s_%s2%s", particle, axis, refPSD1, refPSD2, component, ref_component
                  )};

                  string QQ_name{sprintf("%s_%s_%s", refPSD1, refPSD2, ref_component)};
                  rm.Define(v2_name, {uQQ_name, QQ_name}, FlowV2Opt3);

                }

              } // ref components

            } // component


            // combine non-zero components
            {
              string v2_pattern{sprintf("v2_%s_%s_%s_%s_(X2XX|X2YY|Y2XY|Y2YX)", particle, axis, refPSD1, refPSD2)};
              string v2_name{sprintf("v2_%s_%s_%s_%s_CC", particle, axis, refPSD1, refPSD2)};
              rm.Define(v2_name, rm.GetMatchingName(v2_pattern), Combine);
            }
          } // ref PSD2
        } // ref PSD1

        // combine reference PSD
        for (const string &component : {"X2XX", "X2YY", "Y2YX", "Y2XY"}) {
          string v2_pattern{sprintf("v2_%s_%s_psd(1|2|3)_psd(1|2|3)_%s", particle, axis, component)};
          string v2_name{sprintf("v2_%s_%s_%s_CR", particle, axis, component)};
          rm.Define(v2_name, rm.GetMatchingName(v2_pattern), Combine);
        }

        // Good combination: all PSDs and Y2XY & Y2YX only
        {
          string v2_pattern{sprintf("v2_%s_%s_psd(1|2|3)_psd(1|2|3)_(Y2XY|Y2YX)", particle, axis)};
          string v2_name{sprintf("v2_%s_%s_CA", particle, axis)};
          rm.Define(v2_name, rm.GetMatchingName(v2_pattern), Combine);
        }

      } // axes
    } // particles
  }
  rm.SaveAs(output_name);

  return 0;
}

