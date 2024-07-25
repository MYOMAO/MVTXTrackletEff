#include <trackingdiagnostics/TrackResiduals.h>

#include <trackbase/ClusterErrorPara.h>
#include <trackbase/InttDefs.h>
#include <trackbase/MvtxDefs.h>
#include <trackbase/TpcDefs.h>
#include <trackbase/TrackFitUtils.h>
#include <trackbase/TrkrCluster.h>
#include <trackbase/TrkrClusterContainer.h>
#include <trackbase/TrkrHit.h>
#include <trackbase/TrkrHitSet.h>
#include <trackbase/TrkrHitSetContainer.h>

#include <g4detectors/PHG4CylinderGeomContainer.h>
#include <g4detectors/PHG4TpcCylinderGeom.h>
#include <g4detectors/PHG4TpcCylinderGeomContainer.h>

#include <intt/CylinderGeomIntt.h>

#include <micromegas/CylinderGeomMicromegas.h>
#include <micromegas/MicromegasDefs.h>

#include <mvtx/CylinderGeom_Mvtx.h>

#include <trackbase_historic/ActsTransformations.h>
#include <trackbase_historic/SvtxAlignmentState.h>
#include <trackbase_historic/SvtxAlignmentStateMap.h>
#include <trackbase_historic/SvtxTrack.h>
#include <trackbase_historic/SvtxTrackMap.h>
#include <trackbase_historic/TrackAnalysisUtils.h>
#include <trackbase_historic/TrackSeed.h>
#include <trackbase_historic/TrackSeedContainer.h>

#include <ffarawobjects/Gl1Packet.h>
#include <ffarawobjects/Gl1RawHit.h>
#include <tpc/TpcDistortionCorrectionContainer.h>
#include <tpc/TpcGlobalPositionWrapper.h>

#include <globalvertex/GlobalVertex.h>
#include <globalvertex/GlobalVertexMap.h>
#include <globalvertex/SvtxVertex.h>
#include <globalvertex/SvtxVertexMap.h>

#include <fun4all/Fun4AllReturnCodes.h>

#include <phool/PHCompositeNode.h>
#include <phool/PHNodeIterator.h>
#include <phool/getClass.h>

#include "MvtxHitEff.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TStyle.h"


#include <limits>
//____________________________________________________________________________..
MvtxHitEff::MvtxHitEff(const std::string &name)
	: SubsysReco(name)
{
	std::cout << "Declared" << std::endl;
	fout = new TFile("MissingChip.root","RECREATE");
	fout->cd();
	Event = 0;
	MissingChip = new TTree("MissingChip","MissingChip");
	MissingChip->Branch("Event",&Event);	
	MissingChip->Branch("TrackletID",&TrackletID);	

	MissingChip->Branch("layerindex",&layerindex);
	MissingChip->Branch("staveindex",&staveindex);
	MissingChip->Branch("chipindex",&chipindex);


	ClusHits = new TH2D("ClusHits","",200,-8,8,200,-8,8);

	ClusHits->SetMarkerStyle(20);
	ClusHits->SetMarkerColor(kRed);
	ClusHits->GetXaxis()->SetTitle("x (cm)");
	ClusHits->GetYaxis()->SetTitle("y (cm)");
	ClusHits->GetXaxis()->CenterTitle();
	ClusHits->GetYaxis()->CenterTitle();


	MissingClus = new TH2D("MissingClus","",200,-5,5,200,-5,5);
	MissingClus->SetMarkerStyle(20);
	MissingClus->SetMarkerColor(kBlue);
	MissingClus->GetXaxis()->SetTitle("x (cm)");
	MissingClus->GetYaxis()->SetTitle("y (cm)");
	MissingClus->GetXaxis()->CenterTitle();
	MissingClus->GetYaxis()->CenterTitle();

	TotalTracklets = 0;
	TwoClusTracklets = 0;

}

//____________________________________________________________________________..
int MvtxHitEff::InitRun(PHCompositeNode *topNode)
{
	std::cout << "I am using my own module" << std::endl; 	

	geantGeom_MVTX =  findNode::getClass<PHG4CylinderGeomContainer>(topNode, "CYLINDERGEOM_MVTX");
	actsGeom = findNode::getClass<ActsGeometry>(topNode, "ActsGeometry");
	topNode->print();


	return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int MvtxHitEff::process_event(PHCompositeNode *topNode)
{
	std::cout << "Now processing - ZZ MvtxHitEff" << std::endl; 	




	//	topNode->print();
	//	auto trackmap = findNode::getClass<SvtxTrackSeed_v1>(topNode, "SvtxTrackSeed_v1");

	auto _si_seeds = findNode::getClass<TrackSeedContainer>(topNode, "SiliconTrackSeedContainer");
	auto clustermap = findNode::getClass<TrkrClusterContainer>(topNode, "TRKR_CLUSTER");
	auto geometry = findNode::getClass<ActsGeometry>(topNode, "ActsGeometry");


	if (!_si_seeds)
	{
		std::cerr << PHWHERE << " ERROR: Can't find SiliconTrackSeedContainer" << std::endl;

	}

	//int svtxseedmapsize = _si_seeds->size();



	TrackSeedContainer::ConstIter siseed_beg = _si_seeds->begin();
	TrackSeedContainer::ConstIter siseed_end = _si_seeds->end();


	std::vector<float> EvtTrackletX;
	std::vector<float> EvtTrackletY;
	std::vector<float> EvtTrackletZ;

	std::vector<float> EvtMissingX;
	std::vector<float> EvtMissingY;
	std::vector<float> EvtMissingZ;

	for (TrackSeedContainer::ConstIter siseed_iter = siseed_beg; siseed_iter != siseed_end; ++siseed_iter){  //Looping through tracklets



		int id = _si_seeds->index(siseed_iter);



		TrackSeed *siseed = NULL;


		siseed = _si_seeds->get(id);

		TrackletID = id;


		if(!siseed){

			//			std::cout << "No Silicon Seed" << std::endl;
			continue;
		}else{

			TotalTracklets++;
			
		}


		//int cluskeysize = siseed->size_cluster_keys();

		//	std::cout << "x = " << siseed->get_x() <<  "   y = " << siseed->get_y() <<  "   z = " << siseed->get_z() << std::endl;

		TrackSeed::ClusterKeyIter cluskeybegin = siseed->begin_cluster_keys();
		TrackSeed::ClusterKeyIter cluskeyend = siseed->end_cluster_keys();

		int nmaps = 0;
		int missinglayer = -1;
		int layer;

		std::vector<int> seedlayers;
		std::vector<double> seedx;
		std::vector<double> seedy;
		std::vector<double> seedz;
		std::vector<double> seeddetr;





		for (TrackSeed::ClusterKeyIter cluskey = cluskeybegin; cluskey != cluskeyend; ++cluskey){ //looping through clusters




			//		float x = siseed->get_x();
			//		float y = siseed->get_y();






			if (TrkrDefs::getTrkrId(*cluskey) == TrkrDefs::mvtxId){

				layer =  TrkrDefs::getLayer(*cluskey);
				TrkrCluster* cluster = clustermap->findCluster(*cluskey);

				Acts::Vector3 global = geometry->getGlobalPosition(*cluskey, cluster);  



				nmaps++;
				seedlayers.push_back(layer);

				CylinderGeom_Mvtx* layergeom = dynamic_cast<CylinderGeom_Mvtx*>(geantGeom_MVTX->GetLayerGeom(layer));
				auto surface = actsGeom->maps().getSiliconSurface(*cluskey);




				float x = global[0];
				float y = global[1];
				float z = global[2];



				seedx.push_back(x);
				seedy.push_back(y);
				seedz.push_back(z);

				float r = layergeom->get_radius();

				seeddetr.push_back(r);

				//		EvtTrackletX.push_back(x);
				//		EvtTrackletY.push_back(y);
				//		EvtTrackletZ.push_back(z);


				ClusHits->Fill(x,y);


				//std::cout << "x = " << x << "  y = " << y << std::endl;
			}


			//		std::cout << "missinglayer = " << missinglayer << std::endl;


		}




		if(nmaps== 2){


			for(int i = 0; i < NMVTXLayer; i++){

				// Use std::find to search for the target element
				auto it = std::find(seedlayers.begin(), seedlayers.end(), i);

				if (it == seedlayers.end()) {

					missinglayer = i;
				} 		
			}

		}


		bool MissingOneLayer = false;
		float radius_miss = -1;
		CylinderGeom_Mvtx* layergeom = NULL;

		if(missinglayer != -1) MissingOneLayer = true;

		if(MissingOneLayer)
		{
			TwoClusTracklets++;
			layergeom = dynamic_cast<CylinderGeom_Mvtx*>(geantGeom_MVTX->GetLayerGeom(missinglayer));
			radius_miss = layergeom->get_radius();

			//Straight line parametric function

			//x = (x1-x0) * t + x0
			//y = (y1-y0) * t + y0
			//z = (z1-z0) * t + z0

			double posx0 = seedx[0];
			double posy0 = seedy[0];
			double posz0 = seedz[0];
			double posx1 = seedx[1];
			double posy1 = seedy[1];
			double posz1 = seedz[1];
			double par0 = (posx0-posx1) * (posx0-posx1) + (posy0-posy1) * (posy0-posy1);
			double par1 = 2 * posx0 *(posx1-posx0) + 2 * posy0 * (posy1-posy0);
			double par2 = posx0*posx0 + posy0*posy0 - radius_miss * radius_miss;


			//	double delta = par1*par1 - 4 * par0*par2;
			//	std::cout << "delta = " << delta << std::endl;

			float time = (-par1 + sqrt(par1*par1 - 4 * par0*par2))/(2 * par0);

			double x_miss = (posx1-posx0) * time + posx0;
			double y_miss = (posy1-posy0) * time + posy0; 
			double z_miss = (posz1-posz0) * time + posz0; 

			unsigned int missingstave = 1;
			unsigned int missingchip = 1;

			//		std::cout << "x_miss = " << x_miss << "   y_miss = " << y_miss << "   z_miss = " << z_miss << std::endl;


			std::vector<double> world_inner_vec = {x_miss, y_miss, z_miss};
			layergeom->get_sensor_indices_from_world_coords(world_inner_vec, missingstave, missingchip);



			layerindex = missinglayer; 
			staveindex = missingstave;
			chipindex = missingchip;


			MissingChip->Fill();
			MissingClus->Fill(x_miss,y_miss);




			//std::cout << "MVTX Layer: " <<  missinglayer << "   radius = " << radius << std::endl;
			//		std::cout << "trackseed works" << std::endl;

			//		std::cout << "MVTX Missing Sensor: Layer - " <<  missinglayer << "   missingstave = " << missingstave << "   missingchip = " << missingchip << std::endl;


		}


	}


	if(Event == 10){




		gStyle->SetOptStat(0);
		TCanvas * c = new TCanvas("c","c",600,600);
		c->cd();




		ClusHits->Draw("p");
		MissingClus->Draw("pSAME");


		c->SaveAs("EvtDisplay.png");


	}



	Event++;

	ClusHits->Reset();
	MissingClus->Reset();

	std::cout << "OK DONE" << std::endl; 	

	return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int MvtxHitEff::EndRun()
{

	return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int MvtxHitEff::End(PHCompositeNode * /*unused*/) { 

	fout->cd();

	MissingChip->Write();

	fout->Close();

	std::cout  << "Total Tracklets: " << TotalTracklets << "   TwoClusTracklets: " << TwoClusTracklets << std::endl;
	return Fun4AllReturnCodes::EVENT_OK; 
}




