#include "exp_data/exp_data_NA49.h"
#include "exp_data/exp_data_STAR.h"


const vector <int> colors = 
{
  kBlue, kRed, kGreen+2, kMagenta+2, kBlack, kOrange+2, kPink+2,
  kTeal+2, kCyan+2, kCyan+4, kAzure, kGray+3, kOrange+7, kGreen+4
};
const vector <vector <int>> markers = 
{ 
  {20, 21, 22, 23, 29, 33, 34, 39, 41, 43, 45, 47, 48, 49}, // filled
  {24, 25, 26, 32, 30, 27, 28, 37, 40, 42, 44, 46, 35, 36}  // empty
};

bool rebinCorr = true;
bool rebinFlow = true;

vector <float> centralityBins = {0,15,45};
Qn::Axis centralityAxis ("Centrality", centralityBins);

vector <vector<TString>> pids = 
{
  {"pion", "#pi"},
  {"proton", "p"},
};

std::vector <std::vector<TString>> QQ_names = 
{
		{"track_r1_psd1", "#LTQ_{1,%s}^{TPC1}Q_{1,%s}^{PSD1}#GT"},
		{"track_r1_psd2", "#LTQ_{1,%s}^{TPC1}Q_{1,%s}^{PSD2}#GT"},
		{"track_r1_psd3", "#LTQ_{1,%s}^{TPC1}Q_{1,%s}^{PSD3}#GT"},
		{"track_r2_psd1", "#LTQ_{1,%s}^{TPC2}Q_{1,%s}^{PSD1}#GT"},
		{"track_r2_psd2", "#LTQ_{1,%s}^{TPC2}Q_{1,%s}^{PSD2}#GT"},
		{"track_r2_psd3", "#LTQ_{1,%s}^{TPC2}Q_{1,%s}^{PSD3}#GT"},
		{"psd1_psd2", "#LTQ_{1,%s}^{PSD1}Q_{1,%s}^{PSD2}#GT"},
		{"psd2_psd3", "#LTQ_{1,%s}^{PSD2}Q_{1,%s}^{PSD3}#GT"},
		{"psd3_psd1", "#LTQ_{1,%s}^{PSD1}Q_{1,%s}^{PSD3}#GT"},
};

std::vector <std::vector<TString>> QQQ_names = 
{
		{"track_r1_psd1_psd2", "#LTQ_{2,%s}^{TPC1}Q_{1,%s}^{PSD1}Q_{1,%s}^{PSD2}#GT"},
		{"track_r1_psd2_psd3", "#LTQ_{2,%s}^{TPC1}Q_{1,%s}^{PSD2}Q_{1,%s}^{PSD3}#GT"},
		{"track_r1_psd3_psd1", "#LTQ_{2,%s}^{TPC1}Q_{1,%s}^{PSD1}Q_{1,%s}^{PSD3}#GT"},
		{"track_r2_psd1_psd2", "#LTQ_{2,%s}^{TPC2}Q_{1,%s}^{PSD1}Q_{1,%s}^{PSD2}#GT"},
		{"track_r2_psd2_psd3", "#LTQ_{2,%s}^{TPC2}Q_{1,%s}^{PSD2}Q_{1,%s}^{PSD3}#GT"},
		{"track_r2_psd3_psd1", "#LTQ_{2,%s}^{TPC2}Q_{1,%s}^{PSD1}Q_{1,%s}^{PSD3}#GT"},
};

std::vector <std::vector<TString>> uQ_names = 
{
  {"%s_%s_psd1", "#LTu_{1,%s}^{%s}Q_{1,%s}^{PSD1}#GT"},
  {"%s_%s_psd2", "#LTu_{1,%s}^{%s}Q_{1,%s}^{PSD2}#GT"},
  {"%s_%s_psd3", "#LTu_{1,%s}^{%s}Q_{1,%s}^{PSD3}#GT"},
};

std::vector <std::vector<TString>> uQQ_names = 
{
		{"%s_%s_psd1_psd2", "#LTu_{2,%s}^{%s}Q_{1,%s}^{PSD1}Q_{1,%s}^{PSD2}#GT"},
		{"%s_%s_psd2_psd3", "#LTu_{2,%s}^{%s}Q_{1,%s}^{PSD2}Q_{1,%s}^{PSD3}#GT"},
		{"%s_%s_psd3_psd1", "#LTu_{2,%s}^{%s}Q_{1,%s}^{PSD3}Q_{1,%s}^{PSD1}#GT"},
};

std::vector <std::vector<TString>> res_names = 
{
		{"R1_psd1_3S_", "psd1_psd2_", "psd3_psd1_", "psd2_psd3_", "R_{1,%s}^{PSD1,3S}(PSD2, PSD3)"},
		{"R1_psd2_3S_", "psd1_psd2_", "psd2_psd3_", "psd3_psd1_", "R_{1,%s}^{PSD2,3S}(PSD1, PSD3)"},
		{"R1_psd3_3S_", "psd3_psd1_", "psd2_psd3_", "psd1_psd2_", "R_{1,%s}^{PSD3,3S}(PSD1, PSD2)"},
};

std::vector <std::vector<TString>> res_names_QQQ = 
{
		{"R2_PSD1_1_MH_", "TPC_1_PSD1_PSD2_X2", "PSD1_PSD3_", "TPC_3_PSD2_PSD3_X2", "R_{1,%s}^{VCAL,MH}(PSD2, PSD3, TPC2, TPC3)"},
		{"R2_PSD1_2_MH_", "TPC_1_PSD1_PSD3_X2", "PSD1_PSD2_", "TPC_3_PSD2_PSD3_X2", "R_{1,%s}^{VCAL,MH}(PSD2, PSD3, TPC2, TPC3)"},
		{"R2_PSD1_3_MH_", "TPC_1_PSD1_PSD2_X2", "PSD1_PSD3_", "TPC_3_PSD2_PSD3_X2", "R_{1,%s}^{VCAL,MH}(PSD2, PSD3, TPC2, TPC3)"},
		{"R2_PSD1_4_MH_", "TPC_1_PSD1_PSD3_X2", "PSD1_PSD2_", "TPC_3_PSD2_PSD3_X2", "R_{1,%s}^{VCAL,MH}(PSD2, PSD3, TPC2, TPC3)"},
};

std::vector <std::vector<TString>> V1_names = 
{
		{"%s_V1_%s_psd1_3S", "%s_%s_psd1", "R1_psd1_3S", "v_{1,%s}^{%s}{#psi_{SP}^{p, PSD1}}"},
		{"%s_V1_%s_psd2_3S", "%s_%s_psd2", "R1_psd2_3S", "v_{1,%s}^{%s}{#psi_{SP}^{p, PSD2}}"},
		{"%s_V1_%s_psd3_3S", "%s_%s_psd3", "R1_psd3_3S", "v_{1,%s}^{%s}{#psi_{SP}^{p, PSD3}}"}
};

std::vector <std::vector<TString>> V2_names = 
{
		{"V2_%s_RS_TPC_a", "TPC_%s_a_2_TPC_b_2", "R2_TPC_RS", "v_{2,%s}^{RS, TPCa}(%s)"},
		{"V2_%s_RS_TPC_b", "TPC_%s_b_2_TPC_a_2", "R2_TPC_RS", "v_{2,%s}^{RS, TPCb}(%s)"}
};


////////////////////////////////////////////////////////////////////////////////////////////////
const int nres = 30;
const TString PsdName[] = { "PSD1", "PSD2", "PSD3" };
const TString XY[] = {"X", "Y"};

const int ncentr = 4;

const TString psd_psd[] = {"PSD1_PSD2", "PSD1_PSD3", "PSD2_PSD3"};
const TString var[] = {"pT", "eta"};
const float x_range[2][2] = { {0.,3}, {-0.6,2.4} };
const float y_range[2][2] = { {-0.06,0.06}, {-0.15,0.05} };
const float y_range_v2[2][2] = { {-0.2,0.2}, {-0.2,0.2} };

const bool issim=true;

const std::vector <float> v2axis[2] = { { 0., .2, .4, .6, 1., 1.4, 2.2, 3.0 }, {-0.6, 0., 0.6, 1.2, 1.8, 2.4} } ;
const std::string axisName[2] = { "0Pt", "0Rapidity" } ;

const TString v2[4][3] =
{
    {"X2XX", "XX", ""  },
    {"X2YY", "YY", ""  },
    {"Y2XY", "X", "Y" },
    {"Y2YX", "Y", "X" }
};

const TString sres[][5] =
{
    { "PSD1_PSD2_XX", "PSD1_PSD3_XX", "PSD2_PSD3_XX", "RES_PSD1_X_0", "<Q^{PSD1}_{x}  Q^{PSD2}_{x}> <Q^{PSD1}_{x}  Q^{PSD3}_{x}> / <Q^{PSD2}_{x}  Q^{PSD3}_{x}>"},  //PSD1 X
    { "TPC_R2_PSD1_PSD2_X2XX", "PSD1_PSD3_XX", "TPC_R2_PSD2_PSD3_X2XX", "RES_PSD1_X_1", "<Q^{PSD1}_{x}  Q^{PSD3}_{x}> <Q^{TPC}_{2,x} Q^{PSD1}_{x}  Q^{PSD2}_{x}> / < Q^{TPC}_{2,x} Q^{PSD2}_{x}  Q^{PSD3}_{x}>" }, //PSD1 X
    { "TPC_R2_PSD1_PSD3_X2XX", "PSD1_PSD2_XX", "TPC_R2_PSD2_PSD3_X2XX", "RES_PSD1_X_2", "<Q^{PSD1}_{x}  Q^{PSD2}_{x}> <Q^{TPC}_{2,x} Q^{PSD1}_{x}  Q^{PSD3}_{x}> / < Q^{TPC}_{2,x} Q^{PSD2}_{x}  Q^{PSD3}_{x}>" }, //PSD1 X
    { "TPC_R2_PSD1_PSD2_Y2XY", "PSD1_PSD3_XX", "TPC_R2_PSD2_PSD3_Y2YX", "RES_PSD1_X_3", "<Q^{PSD1}_{x}  Q^{PSD3}_{x}> <Q^{TPC}_{2,y} Q^{PSD1}_{x}  Q^{PSD2}_{y}> / < Q^{TPC}_{2,y} Q^{PSD2}_{y}  Q^{PSD3}_{x}>"  }, //PSD1 X
    { "TPC_R2_PSD1_PSD3_Y2XY", "PSD1_PSD2_XX", "TPC_R2_PSD2_PSD3_Y2XY", "RES_PSD1_X_4", "<Q^{PSD1}_{x}  Q^{PSD2}_{x}> <Q^{TPC}_{2,y} Q^{PSD1}_{x}  Q^{PSD3}_{y}> / < Q^{TPC}_{2,y} Q^{PSD3}_{y}  Q^{PSD2}_{x}>"  }, //PSD1 X

    { "PSD1_PSD2_YY", "PSD1_PSD3_YY", "PSD2_PSD3_YY", "RES_PSD1_Y_0", "<Q^{PSD1}_{y}  Q^{PSD2}_{y}> <Q^{PSD1}_{y}  Q^{PSD3}_{y}> / <Q^{PSD2}_{y}  Q^{PSD3}_{y}>"},  //PSD1 Y
    { "TPC_R2_PSD1_PSD2_X2YY", "PSD1_PSD3_YY", "TPC_R2_PSD2_PSD3_X2YY", "RES_PSD1_Y_1", "<Q^{PSD1}_{y}  Q^{PSD3}_{y}> <Q^{TPC}_{2,x} Q^{PSD1}_{y}  Q^{PSD2}_{y}> / < Q^{TPC}_{2,x} Q^{PSD2}_{y}  Q^{PSD3}_{y}>" }, //PSD1 Y
    { "TPC_R2_PSD1_PSD3_X2YY", "PSD1_PSD2_YY", "TPC_R2_PSD2_PSD3_X2YY", "RES_PSD1_Y_2", "<Q^{PSD1}_{y}  Q^{PSD2}_{y}> <Q^{TPC}_{2,x} Q^{PSD1}_{y}  Q^{PSD3}_{y}> / < Q^{TPC}_{2,x} Q^{PSD2}_{y}  Q^{PSD3}_{y}>" }, //PSD1 Y
    { "TPC_R2_PSD1_PSD2_Y2YX", "PSD1_PSD3_YY", "TPC_R2_PSD2_PSD3_Y2XY", "RES_PSD1_Y_3", "<Q^{PSD1}_{y}  Q^{PSD3}_{y}> <Q^{TPC}_{2,y} Q^{PSD1}_{y}  Q^{PSD2}_{x}> / < Q^{TPC}_{2,y} Q^{PSD2}_{x}  Q^{PSD3}_{y}>"}, //PSD1 Y
    { "TPC_R2_PSD1_PSD3_Y2YX", "PSD1_PSD2_YY", "TPC_R2_PSD2_PSD3_Y2YX", "RES_PSD1_Y_4", "<Q^{PSD1}_{y}  Q^{PSD2}_{y}> <Q^{TPC}_{2,y} Q^{PSD1}_{y}  Q^{PSD3}_{x}> / < Q^{TPC}_{2,y} Q^{PSD3}_{x}  Q^{PSD2}_{y}>"}, //PSD1 Y

    { "PSD1_PSD2_XX", "PSD2_PSD3_XX", "PSD1_PSD3_XX", "RES_PSD2_X_0", "<Q^{PSD1}_{x}  Q^{PSD2}_{x}> <Q^{PSD2}_{x}  Q^{PSD3}_{x}> / <Q^{PSD1}_{x}  Q^{PSD3}_{x}>"},  //PSD2 X
    { "TPC_R2_PSD1_PSD2_X2XX", "PSD2_PSD3_XX", "TPC_R2_PSD1_PSD3_X2XX", "RES_PSD2_X_1", "<Q^{PSD2}_{x}  Q^{PSD3}_{x}> <Q^{TPC}_{2,x} Q^{PSD1}_{x}  Q^{PSD2}_{x}> / < Q^{TPC}_{2,x} Q^{PSD1}_{x}  Q^{PSD3}_{x}>" }, //PSD2 X
    { "TPC_R2_PSD2_PSD3_X2XX", "PSD1_PSD2_XX", "TPC_R2_PSD1_PSD3_X2XX", "RES_PSD2_X_2", "<Q^{PSD2}_{x}  Q^{PSD1}_{x}> <Q^{TPC}_{2,x} Q^{PSD3}_{x}  Q^{PSD2}_{x}> / < Q^{TPC}_{2,x} Q^{PSD1}_{x}  Q^{PSD3}_{x}>" }, //PSD2 X
    { "TPC_R2_PSD1_PSD2_Y2YX", "PSD2_PSD3_XX", "TPC_R2_PSD1_PSD3_Y2YX", "RES_PSD2_X_3", "<Q^{PSD2}_{x}  Q^{PSD3}_{x}> <Q^{TPC}_{2,y} Q^{PSD1}_{y}  Q^{PSD2}_{x}> / < Q^{TPC}_{2,y} Q^{PSD1}_{y}  Q^{PSD3}_{x}>"  }, //PSD2 X
    { "TPC_R2_PSD2_PSD3_Y2XY", "PSD1_PSD2_XX", "TPC_R2_PSD1_PSD3_Y2XY", "RES_PSD2_X_4", "<Q^{PSD2}_{x}  Q^{PSD1}_{x}> <Q^{TPC}_{2,y} Q^{PSD3}_{y}  Q^{PSD2}_{x}> / < Q^{TPC}_{2,y} Q^{PSD1}_{x}  Q^{PSD3}_{y}>"  }, //PSD2 X

    { "PSD1_PSD2_YY", "PSD2_PSD3_YY", "PSD1_PSD3_YY", "RES_PSD2_Y_0", "<Q^{PSD1}_{y}  Q^{PSD2}_{y}> <Q^{PSD2}_{y}  Q^{PSD3}_{y}> / <Q^{PSD1}_{y}  Q^{PSD3}_{y}>"},  //PSD2 Y
    { "TPC_R2_PSD1_PSD2_X2YY", "PSD2_PSD3_YY", "TPC_R2_PSD1_PSD3_X2YY", "RES_PSD2_Y_1", "<Q^{PSD2}_{y}  Q^{PSD3}_{y}> <Q^{TPC}_{2,x} Q^{PSD1}_{y}  Q^{PSD2}_{y}> / < Q^{TPC}_{2,x} Q^{PSD1}_{y}  Q^{PSD3}_{y}>" }, //PSD2 Y
    { "TPC_R2_PSD2_PSD3_X2YY", "PSD1_PSD2_YY", "TPC_R2_PSD1_PSD3_X2YY", "RES_PSD2_Y_2", "<Q^{PSD2}_{y}  Q^{PSD1}_{y}> <Q^{TPC}_{2,x} Q^{PSD3}_{y}  Q^{PSD2}_{y}> / < Q^{TPC}_{2,x} Q^{PSD1}_{y}  Q^{PSD3}_{y}>" }, //PSD2 Y
    { "TPC_R2_PSD1_PSD2_Y2XY", "PSD2_PSD3_YY", "TPC_R2_PSD1_PSD3_Y2YX", "RES_PSD2_Y_3", "<Q^{PSD2}_{y}  Q^{PSD3}_{y}> <Q^{TPC}_{2,y} Q^{PSD1}_{x}  Q^{PSD2}_{y}> / < Q^{TPC}_{2,y} Q^{PSD1}_{x}  Q^{PSD3}_{y}>"}, //PSD2 Y
    { "TPC_R2_PSD2_PSD3_Y2YX", "PSD1_PSD2_YY", "TPC_R2_PSD1_PSD3_Y2YX", "RES_PSD2_Y_4", "<Q^{PSD2}_{y}  Q^{PSD1}_{y}> <Q^{TPC}_{2,y} Q^{PSD3}_{x}  Q^{PSD2}_{y}> / < Q^{TPC}_{2,y} Q^{PSD3}_{x}  Q^{PSD1}_{y}>"}, //PSD2 Y

    { "PSD1_PSD3_XX", "PSD2_PSD3_XX", "PSD1_PSD2_XX", "RES_PSD3_X_0", "<Q^{PSD1}_{x}  Q^{PSD3}_{x}> <Q^{PSD2}_{x}  Q^{PSD3}_{x}> / <Q^{PSD1}_{x}  Q^{PSD2}_{x}>"},  //PSD3 X
    { "TPC_R2_PSD1_PSD3_Y2YX", "PSD2_PSD3_XX", "TPC_R2_PSD1_PSD2_Y2YX", "RES_PSD3_X_1", "<Q^{PSD2}_{x}  Q^{PSD3}_{x}> <Q^{TPC}_{2,y} Q^{PSD1}_{y}  Q^{PSD3}_{x}> / < Q^{TPC}_{2,y} Q^{PSD1}_{y}  Q^{PSD2}_{x}>"  }, //PSD3 X
    { "TPC_R2_PSD2_PSD3_Y2YX", "PSD1_PSD3_XX", "TPC_R2_PSD1_PSD2_Y2XY", "RES_PSD3_X_2", "<Q^{PSD1}_{x}  Q^{PSD3}_{x}> <Q^{TPC}_{2,y} Q^{PSD2}_{y}  Q^{PSD3}_{x}> / < Q^{TPC}_{2,y} Q^{PSD2}_{y}  Q^{PSD1}_{x}>"  }, //PSD3 X
    { "TPC_R2_PSD1_PSD3_X2XX", "PSD2_PSD3_XX", "TPC_R2_PSD1_PSD2_X2XX", "RES_PSD3_X_3", "<Q^{PSD2}_{x}  Q^{PSD3}_{x}> <Q^{TPC}_{2,x} Q^{PSD1}_{x}  Q^{PSD3}_{x}> / < Q^{TPC}_{2,x} Q^{PSD1}_{x}  Q^{PSD2}_{x}>" }, //PSD3 X
    { "TPC_R2_PSD2_PSD3_X2XX", "PSD1_PSD3_XX", "TPC_R2_PSD1_PSD2_X2XX", "RES_PSD3_X_4", "<Q^{PSD1}_{x}  Q^{PSD3}_{x}> <Q^{TPC}_{2,x} Q^{PSD2}_{x}  Q^{PSD3}_{x}> / < Q^{TPC}_{2,x} Q^{PSD1}_{x}  Q^{PSD2}_{x}>" }, //PSD3 X

    { "PSD1_PSD3_YY", "PSD2_PSD3_YY", "PSD1_PSD2_YY", "RES_PSD3_Y_0", "<Q^{PSD1}_{y}  Q^{PSD3}_{y}> <Q^{PSD2}_{y}  Q^{PSD3}_{y}> / <Q^{PSD1}_{y}  Q^{PSD2}_{y}>"},  //PSD3 Y
    { "TPC_R2_PSD1_PSD3_X2YY", "PSD2_PSD3_YY", "TPC_R2_PSD1_PSD2_X2YY", "RES_PSD3_Y_1", "<Q^{PSD2}_{y}  Q^{PSD3}_{y}> <Q^{TPC}_{2,x} Q^{PSD1}_{y}  Q^{PSD3}_{y}> / < Q^{TPC}_{2,x} Q^{PSD1}_{y}  Q^{PSD2}_{y}>" }, //PSD3 Y
    { "TPC_R2_PSD2_PSD3_X2YY", "PSD1_PSD3_YY", "TPC_R2_PSD1_PSD2_X2YY", "RES_PSD3_Y_2", "<Q^{PSD1}_{y}  Q^{PSD3}_{y}> <Q^{TPC}_{2,x} Q^{PSD2}_{y}  Q^{PSD3}_{y}> / < Q^{TPC}_{2,x} Q^{PSD1}_{y}  Q^{PSD2}_{y}>" }, //PSD3 Y
    { "TPC_R2_PSD1_PSD3_Y2XY", "PSD2_PSD3_YY", "TPC_R2_PSD1_PSD2_Y2XY", "RES_PSD3_Y_3", "<Q^{PSD2}_{y}  Q^{PSD3}_{y}> <Q^{TPC}_{2,y} Q^{PSD1}_{x}  Q^{PSD3}_{y}> / < Q^{TPC}_{2,y} Q^{PSD1}_{x}  Q^{PSD2}_{y}>"},  //PSD3 Y
    { "TPC_R2_PSD2_PSD3_Y2XY", "PSD1_PSD3_YY", "TPC_R2_PSD1_PSD2_Y2YX", "RES_PSD3_Y_4", "<Q^{PSD1}_{y}  Q^{PSD3}_{y}> <Q^{TPC}_{2,y} Q^{PSD2}_{x}  Q^{PSD3}_{y}> / < Q^{TPC}_{2,y} Q^{PSD2}_{x}  Q^{PSD1}_{y}>"}  //PSD3 Y
};

void plot_axis(const float xmin, const float xmax, const float ymin, const float ymax,
               const TString title="", const TString xtitle="", const TString ytitle="")
{
    TH1F *h = new TH1F("h" + title, "", 1, xmin, xmax);
    h->SetTitle(title);

    h->GetYaxis()->SetTitle(ytitle);
    h->GetXaxis()->SetTitle(xtitle);

    h->GetYaxis()->SetRangeUser(ymin, ymax);
    h->GetXaxis()->SetRangeUser(xmin, xmax);
    h->Draw("");
}

// // // // // // // // // // // // // // // // // // // // // // // // //
// // // // // // // // // // // // // // // // // // // // // // // // //
// // // // // // // //  Experimental data
// // // // // // // // // // // // // // // // // // // // // // // // //
// // // // // // // // // // // // // // // // // // // // // // // // //

const float *NA49_pi_v1_bins[2][3] = {
    { NA49_v1_centr_pi_40_pT_bins, NA49_v1_mcentr_pi_40_pT_bins, NA49_v1_per_pi_40_pT_bins },
    { NA49_v1_centr_pi_40_Y_bins, NA49_v1_mcentr_pi_40_Y_bins, NA49_v1_per_pi_40_Y_bins}     };

const float *NA49_pi_v1_value[2][3] = {
    { NA49_v1_centr_pi_40_pT_value, NA49_v1_mcentr_pi_40_pT_value, NA49_v1_per_pi_40_pT_value } ,
    { NA49_v1_centr_pi_40_Y_value, NA49_v1_mcentr_pi_40_Y_value, NA49_v1_per_pi_40_Y_value}   };

const float *NA49_pi_v1_error[2][3] = {
    { NA49_v1_centr_pi_40_pT_error, NA49_v1_mcentr_pi_40_pT_error, NA49_v1_per_pi_40_pT_error } ,
    { NA49_v1_centr_pi_40_Y_error, NA49_v1_mcentr_pi_40_Y_error, NA49_v1_per_pi_40_Y_error}  };

const float *NA49_p_v1_bins[2][3] = {
    { NA49_v1_centr_p_40_pT_bins, NA49_v1_mcentr_p_40_pT_bins, NA49_v1_per_p_40_pT_bins },
    { NA49_v1_centr_p_40_Y_bins, NA49_v1_mcentr_p_40_Y_bins, NA49_v1_per_p_40_Y_bins}     };

const float *NA49_p_v1_value[2][3] = {
    { NA49_v1_centr_p_40_pT_value, NA49_v1_mcentr_p_40_pT_value, NA49_v1_per_p_40_pT_value } ,
    { NA49_v1_centr_p_40_Y_value, NA49_v1_mcentr_p_40_Y_value, NA49_v1_per_p_40_Y_value}   };

const float *NA49_p_v1_error[2][3] = {
    { NA49_v1_centr_p_40_pT_error, NA49_v1_mcentr_p_40_pT_error, NA49_v1_per_p_40_pT_error } ,
    { NA49_v1_centr_p_40_Y_error, NA49_v1_mcentr_p_40_Y_error, NA49_v1_per_p_40_Y_error}  };


const float *NA49_pi_v2_bins[2][3] = {
    { NA49_v2_centr_pi_40_pT_bins, NA49_v2_mcentr_pi_40_pT_bins, NA49_v2_per_pi_40_pT_bins },
    { NA49_v2_centr_pi_40_Y_bins, NA49_v2_mcentr_pi_40_Y_bins, NA49_v2_per_pi_40_Y_bins}     };

const float *NA49_pi_v2_value[2][3] = {
    { NA49_v2_centr_pi_40_pT_value, NA49_v2_mcentr_pi_40_pT_value, NA49_v2_per_pi_40_pT_value } ,
    { NA49_v2_centr_pi_40_Y_value, NA49_v2_mcentr_pi_40_Y_value, NA49_v2_per_pi_40_Y_value}   };

const float *NA49_pi_v2_error[2][3] = {
    { NA49_v2_centr_pi_40_pT_error, NA49_v2_mcentr_pi_40_pT_error, NA49_v2_per_pi_40_pT_error } ,
    { NA49_v2_centr_pi_40_Y_error, NA49_v2_mcentr_pi_40_Y_error, NA49_v2_per_pi_40_Y_error}  };

const float *NA49_p_v2_bins[2][3] = {
    { NA49_v2_centr_p_40_pT_bins, NA49_v2_mcentr_p_40_pT_bins, NA49_v2_per_p_40_pT_bins },
    { NA49_v2_centr_p_40_Y_bins, NA49_v2_mcentr_p_40_Y_bins, NA49_v2_per_p_40_Y_bins}     };

const float *NA49_p_v2_value[2][3] = {
    { NA49_v2_centr_p_40_pT_value, NA49_v2_mcentr_p_40_pT_value, NA49_v2_per_p_40_pT_value } ,
    { NA49_v2_centr_p_40_Y_value, NA49_v2_mcentr_p_40_Y_value, NA49_v2_per_p_40_Y_value}   };

const float *NA49_p_v2_error[2][3] = {
    { NA49_v2_centr_p_40_pT_error, NA49_v2_mcentr_p_40_pT_error, NA49_v2_per_p_40_pT_error } ,
    { NA49_v2_centr_p_40_Y_error, NA49_v2_mcentr_p_40_Y_error, NA49_v2_per_p_40_Y_error}  };

const int nBins_pi[2][3] = {
	{13, 12, 11} ,
	{9, 9, 9}	};
	
const int nBins_p[2][3] = {
	{5, 5, 5},
	{9, 9, 9}	};



void plot_star_pion_v1(Int_t iVar, Int_t iCentr, TLegend &legend)
{
    if (iVar == 0) return; //pT
    TGraphErrors *STAR_v1  = new TGraphErrors(10, STAR_v1_pi_Y_bin, STAR_v1_c10_40_pi_7_7_Y_values, 0, STAR_v1_c10_40_pi_7_7_Y_error);

    STAR_v1 -> SetLineWidth(0);

    STAR_v1->SetFillStyle(3014);
    STAR_v1->SetFillColorAlpha(kMagenta-3, 0.5);

    legend.AddEntry(STAR_v1,"STAR","f");

    STAR_v1 -> Draw("PLsameE3");
}


void plot_na49_pion_v1(Int_t iVar, Int_t iCentr, TLegend &legend)
{

    TGraphErrors *NA49_v1  = new TGraphErrors(nBins_pi [iVar][iCentr], NA49_pi_v1_bins[iVar][iCentr], NA49_pi_v1_value[iVar][iCentr], 0, NA49_pi_v1_error[iVar][iCentr]);
    NA49_v1 -> SetLineWidth(0);

    NA49_v1->SetFillStyle(3014);
    NA49_v1->SetFillColor(kGreen-2);

//    legend.AddEntry(NA49_v1,"NA49","f");

    NA49_v1 -> Draw("samePLE3");

}

void plot_na49_pion_v2(Int_t iVar, Int_t iCentr, TLegend &legend)
{

    TGraphErrors *NA49_v2  = new TGraphErrors(nBins_pi [iVar][iCentr], NA49_pi_v2_bins[iVar][iCentr], NA49_pi_v2_value[iVar][iCentr], 0, NA49_pi_v2_error[iVar][iCentr]);
    NA49_v2 -> SetLineWidth(0);

    NA49_v2->SetFillStyle(3014);
    NA49_v2->SetFillColor(kGreen-2);

    legend.AddEntry(NA49_v2,"NA49","f");

    NA49_v2 -> Draw("samePLE3");
}
