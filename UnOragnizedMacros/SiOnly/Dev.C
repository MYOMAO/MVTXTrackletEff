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
//#include "XYZVector.h"

using namespace std;

using std::cout;
using std::endl;
#endif



void Dev(){

	gStyle->SetOptStat(0);
	TCanvas * c = new TCanvas("c","c",600,600);
	c->cd();
	c->SetLeftMargin(0.17);


	int TotalClusSize;
	int Event;

	std::vector<float> * ClusPosX = 0;
	std::vector<float> * ClusPosY = 0;
	std::vector<float> * ClusPosZ = 0;

	float six;
	float siy;
	float siz;
	float siseedpx;
	float siseedpy;
	float siseedpz;
	float siseedpt;

	float X;
	float Y;
	float Z;
	float t;



	int nmaps;
	int nintt;
		
	TFile * fin = new TFile("Files/SiTrackFit.root");


	TTree * TotalTrack = (TTree *) fin->Get("TotalTrack");


	TotalTrack->SetBranchAddress("six",&six);
	TotalTrack->SetBranchAddress("siy",&siy);
	TotalTrack->SetBranchAddress("siz",&siz);

	TotalTrack->SetBranchAddress("siseedpx",&siseedpx);
	TotalTrack->SetBranchAddress("siseedpy",&siseedpy);
	TotalTrack->SetBranchAddress("siseedpz",&siseedpz);
	TotalTrack->SetBranchAddress("siseedpt",&siseedpt);

	TotalTrack->SetBranchAddress("TotalClusSize",&TotalClusSize);
	TotalTrack->SetBranchAddress("ClusPosX",&ClusPosX);
	TotalTrack->SetBranchAddress("ClusPosY",&ClusPosY);
	TotalTrack->SetBranchAddress("ClusPosZ",&ClusPosZ);

	TotalTrack->SetBranchAddress("nmaps",&nmaps);
	TotalTrack->SetBranchAddress("nintt",&nintt);

	TH1D * XDev = new TH1D("XDev","",100,-10,10);
	XDev->GetXaxis()->SetTitle("#Delta x (cm)");
	XDev->GetYaxis()->SetTitle("Number of Silicon Clusters");


	TH1D * YDev = new TH1D("YDev","",100,-10,10);
	YDev->GetXaxis()->SetTitle("#Delta y (cm)");
	YDev->GetYaxis()->SetTitle("Number of Silicon Clusters");

	TH1D * ZDev = new TH1D("ZDev","",100,-10,10);
	ZDev->GetXaxis()->SetTitle("#Delta z (cm)");
	ZDev->GetYaxis()->SetTitle("Number of Silicon Clusters");

	TH1D * XDevZoom = new TH1D("XDev","",100,-1,1);
	XDevZoom->GetXaxis()->SetTitle("#Delta x (mm)");
	XDevZoom->GetYaxis()->SetTitle("Number of Silicon Clusters");


	TH1D * YDevZoom = new TH1D("YDev","",100,-1,1);
	YDevZoom->GetXaxis()->SetTitle("#Delta y (mm)");
	YDevZoom->GetYaxis()->SetTitle("Number of Silicon Clusters");

	TH1D * ZDevZoom = new TH1D("ZDev","",100,-1,1);
	ZDevZoom->GetXaxis()->SetTitle("#Delta z (mm)");
	ZDevZoom->GetYaxis()->SetTitle("Number of Silicon Clusters");


	TH1D * DisHis = new TH1D("DisHis","",100,-10,10);
	DisHis->GetXaxis()->SetTitle("Distance (cm)");
	DisHis->GetYaxis()->SetTitle("Number of Silicon Clusters");

	TFile * fout = new TFile("Files/DevFile.root","RECREATE");


	fout->cd();

	int nmaps2;
	int nintt2;
	vector<float> XDevVec;
	vector<float> YDevVec;
	vector<float> ZDevVec;


	TTree * DevTree = new TTree("DevTree","DevTree");
	DevTree->Branch("Event",&Event);
	DevTree->Branch("nmaps",&nmaps2);
	DevTree->Branch("nintt",&nintt2);
	DevTree->Branch("XDev",&XDevVec);
	DevTree->Branch("YDev",&YDevVec);
	DevTree->Branch("ZDev",&ZDevVec);
	DevTree->Branch("PassCut",&PassCut);



	TTree * TotalTrack = (TTree *) fin->Get("TotalTrack");


	int NSiTracks = TotalTrack->GetEntries();

	float Dx;
	float Dy;
	float Dz;

	float Mom;

	float Dis;

	float DevX;
	float DevY;
	float DevZ;


	for(int i = 0; i < NSiTracks; i++){

		TotalTrack->GetEntry(i); 

		if(siseedpt < 2) continue;

		for(int j = 0; j < TotalClusSize; j++){


			X = ClusPosX->at(j);
			Y = ClusPosY->at(j);
			Z = ClusPosZ->at(j);


			Dx = X - six;
			Dy = Y - siy;
			Dz = Z - siz;


			ROOT::Math::XYZVector DR(Dx,Dy,Dz);

			ROOT::Math::XYZVector DP(siseedpx,siseedpy,siseedpz);

			Dis = (DR.Cross(DP)).R()/DP.R(); 


			DevX = (DR.Cross(DP)).X()/DP.R(); 
			DevY = (DR.Cross(DP)).Y()/DP.R(); 
			DevZ = (DR.Cross(DP)).Z()/DP.R(); 


			//Calculate Distance

			DisHis->Fill(Dis);

			XDev->Fill(DevX);
			YDev->Fill(DevY);
			ZDev->Fill(DevZ);

			XDevZoom->Fill(DevX * 10);
			YDevZoom->Fill(DevY *10);
			ZDevZoom->Fill(DevZ *10);

			if(abs(DevX) < 0.02 && abs(DevY) < 0.02 && abs(DevZ) < 0.02) PassClus = true;

		}

		nmaps2 = nmaps;
		nintt2 = nintt;
	}


	c->cd();

	DisHis->Draw();
	c->SaveAs("SiClusPlots/ClusDis.png");


	XDev->Draw();
	c->SaveAs("SiClusPlots/XDev.png");

	YDev->Draw();
	c->SaveAs("SiClusPlots/YDev.png");

	ZDev->Draw();
	c->SaveAs("SiClusPlots/ZDev.png");

	XDevZoom->Draw();
	c->SaveAs("SiClusPlots/XDevZoom.png");

	YDevZoom->Draw();
	c->SaveAs("SiClusPlots/YDevZoom.png");

	ZDevZoom->Draw();
	c->SaveAs("SiClusPlots/ZDevZoom.png");



}
