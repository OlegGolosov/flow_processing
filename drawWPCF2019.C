//
// Created by eugene on 6/1/19.
//

#ifdef __CLING__

R__LOAD_LIBRARY(libQnCorrections.so)
R__LOAD_LIBRARY(libBase.so)

#endif

#include "include/Utils.h"
#include <TLegend.h>

using namespace std;
using namespace fmt;

const int ww = 800;
const int wh = 600;

const double obsDescrFontSize = 0.035;
const double legendFontSize = 0.035;
const double experimentFontSize = 0.04;

const double xTitleSize = 0.045;
const double yTitleSize = xTitleSize;

const double canvasLeftMargin = 0.13;
const double canvasBottomMargin = 0.11;
const double paddingLeft = 0.02;

TLegend *MakeLegendFromMG(
    TMultiGraph *mg,
    double x1,
    double y1,
    double x2,
    double y2
) {
  auto leg = new TLegend(x1, y1, x2, y2);

  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetTextSize(legendFontSize);

  for (int ig = 0; ig < mg->GetListOfGraphs()->GetEntries(); ++ig) {
    leg->AddEntry(mg->GetListOfGraphs()->At(ig));
  }

  return leg;
}

int drawWPCF2019(
    const string &wd = "/home/eugene/analysis/flow/na61/pbpb/13agev/16_011/20190520",
    const string &pdfOut = "~/Documents/conferences/20190607_WPCF/plots/plots.pdf",
    const string &dirOut = "~/Documents/conferences/20190607_WPCF/plots"
) {

  string profilesFileName = wd + "/" + "profiles.root";
  string resolutionFileName = wd + "/" + "resolution.root";
  string v1FileName = wd + "/" + "v1.root";
  string dv1dyFileName = wd + "/" + "dv1dy.root";
  string v2FileName = wd + "/" + "v2.root";

  auto c = new TCanvas("noname", "noname", ww, wh);
  c->SetLeftMargin(canvasLeftMargin);
  c->SetBottomMargin(canvasBottomMargin);

  auto writePDF = [c, pdfOut]() {
    c->Print("test.pdf", "pdf");
    return;
  };

  auto saveAs = [c, dirOut](const std::string &name) {
    for (const std::string &suffix : {"pdf", "png", "C"}) {
      string path{dirOut + "/" + name + "." + suffix};
      c->SaveAs(path.c_str());
    }
  };

  c->Print("test.pdf(", "pdf");

  /****** 43 resolution ******/
  {
    TFile fResolution(resolutionFileName.c_str(), "read");
    assert(fResolution.IsOpen());

    TMultiGraph *resolution3SMG = nullptr;
    fResolution.GetObject("3S/resolution3SMG", resolution3SMG);

    resolution3SMG->GetXaxis()->SetLimits(0, 85);
    resolution3SMG->SetMinimum(0.0);
    resolution3SMG->SetMaximum(0.3);
    resolution3SMG->Draw("Ap");
    resolution3SMG->GetXaxis()->SetTitle("PSD Centrality (%)");
    resolution3SMG->GetXaxis()->SetTitleSize(xTitleSize);
    resolution3SMG->GetYaxis()->SetTitle("R_{1}");
    resolution3SMG->GetYaxis()->SetTitleOffset(0.9);
    resolution3SMG->GetYaxis()->SetTitleSize(yTitleSize);

    TLegend *legend = MakeLegendFromMG(resolution3SMG, 0.2, 0.15, 0.6, 0.3);
    legend->SetNColumns(2);
    legend->Draw();

    TPaveText *label = MakePaveText({"NA61/SHINE performance", "Pb+Pb @ 13#it{A} GeV/#it{c}"}, {0.5, 0.78, 0.9, 0.87});
    label->SetTextSize(experimentFontSize);
    label->Draw();

    TPaveText *obsLabel = MakePaveText({"3 PSD subevents"}, {0.2, 0.3, 0.6, 0.35});
    obsLabel->SetTextFont(42);
    obsLabel->SetTextSize(obsDescrFontSize);
    obsLabel->Draw();

    writePDF();
    saveAs("resolution3S");
    c->Clear();
  }

  /****** 4S resolution ******/
  {
    TFile fResolution(resolutionFileName.c_str(), "read");
    assert(fResolution.IsOpen());

    TMultiGraph *resolution4SMG = nullptr;
    fResolution.GetObject("4S/resolution4SMG", resolution4SMG);

    resolution4SMG->GetXaxis()->SetLimits(0, 85);
    resolution4SMG->SetMinimum(0.0);
    resolution4SMG->SetMaximum(0.3);
    resolution4SMG->Draw("Ap");
    resolution4SMG->GetXaxis()->SetTitle("PSD Centrality (%)");
    resolution4SMG->GetXaxis()->SetTitleSize(xTitleSize);
    resolution4SMG->GetYaxis()->SetTitle("R_{1}");
    resolution4SMG->GetYaxis()->SetTitleOffset(0.9);
    resolution4SMG->GetYaxis()->SetTitleSize(yTitleSize);

    TLegend *legend = MakeLegendFromMG(resolution4SMG, 0.2, 0.15, 0.6, 0.3);
    legend->SetNColumns(2);
    legend->Draw();

    TPaveText *label = MakePaveText({"NA61/SHINE performance", "Pb+Pb @ 13#it{A} GeV/#it{c}"}, {0.5, 0.78, 0.9, 0.87});
    label->SetTextSize(experimentFontSize);
    label->Draw();

    TPaveText *obsLabel = MakePaveText({"3 PSD + 1 TPC subevents"}, {0.2, 0.3, 0.6, 0.35});
    obsLabel->SetTextFont(42);
    obsLabel->SetTextSize(obsDescrFontSize);
    obsLabel->Draw();

    writePDF();
    saveAs("resolution4S");
    c->Clear();
  }

  /****** v1 systematics (proton) ******/
  {
    TFile fV1(v1FileName.c_str(), "read");
    assert(fV1.IsOpen());

    TMultiGraph *v1vsPtCombiComponent_proton = nullptr;
    fV1.GetObject("v1_systematics/v1vsPtCombiComponentMG_proton", v1vsPtCombiComponent_proton);

    v1vsPtCombiComponent_proton->GetXaxis()->SetLimits(0.0, 2.5);
    v1vsPtCombiComponent_proton->SetMinimum(-0.02);
    v1vsPtCombiComponent_proton->SetMaximum(0.18);
    v1vsPtCombiComponent_proton->Draw("Ap");
    v1vsPtCombiComponent_proton->GetXaxis()->SetTitleSize(xTitleSize);
    v1vsPtCombiComponent_proton->GetYaxis()->SetTitle("v_{1}");
    v1vsPtCombiComponent_proton->GetYaxis()->SetTitleSize(yTitleSize);

    TPaveText *labelKinematics = MakePaveText({"0 < #it{y} < 1.2", "PSD centrality 10-25%"}, {0.55, 0.20, 0.85, 0.29});
    labelKinematics->SetTextSize(experimentFontSize);
    labelKinematics->Draw();

    TPaveText *labelExperiment =
        MakePaveText({"NA61/SHINE performance", "Pb+Pb @ 13#it{A} GeV/#it{c}"},
                     {canvasLeftMargin + paddingLeft, 0.78, 0.4, 0.87});
    labelExperiment->SetTextSize(experimentFontSize);
    labelExperiment->Draw();

    TLegend *legend = MakeLegendFromMG(v1vsPtCombiComponent_proton, canvasLeftMargin + paddingLeft, 0.45, 0.4, 0.60);
    legend->Draw();

    TPaveText *labelObservable =
        MakePaveText({"p  #it{v}_{1}  {#Psi_{proj}, SP}", "XY combined"}, {canvasLeftMargin + paddingLeft, 0.60, 0.4, 0.68});
    labelObservable->SetTextSize(obsDescrFontSize);
    labelObservable->SetTextFont(42);
    labelObservable->Draw();

    TLine *lineZero = new TLine(0.0, 0.0, 2.5, 0.);
    lineZero->SetLineStyle(kDashed);
    lineZero->Draw();

    writePDF();
    saveAs("v1CC_proton");
    c->Clear();
  }

  /****** v1(pT) systematics (proton) ******/
  {
    TFile fV1(v1FileName.c_str(), "read");
    assert(fV1.IsOpen());

    TMultiGraph *v1vsPtCombiReference_proton = nullptr;
    fV1.GetObject("v1_systematics/v1vsPtCombiReferenceMG_proton", v1vsPtCombiReference_proton);

    v1vsPtCombiReference_proton->GetXaxis()->SetLimits(0.0, 2.5);
    v1vsPtCombiReference_proton->SetMinimum(-0.02);
    v1vsPtCombiReference_proton->SetMaximum(0.18);
    v1vsPtCombiReference_proton->Draw("Ap");
    v1vsPtCombiReference_proton->GetXaxis()->SetTitleSize(xTitleSize);
    v1vsPtCombiReference_proton->GetYaxis()->SetTitle("v_{1}");
    v1vsPtCombiReference_proton->GetYaxis()->SetTitleSize(yTitleSize);

    TPaveText *labelKinematics = MakePaveText({"0 < #it{y} < 1.2", "PSD centrality 10-25%"}, {0.55, 0.20, 0.85, 0.29});
    labelKinematics->SetTextSize(experimentFontSize);
    labelKinematics->Draw();

    TPaveText *labelExperiment =
        MakePaveText({"NA61/SHINE performance", "Pb+Pb @ 13#it{A} GeV/#it{c}"},
                     {canvasLeftMargin + paddingLeft, 0.78, 0.4, 0.87});
    labelExperiment->SetTextSize(experimentFontSize);
    labelExperiment->Draw();

    TPaveText *labelObservable =
        MakePaveText({"p  #it{v}_{1}  {#Psi_{proj}, SP}", "PSD combined"}, {canvasLeftMargin + paddingLeft, 0.60, 0.4, 0.68});
    labelObservable->SetTextSize(obsDescrFontSize);
    labelObservable->SetTextFont(42);
    labelObservable->Draw();

    TLegend *legend = MakeLegendFromMG(v1vsPtCombiReference_proton, canvasLeftMargin + paddingLeft, 0.45, 0.4, 0.60);
    legend->Draw();

    TLine *lineZero = new TLine(0.0, 0.0, 2.5, 0.);
    lineZero->SetLineStyle(kDashed);
    lineZero->Draw();

    writePDF();
    saveAs("v1CR_proton");
    c->Clear();
  }

  /****** v1(pT) vs Centrality (pion negative) ******/
  {
    TFile fV1(v1FileName.c_str(), "read");
    assert(fV1.IsOpen());

    TMultiGraph *v1vsPtCentrality_pion_neg = nullptr;
    fV1.GetObject("v1vsCentrality/v1vsPtCentralityMG_pion_neg", v1vsPtCentrality_pion_neg);

    v1vsPtCentrality_pion_neg->GetXaxis()->SetLimits(0.0, 1.8);
    v1vsPtCentrality_pion_neg->SetMinimum(-0.08);
    v1vsPtCentrality_pion_neg->SetMaximum(0.12);

    v1vsPtCentrality_pion_neg->Draw("ApZ");
    v1vsPtCentrality_pion_neg->GetXaxis()->SetTitleSize(xTitleSize);
    v1vsPtCentrality_pion_neg->GetYaxis()->SetTitle("v_{1}");
    v1vsPtCentrality_pion_neg->GetYaxis()->SetTitleSize(yTitleSize);

    TPaveText *labelKinematics = MakePaveText({"0 < #it{y} < 1.2", "PSD centrality"}, {0.55, 0.15, 0.85, 0.24});
    labelKinematics->SetTextSize(experimentFontSize);
    labelKinematics->Draw();

    TPaveText *labelExperiment =
        MakePaveText({"NA61/SHINE preliminary", "Pb+Pb @ 13#it{A} GeV/#it{c}"},
                     {canvasLeftMargin + paddingLeft, 0.78, 0.4, 0.87});
    labelExperiment->SetTextSize(experimentFontSize);
    labelExperiment->Draw();

    TPaveText *labelObservable =
        MakePaveText({"#pi^{-} #it{v}_{1}  {#Psi_{proj}, SP_{x}}, PSD"},
                     {canvasLeftMargin + paddingLeft, 0.68, 0.4, 0.72});
    labelObservable->SetTextSize(obsDescrFontSize);
    labelObservable->SetTextFont(42);
    labelObservable->Draw();

    TLegend *legend = MakeLegendFromMG(v1vsPtCentrality_pion_neg, canvasLeftMargin + paddingLeft, 0.50, 0.4, 0.68);
    legend->Draw();

    TLine *lineZero = new TLine(0.0, 0.0, 1.8, 0.);
    lineZero->SetLineStyle(kDashed);
    lineZero->Draw();

    writePDF();
    saveAs("v1VsCentrality_pion_neg");
    c->Clear();
  }

/****** v1 systematics (pion neg) ******/
  {
    TFile fV1(v1FileName.c_str(), "read");
    assert(fV1.IsOpen());

    TMultiGraph *v1vsPtCombiComponent_pion_neg = nullptr;
    fV1.GetObject("v1_systematics/v1vsPtCombiComponentMG_pion_neg", v1vsPtCombiComponent_pion_neg);

    v1vsPtCombiComponent_pion_neg->GetXaxis()->SetLimits(0.0, 2.5);
    v1vsPtCombiComponent_pion_neg->SetMinimum(-0.02);
    v1vsPtCombiComponent_pion_neg->SetMaximum(0.1);
    v1vsPtCombiComponent_pion_neg->Draw("Ap");
    v1vsPtCombiComponent_pion_neg->GetXaxis()->SetTitleSize(xTitleSize);
    v1vsPtCombiComponent_pion_neg->GetYaxis()->SetTitle("v_{1}");
    v1vsPtCombiComponent_pion_neg->GetYaxis()->SetTitleSize(yTitleSize);

    TPaveText *labelKinematics = MakePaveText({"0 < #it{y} < 1.2", "PSD centrality 10-25%"}, {0.55, 0.20, 0.85, 0.29});
    labelKinematics->SetTextSize(experimentFontSize);
    labelKinematics->Draw();

    TPaveText *labelExperiment =
        MakePaveText({"NA61/SHINE performance", "Pb+Pb @ 13#it{A} GeV/#it{c}"},
                     {canvasLeftMargin + paddingLeft, 0.78, 0.4, 0.87});
    labelExperiment->SetTextSize(experimentFontSize);
    labelExperiment->Draw();

    TLegend *legend = MakeLegendFromMG(v1vsPtCombiComponent_pion_neg, canvasLeftMargin + paddingLeft, 0.45, 0.4, 0.60);
    legend->Draw();

    TPaveText *labelObservable =
        MakePaveText({"p  #it{v}_{1}  {#Psi_{proj}, SP}", "XY combined"}, {canvasLeftMargin + paddingLeft, 0.60, 0.4, 0.68});
    labelObservable->SetTextSize(obsDescrFontSize);
    labelObservable->SetTextFont(42);
    labelObservable->Draw();

    TLine *lineZero = new TLine(0.0, 0.0, 2.5, 0.);
    lineZero->SetLineStyle(kDashed);
    lineZero->Draw();

    writePDF();
    saveAs("v1CC_pion_neg");
    c->Clear();
  }

  /****** v1(pT) systematics (pion_neg) ******/
  {
    TFile fV1(v1FileName.c_str(), "read");
    assert(fV1.IsOpen());

    TMultiGraph *v1vsPtCombiReference_pion_neg = nullptr;
    fV1.GetObject("v1_systematics/v1vsPtCombiReferenceMG_pion_neg", v1vsPtCombiReference_pion_neg);

    v1vsPtCombiReference_pion_neg->GetXaxis()->SetLimits(0.0, 2.5);
    v1vsPtCombiReference_pion_neg->SetMinimum(-0.02);
    v1vsPtCombiReference_pion_neg->SetMaximum(0.1);
    v1vsPtCombiReference_pion_neg->Draw("Ap");
    v1vsPtCombiReference_pion_neg->GetXaxis()->SetTitleSize(xTitleSize);
    v1vsPtCombiReference_pion_neg->GetYaxis()->SetTitle("v_{1}");
    v1vsPtCombiReference_pion_neg->GetYaxis()->SetTitleSize(yTitleSize);

    TPaveText *labelKinematics = MakePaveText({"0 < #it{y} < 1.2", "PSD centrality 10-25%"}, {0.55, 0.20, 0.85, 0.29});
    labelKinematics->SetTextSize(experimentFontSize);
    labelKinematics->Draw();

    TPaveText *labelExperiment =
        MakePaveText({"NA61/SHINE performance", "Pb+Pb @ 13#it{A} GeV/#it{c}"},
                     {canvasLeftMargin + paddingLeft, 0.78, 0.4, 0.87});
    labelExperiment->SetTextSize(experimentFontSize);
    labelExperiment->Draw();

    TPaveText *labelObservable =
        MakePaveText({"#pi^{-}  #it{v}_{1}  {#Psi_{proj}, SP}", "PSD combined"}, {canvasLeftMargin + paddingLeft, 0.60, 0.4, 0.68});
    labelObservable->SetTextSize(obsDescrFontSize);
    labelObservable->SetTextFont(42);
    labelObservable->Draw();

    TLegend *legend = MakeLegendFromMG(v1vsPtCombiReference_pion_neg, canvasLeftMargin + paddingLeft, 0.45, 0.4, 0.60);
    legend->Draw();

    TLine *lineZero = new TLine(0.0, 0.0, 2.5, 0.);
    lineZero->SetLineStyle(kDashed);
    lineZero->Draw();

    writePDF();
    saveAs("v1CR_pion_neg");
    c->Clear();
  }

  /****** v1(pT) vs Centrality (proton) ******/
  {
    TFile fV1(v1FileName.c_str(), "read");
    assert(fV1.IsOpen());

    TMultiGraph *v1vsPtCentrality_proton = nullptr;
    fV1.GetObject("v1vsCentrality/v1vsPtCentralityMG_proton", v1vsPtCentrality_proton);

    v1vsPtCentrality_proton->GetXaxis()->SetLimits(0.0, 2.5);
    v1vsPtCentrality_proton->SetMinimum(-0.01);
    v1vsPtCentrality_proton->SetMaximum(0.15);

    v1vsPtCentrality_proton->Draw("Ap");
    v1vsPtCentrality_proton->GetXaxis()->SetTitleSize(xTitleSize);
    v1vsPtCentrality_proton->GetYaxis()->SetTitle("v_{1}");
    v1vsPtCentrality_proton->GetYaxis()->SetTitleSize(yTitleSize);

    TPaveText *labelKinematics = MakePaveText({"0 < #it{y} < 1.2", "PSD centrality"}, {0.55, 0.15, 0.85, 0.24});
    labelKinematics->SetTextSize(experimentFontSize);
    labelKinematics->Draw();

    TPaveText *labelExperiment =
        MakePaveText({"NA61/SHINE preliminary", "Pb+Pb @ 13#it{A} GeV/#it{c}"},
                     {canvasLeftMargin + paddingLeft, 0.78, 0.4, 0.87});
    labelExperiment->SetTextSize(experimentFontSize);
    labelExperiment->Draw();

    TPaveText *labelObservable =
        MakePaveText({"p #it{v}_{1}  {#Psi_{proj}, SP_{x}}, PSD"},
                     {canvasLeftMargin + paddingLeft, 0.68, 0.4, 0.72});
    labelObservable->SetTextSize(obsDescrFontSize);
    labelObservable->SetTextFont(42);
    labelObservable->Draw();

    TLegend *legend = MakeLegendFromMG(v1vsPtCentrality_proton, canvasLeftMargin + paddingLeft, 0.50, 0.4, 0.68);
    legend->Draw();

    TLine *lineZero = new TLine(0.0, 0.0, 2.5, 0.);
    lineZero->SetLineStyle(kDashed);
    lineZero->Draw();

    writePDF();
    saveAs("v1VsCentrality_proton");
    c->Clear();
  }


  /****** v1(pT) vs Particle ******/
  {
    TFile fV1(v1FileName.c_str(), "read");
    assert(fV1.IsOpen());

    TMultiGraph *v1vsPtParticle = nullptr;
    fV1.GetObject("v1vsParticle/v1vsPtParticleMG", v1vsPtParticle);

    v1vsPtParticle->GetXaxis()->SetLimits(0.0, 2.5);
    v1vsPtParticle->SetMinimum(-0.05);
    v1vsPtParticle->SetMaximum(canvasLeftMargin + paddingLeft);

    v1vsPtParticle->Draw("Ap");
    v1vsPtParticle->GetXaxis()->SetTitleSize(xTitleSize);
    v1vsPtParticle->GetYaxis()->SetTitle("v_{1}");
    v1vsPtParticle->GetYaxis()->SetTitleSize(yTitleSize);

    TPaveText *labelKinematics = MakePaveText({"0 < #it{y} < 1.2", "PSD centrality 15-35%"}, {0.55, 0.15, 0.85, 0.24});
    labelKinematics->SetTextSize(experimentFontSize);
    labelKinematics->Draw();

    TPaveText *labelExperiment =
        MakePaveText({"NA61/SHINE preliminary", "Pb+Pb @ 13#it{A} GeV/#it{c}"},
                     {canvasLeftMargin + paddingLeft, 0.78, 0.4, 0.87});
    labelExperiment->SetTextSize(experimentFontSize);
    labelExperiment->Draw();

    TPaveText *labelObservable =
        MakePaveText({"#it{v}_{1}  {#Psi_{proj}, SP_{x}}, PSD"}, {canvasLeftMargin + paddingLeft, 0.68, 0.4, 0.72});
    labelObservable->SetTextSize(obsDescrFontSize);
    labelObservable->SetTextFont(42);
    labelObservable->Draw();

    TLegend *legend = MakeLegendFromMG(v1vsPtParticle, canvasLeftMargin + paddingLeft, 0.50, 0.4, 0.68);
    legend->Draw();

    TLine *lineZero = new TLine(0.0, 0.0, 2.5, 0.);
    lineZero->SetLineStyle(kDashed);
    lineZero->Draw();

    writePDF();
    saveAs("v1VsParticle");
    c->Clear();
  }

  /****** dv1 / dy vs Particle ******/
  {
    TFile fdV1dy(dv1dyFileName.c_str(), "read");
    assert(fdV1dy.IsOpen());

    TMultiGraph *dv1dyVsCentralityParticle = nullptr;
    fdV1dy.GetObject("dv1dyVsParticle", dv1dyVsCentralityParticle);

    dv1dyVsCentralityParticle->GetXaxis()->SetLimits(0.0, 85.0);
    dv1dyVsCentralityParticle->SetMinimum(-0.1);
    dv1dyVsCentralityParticle->SetMaximum(0.12);

    dv1dyVsCentralityParticle->Draw("Ap");
    dv1dyVsCentralityParticle->GetXaxis()->SetTitle("M_{TPC} Centrality (%)");
    dv1dyVsCentralityParticle->GetXaxis()->SetTitleSize(xTitleSize);
    dv1dyVsCentralityParticle->GetYaxis()->SetTitle("d #it{v}_{1} /d#it{y}");
    dv1dyVsCentralityParticle->GetYaxis()->SetTitleSize(yTitleSize);

    TPaveText *labelKinematics =
        MakePaveText({"p  0.4 < p_{T} < 2.0 GeV/#it{c}", "#pi^{-}  0.2 < p_{T} < 1.6 GeV/#it{c}"},
                     {0.55, 0.78, 0.85, 0.87});
    labelKinematics->SetTextSize(experimentFontSize);
    labelKinematics->Draw();

    TPaveText *labelExperiment =
        MakePaveText({"NA61/SHINE preliminary", "Pb+Pb @ 13#it{A} GeV/#it{c}"},
                     {canvasLeftMargin + paddingLeft, 0.78, 0.4, 0.87});
    labelExperiment->SetTextSize(experimentFontSize);
    labelExperiment->Draw();

    TPaveText *labelObservable =
        MakePaveText({"d#it{v}_{1}  {#Psi_{proj}, SP_{x}}/dy"}, {canvasLeftMargin + paddingLeft, 0.33, 0.4, 0.37});
    labelObservable->SetTextSize(obsDescrFontSize);
    labelObservable->SetTextFont(42);
    labelObservable->Draw();

    TLegend *legend = MakeLegendFromMG(dv1dyVsCentralityParticle, canvasLeftMargin + paddingLeft, 0.15, 0.4, 0.33);
    legend->Draw();

    TLine *lineZero = new TLine(0.0, 0.0, 85., 0.);
    lineZero->SetLineStyle(kDashed);
    lineZero->Draw();

    writePDF();
    saveAs("dv1dy");
    c->Clear();
  }

  {
    TFile v2(v2FileName.c_str(), "read");
    assert(v2.IsOpen());

    TMultiGraph *v1vsPtParticle = nullptr;
    v2.GetObject("v2vsPtParticleMG", v1vsPtParticle);

    v1vsPtParticle->GetXaxis()->SetLimits(0.0, 2.5);
    v1vsPtParticle->SetMinimum(-0.02);
    v1vsPtParticle->SetMaximum(0.15);

    v1vsPtParticle->Draw("Ap");
    v1vsPtParticle->GetXaxis()->SetTitleSize(xTitleSize);
    v1vsPtParticle->GetYaxis()->SetTitle("v_{2}");
    v1vsPtParticle->GetYaxis()->SetTitleSize(yTitleSize);

    TPaveText *labelKinematics = MakePaveText({"0 < #it{y} < 1.2", "PSD centrality 15-35%"}, {0.55, 0.15, 0.85, 0.24});
    labelKinematics->SetTextSize(experimentFontSize);
    labelKinematics->Draw();

    TPaveText *labelExperiment =
        MakePaveText({"NA61/SHINE preliminary", "Pb+Pb @ 13#it{A} GeV/#it{c}"},
                     {canvasLeftMargin + paddingLeft, 0.78, 0.4, 0.87});
    labelExperiment->SetTextSize(experimentFontSize);
    labelExperiment->Draw();

    TPaveText *labelObservable =
        MakePaveText({"#it{v}_{2}  {#Psi_{proj}, SP_{y}}, PSD"}, {canvasLeftMargin + paddingLeft, 0.68, 0.4, 0.72});
    labelObservable->SetTextSize(obsDescrFontSize);
    labelObservable->SetTextFont(42);
    labelObservable->Draw();

    TLegend *legend = MakeLegendFromMG(v1vsPtParticle, canvasLeftMargin + paddingLeft, 0.50, 0.4, 0.68);
    legend->Draw();

    TLine *lineZero = new TLine(0.0, 0.0, 2.5, 0.);
    lineZero->SetLineStyle(kDashed);
    lineZero->Draw();

    writePDF();
    saveAs("v2VsParticle");
    c->Clear();
  }


  /****** v2(pT) vs Centrality (proton) ******/
  {
    TFile fV2(v2FileName.c_str(), "read");
    assert(fV2.IsOpen());

    TMultiGraph *v2vsPtCentrality_pion_neg = nullptr;
    fV2.GetObject("v2vsPtCentralityMG_pion_neg", v2vsPtCentrality_pion_neg);

    v2vsPtCentrality_pion_neg->GetXaxis()->SetLimits(0.0, 2.5);
    v2vsPtCentrality_pion_neg->SetMinimum(-0.02);
    v2vsPtCentrality_pion_neg->SetMaximum(0.1);

    v2vsPtCentrality_pion_neg->Draw("Ap");
    v2vsPtCentrality_pion_neg->GetXaxis()->SetTitleSize(xTitleSize);
    v2vsPtCentrality_pion_neg->GetYaxis()->SetTitle("v_{2}");
    v2vsPtCentrality_pion_neg->GetYaxis()->SetTitleSize(yTitleSize);

    TPaveText *labelKinematics = MakePaveText({"0 < #it{y} < 1.2"}, {0.55, 0.15, 0.85, 0.24});
    labelKinematics->SetTextSize(experimentFontSize);
    labelKinematics->Draw();

    TPaveText *labelExperiment =
        MakePaveText({"NA61/SHINE preliminary", "Pb+Pb @ 13#it{A} GeV/#it{c}"},
                     {canvasLeftMargin + paddingLeft, 0.78, 0.4, 0.87});
    labelExperiment->SetTextSize(experimentFontSize);
    labelExperiment->Draw();

    TPaveText *labelObservable =
        MakePaveText({"#pi^{-} #it{v}_{2}  {#Psi_{proj}, SP_{y}}, PSD"},
                     {canvasLeftMargin + paddingLeft, 0.68, 0.4, 0.72});
    labelObservable->SetTextSize(obsDescrFontSize);
    labelObservable->SetTextFont(42);
    labelObservable->Draw();

    TLegend *legend = MakeLegendFromMG(v2vsPtCentrality_pion_neg, canvasLeftMargin + paddingLeft, 0.50, 0.4, 0.68);
    legend->Draw();

    TLine *lineZero = new TLine(0.0, 0.0, 2.5, 0.);
    lineZero->SetLineStyle(kDashed);
    lineZero->Draw();

    writePDF();
    saveAs("v2VsCentrality_pion_neg");
    c->Clear();
  }

  {
    TFile fV2(v2FileName.c_str(), "read");
    assert(fV2.IsOpen());

    TMultiGraph *v2vsPtCentrality_proton = nullptr;
    fV2.GetObject("v2vsPtCentralityMG_proton", v2vsPtCentrality_proton);

    v2vsPtCentrality_proton->GetXaxis()->SetLimits(0.0, 2.5);
    v2vsPtCentrality_proton->SetMinimum(-0.02);
    v2vsPtCentrality_proton->SetMaximum(0.15);

    v2vsPtCentrality_proton->Draw("Ap");
    v2vsPtCentrality_proton->GetXaxis()->SetTitleSize(xTitleSize);
    v2vsPtCentrality_proton->GetYaxis()->SetTitle("v_{2}");
    v2vsPtCentrality_proton->GetYaxis()->SetTitleSize(yTitleSize);

    TPaveText *labelKinematics = MakePaveText({"0 < #it{y} < 1.2"}, {0.55, 0.15, 0.85, 0.24});
    labelKinematics->SetTextSize(experimentFontSize);
    labelKinematics->Draw();

    TPaveText *labelExperiment =
        MakePaveText({"NA61/SHINE preliminary", "Pb+Pb @ 13#it{A} GeV/#it{c}"},
                     {canvasLeftMargin + paddingLeft, 0.78, 0.4, 0.87});
    labelExperiment->SetTextSize(experimentFontSize);
    labelExperiment->Draw();

    TPaveText *labelObservable =
        MakePaveText({"p #it{v}_{2}  {#Psi_{proj}, SP_{y}}, PSD"},
                     {canvasLeftMargin + paddingLeft, 0.68, 0.4, 0.72});
    labelObservable->SetTextSize(obsDescrFontSize);
    labelObservable->SetTextFont(42);
    labelObservable->Draw();

    TLegend *legend = MakeLegendFromMG(v2vsPtCentrality_proton, canvasLeftMargin + paddingLeft, 0.50, 0.4, 0.68);
    legend->Draw();

    TLine *lineZero = new TLine(0.0, 0.0, 2.5, 0.);
    lineZero->SetLineStyle(kDashed);
    lineZero->Draw();

    writePDF();
    saveAs("v2VsCentrality_proton");
    c->Clear();
  }
  c->Print("test.pdf)", "pdf");

  delete c;
  return 0;
}