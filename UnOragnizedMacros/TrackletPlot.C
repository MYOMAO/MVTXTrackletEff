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



void QuickPlot(){



	gStyle->SetOptStat(0);
	TCanvas * c = new TCanvas("c","c",600,600);
	c->cd();


	


	int layerindex;
	int staveindex;
	int chipindex;
	int ndf;
	float chisq;
	float quality;

	TFile * fin = new TFile("MissingChip.root");



	TTree * MissingChip = (TTree *) fin->Get("MissingChip");



	MissingChip->SetBranchAddress("layerindex",&layerindex);
	MissingChip->SetBranchAddress("staveindex",&staveindex);
	MissingChip->SetBranchAddress("chipindex",&chipindex);
	MissingChip->SetBranchAddress("ndf",&ndf);
	MissingChip->SetBranchAddress("chisq",&chisq);
	

	TH1D * MissingQuality = new TH1D("MissingQuality","",200,-100,1000);
	MissingQuality->GetXaxis->SetTitle("Track Quality (#chi^2/ndf)");
	MissingQuality->GetYaxis->SetTitle("Number of 2-Cluster MVTX Tracks");
	MissingQuality->GetXaxis->SetCenterTitle();
	MissingQuality->GetYaxis->SetCenterTitle();
	MissingQuality->GetYaxis->SetTitleOffset();

	MissingQuality->SetMarkerStyle(20);
	MissingQuality->SetMarkerColor(1);
	MissingQuality->SetLineColor(1);
	MissingQuality->SetMarkerSize(1);

	TH1D * AllQuality = (TH1D *) MissingQuality->Clone("AllQuality");
	AllQuality->GetYaxis->SetTitle("Number of Tracks with MVTX Hits");

	TH1D * Efficiency = (TH1D *) MissingQuality->Clone("Efficiency");
	MissingQuality->GetYaxis->SetTitle("Percent of 2-Cluster MVTX Tracks");



	for(int i = 0; i < NTracklets; i++){

		MissingChip->GetEntry(i);
	
		quality = chisq/ndf;

		if(ndf == -1) quality = -10;

		MissingQuality->Fill(quality);
	
		Efficiency->Fill(quality);
	}

	c->cd();


	int layerfired;
	int stavefired;
	int chipfired;
	int ndfall;
	float chisqall;

	float qualityall;

	TTree * TotalChip = (TTree *) fin->Get("TotalChip");


	TotalChip->SetBranchAddress("layerfired",&layerfired);
	TotalChip->SetBranchAddress("stavefired",&stavefired);
	TotalChip->SetBranchAddress("chipfired",&chipfired);
	TotalChip->SetBranchAddress("ndf",&ndfall);
	TotalChip->SetBranchAddress("chisq",&chisqall);


	int NTrackletsFired = TotalChip->GetEntries();



	for(int i = 0; i < NTrackletsFired; i++){

		TotalChip->GetEntry(i);

		qualityall = chisqfired/ndfall;

		if(ndfall == -1) qualityall = -10;

		AllQuality->Fill(quality);


		//MissingStat->Fill(layerindex);

	}


	Efficiency->Sumw2();
	AllQuality->Sumw2();


	Efficiency->Divde(AllQuality);


	c->cd();
	Efficiency->Draw("ep");


	c->SaveAs("PlotTrack/Efficiency.png");


}
