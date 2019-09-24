
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <TFile.h>
#include <TMultiGraph.h>
#include <TGraph.h>

namespace {
using namespace std;
}

vector<string> psd_names{"psd1", "psd2", "psd3"};
vector<string> axes{"pT", "y"};
vector<string> component{"X", "Y"};

string getName(string part, string var, string psd, string comp, int ic) {
  // v1_0Pt_PSD1_X_2
  transform(psd.begin(), psd.end(), psd.begin(), ::toupper);

  map<string, string> axesMap = {
      {"pT", "Pt"},
      {"y", "Rapidity"}
  };

  return ("v1_0" + axesMap[var] + "_" + psd + "_" + comp + "_" + ::to_string(ic));

}

void plot_v1(TString filename = "../na61_16_015/no_subsampling/profiles.root",
             string part = "pion_neg",
             string output_file = "v1.root",
             TString var = "pT",
             TString psd = "psd1",
             int centr = -1,
             TString x = "X",
             int res = 0) {
  std::unique_ptr<TFile> in_file{TFile::Open(filename, "read")};

  TFile output(output_file.c_str(), "recreate");

  for (const auto &x : component)
    for (const auto &var : axes)
      for (const auto &psd : psd_names) {
        TString multigraph_name{""};

        multigraph_name = "v1/v1_" + part + "_" + var + "_" + psd + "_" + x + "_" + Form("%d", res);

        std::cout << multigraph_name << std::endl;
        auto multigraph = (TMultiGraph *) in_file->Get(multigraph_name);

        int igr = 0;
        for (auto o : *(multigraph->GetListOfGraphs())) {
          auto gr = dynamic_cast<TGraph *>(o);
          string name = getName(part, var, psd, x, igr);
          gr->SetName(name.c_str());
          gr->Write();
          igr++;
        }

      }

}
