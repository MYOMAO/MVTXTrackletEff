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



void MakeCommon(){

	

	gStyle->SetOptStat(0);
	TCanvas * c = new TCanvas("c","c",600,600);
	c->cd();
	c->SetLeftMargin(0.17);


	TString infile = "Clean2.root";
	TFile * fin = new TFile(infile.Data());
	fin->cd();

	
	TString infile2 = "Clean2.root";
	TFile * fin2 = new TFile(infile2.Data());
	fin2->cd();

	int Event;
	int Event2;
	int TrackletID;
	int TrackletID2;

	TTree * MissingChip2 = (TTree * ) fin2->Get("MissingChip");
	

	TTree * TotalTrack = (TTree * ) fin->Get("TotalTrack");
	TotalTrack->SetBranchAddress("Event",&Event);
	TotalTrack->SetBranchAddress("TrackletID",&TrackletID);

	
		
	TTree * MissingChip = (TTree * ) fin->Get("MissingChip");
	MissingChip->SetBranchAddress("Event",&Event2);
	MissingChip->SetBranchAddress("TrackletID",&TrackletID2);


	int NMissing = MissingChip->GetEntries();
	int NTotal = TotalTrack->GetEntries();


	


	TString outfile = "Clean3.root";
	TFile * fout = new TFile(outfile.Data(),"RECREATE");
	fout->cd();

	
	TTree * MissingChipNew = MissingChip2->CloneTree(0);


	bool GoodEntry;
	bool Matched = false;
	for(int i = 0; i < NMissing; i++){
		

		MissingChip->GetEntry(i);
		Matched = false;
		if(i%10 == 0) cout << "Working on Tracklet i = " << i << endl;

		for(int j = 0; j < NTotal; j++){


			TotalTrack->GetEntry(j);
		
			if(Event2 == Event && TrackletID2 == TrackletID){

				Matched = true;
			}

		}
		
		if(!Matched){
			cout << "OK Bro - Event: " <<  Event2 << "  Has a Tracklet with ID: " << TrackletID2 <<  "  that is in Missing but not in total after cleaning -> removing this tracklet " <<  endl;

		}else{

	
			MissingChip2->GetEntry(i);
			MissingChipNew->Fill();

		}
	}


	fout->cd();
	TotalTrack->Write();
	MissingChipNew->Write();
	fout->Close();
}
