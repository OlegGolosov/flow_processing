#include <utility>

//
// Created by eugene on 4/15/19.
//

#ifndef DATATREEFLOW_UTILS_H
#define DATATREEFLOW_UTILS_H

#include <vector>
#include <memory>
#include <regex>
#include <functional>

#include <Rtypes.h>
#include <DataContainer.h>
#include <TDirectory.h>
#include <TKey.h>
#include <TPaveText.h>

#define FMT_HEADER_ONLY
#include "fmt/core.h"
#include "fmt/format.h"
#include "fmt/printf.h"

using Result = Qn::DataContainerStats;
using ResultPtr = std::shared_ptr<Result>;

TPaveText *MakePaveText(const std::vector<std::string> &lines, std::vector<Double_t> position) {
  auto pave = new TPaveText;
  pave->SetX1NDC(position.at(0));
  pave->SetY1NDC(position.at(1));
  pave->SetX2NDC(position.at(2));
  pave->SetY2NDC(position.at(3));

  for (const auto &line : lines) {
    pave->AddText(line.c_str());
  }

  pave->SetBorderSize(0);
  pave->SetFillColor(kWhite);
  pave->SetFillStyle(0); // transparent
  pave->SetTextAlign(11);
  return pave;
}

Result RebinRapidity(const Result &r, const std::vector<float> &binEdges) {
  std::string axRapidityName;
  // find rapidity axis
  for (const Qn::Axis &ax : r.GetAxes()) {
    if (ax.Name().find("Rapidity") < ax.Name().size()) {
      axRapidityName = ax.Name();
      break;
    }
  }

  if (axRapidityName.empty()) {
    throw std::runtime_error("No rapidity axis found");
  }

  Qn::Axis newRapidityAxis(axRapidityName, binEdges);

  return r.Rebin(newRapidityAxis);
}

Result Rebin(const Result &r, std::string axisName, const std::vector<float> &binEdges) {
  std::string newAxisName;
  // find axis
  for (const Qn::Axis &ax : r.GetAxes()) {
    if (ax.Name().find(axisName) < ax.Name().size()) {
      newAxisName = ax.Name();
      break;
    }
  }

  if (newAxisName.empty()) {
    throw std::runtime_error("Rebin axis not found");
  }
  Qn::Axis newAxis(newAxisName, binEdges);

  return r.Rebin(newAxis);
}

Result Projection(const Result &r, vector<string> axesNames) {
  vector<string> newAxesNames;
  // find axis
  for (const Qn::Axis &ax : r.GetAxes()) 
  {
    for (auto &axisName:axesNames)
    {
      if (ax.Name().find(axisName) < ax.Name().size()) {
        newAxesNames.push_back(ax.Name());
      }
    }
  }

  if (newAxesNames.size() < axesNames.size()) {
    throw std::runtime_error("Not all projection axes found");
  }

  return r.Projection(newAxesNames);
}

Result SetReference(const Result &r) {
  Result rNew{r};

  for (Qn::Stats &stats : rNew) {
    stats.SetStatus(Qn::Stats::Status::REFERENCE);
  }

  return rNew;
}

template<class T>
bool GetResource(TDirectory *d, const std::string &name, std::shared_ptr<T> &container) {
  auto obj = dynamic_cast<T *>(d->Get(name.c_str()));
  if (!obj) {
    container.reset();
    return false;
  }

  container = std::shared_ptr<T>(obj);
  return true;
}

/**
 * 
 * @param mg 
 * @param dX 
 */
void ShiftGraphsX(TMultiGraph &mg, double dX = 0.0) {

  for (int ig = 0; ig < mg.GetListOfGraphs()->GetEntries(); ++ig) {
    double xShift = ig * dX;
    if (xShift == 0.) continue;

    auto graph = dynamic_cast<TGraph *>(mg.GetListOfGraphs()->At(ig));

    int n = graph->GetN();
    double *xx = graph->GetX();

    // populate new xx array
    double xx_new[n];
    for (int ix = 0; ix < n; ++ix) {
      xx_new[ix] = xx[ix] + xShift;
    }

    // replace xx with contents of xx_new
    std::memcpy(xx, xx_new, sizeof(double) * n);
  }

}

/**
 * Returns vector of objects with name matching regex pattern
 * from directory
 * @tparam T - type of object
 * @param d - directory containing objects
 * @param pattern - regex pattern string
 * @return vector of tuples with name, vector of group captures and pointer to an object
 */
template<class T>
std::vector<std::tuple<std::string,
                       std::vector<std::string>,
                       std::shared_ptr<T> > > GetResourcesMatchingName(TDirectory *d, const std::string &pattern) {
  std::regex re(pattern);
  std::vector<std::tuple<std::string, std::vector<std::string>, std::shared_ptr<T> > > result{};

  auto keys = d->GetListOfKeys();
  for (auto objPtr : *keys) {
    auto key = dynamic_cast<TKey *>(objPtr);
    std::string name{key->GetName()};
    std::smatch match;

    if (std::regex_match(name, match, re)) {
      std::vector<std::string> matches{};
      for (unsigned int i = 0; i < match.size(); ++i) {
        matches.push_back(match[i].str());
      }

      auto objTPtr = std::shared_ptr<T>(dynamic_cast<T *> (key->ReadObj()));
      if (objTPtr) {
        result.push_back({std::move(name), std::move(matches), objTPtr});
      } else {
        Warning(__func__, "Object '%s' is not of the required type", name.c_str());
      }
    }
  }

  return result;
}

struct ProfileExporter {

  std::shared_ptr<TFile> outputFile_;
  std::string ouputFileName_;

  ProfileExporter() = default;

  explicit ProfileExporter(const std::string &outputFileName) {
    SaveTo(outputFileName);
  }

  ProfileExporter &SaveTo(const std::string &outputFileName) {
    this->ouputFileName_ = outputFileName;
    return *this;
  }

  std::string saveFolder{""};

  ProfileExporter &Folder(const std::string &_saveFolder = "") {
    saveFolder = _saveFolder;
    return *this;
  }

  std::shared_ptr<Qn::Axis> rebinAxisPtr{};
  ProfileExporter &Rebin(const Qn::Axis &_axis) {
    rebinAxisPtr = std::make_shared<Qn::Axis>(_axis);
    return *this;
  }
  ProfileExporter &Rebin() {
    rebinAxisPtr.reset();
    return *this;
  }

  bool isCorrelatedErrors{false};
  ProfileExporter &CorrelatedErrors(bool _correlatedErrors = true) {
    isCorrelatedErrors = _correlatedErrors;
    return *this;
  }

  bool isUnfold{false};
  ProfileExporter &Unfold(bool _unfold = true) {
    isUnfold = _unfold;
    return *this;
  }

  bool isdv1dy{false};
  float dv1dyFitMin{-1.}, dv1dyFitMax{1.};
  string eventAxisName{"Centrality"};
  ProfileExporter &dv1dy(float _dv1dyFitMin = -0.4, float _dv1dyFitMax = 0.4, string _eventAxisName = "Centrality") {
    isdv1dy = true;
    eventAxisName = _eventAxisName;
    dv1dyFitMin = _dv1dyFitMin;
    dv1dyFitMax = _dv1dyFitMax;
    return *this;
  }

  std::string prefix{""};
  std::string suffix{""};
  std::string mgTitlePattern{"{}-{}%"};

  bool CheckOutputFile() {
    if (outputFile_ && outputFile_->IsOpen()) {
      // do nothing
      return true;
    }

    outputFile_.reset(TFile::Open(ouputFileName_.c_str(), "UPDATE"));
    return bool(outputFile_);
  }

  void operator()(const std::string &resourceName, Result result) {
    using std::string;

    // prepare result for exporting
    if (rebinAxisPtr) {
      result = result.Rebin(*rebinAxisPtr);
    }

    if (isCorrelatedErrors) {
      result.SetSetting(Qn::Stats::Settings::CORRELATEDERRORS);
    }

    // generate new name
    string profileExportName;
    if (!prefix.empty()) {
      profileExportName.append(prefix).append("_");
    }
    profileExportName.append(resourceName);
    if (!suffix.empty()) {
      profileExportName.append("_").append(suffix);
    }

    if (!CheckOutputFile()) {
      throw std::runtime_error("No output file");
    }

    TDirectory *exportDir = outputFile_.get();
    if (!saveFolder.empty()) {
      outputFile_->GetObject(saveFolder.c_str(), exportDir);

      if (!exportDir) {
        exportDir = outputFile_->mkdir(saveFolder.c_str());
      }
    }

    if (result.GetAxes().size() == 2) {
      auto profileMultigraph = new TMultiGraph();
      Qn::Axis axis;
      try { axis = result.GetAxis(eventAxisName); } // TODO
      catch (std::exception &) {
        // TODO return?
        throw std::logic_error("axis not found");
      }
      Qn::Axis centralityAxis = result.GetAxis(eventAxisName);
      TGraphAsymmErrors slopeGraph;
      //slopeGraph.SetTitle(centralityAxis.Name().c_str());
      TGraphAsymmErrors offsetGraph;
      //offsetGraph.SetTitle(centralityAxis.Name().c_str());
      TF1 fitFun("fit", "pol1");
      
      for (unsigned int ibin = 0; ibin < axis.size(); ++ibin) {
        
        auto subresult = result.Select({eventAxisName, {axis.GetLowerBinEdge(ibin), axis.GetUpperBinEdge(ibin)}});
        // shift subgraph points to the center of bins
        auto subgraph = Qn::DataContainerHelper::ToTGraphShifted(subresult,
                                                                 1,
                                                                 2,
                                                                 Qn::DataContainerHelper::Errors::Yonly);
        subgraph->SetTitle(fmt::format(mgTitlePattern, axis.GetLowerBinEdge(ibin), axis.GetUpperBinEdge(ibin)).c_str());
        subgraph->SetMarkerStyle(kFullCircle);
        if (isdv1dy)
        {
          subgraph->Fit(&fitFun, "Q", "", dv1dyFitMin, dv1dyFitMax);
          double centralityLo = centralityAxis.GetLowerBinEdge(ibin);
          double centralityHi = centralityAxis.GetUpperBinEdge(ibin);
          double centrality = 0.5 * (centralityLo + centralityHi);

          double offset = fitFun.GetParameter(0);
          double offsetError = fitFun.GetParError(0);
          offsetGraph.SetPoint(ibin, centrality, offset);
          offsetGraph.SetPointError(ibin, 0., 0., offsetError, offsetError);

          double slope = fitFun.GetParameter(1);
          double slopeError = fitFun.GetParError(1);
          slopeGraph.SetPoint(ibin, centrality, slope);
          slopeGraph.SetPointError(ibin, 0., 0., slopeError, slopeError);
        }
        profileMultigraph->Add(subgraph);

        if (isUnfold) {
          string graphName = profileExportName + "_" + std::to_string(ibin);
          exportDir->WriteObject(subgraph, graphName.c_str());
          Info(__func__, "Graph '%s' is exported", graphName.c_str());
        }
      }

      if (isdv1dy)
      {
        exportDir->WriteObject(&offsetGraph, ("offset_"+profileExportName).c_str());
        Info(__func__, "'%s' is exported as TGraph '%s'", resourceName.c_str(), ("offset_"+profileExportName).c_str());
        exportDir->WriteObject(&slopeGraph, ("slope_"+profileExportName).c_str());
        Info(__func__, "'%s' is exported as TGraph '%s'", resourceName.c_str(), ("slope_"+profileExportName).c_str());
      }
        
      exportDir->WriteObject(profileMultigraph, profileExportName.c_str());
      Info(__func__, "'%s' is exported as TMultiGraph '%s'", resourceName.c_str(), profileExportName.c_str());
    } else if (result.GetAxes().size() == 1) {
      auto profileGraph = Qn::DataContainerHelper::ToTGraph(result);
      // 1d graph is shifted such that points are in the bin centers
      // no need to shift it
      exportDir->WriteObject(profileGraph, profileExportName.c_str());
      Info(__func__, "Graph '%s' is exported as '%s'", resourceName.c_str(), profileExportName.c_str());
    }

  }

};

class ResourceManager {

 public:

  void LoadFile(const std::string &fileName) {
    TFile ff(fileName.c_str(), "read");

    if (!ff.IsOpen()) {
      Error(__func__, "Unable to open %s", fileName.c_str());
      return;
    }

    auto keys = ff.GetListOfKeys();

    for (auto o : *keys) {
      auto key = dynamic_cast<TKey *>(o);

      std::string objName{key->GetName()};

      std::shared_ptr<Result> resPtr;

      if (GetResource(&ff, objName, resPtr)) {
        auto insertResult = resourceMap.insert({objName, resPtr});
        if (!insertResult.second) {
          Warning(__func__, "Object of name '%s' already exists", objName.c_str());
        } else {
          Info(__func__, "Resource '%s' loaded", objName.c_str());
        }
      } else {
        Error(__func__, "Object of name '%s' not found", objName.c_str());
      }

    }

  }

  /**
   * TODO
   * @param query
   * @param fct
   * @return
   */
  bool Define(
      const std::string &query,
      std::function<Result(std::vector<Result>, const std::vector<std::string> &)> fct) {

    const std::regex re(R"(\w+:\w+(,\w+)*)");

    return false;
  }

  bool Define(const std::string &what,
              const std::vector<std::string> &argnames,
              std::function<Result(std::vector<Result>, const std::vector<std::string> &)> fct) {

    // vector of copies
    std::vector<Result> args{};

    if (argnames.empty()) {
      return false;
    }

    for (const auto &argname : argnames) {
      auto resIt = resourceMap.find(argname);
      if (resIt == resourceMap.end()) {
        Warning(__func__, "Resource '%s' was not found", argname.c_str());
        return false;
      }

      args.emplace_back(*(*resIt).second);
    }

    auto result = std::make_shared<Result>(fct(std::move(args), argnames));
    Info(__func__, "Adding resource '%s'", what.c_str());
    resourceMap.insert({what, result});
    return true;
  }

  void SaveAs(const std::string &filename) {
    TFile ff(filename.c_str(), "recreate");

    ff.cd();
    for (auto &entry : resourceMap) {
      entry.second->Write(entry.first.c_str());
    }

    ff.Close();
    Info(__func__, "Saved into '%s'", filename.c_str());
  }

  void ForMatchingExec(const std::string &pattern, const std::function<void(const std::string &, Result)> &fct) const {
    int i_m = 0;
    std::regex re(pattern);
    for (const auto &entry : resourceMap) {
      auto name = entry.first;
      if (std::regex_match(name, re)) {
        fct(name, *entry.second);
        i_m++;
      }
    }
  }

  std::vector<std::string> GetMatchingName(const std::string &pattern) {
    std::vector<std::string> result{};

    std::regex re(pattern);
    for (const auto &entry : resourceMap) {
      auto name = entry.first;
      if (std::regex_match(name, re)) {
        result.push_back(name);
      }
    }

    return result;
  }

 private:

  std::map<std::string, std::shared_ptr<Result >> resourceMap{};
};

#endif //DATATREEFLOW_UTILS_H
