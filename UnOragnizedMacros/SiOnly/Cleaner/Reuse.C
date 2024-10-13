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



void Reuse(){



	gStyle->SetOptStat(0);
	TCanvas * c = new TCanvas("c","c",600,600);
	c->cd();
	c->SetLeftMargin(0.17);


	TString infile = "../Files/SiTrackFitAll.root";
	TFile * fin = new TFile(infile.Data());
	fin->cd();


	TString infile2 = "../Files/SiTrackFitAll.root";
	TFile * fin2 = new TFile(infile2.Data());
	fin2->cd();


	TFile * fout = new TFile("Clean1.root","RECREATE");
	fout->cd();

	const int NTree = 2;
	TString TTreName[NTree] = {"TotalTrack","MissingChip"};

	for(int p = 0; p < NTree; p++){

		cout << "p = " << p << endl;

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

		TTree * TotalTrack = (TTree * ) fin->Get(TTreName[p].Data());
		TotalTrack->SetBranchAddress("Event",&Event);
		TotalTrack->SetBranchAddress("TotalClusSize",&TotalClusSize);
		TotalTrack->SetBranchAddress("ClusPosX",&ClusPosX);
		TotalTrack->SetBranchAddress("ClusPosY",&ClusPosY);
		TotalTrack->SetBranchAddress("ClusPosZ",&ClusPosZ);
		TotalTrack->SetBranchAddress("chisq",&chisq1);
		TotalTrack->SetBranchAddress("ndf",&ndf1);
		TotalTrack->SetBranchAddress("siseedpt",&siseedpt);


		TTree * TotalTrack2 = (TTree * ) fin2->Get(TTreName[p].Data());




		int NSiTracks = TotalTrack->GetEntries();

		int EventPre = 0;

		int NRepeatSeed = 0;	
		std::map<float, std::vector<int>> siseedused;
		std::map<float, std::vector<int>> siclusused;


		//std::map<float, int> siclusused;




		TH1D * ReuseSiSeed = new TH1D("ReusedSiseed","",10,-0.5,9.5);
		ReuseSiSeed->GetXaxis()->SetTitle("Number of Repeat Silicon Seeds Per Event");
		ReuseSiSeed->GetYaxis()->SetTitle("Number of Tracks");
		ReuseSiSeed->GetXaxis()->CenterTitle();
		ReuseSiSeed->GetYaxis()->CenterTitle();
		ReuseSiSeed->GetYaxis()->SetTitleOffset(1.4);

		ReuseSiSeed->SetMarkerStyle(20);
		ReuseSiSeed->SetMarkerSize(1);
		ReuseSiSeed->SetMarkerColor(1);
		ReuseSiSeed->SetLineColor(1);



		TH1D * SiClusReuse = new TH1D("SiClusReuse","",10,-0.5,9.5);
		SiClusReuse->GetXaxis()->SetTitle("Number of Repeat Silicon Seeds Per Event");
		SiClusReuse->GetYaxis()->SetTitle("Number of Tracks");
		SiClusReuse->GetXaxis()->CenterTitle();
		SiClusReuse->GetYaxis()->CenterTitle();
		SiClusReuse->GetYaxis()->SetTitleOffset(1.4);

		SiClusReuse->SetMarkerStyle(20);
		SiClusReuse->SetMarkerSize(1);
		SiClusReuse->SetMarkerColor(1);
		SiClusReuse->SetLineColor(1);



		TTree * TotalTrackNew = TotalTrack2->CloneTree(0);



		for(int i = 0; i < NSiTracks; i++){


			//cout << "i = " << i << endl;

			TotalTrack->GetEntry(i); 


			if(Event != EventPre){



				for (const auto& pair : siseedused) {
					//  std::cout << "siseedpt: " << pair.first << ", count: " << pair.second << "\n";

					int UsedCount = pair.second.size();
					ReuseSiSeed->Fill(UsedCount);

					TotalTrack2->GetEntry(pair.second[0]);
					TotalTrackNew->Fill();

				}


				for (const auto& pair : siclusused) {
					//  std::cout << "siseedpt: " << pair.first << ", count: " << pair.second << "\n";

					int UsedCount = pair.second.size();

					SiClusReuse->Fill(UsedCount);

				}


				siseedused.clear();
				siclusused.clear();

			}


			siseedused[siseedpt].push_back(i);

			TotalClusSize = ClusPosX->size();

			for(int j = 0; j < TotalClusSize; j++){


				X = ClusPosX->at(j);
				Y = ClusPosY->at(j);
				Z = ClusPosZ->at(j);


				siclusused[X].push_back(i);
			}



			EventPre = Event;
		}



		if(p == 0){
			c->SetLogy();
			c->cd();
			ReuseSiSeed->Draw("ep");
			c->SaveAs("Plots/ReuseCheck/ReuseSiSeed.png");

			SiClusReuse->Draw("ep");
			c->SaveAs("Plots/ReuseCheck/SiClusReuse.png");

		}


		for (const auto& pair : siseedused) {
			//  std::cout << "siseedpt: " << pair.first << ", count: " << pair.second << "\n";

			int UsedCount = pair.second.size();
			ReuseSiSeed->Fill(UsedCount);

			TotalTrack2->GetEntry(pair.second[0]);
			TotalTrackNew->Fill();

		}

		fout->cd();
		TotalTrackNew->Write();

	}

	fout->Close();



}
