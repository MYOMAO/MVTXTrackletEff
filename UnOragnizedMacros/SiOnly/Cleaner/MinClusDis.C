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



void MinClusDis(){

	gStyle->SetOptStat(0);
	TCanvas * c = new TCanvas("c","c",600,600);
	c->cd();
	c->SetLeftMargin(0.17);

	TString infile = "Clean3.root";
	TFile * fin = new TFile(infile.Data());
	fin->cd();



	float X;
	float Y;
	float Z;

	int Event;
	int TotalClusSize;
	std::vector<float> * ClusPosX = 0;
	std::vector<float> * ClusPosY = 0;
	std::vector<float> * ClusPosZ = 0;

	float siseedpt;
	float chisq1;
	int ndf1;

	TTree * TotalTrack = (TTree * ) fin->Get("TotalTrack");
	TotalTrack->SetBranchAddress("Event",&Event);
	TotalTrack->SetBranchAddress("TotalClusSize",&TotalClusSize);
	TotalTrack->SetBranchAddress("ClusPosX",&ClusPosX);
	TotalTrack->SetBranchAddress("ClusPosY",&ClusPosY);
	TotalTrack->SetBranchAddress("ClusPosZ",&ClusPosZ);
	TotalTrack->SetBranchAddress("chisq",&chisq1);
	TotalTrack->SetBranchAddress("ndf",&ndf1);
	TotalTrack->SetBranchAddress("siseedpt",&siseedpt);


	float MinClusR = 999;
	float MinClusX = 999;
	float MinClusY = 999;
	float MinClusZ = 999;

	float XDis;
	float YDis;
	float ZDis;
	float RDis;

	std::vector<float> XAll;
	std::vector<float> YAll;
	std::vector<float> ZAll;

	TH1D * MinR = new TH1D("MinR","",100,0,2);
	MinR->GetXaxis()->SetTitle("Minimum Silicon Tracklet Cluster Distance in An Event (cm)");
	MinR->GetYaxis()->SetTitle("Number of Events");

	int EventPre = 0;
	int NSiTracks = TotalTrack->GetEntries();


	for(int i = 0; i < NSiTracks; i++){

		TotalTrack->GetEntry(i); 




		if(Event != EventPre){


			int NClusEvt = XAll.size();


			for(int p = 0; p < NClusEvt; p++){

				for(int q = 0; q < NClusEvt; q++){

					if(p != q){
						float X1 = XAll[p];
						float X2 = XAll[q];
						float Y1 = YAll[p];
						float Y2 = YAll[q];
						float Z1 = ZAll[p];
						float Z2 = ZAll[q];

						XDis = X1 - X2;
						YDis = Y1 - Y2;
						ZDis = Z1 - Z2;

						RDis = sqrt(XDis * XDis + YDis * YDis + ZDis * ZDis);

						if(XDis < MinClusX && XDis != 0 ) MinClusX = XDis;
						if(YDis < MinClusY && YDis != 0 ) MinClusY = YDis;
						if(ZDis < MinClusZ && ZDis != 0 ) MinClusZ = ZDis;
						if(RDis < MinClusR && RDis != 0 ) MinClusR = RDis;

					}

				}
			}

			cout << "Event: " << EventPre << "    MinClusR: " << MinClusR << endl;

			MinR->Fill(MinClusR);

			XAll.clear();
			YAll.clear();
			ZAll.clear();



			MinClusR = 999;
			MinClusX = 999;
			MinClusY = 999;
			MinClusZ = 999;

		}

		TotalClusSize = ClusPosX->size();

		for(int j = 0; j < TotalClusSize; j++){


			X = ClusPosX->at(j);
			Y = ClusPosY->at(j);
			Z = ClusPosZ->at(j);

			XAll.push_back(X);
			YAll.push_back(Y);
			ZAll.push_back(Z);

		}



		EventPre = Event;

	}


	MinR->Fill(MinClusR);

	c->cd();
	MinR->Draw();
	c->SaveAs("ClusCheck/MinR.png");
}
