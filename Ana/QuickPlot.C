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


	TH1D * MissingStat = new TH1D("MissingStat","",3,-0.5,2.5);
	MissingStat->GetXaxis()->SetTitle("MVTX Layer");
	MissingStat->GetYaxis()->SetTitle("Number of MVTX Chips Missing");
	MissingStat->GetXaxis()->CenterTitle();
	MissingStat->GetYaxis()->CenterTitle();
	


	int layerindex;
	int staveindex;
	int chipindex;

	TFile * fin = new TFile("MissingChip.root");



	TTree * MissingChip = (TTree *) fin->Get("MissingChip");



	MissingChip->SetBranchAddress("layerindex",&layerindex);
	MissingChip->SetBranchAddress("staveindex",&staveindex);
	MissingChip->SetBranchAddress("chipindex",&chipindex);
	


	TH1D * Layer = new TH1D("Layer","",3,-0.5,2.5);
	Layer->GetXaxis()->SetTitle("Layer");
	Layer->GetYaxis()->SetTitle("Number of Tracklets");
	Layer->SetTitle("Missing Layer Distribution");


	TH2D * Layer0 = new TH2D("Layer0","",9,-0.5,8.5,12,-0.5,11.5);
	Layer0->GetXaxis()->SetTitle("Chip");
	Layer0->GetYaxis()->SetTitle("Stave");
	Layer0->SetTitle("Layer 0");


	
	TH2D * Layer1 = new TH2D("Layer1","",9,-0.5,8.5,16,-0.5,15.5);
	Layer1->GetXaxis()->SetTitle("Chip");
	Layer1->GetYaxis()->SetTitle("Stave");
	Layer1->SetTitle("Layer 1");


	TH2D * Layer2 = new TH2D("Layer2","",9,-0.5,8.5,20,-0.5,19.5);
	Layer2->GetXaxis()->SetTitle("Chip");
	Layer2->GetYaxis()->SetTitle("Stave");
	Layer2->SetTitle("Layer 2");

	int NTracklets = MissingChip->GetEntries();



	for(int i = 0; i < NTracklets; i++){

		MissingChip->GetEntry(i);

		Layer->Fill(layerindex);

		if(layerindex == 0) Layer0->Fill(chipindex,staveindex);
		if(layerindex == 1) Layer1->Fill(chipindex,staveindex);
		if(layerindex == 2) Layer2->Fill(chipindex,staveindex);

		MissingStat->Fill(layerindex);
	
	}

	c->cd();

	
	Layer0->Draw("COLZ");
	c->SaveAs("Plots/Layer0.png");

	Layer1->Draw("COLZ");
	c->SaveAs("Plots/Layer1.png");

	Layer2->Draw("COLZ");
	c->SaveAs("Plots/Layer2.png");
	

	Layer->Draw("hist");
	c->SaveAs("Plots/MissingLayer.png");

	MissingStat->Draw("ep");
	c->SaveAs("Plots/MissingStat.png");


	int layerfired;
	int stavefired;
	int chipfired;


	TTree * TotalChip = (TTree *) fin->Get("TotalChip");



	TotalChip->SetBranchAddress("layerfired",&layerfired);
	TotalChip->SetBranchAddress("stavefired",&stavefired);
	TotalChip->SetBranchAddress("chipfired",&chipfired);
	


	TH1D * FiredLayer = new TH1D("FiredLayer","Fired + Missing Layer Distribution",3,-0.5,2.5);
	Layer->GetXaxis()->SetTitle("Layer");
	Layer->GetYaxis()->SetTitle("Number of Tracklets");
	Layer->SetTitle("Missing + Fired Layer Distribution");


	TH2D * FiredLayer0 = new TH2D("FiredLayer0","Fired + Missing Layer 0",9,-0.5,8.5,12,-0.5,11.5);
	FiredLayer0->GetXaxis()->SetTitle("Chip");
	FiredLayer0->GetYaxis()->SetTitle("Stave");
	FiredLayer0->SetTitle("Layer 0");


	
	TH2D * FiredLayer1 = new TH2D("FiredLayer1","Fired + Missing Layer 1",9,-0.5,8.5,16,-0.5,15.5);
	FiredLayer1->GetXaxis()->SetTitle("Chip");
	FiredLayer1->GetYaxis()->SetTitle("Stave");
	FiredLayer1->SetTitle("Layer 1");


	TH2D * FiredLayer2 = new TH2D("FiredLayer2","Fired + Missing Layer 2",9,-0.5,8.5,20,-0.5,19.5);
	FiredLayer2->GetXaxis()->SetTitle("Chip");
	FiredLayer2->GetYaxis()->SetTitle("Stave");
	FiredLayer2->SetTitle("Layer 2");

	int NTrackletsFired = TotalChip->GetEntries();



	for(int i = 0; i < NTrackletsFired; i++){

		TotalChip->GetEntry(i);
		FiredLayer->Fill(layerfired);

		if(layerfired == 0) FiredLayer0->Fill(chipfired,stavefired);
		if(layerfired == 1) FiredLayer1->Fill(chipfired,stavefired);
		if(layerfired == 2) FiredLayer2->Fill(chipfired,stavefired);

		//MissingStat->Fill(layerindex);

	}

	c->cd();
	FiredLayer0->Draw("COLZ");
	c->SaveAs("Plots/FiredLayer0.png");

	FiredLayer1->Draw("COLZ");
	c->SaveAs("Plots/FiredLayer1.png");

	FiredLayer2->Draw("COLZ");
	c->SaveAs("Plots/FiredLayer2.png");
	

	FiredLayer->Draw("hist");
	c->SaveAs("Plots/FiredLayer.png");

	



	TH1D * LayerEff = (TH1D *) Layer->Clone("LayerEff");
	
	LayerEff->Divide(FiredLayer);

	for(int i = 0; i < 3;i++){


		float Value = LayerEff->GetBinContent(i+1);
		float Eff =  1 - Value;

		LayerEff->SetBinContent(i+1,Eff);

	}



	LayerEff->Draw("hist");
	c->SaveAs("Plots/LayerEff.png");


}
