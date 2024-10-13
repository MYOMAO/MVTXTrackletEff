#ifndef __CINT__
#include "TROOT.h"
#include "TH1.h"
#include "TTree.h"
#include "TH2.h"
#include "TF1.h"
#include "TFile.h"
#include "TMath.h"
#include "TSystem.h"
#include "TVector2.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TRandom.h"
#include <iostream>
#include <fstream>
using namespace std;

using std::cout;
using std::endl;
#endif



void TrackletPlot(){



	gStyle->SetOptStat(0);
	TCanvas * c = new TCanvas("c","c",600,600);
	c->cd();





	int layerindex;
	int staveindex;
	int chipindex;
	int ndf;
	float chisq;
	float quality;
	float trackpt;
	float siseedpt;

	float siz;
	float tpcz;



	float CosTheta;

	TFile * fin = new TFile("CuFilesFinal/Run53285.root");



	TTree * MissingChip = (TTree *) fin->Get("MissingChip");



	MissingChip->SetBranchAddress("layerindex",&layerindex);
	MissingChip->SetBranchAddress("staveindex",&staveindex);
	MissingChip->SetBranchAddress("chipindex",&chipindex);
	MissingChip->SetBranchAddress("ndf",&ndf);
	MissingChip->SetBranchAddress("chisq",&chisq);
	MissingChip->SetBranchAddress("trackpt",&trackpt);
	MissingChip->SetBranchAddress("siseedpt",&siseedpt);
	

	MissingChip->SetBranchAddress("siz",&siz);
	MissingChip->SetBranchAddress("tpcz",&tpcz);
	

	TH1D * MissingQuality = new TH1D("MissingQuality","",50,-120,2000);
	MissingQuality->GetXaxis()->SetTitle("Track Quality (#chi^2/ndf)");
	MissingQuality->GetYaxis()->SetTitle("Number of 2-Cluster MVTX Tracks");
	MissingQuality->GetXaxis()->CenterTitle();
	MissingQuality->GetYaxis()->CenterTitle();
	MissingQuality->GetYaxis()->SetTitleOffset();

	MissingQuality->SetMarkerStyle(20);
	MissingQuality->SetMarkerColor(1);
	MissingQuality->SetLineColor(1);
	MissingQuality->SetMarkerSize(1);

	TH1D * AllQuality = (TH1D *) MissingQuality->Clone("AllQuality");
	AllQuality->GetYaxis()->SetTitle("Number of Tracks with MVTX Hits");

	TH1D * Efficiency = (TH1D *) MissingQuality->Clone("Efficiency");
	MissingQuality->GetYaxis()->SetTitle("Percent of 2-Cluster MVTX Tracks");




	TH1D * MissingPt = new TH1D("MissingPt","",100,0,5);
	MissingPt->GetXaxis()->SetTitle("Track p_{T} (GeV/c)");
	MissingPt->GetYaxis()->SetTitle("Number of 2-Cluster MVTX Tracks");
	MissingPt->GetXaxis()->CenterTitle();
	MissingPt->GetYaxis()->CenterTitle();
	MissingPt->GetYaxis()->SetTitleOffset();

	MissingPt->SetMarkerStyle(20);
	MissingPt->SetMarkerColor(1);
	MissingPt->SetLineColor(1);
	MissingPt->SetMarkerSize(1);

	TH1D * AllPt = (TH1D *) MissingPt->Clone("AllPt");
	AllPt->GetYaxis()->SetTitle("Number of Tracks with MVTX Hits");

	TH1D * EffPt = (TH1D *) MissingPt->Clone("EffPt");
	MissingQuality->GetYaxis()->SetTitle("Percent of 2-Cluster MVTX Tracks");


	TH1D * MissingSiPt = new TH1D("MissingSiPt","",100,0,10);
	MissingSiPt->GetXaxis()->SetTitle("Silicon Seed p_{T} (GeV/c)");
	MissingSiPt->GetYaxis()->SetTitle("Number of 2-Cluster MVTX Tracks");
	MissingSiPt->GetXaxis()->CenterTitle();
	MissingSiPt->GetYaxis()->CenterTitle();
	MissingSiPt->GetYaxis()->SetTitleOffset();

	MissingSiPt->SetMarkerStyle(20);
	MissingSiPt->SetMarkerColor(1);
	MissingSiPt->SetLineColor(1);
	MissingSiPt->SetMarkerSize(1);

	TH1D * AllSiPt = (TH1D *) MissingSiPt->Clone("AllSiPt");
	AllSiPt->GetYaxis()->SetTitle("Number of Tracks with MVTX Hits");

	TH1D * EffSiPt = (TH1D *) MissingSiPt->Clone("EffSiPt");
	EffSiPt->GetYaxis()->SetTitle("Percent of 2-Cluster MVTX Tracks");


	TH1D * MissingDeltaZ = new TH1D("MissingDeltaZ","",200,-100,100);
	MissingDeltaZ->GetXaxis()->SetTitle("Silicon Seed z - TPC Seed z: #Delta z (cm)");
	MissingDeltaZ->GetYaxis()->SetTitle("Number of 2-Cluster MVTX Tracks");
	MissingDeltaZ->GetXaxis()->CenterTitle();
	MissingDeltaZ->GetYaxis()->CenterTitle();
	MissingDeltaZ->GetYaxis()->SetTitleOffset();

	MissingDeltaZ->SetMarkerStyle(20);
	MissingDeltaZ->SetMarkerColor(1);
	MissingDeltaZ->SetLineColor(1);
	MissingDeltaZ->SetMarkerSize(1);

	TH1D * AllDeltaZ = (TH1D *) MissingDeltaZ->Clone("AllDeltaZ");
	AllDeltaZ->GetYaxis()->SetTitle("Number of Tracks with MVTX Hits");

	TH1D * EffDeltaZ = (TH1D *) MissingDeltaZ->Clone("EffDeltaZ");
	EffDeltaZ->GetYaxis()->SetTitle("Percent of 2-Cluster MVTX Tracks");


	float DeltaZ;

	int NTracklets = MissingChip->GetEntries();
	
	for(int i = 0; i < NTracklets; i++){

		MissingChip->GetEntry(i);
	
		quality = chisq/ndf;
		if(ndf == -1) quality = -100;

		MissingQuality->Fill(quality);

		Efficiency->Fill(quality);
		MissingPt->Fill(trackpt);
		EffPt->Fill(trackpt);

	
		MissingSiPt->Fill(siseedpt);
		EffSiPt->Fill(siseedpt);

		DeltaZ = siz - tpcz; 
	
		//cout << "Delta Z = " << DeltaZ << endl;

		MissingDeltaZ->Fill(DeltaZ);
		EffDeltaZ->Fill(DeltaZ);

	}

	c->cd();


	int layerfired;
	int stavefired;
	int chipfired;
	int ndfall;
	float chisqall;

	float qualityall;
	float trackptall;
	float siseedptall;
	
	float sizall;
	float tpczall;

	float tpcseedpx;
	float tpcseedpy;
	float tpcseedpz;
	
	

	float siseedpx;
	float siseedpy;
	float siseedpz;


	TTree * TotalTrack = (TTree *) fin->Get("TotalTrack");



	TotalTrack->SetBranchAddress("ndf",&ndfall);
	TotalTrack->SetBranchAddress("chisq",&chisqall);
	TotalTrack->SetBranchAddress("trackpt",&trackptall);
	TotalTrack->SetBranchAddress("siseedpt",&siseedptall);


	TotalTrack->SetBranchAddress("siz",&sizall);
	TotalTrack->SetBranchAddress("tpcz",&tpczall);
	
	TotalTrack->SetBranchAddress("siseedpx",&siseedptall);
	



	int NTrackletsFired = TotalTrack->GetEntries();



	for(int i = 0; i < NTrackletsFired; i++){

		TotalTrack->GetEntry(i);

		qualityall = chisqall/ndfall;

		if(ndfall == -1) qualityall = -100;



		AllQuality->Fill(qualityall);

		AllPt->Fill(trackptall);

		//MissingStat->Fill(layerindex);

		AllSiPt->Fill(siseedptall);

		//cout << "Delta Z = " << DeltaZ << endl;

		DeltaZ = sizall - tpczall; 
		AllDeltaZ->Fill(DeltaZ);


	}


	Efficiency->Sumw2();
	AllQuality->Sumw2();
	Efficiency->Divide(AllQuality);



	EffPt->Sumw2();
	AllPt->Sumw2();
	EffPt->Divide(AllPt);



	EffSiPt->Sumw2();
	AllSiPt->Sumw2();
	EffSiPt->Divide(AllSiPt);


	EffDeltaZ->Sumw2();
	AllDeltaZ->Sumw2();
	EffDeltaZ->Divide(AllDeltaZ);


	c->cd();


	Efficiency->Draw("ep");
	c->SaveAs("PlotTrack/Efficiency.png");


	EffPt->SetMaximum(1.0);
	EffPt->SetMinimum(0.0);


	EffPt->Draw("ep");
	c->SaveAs("PlotTrack/EffPt.png");


	EffSiPt->Draw("ep");
	c->SaveAs("PlotTrack/EffSiPt.png");

	EffDeltaZ->Draw("ep");
	c->SaveAs("PlotTrack/EffDeltaZ.png");

	c->SetLogy();

	MissingQuality->Draw("ep");
	c->SaveAs("PlotTrack/MissingQuality.png");

	AllQuality->Draw("ep");
	c->SaveAs("PlotTrack/AllQuality.png");





}
