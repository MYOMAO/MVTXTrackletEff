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



void EffAna(int Run, int Align){



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

	int nmaps;

	float tpcseedpx;
	float tpcseedpy;
	float tpcseedpz;

	float siseedpx;
	float siseedpy;
	float siseedpz;
	float siseedpt;

	int TotalClusSize;

	std::vector<float> * ClusPosX = 0;
	std::vector<float> * ClusPosY = 0;
	std::vector<float> * ClusPosZ = 0;


	TString infile;

	if(Align == 0) infile = Form("../CleanWFNew/Outfile/Clean2_%d.root",Run);
	if(Align == 1) infile = Form("Outfile/Clean2_%d.root",Run);

	//	TFile * fin = new TFile("Files/SiTrackFit.root");

	//TFile * fin = new TFile(Form("Outfile/Clean2_%d.root",Run));

	TFile * fin = new TFile(infile.Data());

	TTree * TotalTrack = (TTree *) fin->Get("TotalTrack");



	TotalTrack->SetBranchAddress("layerindex_miss",&layerindex);
	TotalTrack->SetBranchAddress("staveindex_miss",&staveindex);
	TotalTrack->SetBranchAddress("chipindex_miss",&chipindex);
	TotalTrack->SetBranchAddress("ndf",&ndf);
	TotalTrack->SetBranchAddress("chisq",&chisq);
	TotalTrack->SetBranchAddress("six",&six);
	TotalTrack->SetBranchAddress("tpcx",&tpcx);
	TotalTrack->SetBranchAddress("siy",&siy);
	TotalTrack->SetBranchAddress("tpcy",&tpcy);

	TotalTrack->SetBranchAddress("siz",&siz);
	TotalTrack->SetBranchAddress("tpcz",&tpcz);
	TotalTrack->SetBranchAddress("poormvtxclus",&poormvtxclus);
	TotalTrack->SetBranchAddress("poorinttclus",&poorinttclus);

	TotalTrack->SetBranchAddress("trackpt",&trackpt);


	TotalTrack->SetBranchAddress("ninttlayer",&ninttlayer);
	TotalTrack->SetBranchAddress("nmapslayer",&nmapslayer);
	TotalTrack->SetBranchAddress("nmaps",&nmaps);


	TotalTrack->SetBranchAddress("tpcseedpx",&tpcseedpx);
	TotalTrack->SetBranchAddress("tpcseedpy",&tpcseedpy);
	TotalTrack->SetBranchAddress("tpcseedpz",&tpcseedpz);

	TotalTrack->SetBranchAddress("siseedpx",&siseedpx);
	TotalTrack->SetBranchAddress("siseedpy",&siseedpy);
	TotalTrack->SetBranchAddress("siseedpz",&siseedpz);

	TotalTrack->SetBranchAddress("siseedpt",&siseedpt);

	TotalTrack->SetBranchAddress("TotalClusSize",&TotalClusSize);
	TotalTrack->SetBranchAddress("ClusPosX",&ClusPosX);
	TotalTrack->SetBranchAddress("ClusPosY",&ClusPosY);
	TotalTrack->SetBranchAddress("ClusPosZ",&ClusPosZ);


	float X;
	float Y;
	float Z;


	float Dx;
	float Dy;
	float Dz;

	float Mom;

	float Dis;

	float DevX;
	float DevY;
	float DevZ;


	bool PassClus;


	const int NTrial = 4;

	int NPhiBin = 20;
		
	int NPtBin = 40;

	int NTrackletPassed[NTrial];

	TH1D * Layer[NTrial]; 
	TH1D * PhiMiss[NTrial]; 
	TH1D * PhiTotal[NTrial]; 
	TH1D * PhiEff[NTrial]; 

	TH1D * PtMiss[NTrial]; 
	TH1D * PtTotal[NTrial]; 
	TH1D * PtEff[NTrial]; 


	for(int i = 0; i < NTrial; i++){

		Layer[i] = new TH1D("Layer","",3,-0.5,2.5);
		Layer[i]->GetXaxis()->SetTitle("Layer");
		Layer[i]->GetYaxis()->SetTitle("Number of Tracklets");
		Layer[i]->SetTitle("Missing Layer Distribution");
		Layer[i]->SetLineColor(i+1);
		Layer[i]->SetLineWidth(2);


		PhiEff[i] = new TH1D(Form("PhiEff_%d",i),"",NPhiBin,-3.2,3.2);
		PhiEff[i]->GetXaxis()->SetTitle("Silicon Seed #phi Angle (Rad)");
		PhiEff[i]->GetYaxis()->SetTitle("Efficiency");
		PhiEff[i]->SetLineColor(i+1);
		PhiEff[i]->SetLineWidth(1);
		PhiEff[i]->SetMarkerColor(i+1);
		PhiEff[i]->SetMarkerStyle(20);


		PhiMiss[i] = new TH1D(Form("PhiMiss_%d",i),"",NPhiBin,-3.2,3.2);
		PhiMiss[i]->GetXaxis()->SetTitle("Silicon Seed #phi Angle (Rad)");
		PhiMiss[i]->GetYaxis()->SetTitle("Number of Tracklets");
		PhiMiss[i]->SetLineColor(i+1);
		PhiMiss[i]->SetLineWidth(2);
		PhiMiss[i]->SetMarkerColor(i+1);
		PhiMiss[i]->SetMarkerStyle(20);

		PhiTotal[i] = new TH1D(Form("PhiTotal_%d",i),"",NPhiBin,-3.2,3.2);
		PhiTotal[i]->GetXaxis()->SetTitle("Silicon Seed #phi Angle (Rad)");
		PhiTotal[i]->GetYaxis()->SetTitle("Number of Tracklets");
		PhiTotal[i]->SetLineColor(i+1);
		PhiTotal[i]->SetLineWidth(2);
		PhiTotal[i]->SetMarkerColor(i+1);
		PhiTotal[i]->SetMarkerStyle(20);


		PtEff[i] = new TH1D(Form("PtEff_%d",i),"",NPtBin,0,20);
		PtEff[i]->GetXaxis()->SetTitle("Silicon Seed p_{T} (GeV/c)");
		PtEff[i]->GetYaxis()->SetTitle("Efficiency");
		PtEff[i]->SetLineColor(i+1);
		PtEff[i]->SetLineWidth(1);
		PtEff[i]->SetMarkerColor(i+1);
		PtEff[i]->SetMarkerStyle(20);


		PtMiss[i] = new TH1D(Form("PtMiss_%d",i),"",NPtBin,0,20);
		PtMiss[i]->GetXaxis()->SetTitle("Silicon Seed p_{T} (GeV/c)");
		PtMiss[i]->GetYaxis()->SetTitle("Number of Tracklets");
		PtMiss[i]->SetLineColor(i+1);
		PtMiss[i]->SetLineWidth(2);
		PtMiss[i]->SetMarkerColor(i+1);
		PtMiss[i]->SetMarkerStyle(20);

		PtTotal[i] = new TH1D(Form("PtTotal_%d",i),"",NPtBin,0,20);
		PtTotal[i]->GetXaxis()->SetTitle("Silicon Seed p_{T} (GeV/c)");
		PtTotal[i]->GetYaxis()->SetTitle("Number of Tracklets");
		PtTotal[i]->SetLineColor(i+1);
		PtTotal[i]->SetLineWidth(2);
		PtTotal[i]->SetMarkerColor(i+1);
		PtTotal[i]->SetMarkerStyle(20);

		NTrackletPassed[i] = 0;

	}

	TH1D * NewQual = new TH1D("NewQual","",100,0,1000);
	NewQual->GetXaxis()->SetTitle("Quality: #chi^{2}/ndf");
	NewQual->GetYaxis()->SetTitle("Counts");



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

	int NTracklets = TotalTrack->GetEntries();




	float phi;
	float CosTheta;

	for(int i = 0; i < NTracklets; i++){

		TotalTrack->GetEntry(i);

		DeltaZ = siz - tpcz;
		quality = chisq/ndf;


		PassClus = true;
		//CosTheta = (tpcseedpx * siseedpx  + tpcseedpy  * siseedpy  + tpcseedpz  * siseedpz )/(sqrt(tpcseedpx  * tpcseedpx  + tpcseedpy  *tpcseedpy  + tpcseedpz  * tpcseedpz ) * sqrt(siseedpx  * siseedpx  + siseedpy  * siseedpy  + siseedpz  * siseedpz ));

		//cout << "DeltaZ = " << DeltaZ << endl;

		NTrackletPassed[0]++;



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
			//if(abs(DevX) > 0.02 || abs(DevY) > 0.02 || abs(DevZ) > 0.02) PassClus = false;
			if(Dis > 1) PassClus = false;

		}


		phi = TMath::ATan2(siseedpy,siseedpx);

		PhiTotal[0]->Fill(phi);
		PtTotal[0]->Fill(trackpt);


		if(nmaps == 2){


			Layer[0]->Fill(layerindex);
			PhiMiss[0]->Fill(phi);
			PhiEff[0]->Fill(phi);	
			PtMiss[0]->Fill(trackpt);
			PtEff[0]->Fill(trackpt);

		}

		if(quality < 100){
			NTrackletPassed[1]++;
			PhiTotal[1]->Fill(phi);
			PtTotal[1]->Fill(trackpt);

			if(nmaps == 2){


				Layer[1]->Fill(layerindex);
				PhiMiss[1]->Fill(phi);
				PhiEff[1]->Fill(phi);
				PtMiss[1]->Fill(trackpt);
				PtEff[1]->Fill(trackpt);

			}

		}

		if(trackpt > 0.5 && quality < 5){
			NTrackletPassed[2]++;
			PhiTotal[2]->Fill(phi);
			PtTotal[2]->Fill(trackpt);


			if(nmaps == 2){


				Layer[2]->Fill(layerindex);
				PhiMiss[2]->Fill(phi);
				PhiEff[2]->Fill(phi);
				PtMiss[2]->Fill(trackpt);
				PtEff[2]->Fill(trackpt);


			}


		}

		if(PassClus == true){
			NewQual->Fill(quality);
			PhiTotal[3]->Fill(phi);
			PtTotal[3]->Fill(trackpt);
		
			NTrackletPassed[3]++;

			if(nmaps == 2){


				Layer[3]->Fill(layerindex);
				PhiMiss[3]->Fill(phi);
				PhiEff[3]->Fill(phi);
				PtMiss[3]->Fill(trackpt);
				PtEff[3]->Fill(trackpt);


			}

		}

		/*	
			if(ndf != -1 && poormvtxclus == 0 && poorinttclus == 0)  NTrackletPassed[1]++;
			if(ndf != -1 && poormvtxclus == 0 && poorinttclus == 0 && trackpt > 0.5 && abs(DeltaZ) < 5)  NTrackletPassed[2]++;
			if(ndf != -1 && poormvtxclus == 0 && poorinttclus == 0 && trackpt > 0.5 && abs(DeltaZ) < 5 && quality < 100)  NTrackletPassed[3]++;
			if(ndf != -1 && CosTheta > 0.9  && trackpt > 0.5 )  NTrackletPassed[4]++;
			*/
		//	if(ndf == -1) continue;
		//	if(ndf == -1 && trackpt > 0.5 && abs(DeltaZ) < 5) continue; //Cut
		//if(ndf == -1 && trackpt < 0.5 && abs(DeltaZ) > 5) continue; //Cut

		//NTrackletPassed++;
		/*
		   if(layerfired == 0) FiredLayer0->Fill(chipfired,stavefired);
		   if(layerfired == 1) FiredLayer1->Fill(chipfired,stavefired);
		   if(layerfired == 2) FiredLayer2->Fill(chipfired,stavefired);
		   */
		//MissingStat->Fill(layerindex);

	}
	/*
	   c->cd();
	   FiredLayer0->Draw("COLZ");
	   c->SaveAs("Plots/FiredLayer0.png");

	   FiredLayer1->Draw("COLZ");
	   c->SaveAs("Plots/FiredLayer1.png");

	   FiredLayer2->Draw("COLZ");
	   c->SaveAs("Plots/FiredLayer2.png");


	   FiredLayer->Draw("hist");
	   c->SaveAs("Plots/FiredLayer.png");


	   NewQual->Draw("ep");
	   */
	c->SaveAs("QualPlot/NewQual.png");

	for(int q = 0; q < NTrial; q++){

		cout <<  "   NTrackletPassed[q] = " << NTrackletPassed[q] << endl; 

		cout << "Layer[q]->Integral() = " << Layer[q]->Integral()  << endl;
	}

	TH1D * LayerEff[NTrial];


	TLegend * leg = new TLegend(0.13,0.65,0.40,0.90,NULL,"brNDC");
	leg->SetBorderSize(0);
	leg->SetTextSize(0.040);
	leg->SetTextFont(42);
	leg->SetFillStyle(0);
	leg->SetLineWidth(2);


	//TString Name[NTrial] = {"No Selection","Cluster Quality Cut","Track p_{T} > 0.5 and |#Delta z| < 5", "Quality < 100","Open Angle Cos(#theta) > 0.9"};
	TString Name[NTrial] = {"No Selection - Silicon Seed Only","Quality < 100","p_{T} > 0.5 GeV/c and Quality < 5","Cluster to Track Distance Cut with 200 #mum"};

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

	LayerEff[0]->SetTitle("Over MVTX Layer Efficiency");

	LayerEff[0]->SetMaximum(1.2);
	LayerEff[0]->SetMinimum(0.8);


	LayerEff[0]->Draw("hist");


	for(int q = 1; q < NTrial; q++){

		LayerEff[q]->Draw("histSAME");

	}



	leg->Draw("SAME");

	c->SaveAs(Form("Plots/%d/%d/LayerEff.png",Run,Align));






	PhiEff[0]->SetTitle("Over MVTX Efficiency vs #phi");

	PhiEff[0]->SetMaximum(1.2);
	PhiEff[0]->SetMinimum(0.0);


//	PhiEff[0]->Draw("ep");


	TLegend * leg2 = new TLegend(0.13,0.65,0.40,0.90,NULL,"brNDC");
	leg2->SetBorderSize(0);
	leg2->SetTextSize(0.040);
	leg2->SetTextFont(42);
	leg2->SetFillStyle(0);
	leg2->SetLineWidth(2);


	for(int q = 0; q < NTrial; q++){

		PhiEff[q]->Sumw2();
		PhiTotal[q]->Sumw2();

		PhiEff[q]->Divide(PhiTotal[q]);

		for(int i = 0; i < NPhiBin; i++){

			float Eff = 1 -  PhiEff[q]->GetBinContent(i+1);
			float EffErr = PhiEff[q]->GetBinError(i+1);

			PhiEff[q]->SetBinContent(i+1,Eff);
			PhiEff[q]->SetBinError(i+1,EffErr);

		}

		PhiEff[q]->Draw("epSAME");
		leg2->AddEntry(PhiEff[q],Name[q].Data(),"LP");


		if(q == 0) 	PtEff[q]->Draw("ep");
		if(q > 0) PtEff[q]->Draw("epSAME");


	}


	leg2->Draw("SAME");
	c->SaveAs(Form("Plots/%d/%d/PhiEff.png",Run,Align));
	
	



	PtEff[0]->SetTitle("Over MVTX Efficiency vs p_{T}");

	PtEff[0]->SetMaximum(1.2);
	PtEff[0]->SetMinimum(0.0);



	for(int q = 0; q < NTrial; q++){

	
		PtEff[q]->Sumw2();
		PtTotal[q]->Sumw2();
	
		PtEff[q]->Divide(PtTotal[q]);

		for(int i = 0; i < NPtBin; i++){

			float Eff = 1 -  PtEff[q]->GetBinContent(i+1);
			float EffErr = PtEff[q]->GetBinError(i+1);

			PtEff[q]->SetBinContent(i+1,Eff);
			PtEff[q]->SetBinError(i+1,EffErr);

		}

		if(q == 0) 	PtEff[q]->Draw("ep");
		if(q > 0) PtEff[q]->Draw("epSAME");

	}

	leg2->Draw("SAME");
	c->SaveAs(Form("Plots/%d/%d/PtEff.png",Run,Align));


	TString outfile = Form("RootFiles/EffAna_%d_%d.root",Run,Align);

	TFile * fout = new TFile(outfile.Data(),"RECREATE");
	fout->cd();

	for(int q = 0; q < NTrial; q++){
	
		Layer[q]->Write();
		LayerEff[q]->Write();

		PhiTotal[q]->Write();
		PhiMiss[q]->Write();
		PhiEff[q]->Write();

		PtTotal[q]->Write();
		PtMiss[q]->Write();
		PtEff[q]->Write();


	}
	

	fout->Close();

}
