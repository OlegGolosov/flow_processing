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
  assert(args.size() < 2);
  Result result;
  if (args.size() == 3) {
    auto &nom1 = args[0];
    auto &nom2 = args[1];
    auto &denom = args[2];
    result = Qn::Sqrt(nom1 * nom2 / denom);
  }
  else if (args.size() == 2) {
    auto &nom1 = args[0];
    auto &nom2 = args[1];
    result = nom1 * nom2;
  }
  return result;
}

struct ResolutionTrack {
  float yMin{0.4};
  float yMax{1.2};
  float scale{1.};

  ResolutionTrack () = default;
  ResolutionTrack (float _yMin, float _yMax, float _scale) :
	  yMin(_yMin), yMax(_yMax), scale(_scale) {};
  ResolutionTrack (std::vector <float> prop) :
	  yMin(prop.at(0)), yMax(prop.at(1)), scale(prop.at(2)) {};

  Result operator()(std::vector<Result> args, const vector<string> &argNames) const {
    //assert(args.size() == 3);
    assert(args.size() > 0);

    // multiplication of the two observables is undefined
    // -> convert to REFERENCE
    Result result;
    if (args.size() == 3)
    {
      Result nom1 = args[0];
      nom1 = SetReference(nom1);
      nom1 = RebinRapidity(nom1, {yMin, yMax}).Projection({"Centrality"});
      Result nom2 = args[1];
      nom2 = SetReference(nom2);
      nom2 = RebinRapidity(nom2, {yMin, yMax}).Projection({"Centrality"});
      Result denom = args[2];
      result = Qn::Sqrt(nom1 * nom2 / denom);
    }
    else if (args.size() == 2)
    {
      Result nom1 = args[0];
      nom1 = SetReference(nom1);
      nom1 = RebinRapidity(nom1, {yMin, yMax}).Projection({"Centrality"});
      Result nom2 = args[1];
      if(nom2.GetAxes().size() > 1)
      {
        nom2 = SetReference(nom2);
        nom2 = RebinRapidity(nom2, {yMin, yMax}).Projection({"Centrality"});
      }
      result = nom1 * nom2;
    }
    else if (args.size() == 1)
    {
      Result nom1 = args[0];
      nom1 = SetReference(nom1);
      nom1 = RebinRapidity(nom1, {yMin, yMax}).Projection({"Centrality"});
      result = nom1;
    }
    return result*scale;
  }
};

struct Resolution4S {
  float yMin{0.4};
  float yMax{1.2};
  float scale{1.};

  Resolution4S () = default;
  Resolution4S (float _yMin, float _yMax, float _scale) :
	  yMin(_yMin), yMax(_yMax), scale(_scale) {};
    
  Resolution4S (vector<float> props) :
	  yMin(props.at(0)), yMax(props.at(1)), scale(props.at(2)) {};

  Result operator()(vector<Result> args, const vector<string> &) {
    Result result;
    if (args.size() == 3) {
      Result nom = args[0];
      Result RT = args[1];
      Result denom = args[2];
      denom = SetReference(denom);
      denom = RebinRapidity(denom, {yMin, yMax}).Projection({"Centrality"});
      result = nom * RT / denom;
    } else if (args.size() == 2) {
      Result nom = args[0];
      Result RT = args[1];
      nom = SetReference(nom);
      nom = RebinRapidity(nom, {yMin, yMax}).Projection({"Centrality"});
      result = nom / RT;
    } else if (args.size() == 1) {
      Result nom = args[0];
      nom = SetReference(nom);
      nom = RebinRapidity(nom, {yMin, yMax}).Projection({"Centrality"});
      result = nom;
    }

    return result*scale;
  }

};

struct ResolutionMH {
  float yMin{-0.6};
  float yMax{0.6};

  ResolutionMH () = default;
  ResolutionMH (float _yMin, float _yMax) :
	  yMin(_yMin), yMax(_yMax) {};
  ResolutionMH (std::vector<float> prop) :
	  yMin(prop.at(0)), yMax(prop.at(1)) {};

  Result operator()(std::vector<Result> args, const vector<string> &argNames) const {
    assert(args.size() == 3 || args.size() == 1);
    Result result;
    if (args.size() == 3)
    {
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

      result = Qn::Sqrt(nom1 * nom2 / denom);
    }
    else if (args.size() == 1)
    {
      auto nom1 = args[0];

      // flip sign
      if (argNames[0].find("X2YY") < argNames[0].size()) {
        nom1 = nom1 * (-1);
      }

      result = RebinRapidity(nom1, {yMin, yMax}).Projection({"Centrality"});
    }
    return result;
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
    std::string input_name = "corr_2.root",
    std::string output_name = "corr_processed.root"
) {
  using std::vector;
  using std::string;

  ResourceManager rm;
  rm.LoadFile(input_name);

  /** Resolution PSD **/
  {
    
    /** 4S **/
    std::map <const char*, vector<float>> detProp4S = {
      //{"proton",   {-0.6, -0.2,  -1., 1.}},
      {"pion_pos_res", { 0.8,  1.2,  -1., 1.}},
      //{"pion_neg", { 0.8,  1.2,  -1., 1.}},
      //{"mcPid_proton",   {-0.6, -0.2,  -1., 1.}},
      {"mcPid_pion_pos", { 0.8,  1.2,  -1., 1.}},
      {"mcPid_pion_neg", { 0.8,  1.2,  -1., 1.}},
    };
    for (auto props:detProp4S)
    {
      auto particle=props.first;
      auto prop=props.second;
      for (auto comp:{"XX","YY","XY","YX"})
      {
        for (auto psd:{"psd1","psd2","psd3"})
        {
          rm.Define(Form ("%s_%s_%s", particle, psd, comp), {Form ("%s_y_%s_%s", particle, psd, comp)}, ResolutionTrack{prop.at(0), prop.at(1), prop.at(3)});
          rm.Define(Form ("%s_psi_%s_psi_%s", particle, psd, comp), {Form("%s_y_psi_%s", particle, comp), Form("%s_psi_%s", psd, comp)}, ResolutionTrack{prop.at(0), prop.at(1), 1.});
        }
      }
      rm.Define(Form("RES_%s_MC_X",particle), {Form("%s_y_psi_XX", particle)}, ResolutionTrack{prop.at(0), prop.at(1), prop.at(2)});
      rm.Define(Form("RES_%s_MC_Y",particle), {Form("%s_y_psi_YY", particle)}, ResolutionTrack{prop.at(0), prop.at(1), prop.at(2)});
      rm.Define(Form("RES_%s_psd1_psd3_X", particle), {Form("%s_y_psd1_XX", particle), Form("%s_y_psd3_XX", particle), "psd3_psd1_XX"}, ResolutionTrack{prop.at(0), prop.at(1), prop.at(3)});
      rm.Define(Form("RES_%s_psd1_psd3_Y", particle), {Form("%s_y_psd1_YY", particle), Form("%s_y_psd3_YY", particle), "psd3_psd1_YY"}, ResolutionTrack{prop.at(0), prop.at(1), prop.at(3)});
      rm.Define(Form("RES_psd1_%s_4S_X", particle), {"psd3_psd1_XX", Form("RES_%s_psd1_psd3_X", particle), Form("%s_y_psd3_XX", particle)}, Resolution4S{prop.at(0), prop.at(1), prop.at(2)});
      rm.Define(Form("RES_psd2_%s_4S_X", particle), {Form("%s_y_psd2_XX", particle), Form("RES_%s_psd1_psd3_X", particle)}, Resolution4S{prop.at(0), prop.at(1), prop.at(2)});
      rm.Define(Form("RES_psd3_%s_4S_X", particle), {"psd3_psd1_XX", Form("RES_%s_psd1_psd3_X", particle), Form("%s_y_psd1_XX", particle)}, Resolution4S{prop.at(0), prop.at(1), prop.at(2)});
      rm.Define(Form("RES_psd1_%s_4S_Y", particle), {"psd3_psd1_YY", Form("RES_%s_psd1_psd3_Y", particle), Form("%s_y_psd3_YY", particle)}, Resolution4S{prop.at(0), prop.at(1), prop.at(2)});
      rm.Define(Form("RES_psd2_%s_4S_Y", particle), {Form("%s_y_psd2_YY", particle), Form("RES_%s_psd1_psd3_Y", particle)}, Resolution4S{prop.at(0), prop.at(1), prop.at(2)});
      rm.Define(Form("RES_psd3_%s_4S_Y", particle), {"psd3_psd1_YY", Form("RES_%s_psd1_psd3_Y", particle), Form("%s_y_psd1_YY", particle)}, Resolution4S{prop.at(0), prop.at(1), prop.at(2)});
    }

    /** 3S **/
    rm.Define("RES_psd1_3S_X", {"psd1_psd2_XX", "psd3_psd1_XX", "psd2_psd3_XX"}, Resolution3Sub);
    rm.Define("RES_psd2_3S_X", {"psd1_psd2_XX", "psd2_psd3_XX", "psd3_psd1_XX"}, Resolution3Sub);
    rm.Define("RES_psd3_3S_X", {"psd3_psd1_XX", "psd2_psd3_XX", "psd1_psd2_XX"}, Resolution3Sub);

    rm.Define("RES_psd1_3S_Y", {"psd1_psd2_YY", "psd3_psd1_YY", "psd2_psd3_YY"}, Resolution3Sub);
    rm.Define("RES_psd2_3S_Y", {"psd1_psd2_YY", "psd2_psd3_YY", "psd3_psd1_YY"}, Resolution3Sub);
    rm.Define("RES_psd3_3S_Y", {"psd3_psd1_YY", "psd2_psd3_YY", "psd1_psd2_YY"}, Resolution3Sub);
    
    for (auto comp:{"XX","YY","XY","YX"})
    {
      for (auto psd1:{"psd1","psd2","psd3"})
      {
        for (auto psd2:{"psd1","psd2","psd3"})
        {
          rm.Define(Form ("%s_psi_%s_psi_%s", psd1, psd2, comp), {Form("%s_psi_%s", psd1, comp), Form("%s_psi_%s", psd2, comp)}, Resolution3Sub);
        }
      }
    }
    /** Mixed-harmonics **/
    {      
      std::map <const char*, vector<float>> detPropMH = {
        //{"proton",   {-0.4, 0.8, 1.}},
        {"pion_pos_res", {-0.4, 0.8, 1.}},
        //{"pion_neg", {-0.4, 0.8, 1.}},
      };

      for (auto props:detPropMH)
      {
        auto particle = props.first;
        auto prop = props.second;
        for (auto comp:{"X2XX","X2YX","X2XY","X2YY","Y2XX","Y2YX","Y2XY","Y2YY"})
        {
          rm.Define(Form ("%s_psd1_psd2_%s", particle, comp), {Form ("%s_y_psd1_psd2_%s", particle, comp)}, ResolutionMH{prop});
          rm.Define(Form ("%s_psd2_psd3_%s", particle, comp), {Form ("%s_y_psd2_psd3_%s", particle, comp)}, ResolutionMH{prop});
        }
        rm.Define(Form("RES_psd1_%s_MH_X1", particle),
                  {Form("%s_y_psd1_psd2_X2XX", particle), "psd3_psd1_XX", Form("%s_y_psd2_psd3_X2XX", particle)},
                  ResolutionMH{prop});
        rm.Define(Form("RES_psd1_%s_MH_X2", particle),
                  {Form("%s_y_psd1_psd2_Y2XY", particle), "psd3_psd1_XX", Form("%s_y_psd2_psd3_Y2YX", particle)},
                  ResolutionMH{prop});
        rm.Define(Form("RES_psd1_%s_MH_X", particle), {Form("RES_psd1_%s_MH_X1", particle), Form("RES_psd1_%s_MH_X2", particle),}, Combine);
        rm.Define(Form("RES_psd1_%s_MH_Y1", particle),
                  {Form("%s_y_psd1_psd2_X2YY", particle), "psd3_psd1_YY", Form("%s_y_psd2_psd3_Y2XY", particle)},
                  ResolutionMH{prop});
        rm.Define(Form("RES_psd1_%s_MH_Y2", particle),
                  {Form("%s_y_psd1_psd2_Y2YX", particle), "psd3_psd1_YY", Form("%s_y_psd2_psd3_Y2XY", particle)},
                  ResolutionMH{prop});
        rm.Define(Form("RES_psd1_%s_MH_Y", particle), {Form("RES_psd1_%s_MH_Y1", particle), Form("RES_psd1_%s_MH_Y2", particle),}, Combine);

        rm.Define(Form("RES_psd3_%s_MH_X1", particle),
                  {Form("%s_y_psd2_psd3_X2XX", particle), "psd3_psd1_XX", Form("%s_y_psd1_psd2_X2XX", particle)},
                  ResolutionMH{prop});
        rm.Define(Form("RES_psd3_%s_MH_X2", particle),
                  {Form("%s_y_psd2_psd3_Y2YX", particle), "psd3_psd1_XX", Form("%s_y_psd1_psd2_Y2XY", particle)},
                  ResolutionMH{prop});
        rm.Define(Form("RES_psd3_%s_MH_X", particle), {Form("RES_psd3_%s_MH_X1", particle), Form("RES_psd3_%s_MH_X2", particle),}, Combine);
        rm.Define(Form("RES_psd3_%s_MH_Y1", particle),
                  {Form("%s_y_psd2_psd3_Y2XY", particle), "psd3_psd1_YY", Form("%s_y_psd1_psd2_Y2YX", particle)},
                  ResolutionMH{prop});
        rm.Define(Form("RES_psd3_%s_MH_Y2", particle),
                  {Form("%s_y_psd2_psd3_X2YY", particle), "psd3_psd1_YY", Form("%s_y_psd1_psd2_X2YY", particle)},
                  ResolutionMH{prop});
        rm.Define(Form("RES_psd3_%s_MH_Y", particle), {Form("RES_psd3_%s_MH_Y1", particle), Form("RES_psd3_%s_MH_Y2", particle),}, Combine);
      }
    }
    
    /** MC **/
    for (auto det:{"psd1","psd2","psd3"})
    {
      rm.Define(Form("RES_%s_MC_X",det), {Form("%s_psi_XX", det)}, ResolutionMC);
      rm.Define(Form("RES_%s_MC_Y",det), {Form("%s_psi_YY", det)}, ResolutionMC);
    }
  }

  /** V1 **/
  {
    std::vector<std::string> particles{
        "proton_b",
        "proton_f",
        "pion_pos",
        "pion_neg",
        //"kaon_pos",
        //"kaon_neg",
        "mcPid_proton_b",
        "mcPid_proton_f",
        "mcPid_pion_pos",
        "mcPid_pion_neg",
        //"mcPid_kaon_pos",
        //"mcPid_kaon_neg",
        "mc_proton_b",
        "mc_proton_f",
        "mc_pion_pos",
        "mc_pion_neg",
        //"mc_kaon_pos",
        //"mc_kaon_neg",
    };
    std::vector<std::string> axes{"pT", "y"};
    std::vector<std::string> psd_references{"psd1", "psd2", "psd3"};
    std::vector<std::string> components{"XX", "YY"};
    std::vector<std::string> resolution_methods{"3S", "MC", /*"proton_4S", "pion_neg_4S", */"pion_pos_res_4S", /*"proton_MH", "pion_neg_MH", */"pion_pos_res_MH"/*, "proton_4S", "pion_neg_4S", "pion_pos_4S", "proton_MH", "pion_neg_MH", "pion_pos_MH"*/};

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

//  /*** V2 ***/
//  {
//    std::vector<std::string> particles{
//        "proton",
//        "mcPid_proton",
//        "mc_proton",
//        "pion_pos",
//        "mcPid_pion_pos",
//        "mc_pion_pos",
//        "pion_neg",
//        "mcPid_pion_neg",
//        "mc_pion_neg",
//        "kaon_pos",
//        "mcPid_kaon_pos",
//        "mc_kaon_pos",
//        "kaon_neg",
//        "mcPid_kaon_neg",
//        "mc_kaon_neg"};
//    std::vector<std::string> axes{"pT", "y"};
//    std::vector<std::string> components{"X", "Y"};
//    std::vector<std::string> refPsd{"psd1", "psd2", "psd3"};
//    std::vector<std::string> refComponents{"XX", "YY", "XY", "YX"};
//
//    for (const auto &particle : particles) {
//      for (const auto &axis : axes) {
//        for (const auto &refPSD1 : refPsd) {
//          for (const auto &refPSD2 : refPsd) {
//            if (refPSD1 == refPSD2) continue;
//
//            for (const auto &component : components) {
//
//              for (const auto &ref_component : refComponents) {
//                /*********** opt 1 ***********/
//                // this option is relevant for all combinations
//                // let this to be default
//                if (ref_component == "XY" || ref_component == "YX") {
//                  string
//                      v2_name{sprintf("v2_%s_%s_%s_%s_%s2%s", particle, axis, refPSD1, refPSD2, component, ref_component
//                  )};
//
//                  string
//                      uQQ_name{sprintf("%s_%s_%s_%s_%s2%s", particle, axis, refPSD1, refPSD2, component, ref_component
//                  )};
//
//                  string
//                      R1_name{sprintf("RES_%s_3S_%s", refPSD1, string(ref_component.begin(), ref_component.begin() + 1)
//                  )};
//                  string R2_name
//                      {sprintf("RES_%s_3S_%s", refPSD2, string(ref_component.begin() + 1, ref_component.begin() + 2)
//                      )};
//
//                  rm.Define(v2_name, {uQQ_name, R1_name, R2_name}, FlowV2Opt1);
//                }
//                /*********** opt 2 ***********/
//                if (ref_component == "XY" || ref_component == "YX") {
//                  string
//                      v2_name(sprintf("v2_%s_%s_%s_%s_%s2%s", particle, axis, refPSD1, refPSD2, component, ref_component
//                  ));
//
//                  string
//                      uQQ1_name{sprintf("%s_%s_%s_%s_%s2%s", particle, axis, refPSD1, refPSD2, component, ref_component
//                  )};
//                  string uQQ2_name{sprintf("%s_%s_%s_%s_%s2%s",
//                                           particle,
//                                           axis,
//                                           refPSD1,
//                                           refPSD2,
//                                           component,
//                                           string(ref_component.rbegin(), ref_component.rend())
//                  )};
//                  string QQ1_name{sprintf("%s_%s_XX", refPSD1, refPSD2)};
//                  string QQ2_name{sprintf("%s_%s_YY", refPSD1, refPSD2)};
////                  rm.Define(v2_name, {uQQ1_name, uQQ2_name, QQ1_name, QQ2_name}, FlowV2Opt2);
//                }
//                /*********** opt 3 ***********/
//                if (ref_component == "XX" || ref_component == "YY") {
//                  string
//                      v2_name(sprintf("v2_%s_%s_%s_%s_%s2%s", particle, axis, refPSD1, refPSD2, component, ref_component
//                  ));
//
//                  string
//                      uQQ_name{sprintf("%s_%s_%s_%s_%s2%s", particle, axis, refPSD1, refPSD2, component, ref_component
//                  )};
//
//                  string QQ_name{sprintf("%s_%s_%s", refPSD1, refPSD2, ref_component)};
//                  rm.Define(v2_name, {uQQ_name, QQ_name}, FlowV2Opt3);
//
//                }
//
//              } // ref components
//
//            } // component
//
//
//            // combine non-zero components
//            {
//              string v2_pattern{sprintf("v2_%s_%s_%s_%s_(X2XX|X2YY|Y2XY|Y2YX)", particle, axis, refPSD1, refPSD2)};
//              string v2_name{sprintf("v2_%s_%s_%s_%s_CC", particle, axis, refPSD1, refPSD2)};
//              rm.Define(v2_name, rm.GetMatchingName(v2_pattern), Combine);
//            }
//          } // ref PSD2
//        } // ref PSD1
//
//        // combine reference PSD
//        for (const string &component : {"X2XX", "X2YY", "Y2YX", "Y2XY"}) {
//          string v2_pattern{sprintf("v2_%s_%s_psd(1|2|3)_psd(1|2|3)_%s", particle, axis, component)};
//          string v2_name{sprintf("v2_%s_%s_%s_CR", particle, axis, component)};
//          rm.Define(v2_name, rm.GetMatchingName(v2_pattern), Combine);
//        }
//
//        // Good combination: all PSDs and Y2XY & Y2YX only
//        {
//          string v2_pattern{sprintf("v2_%s_%s_psd(1|2|3)_psd(1|2|3)_(Y2XY|Y2YX)", particle, axis)};
//          string v2_name{sprintf("v2_%s_%s_CA", particle, axis)};
//          rm.Define(v2_name, rm.GetMatchingName(v2_pattern), Combine);
//        }
//
//      } // axes
//    } // particles
//  }
  rm.SaveAs(output_name);

  return 0;
}

