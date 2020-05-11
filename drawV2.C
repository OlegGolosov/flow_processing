//
// Created by eugene on 4/17/19.
//

#ifdef __CLING__

R__LOAD_LIBRARY(libQnCorrections.so)
R__LOAD_LIBRARY(libBase.so)

#endif

#include <TSystem.h>
#include "include/Utils.h"
#include <TLegend.h>

const int ww = 1280;
const int wh = 720;

void ExportPtYCanvas(
    TMultiGraph &ptMG,
    TMultiGraph &yMG,
    const std::string &exportTo
    ) {

  TCanvas c("noname", "", ww, wh);
  c.Divide(2, 1);

  c.cd(1);
  gPad->SetGridy();

  ptMG.DrawClone("A");
  gPad->BuildLegend();

  c.cd(2);
  gPad->SetGridy();
  yMG.DrawClone("A");

  c.SaveAs(exportTo.c_str());
}



int drawV2(
    const std::string &filename = "~/analysis/flow/na61/pbpb/13agev/16_011/20190520/profiles.root",
    const std::string &canvasDir = "~/Documents/conferences/20190607_WPCF/v2"
) {

  using namespace std;

  TFile ff(filename.c_str(), "read");
  TDirectory *dir{nullptr};

  string output_path = string(gSystem->DirName(filename.c_str())) + "/" + "v2.root";
  TFile fo(output_path.c_str(), "recreate");

  string name;
  vector<string> matches;
  shared_ptr<TGraphAsymmErrors> graph;

  ff.GetObject("v2_combined", dir);
  // V2 vs centrality
  if (dir) {

    map<string, int> centralityColors{
        {"0", kRed},
        {"1", kOrange + 1},
        {"2", kGreen + 2},
        {"3", kBlue},
        {"4", kMagenta + 2}
    };

    for (const string &particle : {"proton", "pion_pos", "pion_neg"}) {

      TMultiGraph v2vsPtCentralityMG;
      TMultiGraph v2vsYCentralityMG;

      string pattern{Form("v2_%s_(pT|y)_CA_(\\d+)", particle.c_str())};
      for (const auto &match : GetResourcesMatchingName<TGraphAsymmErrors>(dir, pattern)) {
        tie(name, matches, graph) = match;

        string axis = matches[1];
        string centralityClass = matches[2];

        // style
        graph->SetLineColor(centralityColors.at(centralityClass));
        graph->SetMarkerColor(centralityColors.at(centralityClass));
        graph->SetMarkerStyle(kFullCircle);
        graph->SetLineWidth(2);

        if (axis == "pT") {
          v2vsPtCentralityMG.Add(new TGraphAsymmErrors(*graph), "pZ");
        } else if (axis == "y") {
          v2vsYCentralityMG.Add(new TGraphAsymmErrors(*graph), "pZ");
        }
      }

      ShiftGraphsX(v2vsPtCentralityMG, 0.02);
      v2vsPtCentralityMG.GetXaxis()->SetTitle("p_{T} (GeV/#it{c})");

      ShiftGraphsX(v2vsYCentralityMG, 0.01);
      v2vsYCentralityMG.GetXaxis()->SetTitle("#it{y}_{CM}");

      fo.WriteObject(&v2vsPtCentralityMG, Form("v2vsPtCentralityMG_%s", particle.c_str()));
      fo.WriteObject(&v2vsYCentralityMG, Form("v2vsYCentralityMG_%s", particle.c_str()));

      if (!canvasDir.empty()) {
        ExportPtYCanvas(v2vsPtCentralityMG, v2vsYCentralityMG, canvasDir + "/" + "v2Centrality_" + particle + ".C");
      }

    }
  }

  dir = nullptr;
  ff.GetObject("v2_combined_15_35", dir);

  if (dir) {

    map<string, int> color{
        {"proton", kGreen + 2},
        {"pion_pos", kRed},
        {"pion_neg", kBlue},
    };

    map<string, int> markerStyle{
        {"proton", kFullDiamond},
        {"pion_pos", kFullCircle},
        {"pion_neg", kFullSquare},
    };

    map<string, string> label{
        {"proton", "p"},
        {"pion_pos", "#pi^{+}"},
        {"pion_neg", "#pi^{-}"},
    };

    TMultiGraph v2vsPtParticleMG;
    TMultiGraph v2vsYParticleMG;

    string pattern{Form("v2_(pion_neg|proton)_(pT|y)_CA_0")};
    for (const auto &match : GetResourcesMatchingName<TGraphAsymmErrors>(dir, pattern)) {
      tie(name, matches, graph) = match;

      string particle = matches[1];
      string axis = matches[2];

      graph->SetTitle(label.at(particle).c_str());

      graph->SetLineColor(color.at(particle));
      graph->SetMarkerColor(color.at(particle));
      graph->SetMarkerStyle(markerStyle.at(particle));
      graph->SetLineWidth(2.0);

      if (markerStyle.at(particle) == kFullDiamond) {
        graph->SetMarkerSize(1.5);
      }

      if (axis == "pT") {
        v2vsPtParticleMG.Add(new TGraphAsymmErrors(*graph), "pZ");
      } else if (axis == "y") {
        v2vsYParticleMG.Add(new TGraphAsymmErrors(*graph), "pZ");
      }
    }

    ShiftGraphsX(v2vsPtParticleMG, 0.02);
    v2vsPtParticleMG.GetXaxis()->SetTitle("p_{T} (GeV/#it{c})");

    ShiftGraphsX(v2vsYParticleMG, 0.01);
    v2vsYParticleMG.GetXaxis()->SetTitle("#it{y}_{CM}");

    fo.WriteObject(&v2vsPtParticleMG, "v2vsPtParticleMG");
    fo.WriteObject(&v2vsYParticleMG,  "v2vsYParticleMG");

    if (!canvasDir.empty()) {
      ExportPtYCanvas(v2vsPtParticleMG, v2vsYParticleMG, canvasDir + "/" + "v2Particle.C");
    }
  }

  // systematics
  if (dir) {
    map<string, int> color{
        {"psd1_psd2", kGreen + 2},
        {"psd2_psd3", kRed},
        {"psd3_psd1", kBlue},
    };

    for (const string &particle : {"proton", "pion_pos", "pion_neg"}) {
      string pattern{Form("v2_%s_(pT|y)_(psd\\d_psd\\d)_CC_0", particle.c_str())};

      TMultiGraph v2vsYRefSubevents;
      TMultiGraph v2vsPtRefSubevents;
      for (const auto &match : GetResourcesMatchingName<TGraphAsymmErrors>(dir, pattern)) {
        tie(name, matches, graph) = match;

        string axis{matches[1]};
        string refSubevents{matches[2]};

        graph->SetTitle(refSubevents.c_str());
        graph->SetLineColor(color.at(refSubevents));
        graph->SetMarkerColor(color.at(refSubevents));

        if (axis == "y") {
          v2vsYRefSubevents.Add(new TGraphAsymmErrors(*graph), "p");
        } else if (axis == "pT") {
          v2vsPtRefSubevents.Add(new TGraphAsymmErrors(*graph), "p");
        }
      }

      ShiftGraphsX(v2vsYRefSubevents, 0.01);
      v2vsYRefSubevents.GetXaxis()->SetTitle("#it{y}_{CM}");

      ShiftGraphsX(v2vsPtRefSubevents, 0.02);
      v2vsPtRefSubevents.GetXaxis()->SetTitle("p_{T} (GeV/#it{c})");

      fo.WriteObject(&v2vsYRefSubevents, Form("v2vsYRefSubevents_%s", particle.c_str()));
      fo.WriteObject(&v2vsPtRefSubevents, Form("v2vsPtRefSubevents_%s", particle.c_str()));

      if (!canvasDir.empty()) {
        ExportPtYCanvas(v2vsPtRefSubevents, v2vsYRefSubevents, canvasDir + "/" + "v2RefSubevents_" + particle + ".C");
      }
    }

    for (const string &particle : {"proton", "pion_pos", "pion_neg"}) {
      string pattern{Form("v2_%s_(pT|y)_(X2XX|X2YY|Y2XY|Y2YX)_CR_0", particle.c_str())};

      TMultiGraph v2vsYComponents;
      TMultiGraph v2vsPtComponents;
      for (const auto &match : GetResourcesMatchingName<TGraphAsymmErrors>(dir, pattern)) {
        tie(name, matches, graph) = match;

        const map<string, int> color{
            {"X2XX", kRed+1},
            {"X2YY", kMagenta},
            {"Y2XY", kBlue},
            {"Y2YX", kCyan+1}
        };

        string axis{matches[1]};
        string components{matches[2]};

        graph->SetTitle(components.c_str());
        graph->SetLineColor(color.at(components));
        graph->SetMarkerColor(color.at(components));

        if (axis == "y") {
          v2vsYComponents.Add(new TGraphAsymmErrors(*graph), "p");
        } else if (axis == "pT") {
          v2vsPtComponents.Add(new TGraphAsymmErrors(*graph), "p");
        }
      }

      ShiftGraphsX(v2vsYComponents, 0.01);
      v2vsYComponents.GetXaxis()->SetTitle("#it{y}_{CM}");

      ShiftGraphsX(v2vsPtComponents, 0.02);
      v2vsPtComponents.GetXaxis()->SetTitle("p_{T} (GeV/#it{c})");

      fo.WriteObject(&v2vsYComponents, Form("v2vsYComponents_%s", particle.c_str()));
      fo.WriteObject(&v2vsPtComponents, Form("v2vsPtComponents_%s", particle.c_str()));

      if (!canvasDir.empty()) {
        ExportPtYCanvas(v2vsPtComponents, v2vsYComponents, canvasDir + "/" + "v2Components_" + particle + ".C");
      }
    }
  }

  return 0;
}

