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



void Draw(){


	gStyle->SetOptStat(0);
	TCanvas * c = new TCanvas("c","c",600,600);
	c->cd();
	c->SetLeftMargin(0.17);



	TString infile = "Files/SiTrackFitAll.root";
	TFile * fin = new TFile(infile.Data());

	fin->cd();
	int TotalClusSize;
	int Event;

	float X;
	float Y;
	float Z;


	double miss_x;
	double miss_y;
	double miss_z;

	float siseedpt;
	float chisq1;
	int ndf1;

	float quality;

	std::vector<float> * ClusPosX = 0;
	std::vector<float> * ClusPosY = 0;
	std::vector<float> * ClusPosZ = 0;

	TTree * TotalTrack = (TTree * ) fin->Get("TotalTrack");
	TotalTrack->SetBranchAddress("Event",&Event);

	TotalTrack->SetBranchAddress("TotalClusSize",&TotalClusSize);
	TotalTrack->SetBranchAddress("ClusPosX",&ClusPosX);
	TotalTrack->SetBranchAddress("ClusPosY",&ClusPosY);
	TotalTrack->SetBranchAddress("ClusPosZ",&ClusPosZ);

	TotalTrack->SetBranchAddress("chisq",&chisq1);
	TotalTrack->SetBranchAddress("ndf",&ndf1);
	TotalTrack->SetBranchAddress("siseedpt",&siseedpt);
	 
	

	int NSiTracks = TotalTrack->GetEntries();

	const int NEvent = 6;
	int EventToDisplay[NEvent] = {22,114,301,689,1041,2340};



	TH2D * FiredClusHis[NEvent];
	TH2D * MissClusHis[NEvent];


	TH2D * FiredClusHisHQ[NEvent];
	TH2D * MissClusHisHQ[NEvent];




	for(int q = 0; q < NEvent; q++){

		FiredClusHis[q] = new TH2D(Form("FiredClusHis_%d",q),"",100,-12,12,100,-12,18);
		FiredClusHis[q]->GetXaxis()->SetTitle("X (cm)");
		FiredClusHis[q]->GetYaxis()->SetTitle("Y (cm)");
		FiredClusHis[q]->GetXaxis()->CenterTitle();
		FiredClusHis[q]->GetYaxis()->CenterTitle();
		FiredClusHis[q]->SetMarkerColor(kRed);
		FiredClusHis[q]->SetMarkerStyle(20);
		FiredClusHis[q]->SetMarkerSize(1);

		MissClusHis[q] = new TH2D(Form("MissClusHis_%d",q),"",100,-12,12,100,-12,18);
		MissClusHis[q]->GetXaxis()->SetTitle("X (cm)");
		MissClusHis[q]->GetYaxis()->SetTitle("Y (cm)");
		MissClusHis[q]->GetXaxis()->CenterTitle();
		MissClusHis[q]->GetYaxis()->CenterTitle();
		MissClusHis[q]->SetMarkerColor(kBlue);
		MissClusHis[q]->SetMarkerStyle(20);
		MissClusHis[q]->SetMarkerSize(1);


		FiredClusHisHQ[q] = new TH2D(Form("FiredClusHisHQ_%d",q),"",100,-5,5,100,-5,8);
		FiredClusHisHQ[q]->GetXaxis()->SetTitle("X (cm)");
		FiredClusHisHQ[q]->GetYaxis()->SetTitle("Y (cm)");
		FiredClusHisHQ[q]->GetXaxis()->CenterTitle();
		FiredClusHisHQ[q]->GetYaxis()->CenterTitle();
		FiredClusHisHQ[q]->SetMarkerColor(kGreen);
		FiredClusHisHQ[q]->SetMarkerStyle(20);
		FiredClusHisHQ[q]->SetMarkerSize(1);

		MissClusHisHQ[q] = new TH2D(Form("MissClusHisHQ_%d",q),"",100,-5,5,100,-5,8);
		MissClusHisHQ[q]->GetXaxis()->SetTitle("X (cm)");
		MissClusHisHQ[q]->GetYaxis()->SetTitle("Y (cm)");
		MissClusHisHQ[q]->GetXaxis()->CenterTitle();
		MissClusHisHQ[q]->GetYaxis()->CenterTitle();
		MissClusHisHQ[q]->SetMarkerColor(kOrange);
		MissClusHisHQ[q]->SetMarkerStyle(20);
		MissClusHisHQ[q]->SetMarkerSize(1);


	}


	for(int i = 0; i < NSiTracks; i++){

		TotalTrack->GetEntry(i); 

		quality = chisq1/ndf1;


		for(int j = 0; j < TotalClusSize; j++){


			X = ClusPosX->at(j);
			Y = ClusPosY->at(j);
			Z = ClusPosZ->at(j);


			for(int q = 0; q < NEvent; q++){

				if(Event == EventToDisplay[q]){

					FiredClusHis[q]->Fill(X,Y);
					if(quality < 5 && siseedpt > 0.5) 	FiredClusHisHQ[q]->Fill(X,Y);
				}
			}

		}



	}

	int MissEvent;

	float chisq2;
	int ndf2;
	float siseedpt2;

	

	TTree * MissingChip = (TTree * ) fin->Get("MissingChip");
	MissingChip->SetBranchAddress("Event",&MissEvent);
	MissingChip->SetBranchAddress("x",&miss_x);
	MissingChip->SetBranchAddress("y",&miss_y);
	MissingChip->SetBranchAddress("z",&miss_z);

	MissingChip->SetBranchAddress("chisq",&chisq2);
	MissingChip->SetBranchAddress("ndf",&ndf2);
	MissingChip->SetBranchAddress("siseedpt",&siseedpt2);



	int MissSiTracks = MissingChip->GetEntries();


	for(int i = 0; i < MissSiTracks; i++){

		MissingChip->GetEntry(i); 
		quality = chisq2/ndf2;

		
		for(int q = 0; q < NEvent; q++){

				if(MissEvent == EventToDisplay[q]){
					MissClusHis[q]->Fill(miss_x,miss_y);
					if(quality < 5 && siseedpt2 > 0.5) 	MissClusHisHQ[q]->Fill(miss_x,miss_y);


				}
		
		}

		



	}

	

	TLegend * leg = new TLegend(0.18,0.75,0.57,0.90,NULL,"brNDC");
	leg->SetBorderSize(0);
	leg->SetTextSize(0.034);
	leg->SetTextFont(42);
	leg->SetFillStyle(0);
	leg->SetLineWidth(1);


	leg->AddEntry(FiredClusHis[0],"All Silicon Seed Clusters","P");
	leg->AddEntry(MissClusHis[0],"All Missing MVTX Clusters","P");


	TLegend * leg2 = new TLegend(0.18,0.75,0.57,0.90,NULL,"brNDC");
	leg2->SetBorderSize(0);
	leg2->SetTextSize(0.034);
	leg2->SetTextFont(42);
	leg2->SetFillStyle(0);
	leg2->SetLineWidth(1);

	leg2->AddEntry(FiredClusHisHQ[0],"All High Quality Silicon Seed Clusters","P");
	leg2->AddEntry(MissClusHisHQ[0],"All High Quality Missing MVTX Clusters","P");

	c->cd();
	

	for(int q = 0; q < NEvent; q++){
	
		FiredClusHis[q]->Draw("scat");
		MissClusHis[q]->Draw("scatSAME");
	
		leg->Draw("SAME");
		c->SaveAs(Form("EvtDisplay/Evt_%d.png",q));

		FiredClusHisHQ[q]->Draw("scat");
		MissClusHisHQ[q]->Draw("scatSAME");
		leg2->Draw("SAME");
		c->SaveAs(Form("EvtDisplay/HQ/Evt_%d.png",q));

		
	}


}
