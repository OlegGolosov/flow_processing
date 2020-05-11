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

using namespace std;
using namespace fmt;

vector<tuple <vector<string>, vector<string>, string, string>> QQnames=
{
  {{"psd1_psd2_(X|Y)(X|Y)"         }, {"#LTQ^{PSD1}_{1,%s}Q^{PSD2}_{1,%s}#GT"       }, "Centrality (%)", ""},
  {{"psd2_psd3_(X|Y)(X|Y)"         }, {"#LTQ^{PSD2}_{1,%s}Q^{PSD3}_{1,%s}#GT"       }, "Centrality (%)", ""},
  {{"psd3_psd1_(X|Y)(X|Y)"         }, {"#LTQ^{PSD3}_{1,%s}Q^{PSD1}_{1,%s}#GT"       }, "Centrality (%)", ""},
  {{"proton_y_psd1_(X|Y)(X|Y)_4S"  }, {"#LTQ^{STS,p}_{1,%s}Q^{PSD1}_{1,%s}#GT"      }, "Centrality (%)", ""},
  {{"pion_pos_y_psd1_(X|Y)(X|Y)_4S"}, {"#LTQ^{STS,#pi^{+}}_{1,%s}Q^{PSD1}_{1,%s}#GT"}, "Centrality (%)", ""},
  {{"pion_neg_y_psd1_(X|Y)(X|Y)_4S"}, {"#LTQ^{STS,#pi^{-}}_{1,%s}Q^{PSD1}_{1,%s}#GT"}, "Centrality (%)", ""},
  {{"proton_y_psd2_(X|Y)(X|Y)_4S"  }, {"#LTQ^{STS,p}_{1,%s}Q^{PSD2}_{1,%s}#GT"      }, "Centrality (%)", ""},
  {{"pion_pos_y_psd2_(X|Y)(X|Y)_4S"}, {"#LTQ^{STS,#pi^{+}}_{1,%s}Q^{PSD2}_{1,%s}#GT"}, "Centrality (%)", ""},
  {{"pion_neg_y_psd2_(X|Y)(X|Y)_4S"}, {"#LTQ^{STS,#pi^{-}}_{1,%s}Q^{PSD2}_{1,%s}#GT"}, "Centrality (%)", ""},
  {{"proton_y_psd3_(X|Y)(X|Y)_4S"  }, {"#LTQ^{STS,p}_{1,%s}Q^{PSD3}_{1,%s}#GT"      }, "Centrality (%)", ""},
  {{"pion_pos_y_psd3_(X|Y)(X|Y)_4S"}, {"#LTQ^{STS,#pi^{+}}_{1,%s}Q^{PSD3}_{1,%s}#GT"}, "Centrality (%)", ""},
  {{"pion_neg_y_psd3_(X|Y)(X|Y)_4S"}, {"#LTQ^{STS,#pi^{-}}_{1,%s}Q^{PSD3}_{1,%s}#GT"}, "Centrality (%)", ""},
};

vector<tuple <vector<string>, vector<string>, string, string>> Q2QQnames=
{
  {{"proton_y_psd1_psd2_(X|Y)2(X|Y)(X|Y)_MH"  }, {"#LTQ^{STS,p}_{2,%s}Q^{PSD1}_{1,%s}Q^{PSD2}_{1,%s}#GT"       }, "Centrality (%)", ""},
  {{"proton_y_psd2_psd3_(X|Y)2(X|Y)(X|Y)_MH"  }, {"#LTQ^{STS,p}_{2,%s}Q^{PSD1}_{1,%s}Q^{PSD3}_{1,%s}#GT"       }, "Centrality (%)", ""},
  {{"pion_pos_y_psd1_psd2_(X|Y)2(X|Y)(X|Y)_MH"}, {"#LTQ^{STS,#pi^{+}}_{2,%s}Q^{PSD1}_{1,%s}Q^{PSD2}_{1,%s}#GT" }, "Centrality (%)", ""},
  {{"pion_pos_y_psd2_psd3_(X|Y)2(X|Y)(X|Y)_MH"}, {"#LTQ^{STS,#pi^{+}}_{2,%s}Q^{PSD1}_{1,%s}Q^{PSD3}_{1,%s}#GT" }, "Centrality (%)", ""},
  {{"pion_neg_y_psd1_psd2_(X|Y)2(X|Y)(X|Y)_MH"}, {"#LTQ^{STS,#pi^{-}}_{2,%s}Q^{PSD1}_{1,%s}Q^{PSD2}_{1,%s}#GT" }, "Centrality (%)", ""},
  {{"pion_neg_y_psd2_psd3_(X|Y)2(X|Y)(X|Y)_MH"}, {"#LTQ^{STS,#pi^{-}}_{2,%s}Q^{PSD1}_{1,%s}Q^{PSD3}_{1,%s}#GT" }, "Centrality (%)", ""},
};

vector<tuple <vector<string>, vector<string>, string, string>> resNames=
{
  //{{"RES_psd1_3S_(X|Y)"}, {"R_{1,%s}^{PSD1,3S}(PSD2, PSD3)"}, "Centrality (%)", "R_{1}"},
  //{{"RES_psd2_3S_(X|Y)"}, {"R_{1,%s}^{PSD2,3S}(PSD1, PSD3)"}, "Centrality (%)", "R_{1}"},
  //{{"RES_psd3_3S_(X|Y)"}, {"R_{1,%s}^{PSD3,3S}(PSD1, PSD2)"}, "Centrality (%)", "R_{1}"},
  //RES_psd1_proton_3S_X
  {{"RES_psd(.)_proton_3S_(X|Y)", "RES_psd(.)_MC_(X|Y)"}, {"R^{PSD%s,3S}_{1,%s}", "R^{PSD%s,MC}_{1,%s}"}, "Centrality (%)", "R_{1}"},
  {{"RES_psd(.)_proton_4S_(X|Y)", "RES_psd(.)_MC_(X|Y)"}, {"R^{PSD%s,4S}_{1,%s}(STS^{p})", "R^{PSD%s,MC}_{1,%s}"}, "Centrality (%)", "R_{1}"},
  {{"RES_psd(.)_pion_pos_4S_(X|Y)", "RES_psd(.)_MC_(X|Y)"}, {"R^{PSD%s,4S}_{1,%s}(STS^{#pi^{+}})", "R^{PSD%s,MC}_{1,%s}"}, "Centrality (%)", "R_{1}"},
  {{"RES_psd(.)_pion_neg_4S_(X|Y)", "RES_psd(.)_MC_(X|Y)"}, {"R^{PSD%s,4S}_{1,%s}(STS^{#pi^{-}})", "R^{PSD%s,MC}_{1,%s}"}, "Centrality (%)", "R_{1}"},
  {{"RES_psd(1|3)_proton_MH_(X|Y)", "RES_psd(1|3)_MC_(X|Y)"}, {"R^{PSD%s,MH}_{1,%s}(STS^{p})", "R^{PSD%s,MC}_{1,%s}"}, "Centrality (%)", "R_{1}"},
  {{"RES_psd(1|3)_pion_pos_MH_(X|Y)", "RES_psd(1|3)_MC_(X|Y)"}, {"R^{PSD%s,MH}_{1,%s}(STS^{#pi^{+}})", "R^{PSD%s,MC}_{1,%s}"}, "Centrality (%)", "R_{1}"},
  {{"RES_psd(1|3)_pion_neg_MH_(X|Y)", "RES_psd(1|3)_MC_(X|Y)"}, {"R^{PSD%s,MH}_{1,%s}(STS^{#pi^{-}})", "R^{PSD%s,MC}_{1,%s}"}, "Centrality (%)", "R_{1}"},
};

vector<tuple <vector<string>, vector<string>, string, string>> uQnames=
{
  {{"proton_y_psd1_psd2_(X|Y)2(X|Y)(X|Y)_0"}, {"#LTu^{p}_{2,%s}Q^{PSD1}_{1,%s}Q^{PSD2}_{1,%s}#GT"}, "#it{y}", ""},
};

vector<tuple <vector<string>, vector<string>, string, string>> u2QQnames=
{
  {{"proton_y_psd1_psd2_(X|Y)2(X|Y)(X|Y)_0"}, {"#LTu^{p}_{2,%s}Q^{PSD1}_{1,%s}Q^{PSD2}_{1,%s}#GT"}, "#it{y}", ""},
};


vector<tuple <vector<string>, vector<string>, string, string>> v1names=
{
  //{{"v1_proton_y_psd(1|2|3)_3S_(X|Y)_0", "","v1_mc_proton_y_psi_(X|Y)_0"}, {"v^{p,PSD%s}_{1,%s}", "","v^{p,MC}_{1,%s}"}, "#it{y}", "v_{1}"},
  //{{"v1_mcPid_proton_y_psd(1|2|3)_3S_(X|Y)_0", "","v1_mc_proton_y_psi_(X|Y)_0"}, {"v^{p,PSD%s}_{1,%s}(MC-pid)", "","v^{p,MC}_{1,%s}"}, "#it{y}", "v_{1}"},
//
  //{{"v1_proton_y_psd(1|2|3)_proton_4S_(X|Y)_0", "","v1_proton_y_psi_(X|Y)_0"}, {"v^{p,PSD%s}_{1,%s}", "","v^{p,MC}_{1,%s}"}, "#it{y}", "v_{1}"},
  //{{"v1_mcPid_proton_y_psd(1|2|3)_proton_4S_(X|Y)_0", "","v1_proton_y_psi_(X|Y)_0"}, {"v^{p,PSD%s}_{1,%s}(MC-pid)", "","v^{p,MC}_{1,%s}"}, "#it{y}", "v_{1}"},
  //{{"v1_proton_y_psd(1|2|3)_pion_pos_4S_(X|Y)_0", "","v1_proton_y_psi_(X|Y)_0"}, {"v^{p,PSD%s}_{1,%s}", "","v^{p,MC}_{1,%s}"}, "#it{y}", "v_{1}"},
  //{{"v1_mcPid_proton_y_psd(1|2|3)_pion_neg_4S_(X|Y)_0", "","v1_proton_y_psi_(X|Y)_0"}, {"v^{p,PSD%s}_{1,%s}(MC-pid)", "","v^{p,MC}_{1,%s}"}, "#it{y}", "v_{1}"},
//
  //{{"v1_proton_y_psd(1|2|3)_proton_MH_(X|Y)_0", "","v1_proton_y_psi_(X|Y)_0"}, {"v^{p,PSD%s}_{1,%s}", "","v^{p,MC}_{1,%s}"}, "#it{y}", "v_{1}"},
  //{{"v1_mcPid_proton_y_psd(1|2|3)_proton_MH_(X|Y)_0", "","v1_proton_y_psi_(X|Y)_0"}, {"v^{p,PSD%s}_{1,%s}(MC-pid)", "","v^{p,MC}_{1,%s}"}, "#it{y}", "v_{1}"},
  //{{"v1_proton_y_psd(1|2|3)_pion_pos_MH_(X|Y)_0", "","v1_proton_y_psi_(X|Y)_0"}, {"v^{p,PSD%s}_{1,%s}", "","v^{p,MC}_{1,%s}"}, "#it{y}", "v_{1}"},
  //{{"v1_mcPid_proton_y_psd(1|2|3)_pion_neg_MH_(X|Y)_0", "","v1_proton_y_psi_(X|Y)_0"}, {"v^{p,PSD%s}_{1,%s}(MC-pid)", "","v^{p,MC}_{1,%s}"}, "#it{y}", "v_{1}"},
};

const vector <int> colors = 
{
  kBlue, kRed, kGreen+2, kMagenta+2, kBlack, kOrange+2, kPink+2,
  kTeal+2, kCyan+2, kCyan+4, kAzure, kGray+3, kOrange+7, kGreen+4
};

const vector <vector <int>> markers = 
{ 
  {20, 21, 22, 23, 29, 33, 34, 39, 41, 43, 45, 47, 48, 49}, // filled
  {24, 25, 26, 32, 30, 27, 28, 37, 40, 42, 44, 46, 35, 36},  // empty
  { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1}  // none
};

const vector <vector <int>> lineWidths = 
{
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
};

float markerSize = 1.5;
float graphShift = .25;
float xAxisTitleSize = 0.05;
float yAxisTitleSize = 0.05;

TFile *fIn, *fOut;

void drawCorr(vector<tuple <vector<string>, vector<string>, string, string>> corrTuples, const string inDirName, const string outDirName) 
{
  
  TDirectory *inDir{nullptr};
  if (!(inDir = dynamic_cast<TDirectory *>(fIn->Get(inDirName.c_str())))) {
    printf ("No directory \"%s\"", inDirName);
    return;
  }
  fOut->mkdir(outDirName.c_str())->cd();
  vector<string> match_result;
  shared_ptr<TGraphAsymmErrors> g;
  for (auto &corrTuple:corrTuples)
  {
    TMultiGraph mg;
    mg.GetXaxis()->SetTitleSize(xAxisTitleSize);
    mg.GetYaxis()->SetTitleSize(yAxisTitleSize);
    auto &corrNames=get<0>(corrTuple);
    auto &corrTitles=get<1>(corrTuple);
    auto &xAxisTitle=get<2>(corrTuple);
    auto &yAxisTitle=get<3>(corrTuple);
    mg.SetName(corrNames.at(0).c_str());
    mg.SetTitle(corrNames.at(0).c_str());
    for (int j = 0; j < corrNames.size(); j++)
    {
      auto &corrName = corrNames.at(j);
      auto &corrTitle = corrTitles.at(j);
      auto matches = GetResourcesMatchingName<TGraphAsymmErrors>(inDir, corrName);
      string name = corrName;
      cout << name << "\t";
      TString title = Form(corrTitle.c_str(), "", "", "", "", "");
      mg.GetXaxis()->SetTitle(xAxisTitle.c_str());
      mg.GetYaxis()->SetTitle(yAxisTitle.c_str());
      for (uint k = 0; k < matches.size(); k++) {
        auto &match = matches.at(k);
        tie(name, match_result, g) = match;
        
        //cout << match_result.at(0) << "\t";
        vector <string> subs(5);
        copy (match_result.begin()+1, match_result.end(), subs.begin());

        title = Form(corrTitle.c_str(), subs.at(0).c_str(),
                                           subs.at(1).c_str(),
                                           subs.at(2).c_str(),
                                           subs.at(3).c_str(),
                                           subs.at(4).c_str());
        //cout << title << endl;
        g->SetName(name.c_str());
        TString oldTitle = g->GetTitle();
        if (!strcmp(oldTitle, "Graph"))
          oldTitle="";
        else 
          oldTitle="("+oldTitle+")";
        g->SetTitle(title+oldTitle);
        g->SetLineColor(colors.at(k));
        g->SetLineWidth(lineWidths.at(j).at(k));
        g->SetMarkerColor(colors.at(k));
        g->SetMarkerSize(markerSize);
        g->SetMarkerStyle(markers.at(j).at(k));
        mg.Add(new TGraphAsymmErrors(*g), "pl");
      }
    }
    if(mg.GetListOfGraphs())
    {
      ShiftGraphsX(mg, graphShift);
      //mg.Draw();
      mg.Write();
      cout << endl;
    }
  }
  return 0;
}

void drawV1()
{
  vector<tuple <vector<string>, vector<string>, string, string>> v1_names;
  vector<string> parts = {"proton","pion_pos","pion_neg","kaon_pos","kaon_neg"};
  vector<string> partNames = {"p","#pi^{+}","#pi^{-}","K^{+}","K^{-}"};
  vector<string> refs = {"proton","pion_pos","pion_neg"};
  vector<string> pids = {"","mcPid_"};
  vector<string> pidNames = {"","_{MCpid}"};
  vector<string> axes = {"y","pT"};
  vector<string> axesNames = {"#it{y}","p_{T} (GeV/#it{c})"};

  for (uint ax=0;ax<axes.size();ax++)
  {
    string axis=axes.at(ax);
    string axisName=axesNames.at(ax);
    for (uint p=0;p<parts.size();p++)
    {
      string part=parts.at(p);
      string partName=partNames.at(p);
      for (int cent=0;cent<5;cent++)
      {
        for (uint pi=0;pi<pids.size();pi++)
        {
          string pid=pids.at(pi);
          string pidName=pidNames.at(pi);
          v1_names.push_back({{"v1_"+pid+part+"_"+axis+"_psd(1|2|3)_3S_(X|Y)_"+Form("%i",cent), 
                              "","v1_mc_"+part+"_"+axis+"_psi_(X|Y)_"+Form("%i",cent)}, 
                             {"v^{"+partName+pidName+",PSD%s}_{1,%s}", "","v^{"+partName+",MC}_{1,%s}"}, axisName, "v_{1}"});
        }
        for (auto &res:{"4S","MH"})
        {
          for (uint r=0;r<refs.size();r++)
          {
            string ref=refs.at(r);
            string refName=partNames.at(r);
            for (uint pi=0;pi<pids.size();pi++)
            {
              string pid=pids.at(pi);
              string pidName=pidNames.at(pi);
              v1_names.push_back({{"v1_"+pid+part+"_"+axis+"_psd(1|2|3)_"+ref+"_"+res+"_(X|Y)_"+Form("%i",cent), 
                                  "","v1_mc_"+part+"_"+axis+"_psi_(X|Y)_"+Form("%i",cent)}, 
                                 {"v^{"+partName+pidName+",PSD%s}_{1,%s}", "","v^{"+partName+",MC}_{1,%s}"}, axisName, "v_{1}"});
            }
          }
        }
      }
      drawCorr(v1_names, "v1", Form("v1_%s_%s", part.c_str(), axis.c_str()));
      v1_names.clear();
    }
  }
}

void drawV2()
{
  vector<tuple <vector<string>, vector<string>, string, string>> v2_names;
  vector<string> parts = {"proton","pion_pos","pion_neg","kaon_pos","kaon_neg"};
  vector<string> partNames = {"p","#pi^{+}","#pi^{-}","K^{+}","K^{-}"};
  vector<string> refs = {"psd1_psd2","psd2_psd3","psd3_psd1"};
  vector<string> pids = {"","mcPid_"};
  vector<string> pidNames = {"","_{MCpid}"};
  vector<string> axes = {"y","pT"};
  vector<string> axesNames = {"#it{y}","p_{T} (GeV/#it{c})"};

  for (uint ax=0;ax<axes.size();ax++)
  {
    string axis=axes.at(ax);
    string axisName=axesNames.at(ax);
    for (uint p=0;p<parts.size();p++)
    {
      string part=parts.at(p);
      string partName=partNames.at(p);
      for (int cent=0;cent<5;cent++)
      {
        for (auto ref:refs)
        {
          for (uint pi=0;pi<pids.size();pi++)
          {
            string pid=pids.at(pi);
            string pidName=pidNames.at(pi);
            v2_names.push_back({{"v2_"+pid+part+"_"+axis+"_"+ref+"_(X|Y)2(X|Y)(X|Y)_"+Form("%i",cent), 
                                "","v2_"+part+"_"+axis+"_psi_(X|Y)_"+Form("%i",cent)}, 
                               {"v^{"+partName+pidName+"}_{2,%s2%s%s}", "","v^{"+partName+",MC}_{2,%s}"}, axisName, "v_{2}"});
          }
        }
      }
      drawCorr(v2_names, "v2", Form("v2_%s_%s", part.c_str(), axis.c_str()));
      v2_names.clear();
    }
  }
}

void draw(const string &filename = "graphs.root")
{
  fIn = new TFile(filename.c_str(), "read");

  string output_path = string(gSystem->DirName(filename.c_str())) + "/" + "qq.root";
  fOut = new TFile(output_path.c_str(), "recreate");
  drawCorr(QQnames, "raw", "QQ");
  drawCorr(Q2QQnames, "raw", "Q2QQ");
  drawCorr(resNames, "resolution", "res");
  graphShift = 0.;
  //drawCorr(u2QQnames, "uQ", "u2QQ");
  //drawCorr(v1names, "v1", "v1");
  drawV1();
  //drawV2();
}


#ifndef __CLING__
int main(int argc, char **argv) {

  if (argc > 1) {
    return draw(std::string(argv[1]));
  }

  return 1;
}
#endif