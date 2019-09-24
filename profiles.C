//
// Created by eugene on 4/15/19.
//
#ifdef __CLING__

R__LOAD_LIBRARY(libQnCorrections.so)
R__LOAD_LIBRARY(libBase.so)

#endif

#include "include/Utils.h"


int profiles(
    std::string inputfile = "/home/eugene/analysis/flow/na61/pbpb/13agev/16_011/corr_processed.root",
    std::string outputfile = "/home/eugene/analysis/flow/na61/pbpb/13agev/16_011/profiles.root"
) {

  Qn::Axis centralityAxis("Centrality", {0, 10, 25, 45, 60});

  ResourceManager rm;
  rm.LoadFile(inputfile);
  rm.ForMatchingExec(".*", ProfileExporter(outputfile).Folder("raw"));
  rm.ForMatchingExec("RES_.*", ProfileExporter(outputfile).
      Folder("resolution").
      CorrelatedErrors());
  rm.ForMatchingExec("v1_.*", ProfileExporter(outputfile).
      Folder("v1").
      Rebin(centralityAxis).
      CorrelatedErrors().
      Unfold());
  // v1 with the original binning for dV1/dy vs Centrality plots
  rm.ForMatchingExec("v1_.*", ProfileExporter(outputfile).
      Folder("v1_origbin").
      CorrelatedErrors().
      Unfold());
  // Combined v1
  rm.ForMatchingExec("v1_.*(CC|CR|CA)", ProfileExporter(outputfile).
      Folder("v1_combined").
      Rebin(centralityAxis).
      CorrelatedErrors().
      Unfold());
  // Combined v1
  // v1 with the original binning for dV1/dy vs Centrality plots
  rm.ForMatchingExec("v1_.*(CC|CR|CA)", ProfileExporter(outputfile).
      Folder("v1_combined_origbin").
      CorrelatedErrors().
      Unfold());
  rm.ForMatchingExec("v1_.*(CC|CR|CA)", ProfileExporter(outputfile).
      Folder("v1_combined_15_35").
      Rebin({"Centrality", {15, 35}}).
      CorrelatedErrors().
      Unfold());

  rm.ForMatchingExec("v2_.*", ProfileExporter(outputfile).
      Folder("v2").
      Rebin(centralityAxis).
      CorrelatedErrors().
      Unfold());
  rm.ForMatchingExec("v2_.*(CC|CR|CA)", ProfileExporter(outputfile).
      Folder("v2_combined").
      Rebin(centralityAxis).
      CorrelatedErrors().
      Unfold());
  rm.ForMatchingExec("v2_.*(CC|CR|CA)", ProfileExporter(outputfile).
      Folder("v2_combined_15_35").
      Rebin({"Centrality", {15, 35}}).
      CorrelatedErrors().
      Unfold());
  return 0;
}
