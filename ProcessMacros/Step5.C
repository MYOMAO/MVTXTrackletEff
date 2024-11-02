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



void Step5(int Run){



	gStyle->SetOptStat(0);
	TCanvas * c = new TCanvas("c","c",600,600);
	c->cd();
	c->SetLeftMargin(0.17);


	TString infile = Form("Outfile/Clean3_%d.root",Run);
	TFile * fin = new TFile(infile.Data());
	fin->cd();


	TString infile2 = Form("/Users/zhaozhongshi/Desktop/TempDownload/MVTXHitEff/NewLastLANL/Analysis/NewFiles/Run%d.root",Run);
	TFile * fin2 = new TFile(infile2.Data());
	fin2->cd();

	int Event;
	int Event2;
	int TrackletID;
	int TrackletID2;


	float siseedpt;
	float siseedpt2;



	TTree * TotalTrack = (TTree * ) fin->Get("TotalTrack");
	TotalTrack->SetBranchAddress("Event",&Event);
	TotalTrack->SetBranchAddress("TrackletID",&TrackletID);
	TotalTrack->SetBranchAddress("siseedpt",&siseedpt);


	TTree * TotalTrack2 = (TTree * ) fin2->Get("TotalTrack");
	TotalTrack2->SetBranchAddress("Event",&Event2);
	TotalTrack2->SetBranchAddress("siseedpt",&siseedpt2);


	int NMissing = TotalTrack2->GetEntries();
	int NTotal = TotalTrack->GetEntries();


	std::vector<std::vector<float>> Allsiseedpt; 
	std::vector<float> Evtsiseedpt; 
	std::vector<int> EventFinder; 

	TString outfile = Form("IncludeTPC/Clean5_%d.root",Run);
	TFile * fout = new TFile(outfile.Data(),"RECREATE");
	fout->cd();


	TTree * TotalTrackNew = TotalTrack2->CloneTree(0);



	int EventPre = 0;
	for(int j = 0; j < NTotal; j++){


		TotalTrack->GetEntry(j);


		if(EventPre != Event){

			EventFinder.push_back(EventPre);

			Allsiseedpt.push_back(Evtsiseedpt);
		

			Evtsiseedpt.clear();
		}




		Evtsiseedpt.push_back(siseedpt);


		EventPre = Event;

		TotalTrackNew->Fill();
	}


	Allsiseedpt.push_back(Evtsiseedpt);

	Evtsiseedpt.clear();


	cout << "Allsiseedpt.size() = " << Allsiseedpt.size()  << endl;


	//bool GoodEntry;
	bool Matched = false;
	int EventPre2 = 0;
	for(int i = 0; i < NMissing; i++){


		TotalTrack2->GetEntry(i);
		//	Matched = false;
		if(Event2%100 == 0 && EventPre2 != Event2) cout << "Working on Tracklet Event = " << Event2 << endl;

		auto it = std::find(EventFinder.begin(), EventFinder.end(), Event2);

        int position = std::distance(EventFinder.begin(), it);

		auto it3 = std::find(Allsiseedpt[position].begin(), Allsiseedpt[position].end(), siseedpt2);


		if(it3 != Allsiseedpt[position].end()){

		//	MissingChip2->GetEntry(i);
			TotalTrackNew->Fill();




		}else{


			//if(Event2 == 1041)	cout << "OK Bro - Event: " <<  Event2 << "  Has a Tracklet with ID: " << TrackletID2 <<  "  that is in Missing but not in total after cleaning -> removing this tracklet " <<  endl;


		}
		EventPre2 = Event2;
		/*
		   if(!Matched){
		   cout << "OK Bro - Event: " <<  Event2 << "  Has a Tracklet with ID: " << TrackletID2 <<  "  that is in Missing but not in total after cleaning -> removing this tracklet " <<  endl;

		   }else{


		   MissingChip2->GetEntry(i);
		   MissingChipNew->Fill();

		   }
		   */	
	}


	fout->cd();
	TotalTrackNew->Write();
	fout->Close();
}
