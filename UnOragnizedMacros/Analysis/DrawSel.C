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


void DrawSel(){

	gStyle->SetOptStat(0);
	TCanvas * c = new TCanvas("c","c",600,600);
	c->cd();
	c->SetLeftMargin(0.17);


	TFile * fin = new TFile("SaveHis.root");
	
	fin->cd();


	TH1D * ReuseSiSeed =(TH1D *) fin->Get("ReuseSiSeed");
	ReuseSiSeed->SetMarkerColor(kBlack);
	ReuseSiSeed->SetLineColor(kBlack);



	TH1D * DeltaX = (TH1D *) fin->Get("DeltaX");
	TH1D * DeltaXAfter =(TH1D *) fin->Get("DeltaXAfter");


	TH1D * DeltaY = (TH1D *) fin->Get("DeltaY");
	TH1D * DeltaYAfter =(TH1D *) fin->Get("DeltaYAfter");


	TH1D * DeltaZ = (TH1D *) fin->Get("DeltaZ");
	TH1D * DeltaZAfter =(TH1D *) fin->Get("DeltaZAfter");


	TH1D * NTracks = (TH1D *) fin->Get("NTracks");
	TH1D * NTracksAfter =(TH1D *) fin->Get("NTracksAfter");


	TFile * fin2 = new TFile("SaveHisTPC.root");
	TH1D * ReuseSiSeed2 = (TH1D *) fin2->Get("ReuseSiSeed");
	ReuseSiSeed2->SetMarkerColor(kRed);
	ReuseSiSeed2->SetLineColor(kRed);
	TH1D * DeltaXAfter2 = (TH1D *) fin2->Get("DeltaXAfter");
	TH1D * DeltaYAfter2 =(TH1D *) fin2->Get("DeltaYAfter");
	TH1D * DeltaZAfter2 =(TH1D *) fin2->Get("DeltaZAfter");

	TH1D * NTracksAfter2 =(TH1D *) fin2->Get("NTracksAfter");

	c->cd();

	c->SetLogy();
	ReuseSiSeed->GetYaxis()->SetTitleOffset(2.2);

	ReuseSiSeed->SetMaximum(20000);

	ReuseSiSeed->Draw("ep");
	ReuseSiSeed2->Draw("epSAME");

		
	TLegend * leg = new TLegend(0.23,0.75,0.80,0.90,NULL,"brNDC");
	leg->SetBorderSize(0);
	leg->SetTextSize(0.040);
	leg->SetTextFont(42);
	leg->SetFillStyle(0);
	leg->SetLineWidth(2);


	leg->AddEntry(ReuseSiSeed,"Before Silicon Seed Selection","LP");
	leg->AddEntry(ReuseSiSeed2,"Before TPC Seed Selection","LP");

	leg->Draw("SAME");

	c->SaveAs("IncSelPlot/ReuseSiSeed.png");


		
	TLegend * leg2 = new TLegend(0.23,0.68,0.80,0.90,NULL,"brNDC");
	leg2->SetBorderSize(0);
	leg2->SetTextSize(0.040);
	leg2->SetTextFont(42);
	leg2->SetFillStyle(0);
	leg2->SetLineWidth(2);


	leg2->AddEntry(DeltaX,"Before Silicon Seed Selection","LP");
	leg2->AddEntry(DeltaXAfter,"Before TPC Seed Selection","LP");
	leg2->AddEntry(DeltaXAfter2,"After Silicon + TPC Selections","LP");
	

	DeltaX->SetMaximum(0.08);

	DeltaX->Draw("ep");
	DeltaXAfter->Draw("epSAME");
	DeltaXAfter2->Draw("epSAME");

	leg2->Draw("SAME");

	c->SaveAs("IncSelPlot/DeltaX.png");


	DeltaY->SetMaximum(0.08);

	DeltaY->Draw("ep");
	DeltaYAfter->Draw("epSAME");
	DeltaYAfter2->Draw("epSAME");

	leg2->Draw("SAME");

	c->SaveAs("IncSelPlot/DeltaY.png");


	DeltaZ->SetMaximum(0.15);

	DeltaZ->Draw("ep");
	DeltaZAfter->Draw("epSAME");
	DeltaZAfter2->Draw("epSAME");

	leg2->Draw("SAME");

	c->SaveAs("IncSelPlot/DeltaZ.png");



	NTracks->SetMaximum(1.2);

	TLegend * leg3 = new TLegend(0.23,0.72,0.60,0.90,NULL,"brNDC");
	leg3->SetBorderSize(0);
	leg3->SetTextSize(0.032);
	leg3->SetTextFont(42);
	leg3->SetFillStyle(0);
	leg3->SetLineWidth(2);

	float NchMean = NTracks->GetMean();
	float Nch1Mean = NTracksAfter->GetMean();
	float Nch2Mean = NTracksAfter2->GetMean();

	leg3->AddEntry(NTracks,Form("Before Silicon Seed Selection: <N_{ch}> = %.1f",NchMean),"LP");
	leg3->AddEntry(NTracksAfter,Form("Before TPC Seed Selection: <N_{ch}> = %.1f",Nch1Mean),"LP");
	leg3->AddEntry(NTracksAfter2,Form("After Silicon + TPC Selections: <N_{ch}> = %.1f",Nch2Mean),"LP");
	

	NTracks->GetXaxis()->SetTitle("Number of Tracks at Crossing = 0 (N_{ch})");
	NTracks->GetYaxis()->SetTitleOffset(2.2);

	NTracks->Draw("ep");
	NTracksAfter->Draw("epSAME");
	NTracksAfter2->Draw("epSAME");

	leg3->Draw("SAME");

	c->SaveAs("IncSelPlot/NTrack.png");

	c->SetLogy();
	c->SaveAs("IncSelPlot/NTrackLog.png");


}
