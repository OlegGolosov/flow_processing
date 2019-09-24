//
// Created by eugene on 4/17/19.
//

#ifdef __CLING__

R__LOAD_LIBRARY(libQnCorrections.so)
R__LOAD_LIBRARY(libBase.so)

#endif

#include <TSystem.h>
#include <TLatex.h>
#include "include/Utils.h"

int drawResolution(
    const std::string &filename = "/home/eugene/analysis/flow/na61/pbpb/13agev/16_011/20190520/profiles.root"
) {

  using namespace std;
  using namespace fmt;

  TFile ff(filename.c_str(), "read");
  TDirectory *dir{nullptr};
  if (!(dir = dynamic_cast<TDirectory *>(ff.Get("resolution")))) {
    return 1;
  }

  string output_path = string(gSystem->DirName(filename.c_str())) + "/" + "resolution.root";
  TFile fo(output_path.c_str(), "recreate");

  auto dir3S = fo.mkdir("3S");
  auto dir4S = fo.mkdir("4S");

  TMultiGraph resolution3SMG("Resolution", "");
  resolution3SMG.GetXaxis()->SetTitle("Centrality (%)");
  resolution3SMG.GetXaxis()->SetTitleSize(0.05);

  TMultiGraph resolution4SMG("Resolution", "");
  resolution4SMG.GetXaxis()->SetTitle("Centrality (%)");
  resolution4SMG.GetXaxis()->SetTitleSize(0.05);

  string name;
  vector<std::string> match_result;
  shared_ptr<TGraphAsymmErrors> graph;
  for (const auto &match : GetResourcesMatchingName<TGraphAsymmErrors>(dir, "RES_psd(\\d)_(3S|4S)_(X|Y)")) {
    tie(name, match_result, graph) = match;

    const std::map<std::string, int> colors{
        {"1", kRed},
        {"2", kGreen + 2},
        {"3", kBlue}
    };

    const map<pair<string, string>, int> markers{
        {{"1", "X"}, kFullCircle},
        {{"1", "Y"}, kOpenCircle},
        {{"2", "X"}, kFullSquare},
        {{"2", "Y"}, kOpenSquare},
        {{"3", "X"}, kFullDiamond},
        {{"3", "Y"}, kOpenDiamond},
    };

    string psd_subevent{match_result[1]};
    string resolution_method = match_result[2];
    string component = match_result[3];

    string title{Form("R_{1,%s} [PSD%s]", component.c_str(), psd_subevent.c_str())};

    graph->SetName(name.c_str());
    graph->SetTitle(title.c_str());
    graph->SetLineColor(colors.at(psd_subevent));
    graph->SetMarkerColor(colors.at(psd_subevent));
    graph->SetMarkerStyle(markers.at({psd_subevent, component}));


    if (markers.at({psd_subevent, component}) == kFullDiamond || markers.at({psd_subevent, component}) == kOpenDiamond) {
      graph->SetMarkerSize(1.5);
    }

    if (resolution_method == "3S") {
      resolution3SMG.Add(new TGraphAsymmErrors(*graph), "pZ");
    } else if (resolution_method == "4S") {
      resolution4SMG.Add(new TGraphAsymmErrors(*graph), "pZ");
    }
  }

  ShiftGraphsX(resolution3SMG, .25);
  ShiftGraphsX(resolution4SMG, .25);

  dir3S->WriteObject(&resolution3SMG, "resolution3SMG");
  dir4S->WriteObject(&resolution4SMG, "resolution4SMG");

  const char *mhCanvasPattern = "cResolutionMH_psd%s_%s";
  const char *mhMultigraphPattern = "mgResolutionMH_psd%s_%s";

  TObjArray mgResolutionMHarray{};
  mgResolutionMHarray.SetOwner(true);

  TObjArray cResolutionMHarray{};
  cResolutionMHarray.SetOwner(true);

  for (const auto &match : GetResourcesMatchingName<TGraphAsymmErrors>(dir,
                                                            "RES_psd(\\d)_(MH)_(X|Y)_(all|proton|pion|pion_neg)")) {
    tie(name, match_result, graph) = match;

    const map<string, int> colors{
        {"proton", kBlue + 2},
        {"pion", kRed + 1},
        {"pion_neg", kMagenta + 1},
        {"all", kBlack},
    };

    const std::map<string, string> particle_names{
        {"pion", "#pi^{+}"},
        {"pion_neg", "#pi^{-}"},
        {"proton", "p"},
        {"all", "ALL"},
    };

    const std::map<string, vector<string> > subevent_ordering{
        {"1", {"1", "2", "3"}},
        {"2", {"2", "1", "3"}},
        {"3", {"3", "2", "1"}},
    };

    string psd_subevent{match_result[1]};
    string resolution_method = match_result[2];
    string component = match_result[3];
    string ref_particle = match_result[4];

    string multigraphName{Form(mhMultigraphPattern, psd_subevent.c_str(), component.c_str())};
    string canvasName{regex_replace(multigraphName, regex("mg"), "c")};

    // new multigraph, new canvas
    TMultiGraph *mg{};
    TCanvas *c{};
    if (nullptr == (mg = dynamic_cast<TMultiGraph *>(mgResolutionMHarray.FindObject(multigraphName.c_str())))) {
      mg = new TMultiGraph(multigraphName.c_str(), "");
      mg->GetXaxis()->SetTitle("Centrality (%)");
      mgResolutionMHarray.Add(mg);

      // reference 3S resolution
      auto ref_match =
          GetResourcesMatchingName<TGraph>(dir, Form("RES_psd%s_3S_%s", psd_subevent.c_str(), component.c_str()));
      if (ref_match.size() == 1) {
        auto ref_graph = get<2>(ref_match.front());
        ref_graph->SetTitle("3S");
        ref_graph->SetLineStyle(kDashed);
        ref_graph->SetMarkerStyle(kFullCircle);
        mg->Add(dynamic_cast<TGraphAsymmErrors *>(ref_graph->Clone(ref_graph->GetName())), "LP");
      }

      c = new TCanvas(canvasName.c_str(), "", 800, 600);
      mg->Draw("AL");
      cResolutionMHarray.Add(c);

      TLatex labels;
      labels.SetNDC(true);

      // Generate Label
      string nom1{Form("#LT Q^{2}_{TPC} Q^{1}_{PSD%s} Q^{1}_{PSD%s} #GT",
                       subevent_ordering.at(psd_subevent)[0].c_str(),
                       subevent_ordering.at(psd_subevent)[1].c_str()
      )};

      string nom2{Form("#LT Q^{1}_{PSD%s} Q^{1}_{PSD%s} #GT",
                       subevent_ordering.at(psd_subevent)[0].c_str(),
                       subevent_ordering.at(psd_subevent)[2].c_str()
      )};

      string denom{Form("#LT Q^{2}_{TPC} Q^{1}_{PSD%s} Q^{1}_{PSD%s} #GT",
                        subevent_ordering.at(psd_subevent)[1].c_str(),
                        subevent_ordering.at(psd_subevent)[2].c_str()
      )};

      labels.DrawLatex(0.5, 0.8,
                       Form("R_{MH} = #sqrt{#frac{%s %s}{%s}}", nom1.c_str(), nom2.c_str(), denom.c_str()));
      labels.DrawLatex(0.5, 0.65, "Q_{TPC}: -0.6 < #it{y} < 0.8");
    }

    c = dynamic_cast<TCanvas *>(cResolutionMHarray.FindObject(canvasName.c_str()));

    // add graph to MG
    string title{Form("MH (%s)", particle_names.at(ref_particle).c_str())};

    graph->SetTitle(title.c_str());
    graph->SetMarkerColor(colors.at(ref_particle));
    graph->SetMarkerStyle(kOpenCircle);
    graph->SetLineColor(colors.at(ref_particle));

    Option_t *opt = ("all" != ref_particle) ? "P" : "LP";
    mg->Add(dynamic_cast<TGraphAsymmErrors *>(graph->Clone(graph->GetName())), opt);

    // update canvas
    c->Update();
    c->Modified();
  }

  auto dirNH = fo.mkdir("mh");

  for (auto mgObj : mgResolutionMHarray) {
    auto mg = dynamic_cast<TMultiGraph *>(mgObj);
    ShiftGraphsX(*mg, 0.5);
    dirNH->WriteObject(mg, mg->GetName());
  }

  for (auto cObj : cResolutionMHarray) {
    dirNH->WriteObject(cObj, cObj->GetName());
  }

  return 0;
}

#ifndef __CLING__
int main(int argc, char **argv) {

  if (argc > 1) {
    return drawResolution(std::string(argv[1]));
  }

  return 1;
}
#endif