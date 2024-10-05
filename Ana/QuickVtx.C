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


void QuickVtx(){

	TCanvas * c = new TCanvas("c","c",600,600);
	c->cd();

	gStyle->SetOptStat(0);


	TString infile = "ResidualTonyProd.root";


	int eventPre = 0;
	int event = 0;
	int crossing;
	int vertexid = -1;
	int m_nsiseed;


	TFile * fin = new TFile(infile.Data());
	fin->cd();

	TTree * residualtree = (TTree *) fin->Get("residualtree");

	int NEvents = residualtree->GetEntries();

	int NVertex;

	TH1D * NVtx = new TH1D("NVtx","",20,-0.5,19.5);
	NVtx->GetXaxis()->SetTitle("Number of Vertices Per Event");
	NVtx->GetYaxis()->SetTitle("Number of Events");
	NVtx->GetXaxis()->CenterTitle();
	NVtx->GetYaxis()->CenterTitle();


	residualtree->SetBranchAddress("event",&event);
	residualtree->SetBranchAddress("crossing",&crossing);
	residualtree->SetBranchAddress("vertexid",&vertexid);


	int MaxVtxID;


	NEvents = 10000;

	for(int i = 0; i < NEvents; i++){


		residualtree->GetEntry(i);


		if(crossing != 0 ) continue;

		if(eventPre != event){

			if(vertexid > MaxVtxID) MaxVtxID = vertexid;

			NVertex = MaxVtxID + 1;

			NVtx->Fill(NVertex);



			NVertex = 0;

			MaxVtxID = -1;


		}


		//NVertex++;

		eventPre = event;

	}

	c->cd();
	NVtx->Draw("hist");

	c->SaveAs("NVertex.png");
}
