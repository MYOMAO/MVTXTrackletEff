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



void MakeDis(){



	gStyle->SetOptStat(0);
	TCanvas * c = new TCanvas("c","c",600,600);
	c->cd();
	c->SetLeftMargin(0.17);


	TFile * fin = new TFile("Infile/Run53285.root");
	fin->cd();

	int nmaps;
	int nmapslayer;

	int nintt;
	int ninttlayer;

	int ntpc;
	int ntpclayer;


	std::vector<float> * ClusPosX = 0;
	std::vector<float> * ClusPosY = 0;
	std::vector<float> * ClusPosZ = 0;


	
	int TotalClusSize;
	int Event;

	float X;
	float Y;
	float Z;
	float R;

	TTree * TotalTrack = (TTree *) fin->Get("TotalTrack");
	TotalTrack->SetBranchAddress("nmaps",&nmaps);
	TotalTrack->SetBranchAddress("nmapslayer",&nmapslayer);
	TotalTrack->SetBranchAddress("nintt",&nintt);
	TotalTrack->SetBranchAddress("ninttlayer",&ninttlayer);
//	TotalTrack->SetBranchAddress("ntpc",&ntpc);
//	TotalTrack->SetBranchAddress("ntpclayer",&ninttlayer);

	TotalTrack->SetBranchAddress("TotalClusSize",&TotalClusSize);
	TotalTrack->SetBranchAddress("ClusPosX",&ClusPosX);
	TotalTrack->SetBranchAddress("ClusPosY",&ClusPosY);
	TotalTrack->SetBranchAddress("ClusPosZ",&ClusPosZ);


	int NTracklets = TotalTrack->GetEntries();

	TH1D * MVTXHits = new TH1D("MVTXHIts","",5,-0.5,4.5);
	MVTXHits->GetXaxis()->SetTitle("MVTX Clusters Per Tracklet");
	MVTXHits->GetYaxis()->SetTitle("Number of TPC Matched Tracklets");
	MVTXHits->GetXaxis()->CenterTitle();
	MVTXHits->GetYaxis()->CenterTitle();
	MVTXHits->GetYaxis()->SetTitleOffset(2.2);
	MVTXHits->SetLineColor(1);
	MVTXHits->SetLineWidth(2);




	TH1D * MVTXHitsLayer = new TH1D("MVTXHitsLayer","",5,-0.5,4.5);
	MVTXHitsLayer->GetXaxis()->SetTitle("MVTX Clusters Per Tracklet");
	MVTXHitsLayer->GetYaxis()->SetTitle("Number of TPC Matched Tracklets");
	MVTXHitsLayer->GetXaxis()->CenterTitle();
	MVTXHitsLayer->GetYaxis()->CenterTitle();
	MVTXHitsLayer->GetYaxis()->SetTitleOffset(2.2);
	MVTXHitsLayer->SetLineColor(kRed);
	MVTXHitsLayer->SetLineWidth(2);


	TH1D * INTTHits = new TH1D("INTTHits","",6,-0.5,5.5);
	INTTHits->GetXaxis()->SetTitle("INTT Clusters Per Tracklet");
	INTTHits->GetYaxis()->SetTitle("Number of TPC Matched Tracklets");
	INTTHits->GetXaxis()->CenterTitle();
	INTTHits->GetYaxis()->CenterTitle();
	INTTHits->GetYaxis()->SetTitleOffset(2.2);
	INTTHits->SetLineColor(kBlack);
	INTTHits->SetLineWidth(2);

	
	TH1D * INTTHitsLayer = new TH1D("INTTHitsLayer","",6,-0.5,5.5);
	INTTHitsLayer->GetXaxis()->SetTitle("INTT Clusters Per Tracklet");
	INTTHitsLayer->GetYaxis()->SetTitle("Number of TPC Matched Tracklets");
	INTTHitsLayer->GetXaxis()->CenterTitle();
	INTTHitsLayer->GetYaxis()->CenterTitle();
	INTTHitsLayer->GetYaxis()->SetTitleOffset(2.2);
	INTTHitsLayer->SetLineColor(kRed);
	INTTHitsLayer->SetLineWidth(2);


	TH1D * SiliconHits = new TH1D("SiliconHits","",5,-0.5,4.5);
	SiliconHits->GetXaxis()->SetTitle("MVTX + INTT Clusters Per Tracklet");
	SiliconHits->GetYaxis()->SetTitle("Number of TPC Matched Tracklets");
	SiliconHits->GetXaxis()->CenterTitle();
	SiliconHits->GetYaxis()->CenterTitle();
	SiliconHits->GetYaxis()->SetTitleOffset(2.2);
	SiliconHits->SetLineColor(1);
	SiliconHits->SetLineWidth(2);



	TLegend * leg = new TLegend(0.23,0.75,0.80,0.90,NULL,"brNDC");
	leg->SetBorderSize(0);
	leg->SetTextSize(0.040);
	leg->SetTextFont(42);
	leg->SetFillStyle(0);
	leg->SetLineWidth(2);


	leg->AddEntry(MVTXHits,"Clusters Per Tracklets","L");
	leg->AddEntry(MVTXHitsLayer,"Layers Fired Per Tracklets","L");

	float MVTXR = 5;
	float INTTR = 12;



	TH1D * MVTXClusHis = new TH1D("MVTXClusHis","",30,-0.5,29.5);
	MVTXClusHis->GetXaxis()->SetTitle("MVTX Clusters Size");
	MVTXClusHis->GetYaxis()->SetTitle("MVTX Tracklets");
	MVTXClusHis->GetXaxis()->CenterTitle();
	MVTXClusHis->GetYaxis()->CenterTitle();
	MVTXClusHis->GetYaxis()->SetTitleOffset(2.2);


	TH1D * INTTClusHis = new TH1D("INTTClusHis","",10,-0.5,9.5);
	INTTClusHis->GetXaxis()->SetTitle("INTT Clusters Size");
	INTTClusHis->GetYaxis()->SetTitle("INTT Tracklets");
	INTTClusHis->GetXaxis()->CenterTitle();
	INTTClusHis->GetYaxis()->CenterTitle();
	INTTClusHis->GetYaxis()->SetTitleOffset(2.2);


	for(int i = 0; i < NTracklets; i++){


		TotalTrack->GetEntry(i);


		MVTXHits->Fill(nmaps);
		MVTXHitsLayer->Fill(nmapslayer);

		INTTHits->Fill(nintt);
		INTTHitsLayer->Fill(ninttlayer);


		
		int MVTXClusSize = 0;
		int INTTClusSize = 0;

		for(int j = 0; j < TotalClusSize; j++){


			X = ClusPosX->at(j);
			Y = ClusPosY->at(j);
			Z = ClusPosZ->at(j);

			R = sqrt(X*X + Y*Y);

			if(R < 5) MVTXClusSize++;
			if(R < 12 && R > 5) INTTClusSize++;
		
		}
		
		MVTXClusHis->Fill(MVTXClusSize);		
		INTTClusHis->Fill(INTTClusSize);
	}

	
	

	c->cd();

	MVTXHitsLayer->SetMaximum(28000);
	MVTXHitsLayer->Draw("hist");
	MVTXHits->Draw("histSAME");
	leg->Draw("SAME");

	c->SaveAs("DisPlot/MVTXHits.png");

	INTTHitsLayer->SetMaximum(28000);
	INTTHitsLayer->Draw("hist");
	INTTHits->Draw("histSAME");
	leg->Draw("SAME");

	c->SaveAs("DisPlot/INTTHits.png");

	MVTXClusHis->Draw("hist");
	c->SaveAs("DisPlot/MVTXClusHis.png");
	
	INTTClusHis->Draw("hist");
	c->SaveAs("DisPlot/INTTClusHis.png");


}
