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


void Comp(){


	gStyle->SetOptStat(0);
	TCanvas * c = new TCanvas("c","c",600,600);
	c->cd();
	c->SetLeftMargin(0.17);


	float siseedpt1; 
	float siseedpt2; 
	float siseedpt3; 

	float chisq1;
	float chisq2;
	float chisq3;


	int ndf1;
	int ndf2;
	int ndf3;

	

	TH1D * Q1His = new TH1D("Q1His","",100,0,2000);
	Q1His->GetXaxis()->SetTitle("Quality (#chi^2/ndf)");
	Q1His->GetYaxis()->SetTitle("Normalized Counts");
	Q1His->GetXaxis()->CenterTitle();
	Q1His->GetYaxis()->CenterTitle();

	Q1His->SetMarkerSize(1);
	Q1His->SetMarkerStyle(20);
	Q1His->SetMarkerColor(1);
	Q1His->SetLineColor(1);

	TH1D * Q2His = (TH1D *) Q1His->Clone("Q2His");
	Q2His->SetMarkerColor(kRed);
	Q2His->SetLineColor(kRed);



	TH1D * Q3His = (TH1D *) Q1His->Clone("Q3His");
	Q3His->SetMarkerColor(kBlue);
	Q3His->SetLineColor(kBlue);





	TFile * fin1 = new TFile("Files/SiTrackFit.root");
	fin1->cd();

	TTree * TotalTrack = (TTree *) fin1->Get("TotalTrack");
	//TotalTrack->SetBranchAddress("Event",&Event1);
	TotalTrack->SetBranchAddress("chisq",&chisq1);
	TotalTrack->SetBranchAddress("ndf",&ndf1);
	TotalTrack->SetBranchAddress("siseedpt",&siseedpt1);




	TFile * fin2 = new TFile("TPCCut/FullTrackFit.root");
	fin2->cd();

	TTree * TotalTrack2 = (TTree *) fin2->Get("TotalTrack");
	//TotalTrack2->SetBranchAddress("Event",&Event2);
	TotalTrack2->SetBranchAddress("chisq",&chisq2);
	TotalTrack2->SetBranchAddress("ndf",&ndf2);
	TotalTrack2->SetBranchAddress("siseedpt",&siseedpt2);




	TFile * fin3 = new TFile("Matched.root");
	fin3->cd();


	TTree * TotalTrack3 = (TTree *) fin3->Get("TotalTrack");
	//TotalTrack3->SetBranchAddress("Event",&Event3);
	TotalTrack3->SetBranchAddress("chisq",&chisq3);
	TotalTrack3->SetBranchAddress("ndf",&ndf3);
	TotalTrack3->SetBranchAddress("siseedpt",&siseedpt3);


	int NEvent1 = TotalTrack->GetEntries();
	int NEvent2 = TotalTrack2->GetEntries();
	int NEvent3 = TotalTrack3->GetEntries();

	float quality;

	for(int i = 0; i < NEvent1; i++){

		TotalTrack->GetEntry(i);

		quality = chisq1/ndf1;

		Q1His->Fill(quality);

	}

	for(int i = 0; i < NEvent2; i++){

		TotalTrack2->GetEntry(i);

		quality = chisq2/ndf2;

		Q2His->Fill(quality);

	}

	for(int i = 0; i < NEvent3; i++){

		TotalTrack3->GetEntry(i);

		quality = chisq3/ndf3;

		Q3His->Fill(quality);

	}

	Q1His->Scale(1.0/Q1His->Integral());
	Q2His->Scale(1.0/Q2His->Integral());
	Q3His->Scale(1.0/Q3His->Integral());

	c->cd();

	Q1His->SetMaximum(0.4);

	Q1His->Draw("ep");
	Q2His->Draw("epSAME");
	Q3His->Draw("epSAME");
	

	float Mean1 = Q1His->GetMean();
	float Mean2 = Q2His->GetMean();
	float Mean3 = Q3His->GetMean();

	cout << "Mean1: " << Mean1 << "   Mean2: " << Mean2 << "  Mean3: " << Mean3 << endl;
	
	TLegend * leg = new TLegend(0.23,0.70,0.80,0.90,NULL,"brNDC");
	leg->SetBorderSize(0);
	leg->SetTextSize(0.040);
	leg->SetTextFont(42);
	leg->SetFillStyle(0);
	leg->SetLineWidth(2);


	leg->AddEntry(Q1His,Form("Silicon Only Tracklet: Helix Fit, <Q> = %.2f",Mean1),"LP");
	leg->AddEntry(Q2His,Form("Selected Silicon + TPC Track: ACTS Fit: <Q> = %.2f",Mean2),"LP");
	leg->AddEntry(Q3His,Form("Silicon Only with Silicon + TPC Matched Track: Helix Fit: <Q> = %.2f",Mean3),"LP");

	c->SetLogy();

	leg->Draw("SAME");
	c->SaveAs("QualComp.png");


}
