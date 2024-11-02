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



void Step0(int CrossingAna, int Run){


	int Crossing;
	int Crossing2;


	TString infile = Form("Infile/Run%d_All.root",Run);
	TFile * fin = new TFile(infile.Data());
	fin->cd();

	
	TTree * TotalTrack = (TTree *) fin->Get("TotalTrack");
	TotalTrack->SetBranchAddress("CrossingID",&Crossing);


	TString outfile = Form("Infile/Run%d.root",Run);

	
	TFile * fout = new TFile(outfile.Data(),"RECREATE");
	fout->cd();



	TTree * TotalTrackNew = TotalTrack->CloneTree(0);

//	TTree * MissingChipNew = MissingChip->CloneTree(0);


	int NTotalTrack = TotalTrack->GetEntries();

//	int NMissingChip = MissingChip->GetEntries();


	for(int i = 0; i < NTotalTrack; i++){

	
		TotalTrack->GetEntry(i);
		if(Crossing != CrossingAna) continue;
		TotalTrackNew->Fill();

	}

/*
	for(int i = 0; i < NMissingChip; i++){

	
		MissingChip->GetEntry(i);
		if(Crossing2 != CrossingAna) continue;
		MissingChipNew->Fill();
	}

*/


	fout->cd();

	TotalTrackNew->Write();
//	MissingChipNew->Write();

	fout->Close();



}
