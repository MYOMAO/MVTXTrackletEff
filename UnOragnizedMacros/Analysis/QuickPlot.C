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



void QuickPlot(){



	gStyle->SetOptStat(0);
	TCanvas * c = new TCanvas("c","c",600,600);
	c->cd();
	c->SetLeftMargin(1.4);


	TH1D * MissingStat = new TH1D("MissingStat","",3,-0.5,2.5);
	MissingStat->GetXaxis()->SetTitle("MVTX Layer");
	MissingStat->GetYaxis()->SetTitle("Number of MVTX Chips Missing");
	MissingStat->GetXaxis()->CenterTitle();
	MissingStat->GetYaxis()->CenterTitle();



	int layerindex;
	int staveindex;
	int chipindex;
	int ndf;
	float chisq;
	float siz;
	float tpcz;
	float six;
	float tpcx;
	float siy;
	float tpcy;

	float DeltaX;
	float DeltaY;
	float DeltaZ;
	
	float trackpt;
	float quality;

	int poormvtxclus;
	int poorinttclus;

	int ninttlayer;

	int nmapslayer;

	float tpcseedpx;
	float tpcseedpy;
	float tpcseedpz;

	float siseedpx;
	float siseedpy;
	float siseedpz;

	TFile * fin = new TFile("CutFiles/Run53285.root");



	TTree * MissingChip = (TTree *) fin->Get("MissingChip");



	MissingChip->SetBranchAddress("layerindex",&layerindex);
	MissingChip->SetBranchAddress("staveindex",&staveindex);
	MissingChip->SetBranchAddress("chipindex",&chipindex);
	MissingChip->SetBranchAddress("ndf",&ndf);
	MissingChip->SetBranchAddress("chisq",&chisq);
	MissingChip->SetBranchAddress("six",&six);
	MissingChip->SetBranchAddress("tpcx",&tpcx);
	MissingChip->SetBranchAddress("siy",&siy);
	MissingChip->SetBranchAddress("tpcy",&tpcy);

	MissingChip->SetBranchAddress("siz",&siz);
	MissingChip->SetBranchAddress("tpcz",&tpcz);
	MissingChip->SetBranchAddress("poormvtxclus",&poormvtxclus);
	MissingChip->SetBranchAddress("poorinttclus",&poorinttclus);

	MissingChip->SetBranchAddress("trackpt",&trackpt);


	MissingChip->SetBranchAddress("ninttlayer",&ninttlayer);
	MissingChip->SetBranchAddress("nmapslayer",&nmapslayer);
	

	MissingChip->SetBranchAddress("tpcseedpx",&tpcseedpx);
	MissingChip->SetBranchAddress("tpcseedpy",&tpcseedpy);
	MissingChip->SetBranchAddress("tpcseedpz",&tpcseedpz);

	MissingChip->SetBranchAddress("siseedpx",&siseedpx);
	MissingChip->SetBranchAddress("siseedpy",&siseedpy);
	MissingChip->SetBranchAddress("siseedpz",&siseedpz);

	const int NTrial = 6;

	TH1D * Layer[NTrial]; 

	for(int i = 0; i < NTrial; i++){

		Layer[i] = new TH1D("Layer","",3,-0.5,2.5);
		Layer[i]->GetXaxis()->SetTitle("Layer");
		Layer[i]->GetYaxis()->SetTitle("Number of Tracklets");
		Layer[i]->SetTitle("Missing Layer Distribution");
		Layer[i]->SetLineColor(i+1);
		Layer[i]->SetLineWidth(2);

	}

	TH2D * Layer0 = new TH2D("Layer0","",9,-0.5,8.5,12,-0.5,11.5);
	Layer0->GetXaxis()->SetTitle("Chip");
	Layer0->GetYaxis()->SetTitle("Stave");
	Layer0->SetTitle("Layer 0");



	TH2D * Layer1 = new TH2D("Layer1","",9,-0.5,8.5,16,-0.5,15.5);
	Layer1->GetXaxis()->SetTitle("Chip");
	Layer1->GetYaxis()->SetTitle("Stave");
	Layer1->SetTitle("Layer 1");


	TH2D * Layer2 = new TH2D("Layer2","",9,-0.5,8.5,20,-0.5,19.5);
	Layer2->GetXaxis()->SetTitle("Chip");
	Layer2->GetYaxis()->SetTitle("Stave");
	Layer2->SetTitle("Layer 2");

	int NTracklets = MissingChip->GetEntries();


	float CosTheta;

		
	float tpcseedpxall;
	float tpcseedpyall;
	float tpcseedpzall;

	float siseedpxall;
	float siseedpyall;
	float siseedpzall;





	for(int i = 0; i < NTracklets; i++){

		MissingChip->GetEntry(i);
		DeltaZ = siz - tpcz;
		DeltaX = six - tpcx;
		DeltaY = siy - tpcy;

		quality = chisq/ndf;

	//	CosTheta = (tpcseedpxall * siseedpxall  + tpcseedpyall  * siseedpyall  + tpcseedpzall  * siseedpzall )/(sqrt(tpcseedpxall  * tpcseedpxall  + tpcseedpyall  *tpcseedpyall  + tpcseedpzall  * tpcseedpzall ) * sqrt(siseedpxall  * siseedpxall  + siseedpyall  * siseedpyall  + siseedpzall  * siseedpzall ));
		CosTheta = (tpcseedpx * siseedpx  + tpcseedpy  * siseedpy  + tpcseedpz  * siseedpz )/(sqrt(tpcseedpx  * tpcseedpx  + tpcseedpy  *tpcseedpy  + tpcseedpz  * tpcseedpz ) * sqrt(siseedpx  * siseedpx  + siseedpy  * siseedpy  + siseedpz  * siseedpz ));

		if(nmapslayer != 2) continue;
		Layer[0]->Fill(layerindex);

		if(ndf != -1 && poormvtxclus == 0 && poorinttclus == 0) Layer[1]->Fill(layerindex);
		if(ndf != -1 && poormvtxclus == 0 && poorinttclus == 0 && trackpt > 0.5 && abs(DeltaZ) < 5)  Layer[2]->Fill(layerindex);
		if(ndf != -1 && poormvtxclus == 0 && poorinttclus == 0 && trackpt > 0.5 && abs(DeltaZ) < 5 && quality < 100)  Layer[3]->Fill(layerindex);
		if(ndf != -1 && CosTheta > 0.9  && trackpt > 0.5 && abs(DeltaZ) < 5  && quality <  20)  Layer[4]->Fill(layerindex);
		if(ndf != -1 && quality < 10 && trackpt > 0.5 && CosTheta > 0.9 ) Layer[5]->Fill(layerindex);


		if(layerindex == 0) Layer0->Fill(chipindex,staveindex);
		if(layerindex == 1) Layer1->Fill(chipindex,staveindex);
		if(layerindex == 2) Layer2->Fill(chipindex,staveindex);

		MissingStat->Fill(layerindex);

	}

	c->cd();


	Layer0->Draw("COLZ");
	c->SaveAs("Plots/Layer0.png");

	Layer1->Draw("COLZ");
	c->SaveAs("Plots/Layer1.png");

	Layer2->Draw("COLZ");
	c->SaveAs("Plots/Layer2.png");


	for(int q = 0; q < NTrial; q++){
		Layer[q]->Draw("hist");
		c->SaveAs(Form("Plots/Loop/MissingLayer_%d.png",q));
	}


	MissingStat->Draw("ep");
	c->SaveAs("Plots/MissingStat.png");


	int layerfired;
	int stavefired;
	int chipfired;

	int ndfall;
	float chisqall;
	float sizall;
	float tpczall;
	float DeltaZall;
	float trackptall;

	int poormvtxclusall;
	int poorinttclusall;

	float qualityall;

	int ninttlayerall;

	TTree * TotalTrack = (TTree *) fin->Get("TotalTrack");



	TotalTrack->SetBranchAddress("layerfired",&layerfired);
	TotalTrack->SetBranchAddress("stavefired",&stavefired);
	TotalTrack->SetBranchAddress("chipfired",&chipfired);
	TotalTrack->SetBranchAddress("ndf",&ndfall);
	TotalTrack->SetBranchAddress("chisq",&chisqall);
	TotalTrack->SetBranchAddress("siz",&sizall);
	TotalTrack->SetBranchAddress("tpcz",&tpczall);
	TotalTrack->SetBranchAddress("poormvtxclus",&poormvtxclusall);
	TotalTrack->SetBranchAddress("poorinttclus",&poorinttclusall);
	TotalTrack->SetBranchAddress("trackpt",&trackptall);
	TotalTrack->SetBranchAddress("ninttlayer",&ninttlayerall);


	TotalTrack->SetBranchAddress("tpcseedpx",&tpcseedpxall);
	TotalTrack->SetBranchAddress("tpcseedpy",&tpcseedpyall);
	TotalTrack->SetBranchAddress("tpcseedpz",&tpcseedpzall);

	TotalTrack->SetBranchAddress("siseedpx",&siseedpxall);
	TotalTrack->SetBranchAddress("siseedpy",&siseedpyall);
	TotalTrack->SetBranchAddress("siseedpz",&siseedpzall);

	TH1D * FiredLayer = new TH1D("FiredLayer","Fired + Missing Layer Distribution",3,-0.5,2.5);
	FiredLayer->GetXaxis()->SetTitle("Layer");
	FiredLayer->GetYaxis()->SetTitle("Number of Tracklets");
	FiredLayer->SetTitle("Missing + Fired Layer Distribution");


	TH2D * FiredLayer0 = new TH2D("FiredLayer0","Fired + Missing Layer 0",9,-0.5,8.5,12,-0.5,11.5);
	FiredLayer0->GetXaxis()->SetTitle("Chip");
	FiredLayer0->GetYaxis()->SetTitle("Stave");
	FiredLayer0->SetTitle("Layer 0");



	TH2D * FiredLayer1 = new TH2D("FiredLayer1","Fired + Missing Layer 1",9,-0.5,8.5,16,-0.5,15.5);
	FiredLayer1->GetXaxis()->SetTitle("Chip");
	FiredLayer1->GetYaxis()->SetTitle("Stave");
	FiredLayer1->SetTitle("Layer 1");


	TH2D * FiredLayer2 = new TH2D("FiredLayer2","Fired + Missing Layer 2",9,-0.5,8.5,20,-0.5,19.5);
	FiredLayer2->GetXaxis()->SetTitle("Chip");
	FiredLayer2->GetYaxis()->SetTitle("Stave");
	FiredLayer2->SetTitle("Layer 2");

	int NTrackletsFired = TotalTrack->GetEntries();


	int NTrackletPassed[NTrial];

	for(int q = 0; q < NTrial; q++){
	
		NTrackletPassed[q] = 0;

	}

	for(int i = 0; i < NTrackletsFired; i++){

		TotalTrack->GetEntry(i);

		DeltaZall = sizall - tpczall;
		qualityall = chisqall/ndfall;


		CosTheta = (tpcseedpxall * siseedpxall  + tpcseedpyall  * siseedpyall  + tpcseedpzall  * siseedpzall )/(sqrt(tpcseedpxall  * tpcseedpxall  + tpcseedpyall  *tpcseedpyall  + tpcseedpzall  * tpcseedpzall ) * sqrt(siseedpxall  * siseedpxall  + siseedpyall  * siseedpyall  + siseedpzall  * siseedpzall ));

		//cout << "DeltaZall = " << DeltaZall << endl;

		NTrackletPassed[0]++;
		if(ndfall != -1 && poormvtxclusall == 0 && poorinttclusall == 0)  NTrackletPassed[1]++;
		if(ndfall != -1 && poormvtxclusall == 0 && poorinttclusall == 0 && trackptall > 0.5 && abs(DeltaZall) < 5)  NTrackletPassed[2]++;
		if(ndfall != -1 && poormvtxclusall == 0 && poorinttclusall == 0 && trackptall > 0.5 && abs(DeltaZall) < 5 && qualityall < 100)  NTrackletPassed[3]++;
		if(ndf != -1 && CosTheta > 0.9  && trackptall > 0.5 && abs(DeltaZall) < 5 && qualityall <  20)  NTrackletPassed[4]++;
		if(ndf != -1 && qualityall <  10 && trackptall > 0.5 && CosTheta > 0.9 )  NTrackletPassed[5]++;

		//	if(ndfall == -1) continue;
		//	if(ndfall == -1 && trackptall > 0.5 && abs(DeltaZall) < 5) continue; //Cut
		//if(ndfall == -1 && trackptall < 0.5 && abs(DeltaZall) > 5) continue; //Cut

		//NTrackletPassed++;

		if(layerfired == 0) FiredLayer0->Fill(chipfired,stavefired);
		if(layerfired == 1) FiredLayer1->Fill(chipfired,stavefired);
		if(layerfired == 2) FiredLayer2->Fill(chipfired,stavefired);

		//MissingStat->Fill(layerindex);

	}

	c->cd();
	FiredLayer0->Draw("COLZ");
	c->SaveAs("Plots/FiredLayer0.png");

	FiredLayer1->Draw("COLZ");
	c->SaveAs("Plots/FiredLayer1.png");

	FiredLayer2->Draw("COLZ");
	c->SaveAs("Plots/FiredLayer2.png");


	FiredLayer->Draw("hist");
	c->SaveAs("Plots/FiredLayer.png");



	for(int q = 0; q < NTrial; q++){

		cout <<  "   NTrackletPassed[q] = " << NTrackletPassed[q] << endl; 
	
		cout << "Layer[q]->Integral() = " << Layer[q]->Integral()  << endl;
	}

	TH1D * LayerEff [NTrial];


	TLegend * leg = new TLegend(0.13,0.65,0.40,0.90,NULL,"brNDC");
	leg->SetBorderSize(0);
	leg->SetTextSize(0.040);
	leg->SetTextFont(42);
	leg->SetFillStyle(0);
	leg->SetLineWidth(2);


	TString Name[NTrial] = {"No Selection","Cluster Quality Cut","Track p_{T} > 0.5 and |#Delta z| < 5", "Quality < 100","Open Angle Cos(#theta) > 0.9","Track p_{T} > 0.5 and Quality < 5"};

	for(int q = 0; q < NTrial; q++){

		LayerEff[q] = (TH1D *) Layer[q]->Clone(Form("LayerEff_%d",q));

		LayerEff[q]->GetXaxis()->CenterTitle();
		LayerEff[q]->GetYaxis()->CenterTitle();
		LayerEff[q]->GetYaxis()->SetTitle("MVTX Layer Efficiency");
		LayerEff[q]->GetYaxis()->SetTitleOffset(1.4);

		leg->AddEntry(LayerEff[q],Name[q].Data(),"LP");
	}
	//LayerEff->Divide(FiredLayer);

	for(int q = 0; q < NTrial; q++){


		for(int i = 0; i < 4; i++){

//			cout << "LayerEff[q]->Integral(): "<< LayerEff[q]->Integral() << endl;

			float Value = Layer[q]->GetBinContent(i+1)/NTrackletPassed[q];
			float Eff =  1 - Value;
	
		//	cout << "q = " << q << "   i = " << i << "   Eff = " << Eff << "   NTrackletPassed[q] = " << NTrackletPassed[q] << endl; 

			LayerEff[q]->SetBinContent(i+1,Eff);

		}
	}

	LayerEff[0]->SetTitle("Overall MVTX Layer Efficiency");

	LayerEff[0]->SetMaximum(1.3);
	LayerEff[0]->SetMinimum(0.7);


	LayerEff[0]->Draw("hist");


	for(int q = 1; q < NTrial; q++){
		
		LayerEff[q]->Draw("histSAME");

	}

	leg->Draw("SAME");

	c->SaveAs("Plots/Loop/LayerEff.png");




}
