//
// Created by eugene on 5/21/19.
//

#ifdef __CLING__
R__LOAD_LIBRARY(libQnCorrections.so)
R__LOAD_LIBRARY(libBase.so)
#endif

#include <TSystem.h>
#include "include/Utils.h"

const int ww = 1280;
const int wh = 720;

TCanvas *MakeDoubleSidedCanvas(
    TMultiGraph &mgLeft,
    TMultiGraph &mgRight
) {
  auto c = new TCanvas("noname", "", ww, wh);

  c->Divide(2, 1);

  auto padLeft = c->cd(1);
  padLeft->SetGridy();
  mgLeft.Draw("A");

  auto padRight = c->cd(2);
  padRight->SetGridy();
  mgRight.Draw("A");

  return c;
}

int drawdV1dy(
    const std::string &filename = "~/analysis/flow/na61/pbpb/13agev/16_011/20190522_STARacc_T2T4/profiles.root",
    const std::string &canvasDir = "~/Documents/conferences/20190607_WPCF/dv1dy_STARacc"
) {

  using namespace std;
  using namespace fmt;

  TFile ff(filename.c_str(), "read");
  TDirectory *dir{nullptr};
  if (!(dir = dynamic_cast<TDirectory *>(ff.Get("v1_combined_origbin")))) {
    return 1;
  }

  string output_path = string(gSystem->DirName(filename.c_str())) + "/" + "dv1dy.root";
  TFile fo(output_path.c_str(), "recreate");

  TF1 fitFun("fit", "pol1");
  const double fitMin = -0.4;
  const double fitMax = 0.4;

  Qn::Axis centralityAxis{"Centrality", {0, 5, 10, 15, 25, 35, 45, 60, 80, 100.}};

  map<string, TGraphErrors> dv1dyGraphs;
  map<string, TGraphErrors> offsetGraphs;

  TMultiGraph dv1dyMG;
  TMultiGraph offsetMG;

  map<string, int> markerStyle{
      {"proton", kFullDiamond},
      {"pion", kFullCircle},
      {"pion_neg", kFullSquare},
  };

  map<string, int> color{
      {"proton", kGreen + 2},
      {"pion", kRed},
      {"pion_neg", kBlue},
  };

  map<string, string> label{
      {"proton", "p"},
      {"pion", "#pi^{+}"},
      {"pion_neg", "#pi^{-}"},
  };

  for (const string &particle : {"pion_neg", "proton", }) {
    string profilePattern{Form("v1_%s_y_4S_CA_(\\d+)", particle.c_str())};

    string name;
    vector<std::string> match_result;
    shared_ptr<TGraph> graph;
    for (const auto &match : GetResourcesMatchingName<TGraph>(dir, profilePattern)) {
      tie(name, match_result, graph) = match;

      int centralityClass = stoi(match_result[1]);

      graph->Fit(&fitFun, "Q", "", fitMin, fitMax);

      double centralityLo = centralityAxis.GetLowerBinEdge(centralityClass);
      double centralityHi = centralityAxis.GetUpperBinEdge(centralityClass);
      double centrality = 0.5 * (centralityLo + centralityHi);

      double offset = fitFun.GetParameter(0);
      double offsetError = fitFun.GetParError(0);

      double slope = fitFun.GetParameter(1);
      double slopeError = fitFun.GetParError(1);

      {
        auto insertResult = dv1dyGraphs.emplace(particle, TGraphErrors());
        TGraphErrors &dv1dyGraph = (*insertResult.first).second;
        dv1dyGraph.SetTitle(label.at(particle).c_str());
        dv1dyGraph.SetMarkerStyle(markerStyle.at(particle));
        dv1dyGraph.SetMarkerColor(color.at(particle));
        dv1dyGraph.SetLineColor(color.at(particle));
        dv1dyGraph.SetLineWidth(2);

        if (markerStyle.at(particle) == kFullDiamond) {
          dv1dyGraph.SetMarkerSize(1.5);
        }

        dv1dyGraph.SetPoint(centralityClass, centrality, slope);
        dv1dyGraph.SetPointError(centralityClass, 0., slopeError);
      }

      {
        auto insertResult = offsetGraphs.emplace(particle, TGraphErrors());
        TGraphErrors &offsetGraph = (*insertResult.first).second;
        offsetGraph.SetTitle(label.at(particle).c_str());
        offsetGraph.SetMarkerStyle(markerStyle.at(particle));
        offsetGraph.SetMarkerColor(color.at(particle));
        offsetGraph.SetLineColor(color.at(particle));
        offsetGraph.SetLineWidth(2);

        if (markerStyle.at(particle) == kFullDiamond) {
          offsetGraph.SetMarkerSize(1.5);
        }

        offsetGraph.SetPoint(centralityClass, centrality, offset);
        offsetGraph.SetPointError(centralityClass, 0., offsetError);
      }
    }

    string dv1dyGraphPattern{Form("dv1dy_%s", particle.c_str())};
    fo.WriteObject(&dv1dyGraphs.at(particle), dv1dyGraphPattern.c_str());

    string offsetGraphName{Form("offset_%s", particle.c_str())};
    fo.WriteObject(&offsetGraphs.at(particle), offsetGraphName.c_str());

    dv1dyMG.Add(&dv1dyGraphs.at(particle), "pZ");
    offsetMG.Add(&offsetGraphs.at(particle), "pZ");

  }

  dv1dyMG.GetXaxis()->SetTitle("Centrality (%)");
  offsetMG.GetXaxis()->SetTitle("Centrality (%)");

//  ShiftGraphsX(dv1dyMG, 1.);
//  ShiftGraphsX(offsetMG, 1.);

  fo.WriteObject(&dv1dyMG, "dv1dyVsParticle");
  fo.WriteObject(&offsetMG, "offsetVsParticle");


  if (!canvasDir.empty()) {
    auto labelLeft = MakePaveText({
                                      "#it{v}_{1} slope at #it{y} = 0",
                                      "centrality E_{PSD}",
                                      format("{} resolution", "4S"),
                                      format("{} < p_{{T}} < {} GeV/#it{{c}}", 0.0, 0.2),
                                  }, {0.4, 0.15, 0.85, 0.40});
    auto labelRight = MakePaveText({
                                       "#it{v}_{1} offset at #it{y} = 0",
                                  }, {0.4, 0.15, 0.85, 0.40});
    auto labelFitRange = MakePaveText({
                                      "fit range",
                                      format("{} < #it{{y}} < {}", -0.4, 0.4)
                                  }, {0.4, 0.15, 0.85, 0.40});
    auto c = MakeDoubleSidedCanvas(dv1dyMG, offsetMG);
    auto padLeft = c->cd(1);
    labelLeft->Draw();
    labelFitRange->Draw();
    auto padRight = c->cd(2);
    labelRight->Draw();

    c->SaveAs(format("{}/dv1dyAndOffset.C", canvasDir).c_str());

  }

  return 0;
}