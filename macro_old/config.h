
#include "types.h"
#include <string>
#include <TString.h>
#include <vector>
#include <array>

namespace Config {



const std::string CENTR_NAME = "Centrality";
const std::vector<TString> VAR = {"pT", "y"};
const std::vector<TString> X = {"X", "Y"};
const std::vector<TString> XX = {"XX", "YY", "XY", "YX"};
const std::vector<TString> X2XX = {"X2XX", "X2YY", "Y2XY", "Y2YX"};
const std::vector<TString> PSD = {"psd1", "psd2", "psd3"};
const std::vector<TString> PSD_PSD = {PSD[0] + "_" + PSD[1], PSD[1] + "_" + PSD[2], PSD[2] + "_" + PSD[0]};
const std::vector<TString> TRACKS_RES = {"track_r1", "track_r2"};
const std::vector<TString> PARTICLES = {"pion", "proton", "pion_neg"};
const TString PSI = "psi";
const bool ISSIM = false;

constexpr size_t NPSD = 3;

std::vector<TString> PROFILES;
std::vector<TString> MULTIPROF;

const int NRES = 6;
std::array<std::array<TString, 5>, NRES * NPSD * 2> RES;

void Init() {
  PROFILES.clear();

  for (auto xy : XX) {
    if (ISSIM) {
      for (auto psd : PSD)
        PROFILES.push_back(psd + "_" + PSI + "_" + xy);

      for (auto track_res : TRACKS_RES)
        PROFILES.push_back(track_res + "_" + PSI + "_" + xy);
    }

    for (auto psd_psd : PSD_PSD)
      PROFILES.push_back(psd_psd + "_" + xy);
  }

  MULTIPROF.clear();

  for (auto xy : XX) {
    for (auto particle : PARTICLES) {
      for (auto var : VAR) {
        for (auto psd : PSD)
          MULTIPROF.push_back(particle + "_" + var + "_" + psd + "_" + xy);

//                for (auto psd_psd : PSD_PSD)
//                    for (auto x : X)
//                        MULTIPROF.push_back( particle + "_" + var + "_" + psd_psd + "_" + x + "2" + xy );

        if (ISSIM) {
          MULTIPROF.push_back(particle + "_" + var + "_" + PSI + "_" + xy);
          MULTIPROF.push_back("mc_" + particle + "_" + var + "_" + PSI + "_" + xy);
        }
      }
    }
  }

  for (uint ipsd = 0; ipsd < NPSD; ++ipsd)
    for (uint ix = 0; ix < 2; ++ix) {
      uint ires = ipsd * 2 * NRES;
      RES[ires] = {PSD_PSD[ipsd] + "_" + X[ix] + X[ix], PSD_PSD[(ipsd + 2) % NPSD] + "_" + X[ix] + X[ix],
                   PSD_PSD[(ipsd + 1) % NPSD] + "_" + X[ix] + X[ix], "", ""};

      RES[ires + 1] = {PSD_PSD[ipsd] + "_" + X[ix] + X[ix], PSD_PSD[(ipsd + 2) % NPSD] + "_" + X[ix] + X[ix],
                       PSD_PSD[(ipsd + 1) % NPSD] + "_" + X[ix] + X[ix], "", ""};
      RES[ires + 2] = {PSD_PSD[ipsd] + "_" + X[ix] + X[ix], PSD_PSD[(ipsd + 2) % NPSD] + "_" + X[ix] + X[ix],
                       PSD_PSD[(ipsd + 1) % NPSD] + "_" + X[ix] + X[ix], "", ""};
      RES[ires + 3] = {PSD_PSD[ipsd] + "_" + X[ix] + X[ix], PSD_PSD[(ipsd + 2) % NPSD] + "_" + X[ix] + X[ix],
                       PSD_PSD[(ipsd + 1) % NPSD] + "_" + X[ix] + X[ix], "", ""};
      RES[ires + 4] = {PSD_PSD[ipsd] + "_" + X[ix] + X[ix], PSD_PSD[(ipsd + 2) % NPSD] + "_" + X[ix] + X[ix],
                       PSD_PSD[(ipsd + 1) % NPSD] + "_" + X[ix] + X[ix], "", ""};

      RES[ires + 5] = {PSD[ipsd] + "_" + PSI + "_" + X[ix] + X[ix], "", "", "", ""};
    }

}

const float x_range[2][2] = {{0., 3}, {-0.6, 2.4}};
const float y_range[2][2] = {{-0.06, 0.06}, {-0.15, 0.05}};
const float y_range_v2[2][2] = {{-0.2, 0.2}, {-0.2, 0.2}};
const std::vector<float> v2axis[2] = {{0., .2, .4, .6, 1., 1.4, 2.2, 3.0}, {-0.6, 0., 0.6, 1.2, 1.8, 2.4}};

const std::vector<int> colors = {/*kOrange,*/ kBlue, kRed, kGreen + 2, kBlack, kOrange, kMagenta};
const std::vector<int> markers = {20, 21, 22, 23, 27, 1};

const std::string axisName[2] = {"0Pt", "0Rapidity"};
const bool isplotother = false;

const TString v2_xy[4][3] =
    {
        {"X2XX", "X", "X"},
        {"X2YY", "Y", "Y"},
        {"Y2XY", "X", "Y"},
        {"Y2YX", "Y", "X"}
    };

const std::vector<std::array<TString, 5> > sres =
    {
        {"psd1_psd2_XX", "psd3_psd1_XX", "psd2_psd3_XX", "RES_psd1_X_0",
         "<Q^{psd1}_{x}  Q^{psd2}_{x}> <Q^{psd1}_{x}  Q^{psd3}_{x}> / <Q^{psd2}_{x}  Q^{psd3}_{x}>"},  //psd1 X
//    { "track_r1_psd1_psd2_X2XX", "psd3_psd1_XX", "track_r1_psd2_psd3_X2XX", "RES_psd1_X_1", "<Q^{psd1}_{x}  Q^{psd3}_{x}> <Q^{TPC}_{2,x} Q^{psd1}_{x}  Q^{psd2}_{x}> / < Q^{TPC}_{2,x} Q^{psd2}_{x}  Q^{psd3}_{x}>" }, //psd1 X
//    { "track_r1_psd3_psd1_X2XX", "psd1_psd2_XX", "track_r1_psd2_psd3_X2XX", "RES_psd1_X_2", "<Q^{psd1}_{x}  Q^{psd2}_{x}> <Q^{TPC}_{2,x} Q^{psd1}_{x}  Q^{psd3}_{x}> / < Q^{TPC}_{2,x} Q^{psd2}_{x}  Q^{psd3}_{x}>" }, //psd1 X
//    { "track_r1_psd1_psd2_Y2XY", "psd3_psd1_XX", "track_r1_psd2_psd3_Y2YX", "RES_psd1_X_3", "<Q^{psd1}_{x}  Q^{psd3}_{x}> <Q^{TPC}_{2,y} Q^{psd1}_{x}  Q^{psd2}_{y}> / < Q^{TPC}_{2,y} Q^{psd2}_{y}  Q^{psd3}_{x}>"  }, //psd1 X
//    { "track_r1_psd3_psd1_Y2XY", "psd1_psd2_XX", "track_r1_psd2_psd3_Y2XY", "RES_psd1_X_4", "<Q^{psd1}_{x}  Q^{psd2}_{x}> <Q^{TPC}_{2,y} Q^{psd1}_{x}  Q^{psd3}_{y}> / < Q^{TPC}_{2,y} Q^{psd3}_{y}  Q^{psd2}_{x}>"  }, //psd1 X
//    { "psd1_psi_XX", "", "", "RES_psd1_X_5", "<Q^{psd1}_{x} cos(#Psi_{RP})>"},  //psd1 X

        {"psd1_psd2_YY", "psd3_psd1_YY", "psd2_psd3_YY", "RES_psd1_Y_0",
         "<Q^{psd1}_{y}  Q^{psd2}_{y}> <Q^{psd1}_{y}  Q^{psd3}_{y}> / <Q^{psd2}_{y}  Q^{psd3}_{y}>"},  //psd1 Y
//    { "track_r1_psd1_psd2_X2YY", "psd3_psd1_YY", "track_r1_psd2_psd3_X2YY", "RES_psd1_Y_1", "<Q^{psd1}_{y}  Q^{psd3}_{y}> <Q^{TPC}_{2,x} Q^{psd1}_{y}  Q^{psd2}_{y}> / < Q^{TPC}_{2,x} Q^{psd2}_{y}  Q^{psd3}_{y}>" }, //psd1 Y
//    { "track_r1_psd3_psd1_X2YY", "psd1_psd2_YY", "track_r1_psd2_psd3_X2YY", "RES_psd1_Y_2", "<Q^{psd1}_{y}  Q^{psd2}_{y}> <Q^{TPC}_{2,x} Q^{psd1}_{y}  Q^{psd3}_{y}> / < Q^{TPC}_{2,x} Q^{psd2}_{y}  Q^{psd3}_{y}>" }, //psd1 Y
//    { "track_r1_psd1_psd2_Y2YX", "psd3_psd1_YY", "track_r1_psd2_psd3_Y2XY", "RES_psd1_Y_3", "<Q^{psd1}_{y}  Q^{psd3}_{y}> <Q^{TPC}_{2,y} Q^{psd1}_{y}  Q^{psd2}_{x}> / < Q^{TPC}_{2,y} Q^{psd2}_{x}  Q^{psd3}_{y}>"}, //psd1 Y
//    { "track_r1_psd3_psd1_Y2YX", "psd1_psd2_YY", "track_r1_psd2_psd3_Y2YX", "RES_psd1_Y_4", "<Q^{psd1}_{y}  Q^{psd2}_{y}> <Q^{TPC}_{2,y} Q^{psd1}_{y}  Q^{psd3}_{x}> / < Q^{TPC}_{2,y} Q^{psd3}_{x}  Q^{psd2}_{y}>"}, //psd1 Y
//    { "psd1_psi_YY", "", "", "RES_psd1_Y_5", "<Q^{psd1}_{y} sin(#Psi_{RP}) >"},  //psd1 X

        {"psd1_psd2_XX", "psd2_psd3_XX", "psd3_psd1_XX", "RES_psd2_X_0",
         "<Q^{psd1}_{x}  Q^{psd2}_{x}> <Q^{psd2}_{x}  Q^{psd3}_{x}> / <Q^{psd1}_{x}  Q^{psd3}_{x}>"},  //psd2 X
//    { "track_r1_psd1_psd2_X2XX", "psd2_psd3_XX", "track_r1_psd3_psd1_X2XX", "RES_psd2_X_1", "<Q^{psd2}_{x}  Q^{psd3}_{x}> <Q^{TPC}_{2,x} Q^{psd1}_{x}  Q^{psd2}_{x}> / < Q^{TPC}_{2,x} Q^{psd1}_{x}  Q^{psd3}_{x}>" }, //psd2 X
//    { "track_r1_psd2_psd3_X2XX", "psd1_psd2_XX", "track_r1_psd3_psd1_X2XX", "RES_psd2_X_2", "<Q^{psd2}_{x}  Q^{psd1}_{x}> <Q^{TPC}_{2,x} Q^{psd3}_{x}  Q^{psd2}_{x}> / < Q^{TPC}_{2,x} Q^{psd1}_{x}  Q^{psd3}_{x}>" }, //psd2 X
//    { "track_r1_psd1_psd2_Y2YX", "psd2_psd3_XX", "track_r1_psd3_psd1_Y2YX", "RES_psd2_X_3", "<Q^{psd2}_{x}  Q^{psd3}_{x}> <Q^{TPC}_{2,y} Q^{psd1}_{y}  Q^{psd2}_{x}> / < Q^{TPC}_{2,y} Q^{psd1}_{y}  Q^{psd3}_{x}>"  }, //psd2 X
//    { "track_r1_psd2_psd3_Y2XY", "psd1_psd2_XX", "track_r1_psd3_psd1_Y2XY", "RES_psd2_X_4", "<Q^{psd2}_{x}  Q^{psd1}_{x}> <Q^{TPC}_{2,y} Q^{psd3}_{y}  Q^{psd2}_{x}> / < Q^{TPC}_{2,y} Q^{psd1}_{x}  Q^{psd3}_{y}>"  }, //psd2 X
//    { "psd2_psi_XX", "", "", "RES_psd2_X_5", "<Q^{psd2}_{x} cos(#Psi_{RP}) >"},  //psd1 X

        {"psd1_psd2_YY", "psd2_psd3_YY", "psd3_psd1_YY", "RES_psd2_Y_0",
         "<Q^{psd1}_{y}  Q^{psd2}_{y}> <Q^{psd2}_{y}  Q^{psd3}_{y}> / <Q^{psd1}_{y}  Q^{psd3}_{y}>"},  //psd2 Y
//    { "track_r1_psd1_psd2_X2YY", "psd2_psd3_YY", "track_r1_psd3_psd1_X2YY", "RES_psd2_Y_1", "<Q^{psd2}_{y}  Q^{psd3}_{y}> <Q^{TPC}_{2,x} Q^{psd1}_{y}  Q^{psd2}_{y}> / < Q^{TPC}_{2,x} Q^{psd1}_{y}  Q^{psd3}_{y}>" }, //psd2 Y
//    { "track_r1_psd2_psd3_X2YY", "psd1_psd2_YY", "track_r1_psd3_psd1_X2YY", "RES_psd2_Y_2", "<Q^{psd2}_{y}  Q^{psd1}_{y}> <Q^{TPC}_{2,x} Q^{psd3}_{y}  Q^{psd2}_{y}> / < Q^{TPC}_{2,x} Q^{psd1}_{y}  Q^{psd3}_{y}>" }, //psd2 Y
//    { "track_r1_psd1_psd2_Y2XY", "psd2_psd3_YY", "track_r1_psd3_psd1_Y2YX", "RES_psd2_Y_3", "<Q^{psd2}_{y}  Q^{psd3}_{y}> <Q^{TPC}_{2,y} Q^{psd1}_{x}  Q^{psd2}_{y}> / < Q^{TPC}_{2,y} Q^{psd1}_{x}  Q^{psd3}_{y}>"}, //psd2 Y
//    { "track_r1_psd2_psd3_Y2YX", "psd1_psd2_YY", "track_r1_psd3_psd1_Y2YX", "RES_psd2_Y_4", "<Q^{psd2}_{y}  Q^{psd1}_{y}> <Q^{TPC}_{2,y} Q^{psd3}_{x}  Q^{psd2}_{y}> / < Q^{TPC}_{2,y} Q^{psd3}_{x}  Q^{psd1}_{y}>"}, //psd2 Y
//    { "psd2_psi_YY", "", "", "RES_psd2_Y_5", "<Q^{psd2}_{y} sin(#Psi_{RP}) >"},  //psd1 X

        {"psd3_psd1_XX", "psd2_psd3_XX", "psd1_psd2_XX", "RES_psd3_X_0",
         "<Q^{psd1}_{x}  Q^{psd3}_{x}> <Q^{psd2}_{x}  Q^{psd3}_{x}> / <Q^{psd1}_{x}  Q^{psd2}_{x}>"},  //psd3 X
//    { "track_r1_psd3_psd1_Y2YX", "psd2_psd3_XX", "track_r1_psd1_psd2_Y2YX", "RES_psd3_X_1", "<Q^{psd2}_{x}  Q^{psd3}_{x}> <Q^{TPC}_{2,y} Q^{psd1}_{y}  Q^{psd3}_{x}> / < Q^{TPC}_{2,y} Q^{psd1}_{y}  Q^{psd2}_{x}>"  }, //psd3 X
//    { "track_r1_psd2_psd3_Y2YX", "psd3_psd1_XX", "track_r1_psd1_psd2_Y2XY", "RES_psd3_X_2", "<Q^{psd1}_{x}  Q^{psd3}_{x}> <Q^{TPC}_{2,y} Q^{psd2}_{y}  Q^{psd3}_{x}> / < Q^{TPC}_{2,y} Q^{psd2}_{y}  Q^{psd1}_{x}>"  }, //psd3 X
//    { "track_r1_psd3_psd1_X2XX", "psd2_psd3_XX", "track_r1_psd1_psd2_X2XX", "RES_psd3_X_3", "<Q^{psd2}_{x}  Q^{psd3}_{x}> <Q^{TPC}_{2,x} Q^{psd1}_{x}  Q^{psd3}_{x}> / < Q^{TPC}_{2,x} Q^{psd1}_{x}  Q^{psd2}_{x}>" }, //psd3 X
//    { "track_r1_psd2_psd3_X2XX", "psd3_psd1_XX", "track_r1_psd1_psd2_X2XX", "RES_psd3_X_4", "<Q^{psd1}_{x}  Q^{psd3}_{x}> <Q^{TPC}_{2,x} Q^{psd2}_{x}  Q^{psd3}_{x}> / < Q^{TPC}_{2,x} Q^{psd1}_{x}  Q^{psd2}_{x}>" }, //psd3 X
//    { "psd3_psi_XX", "", "", "RES_psd3_X_5", "<Q^{psd3}_{x} cos(#Psi_{RP}) >"},  //psd1 X

        {"psd3_psd1_YY", "psd2_psd3_YY", "psd1_psd2_YY", "RES_psd3_Y_0",
         "<Q^{psd1}_{y}  Q^{psd3}_{y}> <Q^{psd2}_{y}  Q^{psd3}_{y}> / <Q^{psd1}_{y}  Q^{psd2}_{y}>"},  //psd3 Y
//    { "track_r1_psd3_psd1_X2YY", "psd2_psd3_YY", "track_r1_psd1_psd2_X2YY", "RES_psd3_Y_1", "<Q^{psd2}_{y}  Q^{psd3}_{y}> <Q^{TPC}_{2,x} Q^{psd1}_{y}  Q^{psd3}_{y}> / < Q^{TPC}_{2,x} Q^{psd1}_{y}  Q^{psd2}_{y}>" }, //psd3 Y
//    { "track_r1_psd2_psd3_X2YY", "psd3_psd1_YY", "track_r1_psd1_psd2_X2YY", "RES_psd3_Y_2", "<Q^{psd1}_{y}  Q^{psd3}_{y}> <Q^{TPC}_{2,x} Q^{psd2}_{y}  Q^{psd3}_{y}> / < Q^{TPC}_{2,x} Q^{psd1}_{y}  Q^{psd2}_{y}>" }, //psd3 Y
//    { "track_r1_psd3_psd1_Y2XY", "psd2_psd3_YY", "track_r1_psd1_psd2_Y2XY", "RES_psd3_Y_3", "<Q^{psd2}_{y}  Q^{psd3}_{y}> <Q^{TPC}_{2,y} Q^{psd1}_{x}  Q^{psd3}_{y}> / < Q^{TPC}_{2,y} Q^{psd1}_{x}  Q^{psd2}_{y}>"},  //psd3 Y
//    { "track_r1_psd2_psd3_Y2XY", "psd3_psd1_YY", "track_r1_psd1_psd2_Y2YX", "RES_psd3_Y_4", "<Q^{psd1}_{y}  Q^{psd3}_{y}> <Q^{TPC}_{2,y} Q^{psd2}_{x}  Q^{psd3}_{y}> / < Q^{TPC}_{2,y} Q^{psd2}_{x}  Q^{psd1}_{y}>"},  //psd3 Y
//    { "psd3_psi_YY", "", "", "RES_psd3_Y_5", "<Q^{psd3}_{y} sin(#Psi_{RP}) >"}  //psd1 X

    };

}
