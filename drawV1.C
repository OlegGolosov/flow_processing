//
// Created by eugene on 4/17/19.
//

#ifdef __CLING__

R__LOAD_LIBRARY(libQnCorrections.so)
R__LOAD_LIBRARY(libBase.so)

#endif

#include <TSystem.h>
#include <TLegend.h>
#include "include/Utils.h"

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

  c.cd(2);
  gPad->SetGridy();
  yMG.DrawClone("A");

  c.SaveAs(exportTo.c_str());
}

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

int drawV1(
    const std::string &filename = "~/analysis/flow/na61/pbpb/13agev/16_011/20190520/profiles.root",
    const std::string &canvasDir = "/home/eugene/Documents/conferences/20190607_WPCF/v1_4S"
) {

  using namespace std;
  using namespace fmt;

  const string resolutionMethod = {"4S"};

  const double yLo = 0.;
  const double yHi = 1.2;

  const double ptLo = 0.;
  const double ptHi = 2.0;

  TFile ff(filename.c_str(), "read");
  TDirectory *dir{nullptr};

  string output_path = string(gSystem->DirName(filename.c_str())) + "/" + "v1.root";
  TFile fo(output_path.c_str(), "recreate");

  string name;
  vector<std::string> match_result;
  shared_ptr<TGraphAsymmErrors> graphPtr;

  dir = nullptr;
  ff.GetObject("v1", dir);

  if (dir) {
    auto outDir = fo.mkdir("v1_systematics");

    for (const string &particle : {"proton", "pion_pos", "pion_neg"}) {
      string pattern{format("v1_{}_(pT|y)_(psd[1-3])_({})_(X|Y)_1", particle, resolutionMethod)};

      TMultiGraph v1vsPtSystematicsMG;
      TMultiGraph v1vsYSystematicsMG;
      for (const auto &match : GetResourcesMatchingName<TGraphAsymmErrors>(dir, pattern)) {
        tie(name, match_result, graphPtr) = match;

        const map<string, int> color{
            {"psd1", kRed},
            {"psd2", kGreen + 2},
            {"psd3", kBlue}
        };

        const map<pair<string, string>, int> marker{
            {{"psd1", "X"}, kFullDiamond},
            {{"psd1", "Y"}, kOpenDiamond},
            {{"psd2", "X"}, kFullCircle},
            {{"psd2", "Y"}, kOpenCircle},
            {{"psd3", "X"}, kFullSquare},
            {{"psd3", "Y"}, kOpenSquare},
        };

        string axis{match_result[1]};
        string psdReference{match_result[2]};
        string method{match_result[3]};
        string component{match_result[4]};

        graphPtr->SetTitle(Form("R_{%s,%s}", psdReference.c_str(), component.c_str()));
        graphPtr->SetMarkerColor(color.at(psdReference));
        graphPtr->SetLineColor(color.at(psdReference));
        graphPtr->SetMarkerStyle(marker.at(make_pair(psdReference, component)));

        if (axis == "y") {
          v1vsYSystematicsMG.Add(new TGraphAsymmErrors(*graphPtr), "pZ");
        } else if (axis == "pT") {
          v1vsPtSystematicsMG.Add(new TGraphAsymmErrors(*graphPtr), "pZ");
        }
      }
      ShiftGraphsX(v1vsPtSystematicsMG, 0.01);
      v1vsPtSystematicsMG.GetXaxis()->SetTitle("p_{T} (GeV/#it{c})");
//      v1vsPtSystematicsMG.GetXaxis()->SetRangeUser(0.0, 1.5);

      ShiftGraphsX(v1vsYSystematicsMG, 0.01);
      v1vsYSystematicsMG.GetXaxis()->SetTitle("#it{y}_{CM}");
      v1vsYSystematicsMG.GetXaxis()->SetRangeUser(-0.4, 1.25);

      outDir->WriteObject(&v1vsPtSystematicsMG, Form("v1vsPtSystematicsMG_%s", particle.c_str()));
      outDir->WriteObject(&v1vsYSystematicsMG, Form("v1vsYSystematicsMG_%s", particle.c_str()));

      if (!canvasDir.empty()) {
        auto labelLeft = MakePaveText({
                                          "directed flow #it{v}_{1} (p_{T})",
                                          map<string, string>({{"proton", "protons"}, {"pion_neg", "negative pions"},
                                                               {"pion_pos", "positive pions"}}).at(particle),
                                          "centrality 10-25% (E_{PSD})",
                                          format("{} resolution", resolutionMethod),
                                          format("{} < #it{{y}} < {}", yLo, yHi),
                                      }, {0.4, 0.15, 0.85, 0.40});
        auto labelRight = MakePaveText({
                                           "directed flow #it{v}_{1} (#it{y})",
                                           map<string, string>({{"proton", "protons"}, {"pion_neg", "negative pions"},
                                                                {"pion_pos", "positive pions"}}).at(particle),
                                           "centrality 10-25% (E_{PSD})",
                                           format("{} resolution", resolutionMethod),
                                           format("{} < p_{{T}} < {} GeV/#it{{c}}", ptLo, ptHi),
                                       }, {0.4, 0.15, 0.85, 0.40});

        auto c = MakeDoubleSidedCanvas(v1vsPtSystematicsMG, v1vsYSystematicsMG);
        auto padLeft = c->cd(1);
        labelLeft->Draw();
        auto padRight = c->cd(2);
        labelRight->Draw();
        c->SaveAs(fmt::format("{}/v1Systematics_{}.C", canvasDir, particle).c_str());

      }


      // combined component
      pattern = format("v1_{}_(pT|y)_psd(\\d)_{}_CC_1", particle, resolutionMethod);
      TMultiGraph v1vsPtCombiComponentMG;
      TMultiGraph v1vsYCombiComponentMG;
      for (const auto &match : GetResourcesMatchingName<TGraphAsymmErrors>(dir, pattern)) {
        tie(name, match_result, graphPtr) = match;

        const map<string, int> color{
            {"1", kRed},
            {"2", kGreen + 2},
            {"3", kBlue},
        };

        const map<string, int> marker{
            {"1", kFullDiamond},
            {"2", kFullCircle},
            {"3", kFullSquare}
        };

        string axis{match_result[1]};
        string psdReference{match_result[2]};

        graphPtr->SetTitle(Form("R_{1} [PSD%s]", psdReference.c_str()));
        graphPtr->SetLineColor(color.at(psdReference));
        graphPtr->SetMarkerColor(color.at(psdReference));
        graphPtr->SetMarkerStyle(marker.at(psdReference));

        if (marker.at(psdReference) == kFullDiamond) {
          graphPtr->SetMarkerSize(1.5);
        }

        if (axis == "y") {
          v1vsYCombiComponentMG.Add(new TGraphAsymmErrors(*graphPtr), "pZ");
        } else if (axis == "pT") {
          v1vsPtCombiComponentMG.Add(new TGraphAsymmErrors(*graphPtr), "pZ");
        }
      }

      ShiftGraphsX(v1vsPtCombiComponentMG, 0.01);
      v1vsPtCombiComponentMG.GetXaxis()->SetTitle("p_{T} (GeV/#it{c})");
//      v1vsPtCombiComponentMG.GetXaxis()->SetRangeUser(0.0, 1.5);

      ShiftGraphsX(v1vsYCombiComponentMG, 0.01);
      v1vsYCombiComponentMG.GetXaxis()->SetTitle("#it{y}_{CM}");
      v1vsYCombiComponentMG.GetXaxis()->SetRangeUser(-0.4, 1.25);

      outDir->WriteObject(&v1vsPtCombiComponentMG, Form("v1vsPtCombiComponentMG_%s", particle.c_str()));
      outDir->WriteObject(&v1vsYCombiComponentMG, Form("v1vsYCombiComponentMG_%s", particle.c_str()));
      if (!canvasDir.empty()) {
        auto labelLeft = MakePaveText({
                                          "directed flow #it{v}_{1} (p_{T})",
                                          map<string, string>({{"proton", "protons"}, {"pion_neg", "negative pions"},
                                                               {"pion_pos", "positive pions"}}).at(particle),
                                          "combined components",
                                          "centrality 10-25% (E_{PSD})",
                                          format("{} resolution", resolutionMethod),
                                          format("{} < #it{{y}} < {}", yLo, yHi),
                                      }, {0.4, 0.15, 0.85, 0.40});
        auto labelRight = MakePaveText({
                                           "directed flow #it{v}_{1} (#it{y})",
                                           map<string, string>({{"proton", "protons"}, {"pion_neg", "negative pions"},
                                                                {"pion_pos", "positive pions"}}).at(particle),
                                           "combined components",
                                           "centrality 10-25% (E_{PSD})",
                                           format("{} resolution", resolutionMethod),
                                           format("{} < p_{{T}} < {} GeV/#it{{c}}", ptLo, ptHi),
                                       }, {0.4, 0.15, 0.85, 0.40});

        auto c = MakeDoubleSidedCanvas(v1vsPtCombiComponentMG, v1vsYCombiComponentMG);
        auto padLeft = c->cd(1);
        labelLeft->Draw();
        auto padRight = c->cd(2);
        labelRight->Draw();
        c->SaveAs(fmt::format("{}/v1CombiComponent_{}.C", canvasDir, particle).c_str());
      }

      // combined reference
      pattern = string(Form("v1_%s_(pT|y)_4S_(X|Y)_CR_1", particle.c_str()));
      TMultiGraph v1vsPtCombiReferenceMG;
      TMultiGraph v1vsYCombiReferenceMG;
      for (const auto &match : GetResourcesMatchingName<TGraphAsymmErrors>(dir, pattern)) {
        tie(name, match_result, graphPtr) = match;

        const map<string, int> color{
            {"X", kRed},
            {"Y", kBlack},
        };

        const map<string, int> marker{
            {"X", kFullCircle},
            {"Y", kOpenCircle},
        };

        string axis{match_result[1]};
        string component{match_result[2]};

        graphPtr->SetTitle(Form("R_{1}^{%s}", component.c_str()));
        graphPtr->SetLineColor(color.at(component));
        graphPtr->SetMarkerColor(color.at(component));
        graphPtr->SetMarkerStyle(marker.at(component));

        if (axis == "y") {
          v1vsYCombiReferenceMG.Add(new TGraphAsymmErrors(*graphPtr), "pZ");
        } else if (axis == "pT") {
          v1vsPtCombiReferenceMG.Add(new TGraphAsymmErrors(*graphPtr), "pZ");
        }
      }

      ShiftGraphsX(v1vsPtCombiReferenceMG, 0.01);
      v1vsPtCombiReferenceMG.GetXaxis()->SetTitle("p_{T} (GeV/#it{c})");
//      v1vsPtCombiReferenceMG.GetXaxis()->SetRangeUser(0.0, 1.5);

      ShiftGraphsX(v1vsYCombiReferenceMG, 0.01);
      v1vsYCombiReferenceMG.GetXaxis()->SetTitle("#it{y}_{CM}");
      v1vsYCombiReferenceMG.GetXaxis()->SetRangeUser(-0.4, 1.25);

      outDir->WriteObject(&v1vsPtCombiReferenceMG, Form("v1vsPtCombiReferenceMG_%s", particle.c_str()));
      outDir->WriteObject(&v1vsYCombiReferenceMG, Form("v1vsYCombiReferenceMG_%s", particle.c_str()));
      if (!canvasDir.empty()) {

        auto labelLeft = MakePaveText({
                                          "directed flow #it{v}_{1} (p_{T})",
                                          map<string, string>({{"proton", "protons"}, {"pion_neg", "negative pions"},
                                                               {"pion_pos", "positive pions"}}).at(particle),
                                          "combined PSD",
                                          "centrality 10-25% (E_{PSD})",
                                          format("{} resolution", resolutionMethod),
                                          format("{} < #it{{y}} < {}", yLo, yHi),
                                      }, {0.4, 0.15, 0.85, 0.40});
        auto labelRight = MakePaveText({
                                           "directed flow #it{v}_{1} (#it{y})",
                                           map<string, string>({{"proton", "protons"}, {"pion_neg", "negative pions"},
                                                                {"pion_pos", "positive pions"}}).at(particle),
                                           "combined PSD",
                                           "centrality 10-25% (E_{PSD})",
                                           format("{} resolution", resolutionMethod),
                                           format("{} < p_{{T}} < {} GeV/#it{{c}}", ptLo, ptHi),
                                       }, {0.4, 0.15, 0.85, 0.40});

        auto c = MakeDoubleSidedCanvas(v1vsPtCombiReferenceMG, v1vsYCombiReferenceMG);
        auto padLeft = c->cd(1);
        labelLeft->Draw();
        auto padRight = c->cd(2);
        labelRight->Draw();
        c->SaveAs(fmt::format("{}/v1CombiReference_{}.C", canvasDir, particle).c_str());
      }

      if (!canvasDir.empty()) {

        auto labelCommon = MakePaveText({
                                            "directed flow #it{v}_{1} (p_{T})",
                                            map<string, string>({{"proton", "protons"}, {"pion_neg", "negative pions"},
                                                                 {"pion_pos", "positive pions"}}).at(particle),
                                            "centrality 10-25% (E_{PSD})",
                                            format("{} resolution", resolutionMethod),
                                            format("{} < #it{{y}} < {}", yLo, yHi),
                                        }, {0.4, 0.15, 0.85, 0.40});

        auto c = MakeDoubleSidedCanvas(v1vsPtCombiComponentMG, v1vsPtCombiReferenceMG);
        auto padLeft = c->cd(1);
        labelCommon->Draw();
        auto padRight = c->cd(2);
        c->SaveAs(fmt::format("{}/v1VsPtCCandCR_{}.C", canvasDir, particle).c_str());
      }
    }

  }



  /**
   * v1 as a function of centrality
   */
  dir = nullptr;
  ff.GetObject("v1_combined", dir);

  if (dir) {
    auto outDir = fo.mkdir("v1vsCentrality");

    map<string, int> color{
        {"0", kRed},
        {"1", kOrange + 1},
        {"2", kGreen + 2},
        {"3", kBlue},
        {"4", kMagenta + 2}

    };

    for (const string &particle : {"proton", "pion_pos", "pion_neg"}) {

      string pattern{Form("v1_%s_(pT|y)_4S_CA_(\\d+)", particle.c_str())};

      TMultiGraph v1vsPtCentralityMG;
      TMultiGraph v1vsYCentralityMG;

      for (const auto &match : GetResourcesMatchingName<TGraphAsymmErrors>(dir, pattern)) {
        tie(name, match_result, graphPtr) = match;

        string axis{match_result[1]};
        string centralityClass{match_result[2]};

        graphPtr->SetLineColor(color.at(centralityClass));
        graphPtr->SetMarkerColor(color.at(centralityClass));
        graphPtr->SetLineWidth(2);

        if (axis == "pT") {
          v1vsPtCentralityMG.Add(new TGraphAsymmErrors(*graphPtr), "pZ");
        } else if (axis == "y") {
          v1vsYCentralityMG.Add(new TGraphAsymmErrors(*graphPtr), "pZ");
        }

      }

      ShiftGraphsX(v1vsPtCentralityMG, 0.02);
      v1vsPtCentralityMG.GetXaxis()->SetTitle("p_{T} (GeV/#it{c})");
//      v1vsPtCentralityMG.GetXaxis()->SetRangeUser(0.0, 1.5);

      ShiftGraphsX(v1vsYCentralityMG, 0.01);
      v1vsYCentralityMG.GetXaxis()->SetTitle("#it{y}_{CM}");
      v1vsYCentralityMG.GetXaxis()->SetRangeUser(-0.4, 1.25);

      outDir->WriteObject(&v1vsPtCentralityMG, Form("v1vsPtCentralityMG_%s", particle.c_str()));
      outDir->WriteObject(&v1vsYCentralityMG, Form("v1vsYCentralityMG_%s", particle.c_str()));
      if (!canvasDir.empty()) {
        ExportPtYCanvas(v1vsPtCentralityMG,
                        v1vsYCentralityMG,
                        canvasDir + "/" + Form("v1Centrality_%s.C", particle.c_str()));
      }

    }

  }

  /**
   * v1 vs particle(10-35%)
   */
  dir = nullptr;
  ff.GetObject("v1_combined_15_35", dir);

  if (dir) {
    auto outDir = fo.mkdir("v1vsParticle");

    map<string, int> color{
        {"proton", kGreen + 2},
        {"pion_pos", kRed},
        {"pion_neg", kBlue}
    };

    map<string, int> markerStyle{
        {"proton", kFullDiamond},
        {"pion_pos", kFullCircle},
        {"pion_neg", kFullSquare},
    };

    map<string, const char *> label{
        {"proton", "p"},
        {"pion_pos", "#pi^{+}"},
        {"pion_neg", "#pi^{-}"},
    };

    string pattern{"v1_(proton|pion_neg)_(pT|y)_4S_CA_0"};

    TMultiGraph v1vsPtParticleMG;
    TMultiGraph v1vsYParticleMG;

    for (const auto &match : GetResourcesMatchingName<TGraphAsymmErrors>(dir, pattern)) {
      tie(name, match_result, graphPtr) = match;

      string particle = match_result[1];
      string axis = match_result[2];

      graphPtr->SetTitle(label.at(particle));

      graphPtr->SetMarkerColor(color.at(particle));
      graphPtr->SetLineColor(color.at(particle));
      graphPtr->SetMarkerStyle(markerStyle.at(particle));
      graphPtr->SetLineWidth(2);

      if (markerStyle.at(particle) == kFullDiamond) {
        graphPtr->SetMarkerSize(1.5);
      }

      if (axis == "y") {
        v1vsYParticleMG.Add(new TGraphAsymmErrors(*graphPtr), "pZ");
      } else if (axis == "pT") {
        v1vsPtParticleMG.Add(new TGraphAsymmErrors(*graphPtr), "pZ");
      }
    }

//    ShiftGraphsX(v1vsPtParticleMG, 0.02);
    v1vsPtParticleMG.GetXaxis()->SetTitle("p_{T} (GeV/#it{c})");
//    v1vsPtParticleMG.GetXaxis()->SetRangeUser(0.0, 1.5);

//    ShiftGraphsX(v1vsYParticleMG, 0.01);
    v1vsYParticleMG.GetXaxis()->SetTitle("#it{y}_{CM}");
    v1vsYParticleMG.GetXaxis()->SetRangeUser(-0.4, 1.25);

    outDir->WriteObject(&v1vsPtParticleMG, "v1vsPtParticleMG");
    outDir->WriteObject(&v1vsYParticleMG, "v1vsYParticleMG");
    if (!canvasDir.empty()) {

      auto labelLeft = MakePaveText({
                                        "directed flow #it{v}_{1} (p_{T})",
                                        "centrality 15-55% (E_{PSD})",
                                        format("{} resolution", resolutionMethod),
                                        format("{} < #it{{y}} < {}", yLo, yHi),
                                    }, {0.4, 0.15, 0.85, 0.40});

      auto labelRight = MakePaveText({
                                         "directed flow #it{v}_{1} (#it{y})",
                                         "centrality 15-35% (E_{PSD})",
                                         format("{} resolution", resolutionMethod),
                                         format("{} < p_{{T}} < {} GeV/#it{{c}}", ptLo, ptHi),
                                     }, {0.4, 0.15, 0.85, 0.40});

      auto c = MakeDoubleSidedCanvas(v1vsPtParticleMG, v1vsYParticleMG);
      auto padLeft = c->cd(1);
      labelLeft->Draw();
      auto padRight = c->cd(2);
      labelRight->Draw();
      c->SaveAs(fmt::format("{}/v1Particle.C", canvasDir).c_str());
    }

  }

  return 0;
}

#ifndef __CLING__
int main(int argc, char **argv) {

  if (argc > 1) {
    return drawV1(std::string(argv[1]));
  }

  return 1;
}
#endif
