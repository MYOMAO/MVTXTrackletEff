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



void SelectBestTPC(int Type){

	gStyle->SetOptStat(0);
	TCanvas * c = new TCanvas("c","c",600,600);
	c->cd();

	TString Name;

	if(Type == 0) Name = "FullTrackFit";
	if(Type == 1) Name = "MisMatchedTrackFit";



	//TString infile = "CutFiles/Run53285.root";
	TString infile = Form("SiCut/%s.root",Name.Data());



	TFile * fin = new TFile(infile.Data());
	fin->cd();


	//TString outfile = "CutFilesFinal/Run53285.root";
	TString outfile = Form("TPCCut/%s.root",Name.Data());
	
	TFile * fout = new TFile(outfile.Data(),"RECREATE");


	const int NTree = 2;


	TString TreeName[NTree] = {"TotalTrack","MissingChip"};

	float tpcseedpt;
	float chisq;
	int ndf;
	float quality;

	int event;

	int eventPre = 0;

	float six;
	float siy;
	float siz;

	
	float tpcx;
	float tpcy;
	float tpcz;

	float dx;
	float dy;
	float dz;

	//float tpcseedpt;

	TFile * SaveHis = new TFile("SaveHisTPC.root","RECREATE");
	SaveHis->cd();


	for(int q = 0; q < NTree; q++){

		TTree * TotalTrack = (TTree *) fin->Get(TreeName[q].Data());

		TotalTrack->SetBranchAddress("Event",&event);
		TotalTrack->SetBranchAddress("tpcseedpt",&tpcseedpt);
		TotalTrack->SetBranchAddress("tpcseedpt",&tpcseedpt);
		
		TotalTrack->SetBranchAddress("chisq",&chisq);
		TotalTrack->SetBranchAddress("ndf",&ndf);
		TotalTrack->SetBranchAddress("six",&six);
		TotalTrack->SetBranchAddress("siy",&siy);
		TotalTrack->SetBranchAddress("siz",&siz);
		TotalTrack->SetBranchAddress("tpcx",&tpcx);
		TotalTrack->SetBranchAddress("tpcy",&tpcy);
		TotalTrack->SetBranchAddress("tpcz",&tpcz);

		TH1D * ReuseSiSeed = new TH1D("ReuseSiSeed","",100,0,200);
		ReuseSiSeed->GetXaxis()->SetTitle("Number of TPC Seeds 1 Silicon Seed Matched to");
		ReuseSiSeed->GetYaxis()->SetTitle("Number of Tracks");
		ReuseSiSeed->GetXaxis()->CenterTitle();
		ReuseSiSeed->GetYaxis()->CenterTitle();
		ReuseSiSeed->GetYaxis()->SetTitleOffset(1.4);

		ReuseSiSeed->SetMarkerStyle(20);
		ReuseSiSeed->SetMarkerSize(1);
		ReuseSiSeed->SetMarkerColor(1);
		ReuseSiSeed->SetLineColor(1);




		TH1D * DeltaX = new TH1D("DeltaX","",100,-10,10);
		DeltaX->GetXaxis()->SetTitle("Silicon Seed x - TPC Seed x: #Delta x (cm)");
		DeltaX->GetYaxis()->SetTitle("Normalized Counts");
		DeltaX->GetXaxis()->CenterTitle();
		DeltaX->GetYaxis()->CenterTitle();
		DeltaX->GetYaxis()->SetTitleOffset(1.4);

		DeltaX->SetMarkerStyle(20);
		DeltaX->SetMarkerSize(1);
		DeltaX->SetMarkerColor(2);
		DeltaX->SetLineColor(2);

		TH1D * DeltaXAfter = (TH1D *) DeltaX->Clone("DeltaXAfter");
		DeltaXAfter->SetMarkerColor(4);
		DeltaXAfter->SetLineColor(4);

		TH1D * DeltaY = new TH1D("DeltaY","",100,-10,10);
		DeltaY->GetXaxis()->SetTitle("Silicon Seed y - TPC Seed y: #Delta y (cm)");
		DeltaY->GetYaxis()->SetTitle("Normalized Counts");
		DeltaY->GetXaxis()->CenterTitle();
		DeltaY->GetYaxis()->CenterTitle();
		DeltaY->GetYaxis()->SetTitleOffset(1.4);

		DeltaY->SetMarkerStyle(20);
		DeltaY->SetMarkerSize(1);
		DeltaY->SetMarkerColor(2);
		DeltaY->SetLineColor(2);

		TH1D * DeltaYAfter = (TH1D *) DeltaY->Clone("DeltaYAfter");
		DeltaYAfter->SetMarkerColor(4);
		DeltaYAfter->SetLineColor(4);

		TH1D * DeltaZ = new TH1D("DeltaZ","",100,-100,100);
		DeltaZ->GetXaxis()->SetTitle("Silicon Seed z - TPC Seed z: #Delta z (cm)");
		DeltaZ->GetYaxis()->SetTitle("Normalized Counts");
		DeltaZ->GetXaxis()->CenterTitle();
		DeltaZ->GetYaxis()->CenterTitle();
		DeltaZ->GetYaxis()->SetTitleOffset(1.4);

		DeltaZ->SetMarkerStyle(20);
		DeltaZ->SetMarkerSize(1);
		DeltaZ->SetMarkerColor(2);
		DeltaZ->SetLineColor(2);

		TH1D * DeltaZAfter = (TH1D *) DeltaZ->Clone("DeltaZAfter");
		DeltaZAfter->SetMarkerColor(4);
		DeltaZAfter->SetLineColor(4);

		TH1D * NTracks = new TH1D("NTracks","",200,0,200);
		NTracks->GetXaxis()->SetTitle("Number of Tracks at Crossing = 0");
		NTracks->GetYaxis()->SetTitle("Number of Crossings");
		NTracks->GetXaxis()->CenterTitle();
		NTracks->GetYaxis()->CenterTitle();
		NTracks->GetYaxis()->SetTitleOffset(1.4);
		NTracks->SetMarkerStyle(20);
		NTracks->SetMarkerSize(1);
		NTracks->SetMarkerColor(2);
		NTracks->SetLineColor(2);


		TH1D * NTracksAfter = (TH1D *) NTracks->Clone("NTracksAfter");
		NTracksAfter->SetMarkerColor(4);
		NTracksAfter->SetLineColor(4);

		int NTracklets = TotalTrack->GetEntries();

		std::map<float, std::pair<int, float>> BestFit;
		std::map<float, int> siseedused;

		std::vector<int> BestTracksEntry;

		int NumberofTracks = 0;

		for(int i = 0; i < NTracklets; i++){

			TotalTrack->GetEntry(i);
	
			NumberofTracks++;

			dx = six - tpcx;
			dy = siy - tpcy;
			dz = siz - tpcz;
	

			DeltaX->Fill(dx);
			DeltaY->Fill(dy);
			DeltaZ->Fill(dz);

			if(eventPre != event){



				NTracks->Fill(NumberofTracks);
				

				std::cout << "OK DONE with Event: " << eventPre << endl;

				for (const auto& pair : BestFit) {

					int Entry = pair.second.first;
					float chi_ndf = pair.second.second;

					if(chi_ndf != 999999) BestTracksEntry.push_back(Entry);
				}

				for (const auto& pair : siseedused) {
					//  std::cout << "tpcseedpt: " << pair.first << ", count: " << pair.second << "\n";

					int UsedCount = pair.second;
					ReuseSiSeed->Fill(UsedCount);
				}

				BestFit.clear();
				siseedused.clear();
				NumberofTracks = 0;


				//std::cout << "----------------------------------------" << endl;


			}


			if(ndf > 0) quality = abs(chisq/ndf - 1);
			else quality = 999999;


			siseedused[tpcseedpt]++;

			if (BestFit.find(tpcseedpt) == BestFit.end() || 
					std::abs(BestFit[tpcseedpt].second - 1) > quality) {
				BestFit[tpcseedpt] = {i, quality};
			}

			eventPre = event;
		}

		/*

		   std::cout << "OK DONE with the Last Event: " << eventPre << endl;

		   for (const auto& pair : BestFit) {
		   std::cout << "Row: " << pair.second.first 
		   << ", tpcseedpt: " << pair.first 
		   << ", chisq/ndf: " << pair.second.second << "\n";

		   }

		   BestFit.clear();

		   std::cout << "----------------------------------------" << endl;

*/


		for (const auto& pair : BestFit) {

			int Entry = pair.second.first;

			BestTracksEntry.push_back(Entry);
		}

		for (const auto& pair : siseedused) {
			//  std::cout << "tpcseedpt: " << pair.first << ", count: " << pair.second << "\n";

			int UsedCount = pair.second;
			ReuseSiSeed->Fill(UsedCount);
		}

		BestFit.clear();
		siseedused.clear();




		int TotalBestTrack = BestTracksEntry.size();



		fout->cd();

		TTree * TotalTrackNew = TotalTrack->CloneTree(0);


		NumberofTracks = 0;

		for(int i = 0; i < TotalBestTrack; i++){

			TotalTrack->GetEntry(BestTracksEntry[i]);


			dx = six - tpcx;
			dy = siy - tpcy;
			dz = siz - tpcz;
	

			DeltaXAfter->Fill(dx);
			DeltaYAfter->Fill(dy);
			DeltaZAfter->Fill(dz);

			TotalTrackNew->Fill();

			if(eventPre != event){
			
				NTracksAfter->Fill(NumberofTracks);
				NumberofTracks = 0;
			}
		
			NumberofTracks++;

			eventPre = event;
			

		}



		c->cd();


		ReuseSiSeed->Draw("ep");

		c->SaveAs(Form("SelPlotTPC/ReuseSiSeed_%d.png",q));

		
		TLegend * leg = new TLegend(0.13,0.65,0.40,0.90,NULL,"brNDC");
		leg->SetBorderSize(0);
		leg->SetTextSize(0.040);
		leg->SetTextFont(42);
		leg->SetFillStyle(0);
		leg->SetLineWidth(2);


		DeltaXAfter->Scale(1.0/DeltaXAfter->Integral());
		DeltaX->Scale(1.0/DeltaX->Integral());
		

		leg->AddEntry(DeltaX,"Before Best Quality Selection","LP");
		leg->AddEntry(DeltaXAfter,"After Best Quality Selection","LP");

		DeltaXAfter->Draw("ep");
		DeltaX->Draw("epSAME");
		leg->Draw("SAME");

		c->SaveAs(Form("SelPlotTPC/Match/DeltaX_%d.png",q));
	


		DeltaYAfter->Scale(1.0/DeltaYAfter->Integral());
		DeltaY->Scale(1.0/DeltaY->Integral());

	
		DeltaYAfter->Draw("ep");
		DeltaY->Draw("epSAME");
		leg->Draw("SAME");


		c->SaveAs(Form("SelPlotTPC/Match/DeltaY_%d.png",q));
	


		DeltaZAfter->Scale(1.0/DeltaZAfter->Integral());
		DeltaZ->Scale(1.0/DeltaZ->Integral());

	
		DeltaZAfter->Draw("ep");
		DeltaZ->Draw("epSAME");
		leg->Draw("SAME");




		c->SaveAs(Form("SelPlotTPC/Match/DeltaZ_%d.png",q));
		



		DeltaXAfter->Scale(1.0/DeltaXAfter->Integral());
		DeltaX->Scale(1.0/DeltaX->Integral());
		


		
		TLegend * leg2 = new TLegend(0.13,0.65,0.40,0.90,NULL,"brNDC");
		leg2->SetBorderSize(0);
		leg2->SetTextSize(0.040);
		leg2->SetTextFont(42);
		leg2->SetFillStyle(0);
		leg2->SetLineWidth(2);


		float NchMean = NTracks->GetMean();
		float NchMeanAfter = NTracksAfter->GetMean();

		leg2->AddEntry(NTracks,Form("Before Best Quality Selection: <N_{ch}> = %0.1f",NchMean),"LP");
		leg2->AddEntry(NTracksAfter,Form("After Best Quality Selection: <N_{ch}> = %0.1f",NchMeanAfter),"LP");

		NTracksAfter->Scale(1.0/NTracksAfter->Integral());
		NTracks->Scale(1.0/NTracks->Integral());


		NTracksAfter->Draw("ep");
		NTracks->Draw("epSAME");
		leg2->Draw("epSAME");
		c->SaveAs(Form("SelPlotTPC/Match/NTrack_%d.png",q));
	
		fout->cd();
		TotalTrackNew->Write();

		SaveHis->cd();
		ReuseSiSeed->Write();
		DeltaXAfter->Write();
		DeltaX->Write();
		DeltaYAfter->Write();
		DeltaY->Write();
		DeltaZAfter->Write();
		DeltaZ->Write();
		NTracksAfter->Write();
		NTracks->Write();


	}


	fout->Close();



}
