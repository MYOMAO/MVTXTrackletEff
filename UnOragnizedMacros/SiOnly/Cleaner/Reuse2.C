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



void Reuse2(){

	float precision = 0.01;


	gStyle->SetOptStat(0);
	TCanvas * c = new TCanvas("c","c",600,600);
	c->cd();
	c->SetLeftMargin(0.17);


	TString infile = "Clean1.root";
	TFile * fin = new TFile(infile.Data());
	fin->cd();


	TString infile2 = "Clean1.root";
	TFile * fin2 = new TFile(infile2.Data());
	fin2->cd();


	TFile * fout = new TFile("Clean2.root","RECREATE");
	fout->cd();

	const int NTree = 2;
	TString TTreName[NTree] = {"TotalTrack","MissingChip"};

	for(int p = 0; p < NTree; p++){

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

		int EventPre = 1041;

		int NRepeatSeed = 0;	
		std::map<float, std::vector<int>> siseedused;
		std::map< std::array<float,3>, std::vector<std::pair<int,float>> > siclusused;


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



		TH1D * SiClusReuseEvt = new TH1D("SiClusReuseEvt","",100,-0.5,99.5);
		SiClusReuseEvt->GetXaxis()->SetTitle("Event ID");
		SiClusReuseEvt->GetYaxis()->SetTitle("Number of Reuse (Duplicatly Refitted) Silicon Seeds");
		SiClusReuseEvt->GetXaxis()->CenterTitle();
		SiClusReuseEvt->GetYaxis()->CenterTitle();
		SiClusReuseEvt->GetYaxis()->SetTitleOffset(1.4);

		SiClusReuseEvt->SetMarkerStyle(20);
		SiClusReuseEvt->SetMarkerSize(1);
		SiClusReuseEvt->SetMarkerColor(1);
		SiClusReuseEvt->SetLineColor(1);


		TTree * TotalTrackNew = TotalTrack2->CloneTree(0);

		int TotalTracks = 0;

		float quality;

		for(int i = 0; i < NSiTracks; i++){

			TotalTrack->GetEntry(i); 

			//if(Event > 1) break;
			//if(Event < 1041 || Event > 1042) continue;

			//cout << "Event = " << Event << endl;
			//if(Event != 1041 || Event != 1042 ) continue;

 
			quality = abs(chisq1/ndf1 - 1);

			if(Event != EventPre){


				/*
				   for (const auto& pair : siseedused) {
				//  std::cout << "siseedpt: " << pair.first << ", count: " << pair.second << "\n";

				int UsedCount = pair.second.size();
				ReuseSiSeed->Fill(UsedCount);

				TotalTrack2->GetEntry(pair.second[0]);
				TotalTrackNew->Fill();

				}

*/

				std::vector<int> EntryToFill;
				std::vector<int> EntryNotToFill;
				
				for (const auto& pair : siclusused) {
					//  std::cout << "siseedpt: " << pair.first << ", count: " << pair.second << "\n";

					int UsedCount = pair.second.size();




					//SiClusReuse->Fill(UsedCount);

					if(UsedCount > 1){


						float BestQuality = 99999;
						float BestQualityIndex = 0;

						for(int q = 0; q < UsedCount; q++){

							int trackentry = pair.second[q].first;

							float qual = pair.second[q].second;

							//cout << "Event = "  << EventPre << "   q = " << q << "   X = " <<  pair.first[0] << "  track entry: " << pair.second[q].first  << "  track quality: " << pair.second[q].second  << endl;

							if(quality < BestQuality){
								BestQuality = quality;
								BestQualityIndex = trackentry;

							}else{
								EntryNotToFill.push_back(trackentry);
								
							}



						}

						//std::cout << "Best: " << BestQualityIndex << endl;

						EntryToFill.push_back(BestQualityIndex);
						//TotalTrack2->GetEntry(BestQualityIndex);
					

						SiClusReuseEvt->Fill(EventPre);
					}else{

						//std::cout << "Else Best: " << pair.second[0].first << endl;
						//cout << "Else: Event = "  << EventPre << "   X = " <<  pair.first[0] << "  track entry: " << pair.second[0].first  << "  track quality: " << pair.second[0].second  << endl;

						//TotalTrack2->GetEntry(pair.second[0].first);

						EntryToFill.push_back(pair.second[0].first);

					}


				}

				int BeforeSize = EntryToFill.size();

				std::sort(EntryToFill.begin(),EntryToFill.end());
				auto it = std::unique(EntryToFill.begin(), EntryToFill.end());
				EntryToFill.resize(std::distance(EntryToFill.begin(), it));

				std::sort(EntryNotToFill.begin(),EntryNotToFill.end());
				auto it2 = std::unique(EntryNotToFill.begin(), EntryNotToFill.end());
				EntryNotToFill.resize(std::distance(EntryNotToFill.begin(), it2));


				int FinalSize = EntryToFill.size();


				//std::cout << "Event: " << Event << "   BeforeSize = " << BeforeSize << "   FinalSize = " << FinalSize << endl;

				TotalTracks = TotalTracks + FinalSize;


				for(int s = 0; s < FinalSize; s++){

					int FinallyEntry = EntryToFill[s];

					//cout << "s = " << s << "   FinallyEntry = " << FinallyEntry << endl;

				    auto it3 = std::find(EntryNotToFill.begin(), EntryNotToFill.end(), FinallyEntry);
					
				
					if(it3 == EntryNotToFill.end()){
						TotalTrack2->GetEntry(FinallyEntry);
						TotalTrackNew->Fill();

					}else{

						std::cout << "Reject Not To Fill Entry: " << FinallyEntry << endl;
					}
				}

				siseedused.clear();
				siclusused.clear();

			}

			TotalClusSize = ClusPosX->size();


			for(int j = 0; j < TotalClusSize; j++){


				X = ClusPosX->at(j);
				Y = ClusPosY->at(j);
				Z = ClusPosZ->at(j);
/*
				X = std::round(X / precision) * precision;
				Y = std::round(Y / precision) * precision;
				Z = std::round(Z / precision) * precision;
*/

				std::array<float, 3> cluspos = {X, Y, Z};
				siclusused[cluspos].push_back({i,quality});
			}



			EventPre = Event;
		}



		c->cd();
		ReuseSiSeed->Draw("ep");
		c->SaveAs("Plots/ReuseSiSeed2.png");

		SiClusReuseEvt->Draw("hist");
		c->SaveAs("Plots/SiClusReuseEvt2.png");



		std::vector<int> EntryToFill;
		for (const auto& pair : siclusused) {
			//  std::cout << "siseedpt: " << pair.first << ", count: " << pair.second << "\n";

			int UsedCount = pair.second.size();




			//SiClusReuse->Fill(UsedCount);

			if(UsedCount > 1){


				float BestQuality = 9999;
				float BestQualityIndex = 0;

				for(int q = 0; q < UsedCount; q++){

					int trackentry = pair.second[q].first;

					float qual = pair.second[q].second;

					//		cout << "Event = "  << EventPre << "   q = " << q << "  track entry: " << pair.second[q].first  << "  track quality: " << pair.second[q].second  << endl;

					if(quality < BestQuality){
						BestQuality = quality;
						BestQualityIndex = trackentry;

					}



				}


				EntryToFill.push_back(BestQualityIndex);
				//TotalTrack2->GetEntry(BestQualityIndex);


				SiClusReuseEvt->Fill(EventPre);
			}else{


				//TotalTrack2->GetEntry(pair.second[0].first);

				EntryToFill.push_back(pair.second[0].first);

			}


		}

		int BeforeSize = EntryToFill.size();

		std::sort(EntryToFill.begin(),EntryToFill.end());
		auto it = std::unique(EntryToFill.begin(), EntryToFill.end());

		EntryToFill.resize(std::distance(EntryToFill.begin(), it));

		int FinalSize = EntryToFill.size();


		//std::cout << "BeforeSize = " << BeforeSize << "   FinalSize = " << FinalSize << endl;



		for(int s = 0; s < FinalSize; s++){

			int FinallyEntry = EntryToFill[s];

			TotalTrack2->GetEntry(FinallyEntry);
			TotalTrackNew->Fill();

		}


		TotalTracks = TotalTracks + FinalSize;

		cout << "TotalTracks = " << TotalTracks << endl;

		fout->cd();

		TotalTrackNew->Write();
	
	}
	fout->Close();



}


