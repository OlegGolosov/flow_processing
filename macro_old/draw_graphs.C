bool saveMacro = 0;
bool savePdf = 1;
bool savePng = 0;
TString fCompName = "v1_pi_neg.root";
//vector <TString> subevents = {"TPC_pt", "TPC_y"};
vector <TString> subevents = {"TPC_u"};
//vector <TString> qa = {"h2ypt_", "h2phipt_", "h2phiy_"};
vector <TString> qa = {"h2phiy_"};
vector <TString> QQ = {"PSD1_PSD2_SP", "PSD1_PSD3_SP", "PSD2_PSD3_SP",};
vector <TString> res = {"R1_PSD1_3S_EP_", "R1_PSD2_3S_EP_",  "R1_PSD3_3S_EP_"};
vector <TString> resTitle = {"R_{1} 3-sub: VCAL [RCAL1, RCAL2]", "R_{1} 3-sub: RCAL1 [VCAL, RCAL2]",  "R_{1} 3-sub: RCAL2 [VCAL, RCAL1]"};
vector <TString> resComp = {"x-component", "y-component", "full", "x-component", "y-component", "full"};
vector <TString> flowComp = {"x", "y", "x&y", "x", "y", "x&y"};
float graphShift [4] = {0.008, 0., -0.004, 0.004};
float resLim [4] = {0, 60., 0., 0.59};
float QQLim [4] = {0, 60., -0.0003, 0.00449};
//vector <TString> v1 = {"V_1_x_%s", "V_1_y_%s", "V_1_x+y_%s", "V1_%s_3S_PSD1", "V1_%s_3S_PSD2"};
//vector <TString> v1 = {"V1_%s_3S_PSD1", "V1_%s_3S_PSD2"};
vector <TString> v1 = {"V1_%s_3S_PSD1"};
//vector <TString> v1_all = {"V1_%s_3S_PSD1_x_%s", "V1_%s_3S_PSD1_y_%s", "V1_%s_3S_PSD1_x+y_%s",
//													 "V1_%s_3S_PSD2_x_%s", "V1_%s_3S_PSD2_y_%s", "V1_%s_3S_PSD2_x+y_%s"};
vector <TString> v1_all = {"V1_%s_3S_PSD1_x_%s", "V1_%s_3S_PSD1_y_%s", "V1_%s_3S_PSD1_x+y_%s"};
//vector <TString> compNames = {"v1_0%s_Combined_X_%d", "v1_0%s_Combined_Y_%d", "null"};
vector <TString> compNames = {"v1_0%s_PSD1_X_%d", "v1_0%s_PSD1_Y_%d", "null"};
vector <TString> compAxes = {"Pt", "Rapidity"};

TString v1_pub = "v1_%s_pub_%s";

TString particle = "#pi^{-}";
float v1Lim [2][4] = {{0., 1.69, -0.035, 0.079},
											{0., 1.4, -0.06, 0.01}}; // pions
float v1LimComp [2][4] = {{0., 1.69, -0.035, 0.059},
													{0., 1.4, -0.039, 0.009}}; // pions
float sysPos [2][2] = {{0.65, 0.9},
											 {0.65, 0.9}}; // pions
//float sysPosComp [2][2] = {{0.45, 0.93}, 
//													 {0.65, 0.9}}; // for victor
float sysPosComp [2][2] = {{0.5, 0.9},
													 {0.65, 0.9}}; // pions
float resLegPos [3][4] = {{0.35, 0.15, 0.75, 0.35},
													{0.3, 0.15, 0.7, 0.35},
													{0.3, 0.15, 0.7, 0.35}};
float legPos [2][4] = {{0.15, 0.55, 0.45, 0.8},
											 {0.15, 0.17, 0.45, 0.42}};
//float legPosComp [2][4] = {{0.15, 0.7, 0.45, 0.8}, 
//													 {0.15, 0.18, 0.45, 0.28}};// for victor
float legPosComp [2][4] = {{0.15, 0.65, 0.45, 0.8},
													 {0.15, 0.22, 0.45, 0.32}};
float partPos [2][2] = {{0.15, 0.9},
											 {0.15, 0.9}};
//float partPosComp [2][2] = {{0.15, 0.93},
//													 {0.15, 0.9}}; // for victor
float partPosComp [2][2] = {{0.15, 0.9},
													 {0.15, 0.9}};
float statPosComp [2][2] = {{0.75, 0.15},
													 {0.15, 0.15}};
													 
													 
//TString particle = "p";
//float v1Lim [2][4] = {{0., 2., -0.06, 0.2},
//											{0., 2., -0.02, 0.2}}; // protons

//int imin = 0; // show published
int imin = 1; // do not show published
int centmax = 2;
TString centralities [3] = {"central", "midcentral", "peripheral"};
TString centRange [3] = {"0-12.5%", "12.5-33.5%", "33.5-60%"};
//TString centRange [3] = {"0 12.5 33.5 60"};
TString xAxes [2] = {"pt", "y"};
TString xAxesTitles [2] = {"p_{T} (GeV/#it{c})", "#it{y}"};
TString integration [2] = {"0 < #it{y} < 1.4", "0 < p_{T} < 2.0 GeV/#it{c}"};
TString steps [2] = {"rec", "twr"};
TString stepNames [2] = {"recentring", "twist and rescale"};
TString system_ = "Pb+Pb @ 40#it{A} GeV";
const int colors[6] = {kBlue, kRed, kGreen+3, kMagenta+2, kBlack, kOrange};
const int colors2[6] = {kMagenta+2, kBlue, kRed, kGreen+3, kBlack, kOrange};
const int markers[6] = { 20, 21, 22, 26, 27, 1};
const int markers2[6] = {kFullCircle, kFullCircle, kOpenSquare, kOpenTriangleUp, 26, 27};
const int markers3[6] = {kFullCircle, kOpenSquare, kOpenSquare, kOpenTriangleUp, 26, 27};
TString drawOptions = "P Z";
float margins [4] = {0.1, 0.02, 0.1, 0.02};

											
TGraphErrors* ShiftGraph (TGraphErrors *g, float shift);

//void draw_graphs (TString inFolder = "/home/ogolosov/Desktop/analysis/NA49_flow/lowY_eff/piminus/centr+mb/")
void draw_graphs (TString inFolder = "./")
{
	TString outFolder = inFolder;
//	TString outFolder = inFolder + "/pdf";
//	gSystem -> Exec ("mkdir -p " + outFolder);
	TString objectName, xaxis, yaxis, printName;
	TGraphErrors *g [5], *g_pub [3];
	TMultiGraph *mg, *mg_all, *mg_all_pub;
	TH2F *h2;
	gStyle -> SetOptTitle (0);
	gStyle -> SetLegendBorderSize (0);
	gStyle -> SetLegendTextSize (0.035);
	gStyle -> SetLabelSize (0.04, "X");
	gStyle -> SetLabelSize (0.04, "Y");
	gStyle -> SetTitleFont (42, "X");
	gStyle -> SetTitleFont (42, "Y");
	gStyle -> SetTitleXSize (0.05);
	gStyle -> SetTitleYSize (0.05);
	gStyle -> SetTitleXOffset (0.9);
	gStyle -> SetTitleYOffset (0.9);
	gStyle -> SetHatchesSpacing(0);
	TCanvas *c = new TCanvas ("c","c", 800, 600);
	TLegend *l, *l2;
	TList *glist;
	int counter = 1;
	TLatex *text = new TLatex();
	text-> SetNDC();
	text -> SetTextSize(0.035);
	text -> DrawLatex(0.6, 0.8, system_);
	gPad->Print(outFolder + "/output.pdf(","pdf");

	TH1F *hAxis = new TH1F("axis","",10000,-100.,100.);
	hAxis->SetStats (0);
	hAxis->SetLineColor (kWhite);
	hAxis -> GetXaxis () -> SetNdivisions(505);
	hAxis -> GetYaxis () -> SetNdivisions(505);
	TF1 *zeroLine = new TF1 ("zeroLine", "0", -10.,10.);
	TF1 *fitV1 = new TF1 ("fitV1", "[0] + [1] * x + [2] * x * x + [3] * x**3 + [4] * x**4 + [5] * x**5 ", -10.,10.);
	zeroLine -> SetLineWidth (1);
	zeroLine -> SetLineStyle (2);
	zeroLine -> SetLineColor (kBlack);
	
	TFile *fIn = new TFile (inFolder + "/qa.root", "read");
	TFile *fComp = new TFile (fCompName, "read");
	gROOT -> ForceStyle();
	
	for (int sub = 0; sub < subevents.size (); sub++)
	{ 
		for (int iqa = 0; iqa < qa.size (); iqa++) 
		{
			h2 = (TH2F*) fIn -> Get (subevents [sub] + "/" + qa [iqa] + subevents [sub]);
			h2 -> Draw ("colz");
			h2 -> GetXaxis () -> SetTitle ("#varphi (rad)"); // patch - change in qa code!!!
			h2 -> GetYaxis () -> SetRangeUser (-2., 3.6); // patch
			h2 -> GetXaxis () -> SetNdivisions(505);
			h2 -> GetYaxis () -> SetNdivisions(505);
			h2->SetStats(0);
			gPad -> SetLogz ();
			gPad -> SetTopMargin (margins [3]);
			text -> DrawLatex(0.65, 0.8, "NA49 performance");
			text -> DrawLatex(0.65, 0.75, system_);
			text -> DrawLatex(0.15, 0.8, Form ("#scale[1.5]{%s}", particle.Data ()));
			text -> DrawLatex(0.15, 0.75, "0 < p_{T} < 3 GeV/#it{c}");
			gPad->Print(outFolder + "/output.pdf","pdf");
			printName = outFolder + "/" + qa [iqa] + subevents [sub];
			if (saveMacro) gPad -> SaveAs (printName + ".C");
			if (savePdf) gPad -> SaveAs (printName + ".pdf");
			if (savePng) gPad -> SaveAs (printName + ".png");
		}
	}
	
	for (int step = 1; step < 2; step++) {
		fIn = new TFile (inFolder + Form ("/graph_%d.root", step + 1), "read"); 
		if (step == 1) // correlations and resolution
		{
			for (int i = 0; i < QQ.size (); i++) 
			{
				hAxis -> GetYaxis () -> SetTitle ("QQ");
				hAxis -> GetXaxis () -> SetTitle ("VCAL centrality (%)");
				hAxis -> Draw();
				hAxis -> GetXaxis() -> SetRangeUser (QQLim[0], QQLim[1]);
				hAxis -> GetYaxis() -> SetRangeUser (QQLim[2], QQLim[3]);
				mg = (TMultiGraph*) fIn -> Get ("QQ/" + QQ [i]);
				mg -> Draw ("P");
				gPad -> SetMargin (margins [0], margins [1], margins [2], margins [3]);
				l = new TLegend (0.66, 0.62, 0.88, 0.87);
				glist = mg -> GetListOfGraphs ();
				for (int i = 0; i < glist -> GetSize (); i++) 
				{		
					g [i] = (TGraphErrors*) glist -> At(i);
					
						// patch
						int n = g [i] -> GetN ();
						double x [7] = {0., 5., 12.5, 23.5, 33.5, 43.5, 60.};
						double *y = g [i] -> GetY ();
						for (int j = 0; j < n; j++) g [i] -> SetPoint (j, x [j] + 0.5 * (x [j + 1] - x [j]), y [j]);
						//patch
						
					l -> AddEntry (g [i] -> GetName (), g [i] -> GetTitle (), "p");
				}
				l -> Draw ();
				text -> DrawLatex(0.05 + margins [0], 0.8 + margins [3], "NA49 performance");
				text -> DrawLatex(0.05 + margins [0], 0.75 + margins [3], system_);
				gPad->Print(outFolder + "/output.pdf","pdf");
				printName = outFolder + "/" + QQ [i];
				if (savePng)	gPad -> SaveAs (printName + ".png");
				if (savePdf) gPad -> SaveAs (printName + ".pdf");
				if (saveMacro) gPad -> SaveAs (printName + ".C");
			}
			for (int ires = 0; ires < res.size (); ires++) 
			{
				hAxis -> GetYaxis () -> SetTitle ("R_{1}");
				hAxis -> GetXaxis () -> SetTitle ("VCAL centrality (%)");
				hAxis -> Draw();
				hAxis -> GetXaxis() -> SetRangeUser (resLim[0], resLim[1]);
				hAxis -> GetYaxis() -> SetRangeUser (resLim[2], resLim[3]);
				mg = (TMultiGraph*) fIn -> Get ("resolution/" + res [ires]);
				l = new TLegend (resLegPos [ires] [0], resLegPos [ires] [1], resLegPos [ires] [2], resLegPos [ires] [3]);
				text -> DrawLatex (resLegPos [ires] [0], resLegPos [ires] [3] + 0.03, Form ("#font[42]{%s}", resTitle [ires].Data ()));
				l -> SetHeader ("Component");
				mg -> Draw ("P");
				glist = mg -> GetListOfGraphs ();
				for (int i = 0; i < glist -> GetSize (); i++) 
				{		
					g [i] = (TGraphErrors*) glist -> At(i);
					g [i] -> SetLineWidth (2);
					
					// patch
					int n = g [i] -> GetN ();
					double x [7] = {0., 5., 12.5, 23.5, 33.5, 43.5, 60.};
					double *y = g [i] -> GetY ();
					for (int j = 0; j < n; j++) 
					{
						g [i] -> SetPoint (j, x [j] + 0.5 * (x [j + 1] - x [j]), y [j]);
					}
					//patch
					
					l -> AddEntry (g [i] -> GetName (), flowComp [i], "p");
				}
				l -> Draw ();
				text -> DrawLatex(0.7, 0.9, "NA49 performance");
				text -> DrawLatex(0.7, 0.85, system_);
				gPad->Print(outFolder + "/output.pdf","pdf");
				printName = outFolder + "/" + res [ires];
				if (savePdf) gPad -> SaveAs (printName + ".pdf");
				if (savePng) gPad -> SaveAs (printName + ".png");
				if (saveMacro) gPad -> SaveAs (printName + ".C");
			}
		}

		for (int axis = 0; axis < 2; axis++)
		{		
			// V1 different components
			gPad -> SetMargin (margins [0], margins [1], margins [2], margins [3]);
			for (int iname = 0; iname < v1.size (); iname++) 
			{
				for (int cent = 0; cent < 3; cent++) 
				{
					hAxis -> SetTitle (Form ("%s, %s", centralities [cent].Data(), stepNames [step].Data()));
					hAxis -> GetYaxis ()->SetTitle("V_{1}");
//					hAxis -> GetYaxis () -> SetTitleOffset(0.9);
					hAxis -> GetXaxis ()->SetTitle(xAxesTitles [axis]);
					hAxis -> GetXaxis () -> SetRangeUser (v1Lim[axis][0], v1Lim[axis][1]);
					hAxis -> GetYaxis () -> SetRangeUser (v1Lim[axis][2], v1Lim[axis][3]);
					hAxis -> Draw ();
					zeroLine -> Draw ("same");
					objectName = Form ("V1/%s/" + v1 [iname], centralities [cent].Data(), xAxes [axis].Data());
					cout << objectName << endl;
					mg = (TMultiGraph*) fIn -> Get (objectName);
					l = new TLegend (legPos[axis][0], legPos[axis][1], legPos[axis][2], legPos[axis][3]);
					l -> SetHeader ("Components", "");
					glist = mg -> GetListOfGraphs ();
					for (int i = imin; i < glist -> GetSize (); i++) 
					{		
						g [i] = (TGraphErrors*) glist -> At(i);
						g [i] -> SetLineWidth (2);
						g [i] -> SetMarkerStyle (markers2 [i]);
						g [i] -> SetMarkerColor (colors2 [i]);
						g [i] -> SetLineColor (colors2 [i]);
						ShiftGraph (g [i], graphShift [i]) -> Draw (drawOptions);
						l -> AddEntry (g [i] -> GetName (), Form ("%s", flowComp [i - 1].Data()), "p");
					}
					g [imin] -> Draw (drawOptions);
					fitV1->FixParameter (0, 0);
					fitV1->FixParameter (2, 0);
					fitV1->FixParameter (4, 0);
					g [imin] -> Fit (fitV1, "", "", 0., 1.7);
					TF1 *currentFit = (TF1 *) g [imin] -> GetListOfFunctions () -> FindObject (fitV1 -> GetName());
					currentFit -> SetLineColor (g [imin] -> GetLineColor());
					l -> AddEntry (g [imin] -> GetName (), "polynomial fit", "l");
					l -> Draw();				
					text -> DrawLatex(sysPos[axis][0], sysPos[axis][1], "NA49 performance");
					text -> DrawLatex(sysPos[axis][0], sysPos[axis][1] - 0.05, system_);
					text -> DrawLatex(sysPos[axis][0], sysPos[axis][1] - 0.1, centRange [cent] + " (VCAL)");
					text -> DrawLatex(partPos [axis][0], partPos [axis][1], Form ("#scale[1.5]{%s}   %s", particle.Data (), integration [axis].Data ()));  
					text -> SetTextFont(42);
					text -> DrawLatex (legPos[axis][0], legPos[axis][3] + 0.01, "v_{1} {#Psi_{proj} ; SP} VCAL");
					text -> SetTextFont(62);
					gPad -> Print(outFolder + "/output.pdf","pdf");	
					
					printName = outFolder + "/" + Form (v1 [iname], xAxes [axis].Data()) + "_" + steps [step] + "_" + centralities [cent];
					if (savePdf) gPad -> SaveAs (printName + ".pdf");
					if (savePng) gPad -> SaveAs (printName + ".png");
					if (saveMacro) gPad -> SaveAs (printName + ".C");

				}
			}
			
			// NA49 comparison
			for (int iname = 0; iname < v1_all.size (); iname++) 
			{	
				hAxis -> SetTitle (Form ("%s", stepNames [step].Data()));
				hAxis -> GetXaxis () -> SetTitle(xAxesTitles [axis]);
				hAxis -> GetYaxis () -> SetTitle("V_{1}");
//				hAxis -> GetYaxis () -> SetTitleOffset(0.85);
				hAxis -> GetXaxis () -> SetRangeUser (v1LimComp[axis][0], v1LimComp[axis][1]);
				hAxis -> GetYaxis () -> SetRangeUser (v1LimComp[axis][2], v1LimComp[axis][3]);
				hAxis -> Draw ();  
				l = new TLegend (legPosComp[axis][0], legPosComp[axis][1], legPosComp[axis][2], legPosComp[axis][3]);    
				l -> SetNColumns(centmax);
				l2 = new TLegend (legPosComp[axis][0] - 0.035, legPosComp[axis][3], legPosComp[axis][0] + 0.16, legPosComp[axis][3] + 0.05);
				l2 -> SetNColumns(centmax);
				for (int cent = 0; cent < centmax; cent++) 
				{
					objectName = Form ("V1/" + v1_pub, xAxes [axis].Data(), centralities [cent].Data());
					cout << objectName << endl;
					g_pub [cent] = (TGraphErrors*) fIn -> Get (objectName);
					g_pub [cent] -> SetFillColorAlpha(colors [cent] - 7, 0.02);   
					g_pub [cent] -> SetFillStyle(3144);
					g_pub [cent] -> SetLineColorAlpha(colors [cent] - 7, 0.2);  
					g_pub [cent] -> SetLineWidth (14);  
					g_pub [cent] -> Draw ("3");
				}
					
				zeroLine -> Draw ("same");
				for (int cent = 0; cent < centmax; cent++) 
				{
					objectName = Form ("V1/" + v1_all [iname], xAxes [axis].Data(), centralities [cent].Data());
					cout << objectName << endl;
					g [cent] = (TGraphErrors*) fIn -> Get (objectName);
					g [cent] -> SetLineColor (colors [cent]);   
					g [cent] -> SetMarkerColor (colors [cent]);   
					g [cent] -> SetMarkerStyle(markers3 [cent]);
					g [cent] -> SetMarkerSize(1.5);  
					g [cent] -> SetLineWidth (2);  
					g [cent] -> Draw (drawOptions);
				}
				
				for (int cent = 0; cent < centmax; cent++) 
				{
					if (cent == centmax - 1) l -> AddEntry (g [cent] -> GetName (), Form ("   v_{1} {#Psi_{proj} ; SP_{%s}} VCAL 3-sub", flowComp[iname].Data()), "p");
					else l -> AddEntry (g [cent] -> GetName (), "                  ", "p");
				}
				
				for (int cent = 0; cent < centmax; cent++)
				{
					if (cent == centmax - 1) l -> AddEntry (g_pub [cent] -> GetName (), "   v_{1} {#Psi_{pp} ; EP} TPC RNDM-sub", "l");
					else l -> AddEntry (g_pub [cent] -> GetName (), "                  ", "l");
				}
				l -> AddEntry ("", "", "");
				l -> AddEntry ("", "   (Phys.Rev. C68 (2003) 034903)", "");
				
				l -> SetTextSize (0.03);
				l2 -> SetTextSize (0.03);
				for (int cent = 0; cent < centmax; cent++)
				{
					l2 -> AddEntry ("", Form ("#color[%d]{%s}", colors [cent], centRange [cent].Data()), "");
				}
				
				text -> DrawLatex(sysPosComp[axis][0], sysPosComp[axis][1], "NA49 preliminary");
				text -> DrawLatex(sysPosComp[axis][0], sysPosComp[axis][1] - 0.05, system_);
				text -> DrawLatex(sysPosComp[axis][0], sysPosComp[axis][1] - 0.1, "Centrality: VCAL"); 
				text -> DrawLatex(partPosComp [axis][0], partPosComp [axis][1], Form ("#scale[1.5]{%s}   %s", particle.Data (), integration [axis].Data ())); 
				text -> DrawLatex(statPosComp [axis][0], statPosComp [axis][1], "Stat. errors only"); 
				l -> Draw ();
				l2 -> Draw ();
//				gPad -> BuildLegend (legPos[axis][0], legPos[axis][1], legPos[axis][2], legPos[axis][3]);
				
				printName = outFolder + "/" + Form (v1_all [iname], xAxes [axis].Data(), "all_49") + "_" + steps [step];
				if (savePdf) gPad -> SaveAs (printName + ".pdf");
				if (savePng) gPad -> SaveAs (printName + ".png");
				if (saveMacro) gPad -> SaveAs (printName + ".C");
				gPad -> Print(outFolder + "/output.pdf","pdf");
			}
			
			
			// NA61 comparison
			float shift = 0.0;
			for (int iname = 0; iname < v1_all.size (); iname++) 
			{	
				if (compNames [iname] == "null") continue;
				hAxis -> SetTitle (Form ("%s", stepNames [step].Data()));
				hAxis -> GetXaxis () -> SetTitle(xAxesTitles [axis]);
				hAxis -> GetYaxis () -> SetTitle("v_{1} {#Psi_{proj}}");
				hAxis -> GetXaxis () -> SetRangeUser (v1LimComp[axis][0], v1LimComp[axis][1]);
				hAxis -> GetYaxis () -> SetRangeUser (v1LimComp[axis][2], v1LimComp[axis][3]);
				hAxis -> Draw ();
				
				l = new TLegend (legPosComp[axis][0], legPosComp[axis][1], legPosComp[axis][2] + 0.05, legPosComp[axis][3]);    
				l -> SetNColumns(centmax);
				l2 = new TLegend (legPosComp[axis][0] - 0.035, legPosComp[axis][3], legPosComp[axis][0] + 0.2, legPosComp[axis][3] + 0.05);
				l2 -> SetNColumns(centmax);
				for (int cent = 0; cent < centmax; cent++) 
				{
//					objectName = Form ("V1/" + v1_all [iname], xAxes [axis].Data(), centralities [cent].Data());  // for victor
//					cout << objectName << endl;  // for victor
//					g_pub [cent] = (TGraphErrors*) fIn -> Get (objectName); // for victor
					objectName = Form (compNames [iname], compAxes [axis].Data(), cent);
					cout << objectName << endl;
					g_pub [cent] = (TGraphErrors*) fComp -> Get (objectName);
					g_pub [cent] -> SetName (objectName);
					g_pub [cent] -> SetFillColorAlpha(colors [cent] - 7, 0.02);   
					g_pub [cent] -> SetFillStyle(3144);
					g_pub [cent] -> SetLineColorAlpha(colors [cent] - 7, 0.2);  
					g_pub [cent] -> SetLineWidth (14);  
					g_pub [cent] -> Draw ("3");
				}
					
				zeroLine -> Draw ("same");
				for (int cent = 0; cent < centmax; cent++) 
				{
//					objectName = Form (compNames [iname], compAxes [axis].Data(), cent); // for victor
//					cout << objectName << endl; // for victor
//					g [cent] = (TGraphErrors*) fComp -> Get (objectName); // for victor
					objectName = Form ("V1/" + v1_all [iname], xAxes [axis].Data(), centralities [cent].Data());
					cout << objectName << endl;
					g [cent] = (TGraphErrors*) fIn -> Get (objectName);
					g [cent] -> SetName (objectName);
					g [cent] -> SetLineColor (colors [cent]);   
					g [cent] -> SetMarkerColor (colors [cent]);   
					g [cent] -> SetMarkerStyle(markers3 [cent]);
					g [cent] -> SetMarkerSize(1.5);  
					g [cent] -> SetLineWidth (2);  
					g [cent] -> Draw (drawOptions);
				}
				
				for (int cent = 0; cent < centmax; cent++) 
				{
//					if (cent == centmax - 1) l -> AddEntry (g [cent] -> GetName (), "   NA61/SHINE Pb+Pb @ 30 #it{A} GeV/#it{c}, central PSD", "p"); // for victor
//					else l -> AddEntry (g [cent] -> GetName (), "                  ", "p"); // for victor
					if (cent == centmax - 1) l -> AddEntry (g [cent] -> GetName (), "   NA49 Pb+Pb @ 40 #it{A} GeV, VCAL", "p");
					else l -> AddEntry (g [cent] -> GetName (), "                  ", "p");
				}
				
				for (int cent = 0; cent < centmax; cent++)
				{
//					if (cent == centmax - 1) l -> AddEntry (g_pub [cent] -> GetName (), "   NA49 Pb+Pb @ 40 #it{A} GeV, VCAL", "l"); // for victor
//					else l -> AddEntry (g_pub [cent] -> GetName (), "                  ", "l"); // for victor
					if (cent == centmax - 1) l -> AddEntry (g_pub [cent] -> GetName (), "   NA61/SHINE Pb+Pb @ 30 #it{A} GeV/#it{c}, central PSD", "l");
					else l -> AddEntry (g_pub [cent] -> GetName (), "                  ", "l");
				}
				
				l -> SetTextSize (0.03);
				l2 -> SetTextSize (0.03);
				for (int cent = 0; cent < centmax; cent++)
				{
					if (cent == centmax - 1) l2 -> AddEntry ("", Form ("#color[%d]{%s}   SP_{%s} 3-sub", colors [cent], centRange [cent].Data(), flowComp [iname].Data ()), "");
					else l2 -> AddEntry ("", Form ("#color[%d]{%s}", colors [cent], centRange [cent].Data()), "");
				}
				
				text -> DrawLatex(sysPosComp[axis][0], sysPosComp[axis][1], "NA49 preliminary");
				text -> DrawLatex(sysPosComp[axis][0], sysPosComp[axis][1] - 0.05, "NA61/SHINE preliminary");
//				text -> DrawLatex(sysPosComp[axis][0], sysPosComp[axis][1], "NA61/SHINE preliminary"); // for victor
//				text -> DrawLatex(sysPosComp[axis][0], sysPosComp[axis][1] - 0.05, "NA49 preliminary"); // for victor
				text -> DrawLatex(partPosComp[axis][0], partPosComp[axis][1], Form ("#scale[1.5]{%s}   %s", particle.Data (), integration [axis].Data ())); 
				text -> DrawLatex(statPosComp [axis][0], statPosComp [axis][1], "Stat. errors only");
				l -> Draw ();
				l2 -> Draw ();
//				gPad -> BuildLegend (legPos[axis][0], legPos[axis][1], legPos[axis][2], legPos[axis][3]);
				
				printName = outFolder + "/" + Form (v1_all [iname], xAxes [axis].Data(), "all_61") + "_" + steps [step];
				if (savePdf) gPad -> SaveAs (printName + ".pdf");
				if (savePng) gPad -> SaveAs (printName + ".png");
				if (saveMacro) gPad -> SaveAs (printName + ".C");
				gPad -> Print(outFolder + "/output.pdf","pdf");
			}
		}
	}
	c -> Clear();
	text -> DrawLatex(0.6, 0.8, "The end!");
	gPad -> Print(outFolder + "/output.pdf)","pdf");
}

TGraphErrors* ShiftGraph (TGraphErrors *g, float shift) 
{
	TGraphErrors* shifted = (TGraphErrors*) g -> Clone ();
	shifted -> Sort ();
	int n = g -> GetN();
	double *x = g -> GetX ();
	double *y = g -> GetY ();
	for (int i = 0; i < n; i++) 
	{
		x [i] += shift * (x [n - 1] - x [0]);
		shifted -> SetPoint (i, x [i], y [i]);
	}
	return shifted;
}
