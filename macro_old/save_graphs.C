#include "graph_config.h"


void Save_QQ ();
void Save_QQQ ();
void Save_uQ ();
void Save_uQQ ();
void SaveResolution();
void SaveFlow(int harmonic);
//void SaveV1();
//void SaveV2();

TMultiGraph* GetV1(const TString corr, const TString res);
TMultiGraph* GetV2(const TString corr, const TString res1, const TString res2, const int iVar);

Qn::DataContainer<Qn::Stats>  GetResolution( const TString top1, const TString top2, const TString bot);

TFile *fIn {nullptr};
TFile *fOut {nullptr};
TFile *fTemp {nullptr};

void save_graphs(TString inputFileName = "../corr_3.root",
                   TString outputFileName = "../graph_3.root")
{		
		cout << inputFileName << endl;
		cout << outputFileName << endl;
		
    fIn = TFile::Open (inputFileName);
    fTemp = new TFile (outputFileName + "_", "recreate");
    fOut = new TFile (outputFileName, "recreate");

    Save_QQ ();
    Save_QQQ ();
    Save_uQ ();
    Save_uQQ ();
    SaveResolution ();
    SaveFlow(1);
//    SaveFlow(2);
	
    fTemp->Close();
    fOut->Close();
}

void Save_QQ ()
{
	TString xAxisTitle = "Centrality, %";
	TString comp [5] = {"_XX", "_YY", "_QQ", "_XY", "_YX"}; 
	TString comp1 [5] = {"x", "y", "x+y", "x", "y"}; 
	TString comp2 [5] = {"x", "y", "x+y", "y", "x"};  
  TString objectName;
	
	TGraphAsymmErrors *corr;
	vector <TGraphAsymmErrors*> graphs;
	Qn::DataContainer<Qn::Stats> *profile [5];
	profile [2] = new Qn::DataContainer<Qn::Stats>;
	TDirectory *QQ_dir = fOut->mkdir( "QQ" );
	QQ_dir->cd();

	for (ushort i = 0; i < QQ_names.size (); i++)
	{		
		TMultiGraph mg;
    objectName = QQ_names[i][0];
		mg.SetName (objectName);
		mg.SetTitle (Form (QQ_names[i][1] + ";", "", "") + xAxisTitle);
		cout << endl << QQ_names[i][0];
		for (int j = 0; j < 5; j++) 
		{
			if (j != 2) fIn -> GetObject(objectName + comp [j], profile [j]);
			else if (profile [0] && profile [1]) *profile [2] = (*profile [0] + *profile [1]) * 2.;
			if (profile [j]) cout << "\t" << comp [j];
			else continue;
			corr = Qn::DataContainerHelper::ToTGraph(*profile [j]);
			corr -> SetName (QQ_names[i][0] + comp [j].Data());
			corr -> SetTitle (Form (QQ_names[i][1], comp1 [j].Data(), comp2 [j].Data()));
			corr -> GetXaxis () -> SetTitle (xAxisTitle);
			corr -> SetMarkerStyle (markers [0][j]);
      //    fOut->Write();
			corr -> SetMarkerColor (colors [j]);
			corr -> SetMarkerSize (1.5);
			corr -> SetLineColor (colors [j]);
			graphs.push_back ((TGraphAsymmErrors*) corr -> Clone ());
			mg.Add (corr);
		}
		mg.Write ();
	}
	cout << endl;
	
  TDirectory *allDir = QQ_dir->mkdir("all");
  allDir -> cd ();
	for (ushort i = 0; i < graphs.size (); i++)
	{
		graphs [i] -> Write ();
	}
}


void Save_QQQ ()
{
	TString xAxisTitle = "Centrality, %";
	TString COMP1 [2] = {"_X2", "_Y2"}; 
	TString COMP23 [5] = {"XX", "YY", "XY", "YX", "QQ"}; 
	TString comp1 [2] = {"x", "y"}; 
	TString comp2 [5] = {"x", "y", "x", "y", "x+y"}; 
	TString comp3 [5] = {"x", "y", "y", "x", "x+y"}; 
	
  TString objectName;
	TGraphAsymmErrors *corr;
	vector <TGraphAsymmErrors*> graphs;
	Qn::DataContainer<Qn::Stats> *profile [5];
	profile [4] = new Qn::DataContainer<Qn::Stats>;
	TDirectory *QQQ_dir = fOut->mkdir( "QQQ" );
	QQQ_dir->cd();
  
  for (ushort i = 0; i < QQQ_names.size (); i++)
  {		
    for (ushort k = 0; k < 2; k++)
    {
      TMultiGraph mg;
      objectName = QQQ_names[i][0] + COMP1 [k];
  		mg.SetName (objectName);
  		mg.SetTitle (Form (QQQ_names[i][1] + ";", comp1 [k].Data(), "", "") + xAxisTitle);
      cout << endl << objectName;
      for (int j = 0; j < 4; j++) // check! (4 or 5)
      {
        if (j < 4) fIn -> GetObject(objectName + COMP23 [j], profile [j]);
        else if (profile [0] && profile [1]) *profile [4] = (*profile [2] + *profile [3]) * 2.;
        if (profile [j]) cout << "\t" << COMP23 [j];
        else continue;
        corr = Qn::DataContainerHelper::ToTGraph(*profile [j]);
        corr -> SetName (objectName + COMP23 [j]);
        corr -> SetTitle (Form (QQQ_names[i][1], comp1 [k].Data (), comp2 [j].Data (), comp3 [j].Data ()));
        corr -> GetXaxis () -> SetTitle (xAxisTitle);
        corr -> SetMarkerStyle (markers [0][j]);
        //    fOut->Write();
        corr -> SetMarkerColor (colors [j]);
        corr -> SetMarkerSize (1.5);
        corr -> SetLineColor (colors [j]);
        graphs.push_back ((TGraphAsymmErrors*) corr -> Clone ());
        mg.Add (corr);
      }
      mg.Write ();
    }
  }
	cout << endl;
	
  TDirectory *allDir = QQQ_dir->mkdir("all");
  allDir -> cd ();
	for (ushort i = 0; i < graphs.size (); i++)
	{
		graphs [i] -> Write ();
	}
}


void Save_uQ ()
{
	vector <TString> xAxes = {"pT", "y"};
	vector <TString> xAxesTitles = {"p_{T} (GeV/#it{c})", "#it{y}"};
	TString comp [5] = {"_XX", "_YY", "_QQ", "_XY", "_YX"}; 
	TString comp1 [5] = {"x", "y", "x+y", "x", "y"}; 
	TString comp2 [5] = {"x", "y", "x+y", "y", "x"}; 
	TString centralities [3] = {"central", "midcentral", "peripheral"}; 
	TString objectName;
	TList *glist;
	vector <TGraphAsymmErrors*> graphs;
	vector <TString> profileNames;
	vector <Qn::DataContainer<Qn::Stats>*> profiles;
	TGraphAsymmErrors *g, *gc;
	TMultiGraph *mg, *mg2 [3][3], *mg3 [3][10];
  int marker, color;
	
	Qn::DataContainer<Qn::Stats> *profile [5];
	
	TDirectory *uQ_dir = fOut->mkdir("uQ");
	uQ_dir->cd();
  for (auto pid : pids)
  {    
    TDirectory *pid_dir = uQ_dir->mkdir(pid[0]);
    for (int axis = 0; axis < xAxes.size (); axis++)
    {
      TDirectory *axis_dir = pid_dir->mkdir(xAxes[axis]);
      axis_dir->cd();
      for (ushort i = 0; i < uQ_names.size (); i++)
      {		
        profile [2] = new Qn::DataContainer<Qn::Stats>;
        objectName = Form (uQ_names[i][0], pid[0].Data(), xAxes [axis].Data());
        cout << endl << objectName;
        for (int j = 0; j < 5; j++) 
        {
          if (j != 2) fIn -> GetObject(objectName + comp [j], profile [j]);
          else if (profile [0] && profile [1]) *profile [2] = (*profile [0] + *profile [1]) * 2.;
          if (profile [j]) cout << "\t" << comp [j];
          else continue;
          if (rebinCorr) *profile [j] = profile [j] -> Rebin( centralityAxis, [](Qn::Stats &a, const Qn::Stats &b) { return  a+b; } );
          profiles.push_back (profile [j]);
          profileNames.push_back (objectName + comp [j]);
          mg = Qn::DataContainerHelper::ToTMultiGraph(*profile [j], centralityAxis.Name());
          mg -> SetName (objectName + comp [j]);
          mg -> SetTitle (Form (uQ_names[i][1] + ";", comp1 [j].Data(), pid[1].Data(), comp2 [j].Data()) + xAxesTitles [axis]);
          glist = mg -> GetListOfGraphs ();
          for (int k = 0; k < glist -> GetSize (); k++) 
          {
            if (k < markers [0].size ()) 
            {
              marker = markers [0][k];
              color = colors [k];
            }
            else 
            {
              marker = markers [1][k - markers [0].size ()];
              color = colors [k - markers [0].size ()];
            }
            g = (TGraphAsymmErrors*) glist -> At(k);
            g -> GetXaxis () -> SetTitle (xAxesTitles [axis]);
            g -> SetMarkerStyle (marker);
            g -> SetMarkerColor (color);
            g -> SetLineColor (color);
            g -> SetMarkerSize (1.5);
            g -> SetName (objectName + comp [j] + Form ("_%i", k + 1));
            graphs.push_back (g);
          }
          mg -> Write();
        }
      }
    }
  }
  
  TDirectory *allDir = uQ_dir->mkdir("all");
  allDir -> cd ();
  for (ushort i = 0; i < graphs.size (); i++)
  {
    graphs [i] -> Write ();
  }
  
  fTemp -> cd ();
  for (ushort i = 0; i < profiles.size (); i++)
  {
    profiles [i] -> Write (profileNames [i]);
  }
}


void Save_uQQ ()
{
	vector <TString> xAxes = {"pT", "y"};
	vector <TString> xAxesTitles = {"p_{T} (GeV/#it{c})", "#it{y}"};
	TString COMP1 [2] = {"_X2", "_Y2"}; 
	TString COMP23 [5] = {"XX", "YY", "XY", "YX", "QQ"}; 
	TString comp1 [2] = {"x", "y"}; 
	TString comp2 [5] = {"x", "y", "x", "y", "x+y"}; 
	TString comp3 [5] = {"x", "y", "y", "x", "x+y"}; 
	TString objectName;
	TGraphAsymmErrors *g;
	TMultiGraph *mg;
	TList *glist;
	vector <TGraphAsymmErrors*> graphs;
	vector <TString> profileNames;
	vector <Qn::DataContainer<Qn::Stats>*> profiles;
  int marker, color;
	
	Qn::DataContainer<Qn::Stats> *profile [5];
	
	TDirectory *uQQ_dir = fOut->mkdir( "uQQ" );
	uQQ_dir->cd();
  for (auto pid : pids)
  {
    TDirectory *pid_dir = uQQ_dir->mkdir(pid[0]);
    for (int axis = 0; axis < xAxes.size (); axis++)
    {
      TDirectory *axis_dir = pid_dir->mkdir(xAxes[axis]);
      axis_dir->cd();    
      for (ushort i = 0; i < uQQ_names.size (); i++)
      {		
        for (ushort l = 0; l < 2; l++)
        {
          profile [4] = new Qn::DataContainer<Qn::Stats>;
          objectName = Form (uQQ_names[i][0], pid [0].Data(), xAxes [axis].Data()) + COMP1 [l];
          cout << endl << objectName;
          for (int j = 0; j < 4; j++) // check: 4 or 5
          {
            if (j < 4) fIn -> GetObject(objectName + COMP23 [j], profile [j]);
            else if (profile [0] && profile [1]) *profile [4] = (*profile [0] + *profile [1]) * 2.;
            if (profile [j]) cout << "\t" << COMP23 [j];
            else continue;
            profiles.push_back (profile [j]);
            profileNames.push_back (objectName + COMP23 [j]);
            if (rebinCorr) *profile [j] = profile [j] -> Rebin( centralityAxis, [](Qn::Stats &a, const Qn::Stats &b) { return  a+b; } );
            mg = Qn::DataContainerHelper::ToTMultiGraph(*profile [j], centralityAxis.Name());
            mg -> SetName (objectName + COMP23 [j]);
            mg -> SetTitle (Form (uQQ_names[i][1] + ";", comp1 [l].Data(), pid [1].Data(), comp2 [j].Data(), comp3 [j].Data()) + xAxesTitles [axis]);
            glist = mg -> GetListOfGraphs ();
            for (int k = 0; k < glist -> GetSize (); k++) 
            {
              if (k < markers [0].size ()) 
              {
                marker = markers [0][k];
                color = colors [k];
              }
              else 
              {
                marker = markers [1][k - markers [0].size ()];
                color = colors [k - markers [0].size ()];
              }
              g = (TGraphAsymmErrors*) glist -> At(k);
              g -> GetXaxis () -> SetTitle (xAxesTitles [axis].Data());
              g -> SetMarkerStyle (marker);
              g -> SetMarkerColor (color);
              g -> SetLineColor (color);
              g -> SetMarkerSize (1.5);
              g -> SetName (objectName + COMP23 [j]+ Form ("_%i", k + 1));
              graphs.push_back (g);
            }
            mg -> Write();
          }
        }
      }
    }
  }
  
  TDirectory *allDir = uQQ_dir->mkdir("all");
  allDir -> cd ();
	for (ushort i = 0; i < graphs.size (); i++)
	{
		graphs [i] -> Write ();
	}
	
	fTemp -> cd ();
	for (ushort i = 0; i < profiles.size (); i++)
	{
		profiles [i] -> Write (profileNames [i]);
	}
}

void SaveResolution ()
{
	TString xAxisTitle = "Centrality, %";
	TString comp [3] = {"XX", "YY", "QQ"}; 
	TString comp1 [3] = {"x", "y", "x+y"}; 
	TString comp2 [3] = {"x", "y", "x+y"}; 
	
	TGraphAsymmErrors *g;
	vector <TGraphAsymmErrors*> graphs;
	Qn::DataContainer<Qn::Stats> *profile [3][3], res;
	vector <Qn::DataContainer<Qn::Stats>> profiles;
	vector <TString> profileNames;
	TString resName;
	TString resTitle;
	
	profile [2][0] = new Qn::DataContainer<Qn::Stats>; // [comp][ab-ac-bc]
	profile [2][1] = new Qn::DataContainer<Qn::Stats>;
	profile [2][2] = new Qn::DataContainer<Qn::Stats>;
	
	TDirectory *res_dir = fOut->mkdir( "resolution" );
	res_dir->cd();

	for (ushort i = 0; i < res_names.size (); i++)
	{		
		TMultiGraph mg;
		cout << endl << res_names[i][0];
		for (int j = 0; j < 3; j++) 
		{
			if (j < 2) 
			{
				fIn -> GetObject(res_names[i][1]+ comp [j], profile [j][0]);
				fIn -> GetObject(res_names[i][2]+ comp [j], profile [j][1]);
				fIn -> GetObject(res_names[i][3]+ comp [j], profile [j][2]);
			}
			else if (profile [0][0] && profile [0][1] &&  profile [0][2] &&
							 profile [1][0] && profile [1][1] &&  profile [1][2]) 
			{
				*profile [2][0] = (*profile [0][0] + *profile [1][0]) * 2.;
				*profile [2][1] = (*profile [0][1] + *profile [1][1]) * 2.;
				*profile [2][2] = (*profile [0][2] + *profile [1][2]) * 2.;
			}
			if (profile [j][0] && profile [j][1] && profile [j][2]) 
			{
				cout << "\t" << comp [j];
				res = Sqrt((*profile [j][0]) * (*profile [j][1]) / (*profile [j][2]));
			}
			else continue;
			
			resName = res_names[i][0]+ comp [j];
			resTitle = Form (res_names[i][4], comp1 [j].Data(), comp1 [j].Data());
			
			g = Qn::DataContainerHelper::ToTGraph(res);
			g -> SetName (resName);
			g -> SetTitle (resTitle);
			g -> GetXaxis () -> SetTitle (xAxisTitle);
			g -> SetMarkerStyle (markers [0][j]);
			g -> SetMarkerColor (colors [j]);
			g -> SetMarkerSize (1.5);
			g -> SetLineColor (colors [j]);
			profiles.push_back (res);
			profileNames.push_back (resName);
			graphs.push_back ((TGraphAsymmErrors*) g -> Clone ());
			mg.Add (g);
		}
		mg.SetTitle(Form (res_names [i][4] + ";%s;R_{1}", "", xAxisTitle.Data()));
		mg.Write(res_names[i][0]);
	}
	cout << endl;
	
  TDirectory *allDir = res_dir->mkdir("all");
  allDir -> cd ();
	for (ushort i = 0; i < graphs.size (); i++)
	{
		graphs [i] -> Write ();
	}
	
	fTemp -> cd ();
	
	for (ushort i = 0; i < profiles.size (); i++)
	{
		profiles [i].Write (profileNames [i]);
	}
}


void SaveFlow (int harmonic)
{
	vector <TString> xAxes = {"pT", "y"};
	vector <TString> xAxesTitles = {"p_{T} (GeV/#it{c})", "#it{y}"};
	TString folderNames [2] = {"V1", "V2"};
	TString comp [3] = {"_XX", "_YY", "_QQ"}; 
	TString comp1 [3] = {"x", "y", "x+y"}; 
	vector <TString> sCentralities;
  int nCentClasses = centralityBins.size () - 1;
	
	const int (*pubB)[3];
	const float *(*pubX)[3];
	const float *(*pubY)[3];
	const float *(*pubE)[3];
	
	// pions
	pubB = nBins_pi;
	pubX = NA49_pi_v1_bins;
	pubY = NA49_pi_v1_value;
	pubE = NA49_pi_v1_error;
	// pions
		
	// protons
//	pubB = nBins_p;
//	pubX = NA49_p_v1_bins;
//	pubY = NA49_p_v1_value;
//	pubE = NA49_p_v1_error;
	// protons
	
	TGraphErrors* pub [3];
	
	vector <vector<TString>> flow_names;
	if (harmonic == 1) flow_names = V1_names; 
	if (harmonic == 2) flow_names = V2_names; 
	TString corrName, resName, flowName;
	TGraphAsymmErrors *g, *gc;
	TMultiGraph *mg; 
	TList *glist;
  int marker, color;
	vector <TGraphAsymmErrors*> graphs;
	Qn::DataContainer<Qn::Stats> *corr [3], *res [3], flow [3];
  corr [2] = new Qn::DataContainer<Qn::Stats>;
	
  for (int cent = 0; cent < nCentClasses; cent++)
    sCentralities.push_back(Form ("%.1f-%.1f", centralityAxis.GetLowerBinEdge(cent), centralityAxis.GetUpperBinEdge(cent)));
  
	TDirectory *flow_dir = fOut->mkdir( folderNames [harmonic - 1] );
  for (auto pid : pids) 
  {    
    TDirectory *pid_dir = flow_dir->mkdir (pid[0]);
      
    for (int axis = 0; axis < xAxes.size (); axis++)
    {
//      vector <TMultiGraph*> mg2 [3], mg3 [3];
      TDirectory *axis_dir = pid_dir -> mkdir (xAxes[axis]);
      axis_dir -> cd ();
//      for (int cent = 0; cent < nCentClasses; cent++) 
//      {
        /*
        pub [cent] = new TGraphErrors (pubB [axis][cent], pubX [axis][cent], pubY [axis][cent], 0, pubE [axis][cent]);
        pub [cent] -> SetTitle ("v_{1,x+y}{#psi_{p}^{PP}}");
        pub [cent] -> SetName (Form("v1_%s_pub_%s", xAxes [axis].c_str(), centralities [cent].Data()));
        pub [cent] -> SetLineColor (kBlack);
        pub [cent] -> SetMarkerColor (kBlack);
        pub [cent] -> SetMarkerStyle (28);
        pub [cent] -> SetMarkerSize (1.5);
        graphs.push_back(pub [cent]);
        */
        
//        for (int j = 0; j < 3; j++) {
//          mg2 [cent][j] = new TMultiGraph ();
//          mg2 [cent][j] -> SetTitle (Form ("V_{%d}^{%s} (%s) (%s);%s;V_{1}^{%s}", harmonic, comp1 [j].Data(), xAxes [axis].c_str(), centralities [cent].Data(), xAxesTitles [axis].c_str(), comp1 [j].Data()));
//        }
//        for (int name = 0; name < flow_names.size (); name++) 
//        {
//          mg3 [cent][name] = new TMultiGraph ();
//        }
//      }

      for (ushort i = 0; i < flow_names.size (); i++)
      {
		//{"%s_V1_%s_psd1_3S", "%s_%s_psd1", "R1_psd1_3S", "v_{1,%s}^{%s}{#psi_{SP}^{p, PSD1}}"},
        corrName = Form (flow_names[i][1], pid[0].Data(), xAxes [axis].Data());
        resName = Form (flow_names[i][2], pid[0].Data(), xAxes [axis].Data());
        flowName = Form (flow_names[i][0], pid[0].Data(), xAxes [axis].Data());
        cout << endl << flowName << " = " << corrName << " / " << resName << "\t";
        for (int j = 0; j < 3; j++) 
        {
          if (j != 2) fIn -> GetObject (corrName + comp [j], corr [j]);
          else if (corr [0] && corr [1]) *corr [2] = (*corr [0] + *corr [1]) * 2.;
//          fTemp -> GetObject (corrName + comp [j], corr [j]);
          fTemp -> GetObject (resName + comp [j], res [j]);
          if (corr [j] && res [j]) cout << "\t" << comp [j];
          else continue;
  //				if (j == 2) flow [j] = *corr [j] / (*res [j]); // like I like)
          if (j == 2) flow [j] = ((*corr [0] / (*res [0])) + (*corr [1] / (*res [1]))) * sqrt (2.); // alice-like 1 - average before centrality rebin
  //				flow [j] = (*corr [j]) * sqrt (2.) / (*res [j]); // alice-like 2
          else flow [j] = (*corr [j]) * sqrt (2.) / (*res [j]); // everything besides alice-like 2
          if (rebinFlow) flow [j] = flow [j].Rebin (centralityAxis, [](Qn::Stats &a, const Qn::Stats &b) {return  a+b;});
  //				if (j == 2) flow [j] = (flow [0] + flow [1]); // alice-like 2 - average after centrality rebin
          mg = Qn::DataContainerHelper::ToTMultiGraph (flow [j], centralityAxis.Name());
          mg -> SetName (flowName + comp [j]);
          mg -> SetTitle (Form (flow_names[i][3], comp1 [j].Data(), pid [1].Data()));
          glist = mg -> GetListOfGraphs ();
          for (int cent = 0; cent < glist -> GetSize (); cent++) 
          {		
            if (cent < markers [0].size ()) 
            {
              marker = markers [0][cent];
              color = colors [cent];
            }
            else 
            {
              marker = markers [1][cent - markers [0].size ()];
              color = colors [cent - markers [0].size ()];
            }
            g = (TGraphAsymmErrors*) glist -> At(cent);
            g -> SetName (flowName + comp [j] + "_" + sCentralities [cent]);
//            g -> SetTitle (Form (flow_names[i][3], comp1 [j].Data(), pid[1].Data()));
            g -> GetXaxis () -> SetTitle (xAxesTitles [axis].Data());
            g -> SetMarkerSize (1.5);
            g -> SetMarkerStyle (marker);
            g -> SetMarkerColor (color);
            g -> SetLineColor (color);
            graphs.push_back (g);
  //					if (i == 0) mg2 [cent][j] -> Add (pub [cent]);
//            mg2 [cent][j] -> Add (g);
//            gc = (TGraphAsymmErrors*) g -> Clone ();
//            gc -> SetMarkerStyle (markers [j]);
//            gc -> SetMarkerColor (colors [j]);
//            gc -> SetLineColor (colors [j]);
  //					if (j == 0) mg3 [cent][i] -> Add (pub [cent]);
//            mg3 [cent][i] -> Add (gc);
//            mg3 [cent][i] -> SetTitle (Form (flow_names[i][3] + " (%s);%s;V_{1}", "", centralities [cent].Data(), xAxesTitles [axis].c_str()));
          }
  //				g = new TGraphErrors (NA49_pi_v1_bins[][].size(), NA49_pi_v1_bins[][], NA49_pi_v1_value[][], NA49_pi_v1_error[][]);
  				mg -> Write(flowName + comp [j]);
        }
      }
      
//      for (int cent = 0; cent < nCentClasses; cent++) 
//      {
//        TDirectory *cent_dir = axis_dir -> mkdir (centralities [cent]));
//        cent_dir -> cd ();
//        for (int j = 0; j < 3; j++) 
//        {
//          mg2 [cent][j] -> Write (Form ("V_1_%s_%s", comp1 [j].Data(), xAxes [axis].c_str()));
//        }
//        for (int name = 0; name < flow_names.size (); name++) 
//        {
//          mg3 [cent][name] -> Write (Form (flow_names[name][0], xAxes [axis].c_str()));
//        }
//      }
    }
  }	
  
	cout << endl;
	TDirectory *all_dir = flow_dir -> mkdir ("all");
	all_dir -> cd ();
	for (ushort i = 0; i < graphs.size (); i++)
	{
		graphs [i] -> Write ();
	}
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////
//void SaveV1()
//{
//    vector <TMultiGraph*> mgraphs;
//    TDirectory *v1_dir = fOut->mkdir( "v1" );
//    v1_dir->cd();
//
//    for (int iVar=0; iVar<2; ++iVar)
//    for (int iPsd=0; iPsd<3; ++iPsd)
//        for (int iXy=0; iXy<2; ++iXy)
//            for (int iRes=0; iRes<6; ++iRes)
//            {
//                if (iRes == 5 && !issim) continue;
//
//                const TString corr_name = Form("TPC_%s_%s_%s%s", var[iVar].Data(), PsdName[iPsd].Data(), XY[iXy].Data(), XY[iXy].Data() );
//                TString res_name = Form("RES_%s_%s_%d", PsdName[iPsd].Data(), XY[iXy].Data(), iRes ) ;
//                if (iRes == 5) res_name = Form("%s_PSI_%s%s", PsdName[iPsd].Data(), XY[iXy].Data(), XY[iXy].Data()) ;
//                cout << corr_name << "  /  " << res_name << endl;
//
//                mgraphs.push_back( GetV1( corr_name, res_name ) );
//                mgraphs.back()->SetName( Form("v1_TPC_%s_%s_%s_%d", var[iVar].Data(), PsdName[iPsd].Data(), XY[iXy].Data(), iRes)  );
//                mgraphs.back()->Write();
//            }
//}
//
//
//void SaveV2()
//{
//    vector <TMultiGraph*> mgraphs;
//
//    TDirectory *v2_dir = fOut->mkdir( "v2" );
//    v2_dir->cd();
//
//    for (int iVar=0; iVar<2; ++iVar)
//    for (int iPsd=0; iPsd<3; ++iPsd)
//        for (int iXy=0; iXy<4; ++iXy)
//        {
//            const TString corr_name = Form("TPC_%s_%s_%s", var[iVar].Data(), psd_psd[iPsd].Data(), v2[iXy][0].Data() );
//            TString res_name1 = "";
//            TString res_name2 = "";
//
//            if (iXy<2)
//               res_name1 = Form("%s_%s", psd_psd[iPsd].Data(), v2[iXy][1].Data() ) ;
//            else
//            {
//                res_name1 = Form("RES_PSD%d_%s_0", iPsd/2+1, v2[iXy][1].Data() ) ;
//                res_name2 = Form("RES_PSD%d_%s_0", (iPsd+1)/2+2, v2[iXy][2].Data() ) ;
//            }
//
//            cout << corr_name << "  /  " << res_name1 << " " << res_name2  << endl;
//
//            mgraphs.push_back( GetV2( corr_name, res_name1, res_name2, iVar ) );
//
//            mgraphs.back()->SetName( Form("v2_TPC_%s_%s_%s", var[iVar].Data(), psd_psd[iPsd].Data(), v2[iXy][0].Data() ) );
//            mgraphs.back()->Write();
//        }
//}
//
//
//
//
//TMultiGraph* GetV1(const TString corr, const TString res)
//{
//    Qn::DataContainer<Qn::Stats> *pcorr;
//    Qn::DataContainer<Qn::Stats> *pres;
//
//    fIn->GetObject(corr, pcorr);
//    fOut->GetObject( "resolution/correlation/" + res, pres);
//
//    Qn::DataContainer<Qn::Stats> flow = *pcorr / *pres;
//
////     flow = flow.Rebin( {"Centrality", centrality_bins }, [](Qn::Stats &a, Qn::Stats &b) { return  a+b; } );
//
////    TMultiGraph* ret =  DataToMultiGraph( flow, "Centrality"  ) ;
////    TMultiGraph* ret =  DataToMultiGraph( flow, "Multiplicity"  ) ;
//
////    flow = flow.Rebin( {"Eveto", eVetoBins3}, [](Qn::Stats &a, Qn::Stats &b) { return  a+b; } );
//
//    TMultiGraph* ret =  DataToMultiGraph( flow, "Eveto"  ) ;
//
//    return ret;
//}
//
//TMultiGraph* GetV2(const TString corr, const TString res1, const TString res2, const int iVar)
//{
//    Qn::DataContainer<Qn::Stats> *pcorr;
//    Qn::DataContainer<Qn::Stats> *pres1;
//    Qn::DataContainer<Qn::Stats> *pres2;
//
//    Qn::DataContainer<Qn::Stats> flow;
//
//    float scale = 8.0;
//
//    fIn->GetObject(corr, pcorr);
//    if (res2=="")
//    {
//        fIn->GetObject(res1, pres1);
//        flow = (*pcorr * scale ) / (*pres1) ;
//    }
//    else
//    {
//        fOut->GetObject("resolution/correlation/" + res1, pres1);
//        fOut->GetObject("resolution/correlation/" + res2, pres2);
//        flow =/* 4. **/ (*pcorr * scale) / ( (*pres1) * (*pres2) );
//    }
//
////     flow = flow.Rebin( {"Centrality", centrality_bins }, [](Qn::Stats &a, Qn::Stats &b) { return  a+b; } );
////     for (auto i : flow.GetAxes())
////         cout << i.Name() << endl;
//
////    flow = flow.Rebin( {axisName[iVar], v2axis[iVar]}, [](Qn::Stats &a, Qn::Stats &b) { return  a+b; } );
//
////    TMultiGraph* ret =  DataToMultiGraph( flow, "Centrality"  ) ;
////    TMultiGraph* ret =  DataToMultiGraph( flow, "Multiplicity"  ) ;
//    TMultiGraph* ret =  DataToMultiGraph( flow, "Eveto"  ) ;
//    return ret;
//}
