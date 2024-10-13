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


void QualEff(){



	gStyle->SetOptStat(0);
	TCanvas * c = new TCanvas("c","c",600,600);
	c->cd();
	c->SetLeftMargin(1.4);

	TFile * fin = new TFile("MVTXEffAna.root");

	TTree * TotalTrack = (TTree *) fin->Get("TotalTrack");

	int ndf;
	float chisq;

	float quality;
	float siseedpt;

	int nmaps;

	TotalTrack->SetBranchAddress("ndf",&ndf);
	TotalTrack->SetBranchAddress("chisq",&chisq);	
	TotalTrack->SetBranchAddress("siseedpt",&siseedpt);	
	TotalTrack->SetBranchAddress("nmaps",&nmaps);	

	int NSiTracklets = TotalTrack->GetEntries();


	const int NTypes = 3;

	TH1D * Total[NTypes];
	TH1D * Pass[NTypes];
	TH1D * Eff[NTypes];

	float Max[NTypes] = {100,2,10};

	TString XName[NTypes] = {"Quality","p_{T} (GeV/c)","Quality Zoom"};

	for(int q = 0; q < NTypes; q++){
		Total[q]= new TH1D(Form("Total_%d",q),"",100,0,Max[q]);
		Total[q]->GetXaxis()->SetTitle(XName[q].Data());
		Total[q]->GetYaxis()->SetTitle("Efficiency");

		Total[q]->SetMarkerColor(kBlack);
		Total[q]->SetLineColor(kBlack);
		Total[q]->SetMarkerStyle(20);
		Total[q]->SetMarkerSize(1);

		Eff[q]= (TH1D *) Total[q]->Clone(Form("Eff_%d",q));

	}

	/*
	   TH1D * Pass = new TH1D("Pass","",100,0,100);
	   Pass->GetXaxis()->SetTitle("Silicon Tracklet Quality (#chi^2/ndf)");
	   Pass->GetYaxis()->SetTitle("Number of Tracklets with at Least 3 Clusters");

	   Pass->SetMarkerColor(kBlack);
	   Pass->SetLineColor(kBlack);
	   Pass->SetMarkerStyle(20);
	   Pass->SetMarkerSize(1);

	   TH1D * Eff = new TH1D("Pass","",100,0,100);
	   Eff->GetXaxis()->SetTitle("Silicon Tracklet Quality (#chi^2/ndf)");
	   Eff->GetYaxis()->SetTitle("Efficiency of 3 Layers");
	   Eff->SetMarkerColor(kBlack);
	   Eff->SetLineColor(kBlack);
	   Eff->SetMarkerStyle(20);
	   Eff->SetMarkerSize(1);
	   */



	for(int i = 0; i < NSiTracklets; i++){


		TotalTrack->GetEntry(i);

		quality = chisq/ndf;

		Total[0]->Fill(quality);

		Total[1]->Fill(siseedpt);
		Total[2]->Fill(quality);

		//		if(nmaps > 2) Pass->Fill(quality);
		if(nmaps > 2){
			Eff[0]->Fill(quality);
			Eff[1]->Fill(siseedpt);
			Eff[2]->Fill(quality);
			
		}


	}

	c->cd();


	for(int q = 0; q < NTypes; q++){
		Eff[q]->Sumw2();
		Total[q]->Sumw2();
		Eff[q]->Divide(Total[q]);



		Eff[q]->Draw("ep");
		c->SaveAs(Form("QualEff/Eff_%d.png",q));

	}
	/*
	   Total->Draw("ep");
	   c->SaveAs("QualEff/Total.png");

	   Pass->Draw("ep");
	   c->SaveAs("QualEff/Pass.png");
	   */

}
