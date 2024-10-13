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


void Pick(){



	int Event1;
	int Event2;

	float siseedpt1;
	float siseedpt2;

	float chisq1;
	float chisq2;



	int ndf1;
	int ndf2;


	


	TFile * fin = new TFile("Files/SiTrackFit.root");
	fin->cd();

	TTree * TotalTrack = (TTree *) fin->Get("TotalTrack");
	TotalTrack->SetBranchAddress("Event",&Event1);
	TotalTrack->SetBranchAddress("chisq",&chisq1);
	TotalTrack->SetBranchAddress("ndf",&ndf1);
	TotalTrack->SetBranchAddress("siseedpt",&siseedpt1);


	int NSiTracklet = TotalTrack->GetEntries();

	TFile * fin2 = new TFile("TPCCut/FullTrackFit.root");
	fin2->cd();

	
	TTree * TotalTrack2 = (TTree *) fin2->Get("TotalTrack");
	TotalTrack2->SetBranchAddress("Event",&Event2);
	TotalTrack2->SetBranchAddress("chisq",&chisq2);
	TotalTrack2->SetBranchAddress("ndf",&ndf2);
	TotalTrack2->SetBranchAddress("siseedpt",&siseedpt2);

	int NFullTracklet = TotalTrack2->GetEntries();

	TFile * fout = new TFile("Matched.root","RECREATE");
	fout->cd();
	

	float sichisq;
	int sindf;
	
	TTree * SiMatched = TotalTrack2->CloneTree();

	SiMatched->Branch("sichisq",&sichisq);
	SiMatched->Branch("sindf",&sindf);



	for(int i = 0; i < NFullTracklet; i++){


		TotalTrack2->GetEntry(i);
	

		if(i%10 == 0) std::cout << "Now Working on Entry i = " << i << endl;

		for(int j = 0; j < NSiTracklet; j++){

			TotalTrack->GetEntry(j);

			if(siseedpt1 == siseedpt2){


			//	std::cout << "Matched" << std::endl;
	

				SiMatched->Fill();

				sichisq = chisq1;
				sindf = ndf1;


			}

		}

	}

	
	SiMatched->Write();


	fout->Close();



}
