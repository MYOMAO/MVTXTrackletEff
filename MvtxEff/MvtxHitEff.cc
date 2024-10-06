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

TH1D * NHitStat; 
//____________________________________________________________________________..
MvtxHitEff::MvtxHitEff(const std::string &name)
	: SubsysReco(name)
{
	std::cout << "Declared -> OK" << std::endl;
	std::cout << "Add More Quality Info for Further Selections -> Also Added TPC Seed within the Matched Tracks" << std::endl;
	std::cout << "Debugging -> Bettere Info" << std::endl;
	std::cout << "Reject TPC Info -> Test Again" << std::endl;
	std::cout << "Fixed and Remove Extra Print Out" << std::endl;
	std::cout << "Use Total Track" << std::endl;

	fout = new TFile("MVTXEffAna.root","RECREATE");
	fout->cd();
	Event = 0;
	MissingChip = new TTree("MissingChip","MissingChip");
	MissingChip->Branch("Event",&Event);	
	MissingChip->Branch("TrackletID",&TrackletID);	
	MissingChip->Branch("chisq",&chisq);	
	MissingChip->Branch("ndf",&ndf);	

	MissingChip->Branch("layerindex",&layerindex);
	MissingChip->Branch("staveindex",&staveindex);
	MissingChip->Branch("chipindex",&chipindex);

	MissingChip->Branch("chipphi",&chipphi);
	MissingChip->Branch("chipz",&chipz);

	MissingChip->Branch("x",&x_miss);
	MissingChip->Branch("y",&y_miss);
	MissingChip->Branch("z",&z_miss);

	MissingChip->Branch("six",&six);
	MissingChip->Branch("siy",&siy);
	MissingChip->Branch("siz",&siz);

	MissingChip->Branch("tpcx",&tpcx);
	MissingChip->Branch("tpcy",&tpcy);
	MissingChip->Branch("tpcz",&tpcz);

	MissingChip->Branch("nmaps",&nmaps);	
	MissingChip->Branch("nintt",&nintt);	
	MissingChip->Branch("ntpc",&ntpc);	


	MissingChip->Branch("nmapslayer",&nmapslayer);	
	MissingChip->Branch("ninttlayer",&ninttlayer);	
	MissingChip->Branch("ntpclayer",&ntpclayer);	
	MissingChip->Branch("poormvtxclus",&poormvtxclus);	
	MissingChip->Branch("poorinttclus",&poorinttclus);	


	TotalChip = new TTree("TotalChip","TotalChip");
	TotalChip->Branch("Event",&Event);	
	TotalChip->Branch("TrackletID",&TrackletID);	
	TotalChip->Branch("chisq",&chisq);	
	TotalChip->Branch("ndf",&ndf);	
	TotalChip->Branch("layerfired",&layerfired);
	TotalChip->Branch("stavefired",&stavefired);
	TotalChip->Branch("chipfired",&chipfired);
	TotalChip->Branch("chipphifired",&chipphifired);
	TotalChip->Branch("chipzfired",&chipzfired);



	TotalChip->Branch("six",&six);
	TotalChip->Branch("siy",&siy);
	TotalChip->Branch("siz",&siz);
	TotalChip->Branch("tpcx",&tpcx);
	TotalChip->Branch("tpcy",&tpcy);
	TotalChip->Branch("tpcz",&tpcz);
	TotalChip->Branch("nmaps",&nmaps);	
	TotalChip->Branch("nintt",&nintt);	
	TotalChip->Branch("ntpc",&ntpc);	
	TotalChip->Branch("nmapslayer",&nmapslayer);	
	TotalChip->Branch("ninttlayer",&ninttlayer);	
	TotalChip->Branch("ntpclayer",&ntpclayer);	
	TotalChip->Branch("poormvtxclus",&poormvtxclus);	
	TotalChip->Branch("poorinttclus",&poorinttclus);	




	TotalTrack = new TTree("TotalTrack","TotalTrack");
	TotalTrack->Branch("Event",&Event);	
	TotalTrack->Branch("TrackletID",&TrackletID);	
	TotalTrack->Branch("chisq",&chisq);	
	TotalTrack->Branch("ndf",&ndf);	
	TotalTrack->Branch("six",&six);
	TotalTrack->Branch("siy",&siy);
	TotalTrack->Branch("siz",&siz);
	TotalTrack->Branch("tpcx",&tpcx);
	TotalTrack->Branch("tpcy",&tpcy);
	TotalTrack->Branch("tpcz",&tpcz);
	TotalTrack->Branch("nmaps",&nmaps);	
	TotalTrack->Branch("nintt",&nintt);	
	TotalTrack->Branch("ntpc",&ntpc);	
	TotalTrack->Branch("nmapslayer",&nmapslayer);	
	TotalTrack->Branch("ninttlayer",&ninttlayer);	
	TotalTrack->Branch("ntpclayer",&ntpclayer);	
	TotalTrack->Branch("poormvtxclus",&poormvtxclus);	
	TotalTrack->Branch("poorinttclus",&poorinttclus);	



	SiSeedAna = new TTree("SiSeedAna","SiSeedAna");
	SiSeedAna->Branch("Event",&Event);
	SiSeedAna->Branch("SeedID",&SeedID);

	SiSeedAna->Branch("TotalClus",&TotalClus);
	SiSeedAna->Branch("TotalClusAssoc",&TotalClusAssoc);
	SiSeedAna->Branch("SiSeedEta",&SiSeedEta);
	SiSeedAna->Branch("SiSeedPt",&SiSeedPt);

	SiSeedAna->Branch("nmaps",&nmaps);
	SiSeedAna->Branch("nintt",&nintt);
	SiSeedAna->Branch("chisq",&chisq);	
	SiSeedAna->Branch("ndf",&ndf);	

	SiSeedAna->Branch("vtxId",&vtxId);	
	SiSeedAna->Branch("vtxX",&vtxX);	
	SiSeedAna->Branch("vtxY",&vtxY);	
	SiSeedAna->Branch("vtxZ",&vtxZ);	

	SiSeedAna->Branch("SiSeedSize",&SiSeedSize);

	SiSeedAna->Branch("ClusPosX",&ClusPosX);
	SiSeedAna->Branch("ClusPosY",&ClusPosY);
	SiSeedAna->Branch("ClusPosZ",&ClusPosZ);
	SiSeedAna->Branch("INTTTimeBucket",&INTTTimeBucketVec);

	SiSeedAna->Branch("Crossing",&Crossing);
	SiSeedAna->Branch("ClusKeyVec",&ClusKeyVec);
	//SiSeedAna->Branch("ClusIdVec",&ClusIdVec);


	VertexTree = new TTree("VertexTree","VertexTree");
	VertexTree->Branch("Event",&Event);
	VertexTree->Branch("VertexCrossing",&VertexCrossing);
	VertexTree->Branch("Ntracks",&Ntracks);
	VertexTree->Branch("m_nvertices",&m_nvertices);

	VertexTree->Branch("Vx",&Vx);
	VertexTree->Branch("Vy",&Vy);
	VertexTree->Branch("Vz",&Vz);



	MVTXClusSize = new TH1D("MVTXClusSize","",100,-0.5,100);
	MVTXClusSize->GetXaxis()->SetTitle("MVTX Cluster Size Associated to Silicon Tracklets");
	MVTXClusSize->GetYaxis()->SetTitle("Number of Clusters");
	MVTXClusSize->GetXaxis()->CenterTitle();
	MVTXClusSize->GetYaxis()->CenterTitle();


	INTTClusSize = new TH1D("INTTClusSize","",100,-0.5,100);
	INTTClusSize->GetXaxis()->SetTitle("INTT Cluster Size Associated to Silicon Tracklets");
	INTTClusSize->GetYaxis()->SetTitle("Number of Clusters");
	INTTClusSize->GetXaxis()->CenterTitle();
	INTTClusSize->GetYaxis()->CenterTitle();



	ClusHits = new TH2D("ClusHits","",200,-12,12,200,-12,12);
	ClusHits->SetMarkerStyle(20);
	ClusHits->SetMarkerColor(kRed);
	ClusHits->GetXaxis()->SetTitle("x (cm)");
	ClusHits->GetYaxis()->SetTitle("y (cm)");
	ClusHits->GetXaxis()->CenterTitle();
	ClusHits->GetYaxis()->CenterTitle();




	ClusHits3D = new TH3D("ClusHits3D","",100,-30,30,200,-12,12,200,-12,12);


	ClusHits3D->SetMarkerStyle(20);
	ClusHits3D->SetMarkerColor(kRed);
	ClusHits3D->GetXaxis()->SetTitle("x (cm)");
	ClusHits3D->GetYaxis()->SetTitle("y (cm)");
	ClusHits3D->GetZaxis()->SetTitle("z (cm)");

	ClusHits3D->GetXaxis()->CenterTitle();
	ClusHits3D->GetYaxis()->CenterTitle();
	ClusHits3D->GetZaxis()->CenterTitle();


	SiClusHits = new TH2D("ClusHits","",200,-12,12,200,-12,12);

	SiClusHits->SetMarkerStyle(20);
	SiClusHits->SetMarkerColor(kRed);
	SiClusHits->GetXaxis()->SetTitle("x (cm)");
	SiClusHits->GetYaxis()->SetTitle("y (cm)");
	SiClusHits->GetXaxis()->CenterTitle();
	SiClusHits->GetYaxis()->CenterTitle();


	MissingClus = new TH2D("MissingClus","",200,-12,12,200,-12,12);
	MissingClus->SetMarkerStyle(20);
	MissingClus->SetMarkerColor(kBlue);
	MissingClus->GetXaxis()->SetTitle("x (cm)");
	MissingClus->GetYaxis()->SetTitle("y (cm)");
	MissingClus->GetXaxis()->CenterTitle();
	MissingClus->GetYaxis()->CenterTitle();


	MissingClus3D = new TH3D("MissingClus3D","",100,-30,30,200,-12,12,200,-12,12);
	MissingClus3D->SetMarkerStyle(20);
	MissingClus3D->SetMarkerColor(kBlue);
	MissingClus3D->GetXaxis()->SetTitle("x (cm)");
	MissingClus3D->GetYaxis()->SetTitle("y (cm)");
	MissingClus3D->GetYaxis()->SetTitle("z (cm)");

	MissingClus3D->GetXaxis()->CenterTitle();
	MissingClus3D->GetYaxis()->CenterTitle();


	TotalTracklets = 0;
	TwoClusTracklets = 0;

	NHitStat = new TH1D("NHitStat","",6,-0.5,5.5);
	NHitStat->GetXaxis()->SetTitle("N MVTX Clusters");
	NHitStat->GetYaxis()->SetTitle("Number of Tracklets");


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
	std::cout << "Now processing - ZZ MvtxHitEff - use track map -> Find nskip -> Changed ZZ -> Vertex -> Updated 10/04 New -> More Print Out"  << std::endl; 	




	//	topNode->print();
	//	auto trackmap = findNode::getClass<SvtxTrackSeed_v1>(topNode, "SvtxTrackSeed_v1");

	auto _si_seeds = findNode::getClass<TrackSeedContainer>(topNode, "SiliconTrackSeedContainer");
	auto clustermap = findNode::getClass<TrkrClusterContainer>(topNode, "TRKR_CLUSTER");
	auto geometry = findNode::getClass<ActsGeometry>(topNode, "ActsGeometry");

	auto _trackmap = findNode::getClass<SvtxTrackMap>(topNode, "SvtxTrackMap");
	auto _vertexmap = findNode::getClass<SvtxVertexMap>(topNode, "SvtxVertexMap");



	if (!_trackmap)
	{
		std::cout << " SvtxTrackMap node not found on node tree"   <<  std::endl;
		return Fun4AllReturnCodes::ABORTEVENT;

	}

	TotalClus = 0;
	TotalClusAssoc = 0;

	for (auto &hsk : clustermap->getHitSetKeys(TrkrDefs::TrkrId::mvtxId))
	{
		auto range = clustermap->getClusters(hsk);
		for (auto iter = range.first; iter != range.second; ++iter)
		{
			TotalClus++;
		}
	}

	//	for (TrackSeedContainer::ConstIter siseed_iter = siseed_beg; siseed_iter != siseed_end; ++siseed_iter){  //Looping through tracklets


	if (!_si_seeds)
	{
		std::cerr << PHWHERE << " ERROR: Can't find SiliconTrackSeedContainer" << std::endl;

	}

	int svtxseedmapsize = _si_seeds->size();

	std::cout << "svtxseedmapsize = " << svtxseedmapsize << std::endl;


	//	TrackSeedContainer::ConstIter siseed_beg = _si_seeds->begin();
	//	TrackSeedContainer::ConstIter siseed_end = _si_seeds->end();


	std::vector<float> EvtTrackletX;
	std::vector<float> EvtTrackletY;
	std::vector<float> EvtTrackletZ;

	std::vector<float> EvtMissingX;
	std::vector<float> EvtMissingY;
	std::vector<float> EvtMissingZ;

	m_nvertices = _vertexmap->size();

	std::cout << "m_nvertices = " << m_nvertices << std::endl;
	/*
	   for (const auto& [key, vertex] : *_vertexmap)
	   {

	   VertexCrossing = vertex->get_beam_crossing();
	   Vx = vertex->get_x();
	   Vy = vertex->get_y();
	   Vz = vertex->get_z();
	   Ntracks = vertex->size_tracks();
	   */		

	std::cout << "_trackmap->size() = " << _trackmap->size() << std::endl;
	for (auto& iter : *_trackmap)
	{   

		
		//Seed loop
		//cout << itrack << " of " << _trackmap->size();
		SvtxTrack* track = iter.second;

		if(!track) continue;


		//cout << " : SvtxTrack: -> New" << endl;
		ClusKeyVec.clear();

		chisq = track->get_chisq();
		ndf = track->get_ndf();
		quality = track->get_quality();

		vtxId = track->get_vertex_id();


		SvtxVertex * svtxVertex = NULL;
		svtxVertex = _vertexmap->find(vtxId)->second;

		//std::cout << "OK -> Before Pass 2" << "   Vertex Id - " << vtxId << std::endl;



		


		if(vtxId != -1){

			vtxX =  svtxVertex->get_x();
			vtxY =  svtxVertex->get_y();
			vtxZ =  svtxVertex->get_z();

		}else{

			//std::cout << "No vertex, bad " << std::endl;

		}

		TrackSeed* siseed = track->get_silicon_seed();



		//		TrackSeed *siseed = NULL;


		//		siseed = _si_seeds->get(id);

		TotalClusAssoc = 0;



		if(!siseed){

			//std::cout << "No Silicon Seed" << std::endl;
			continue;
		}else{

			TotalTracklets++;

		}

		SiSeedPt = siseed->get_pt();
		SiSeedEta = siseed->get_eta();
		Crossing = siseed->get_crossing();



		//if(Crossing != 0 || SiSeedPt < 0.5) continue;  //Apply Cuts
		
		if(Crossing != 0) continue;  
		
		TrackletID = track->get_id();

		//std::cout << "TotalTracklets = " << TotalTracklets << std::endl;
		//int cluskeysize = siseed->size_cluster_keys();

		//std::cout << "x = " << siseed->get_x() <<  "   y = " << siseed->get_y() <<  "   z = " << siseed->get_z() << std::endl;

		TrackSeed::ClusterKeyIter cluskeybegin = siseed->begin_cluster_keys();
		TrackSeed::ClusterKeyIter cluskeyend = siseed->end_cluster_keys();
		nmaps = 0;
		nintt = 0;




		int missinglayer = -1;
		int layer;

		std::vector<int> seedlayers;
		std::vector<int> seedstave;
		std::vector<int> seedchip;

		std::vector<double> seedx;
		std::vector<double> seedy;
		std::vector<double> seedz;
		std::vector<double> seeddetr;



		six = siseed->get_x();
		siy = siseed->get_y();
		siz = siseed->get_z();



		//bool goodclussize = true;
		//int SeedswithOverlapClus = 0;

		std::vector<int> mvtxlaycount;
		std::vector<int> inttlaycount;
		std::vector<int> tpclaycount;


		//std::cout << "Si - Pass 1" << std::endl;

		poormvtxclus = 0;
		poorinttclus = 0;

		for (TrackSeed::ClusterKeyIter cluskey = cluskeybegin; cluskey != cluskeyend; ++cluskey){ //looping through silicon clusters



			//		float x = siseed->get_x();
			//		float y = siseed->get_y();



			if (TrkrDefs::getTrkrId(*cluskey) == TrkrDefs::mvtxId || TrkrDefs::getTrkrId(*cluskey) == TrkrDefs::inttId){

				TrkrCluster* cluster = clustermap->findCluster(*cluskey);
				Acts::Vector3 global = geometry->getGlobalPosition(*cluskey, cluster);  

				float x = global[0];
				float y = global[1];
				float z = global[2];

				ClusHits->Fill(x,y);
				ClusHits3D->Fill(z,y,x);

				int size = cluster->getSize();

				if (TrkrDefs::getTrkrId(*cluskey) == TrkrDefs::mvtxId){
					MVTXClusSize->Fill(size);
					if(size > mvtxclusmax){
				//		goodclussize = false;
						poormvtxclus++;
					}
				
				}

				if (TrkrDefs::getTrkrId(*cluskey) == TrkrDefs::inttId){				
					INTTClusSize->Fill(size);	
					if(size > inttclusmax){
				//		goodclussize = false;
						poorinttclus++;
					}
				}

				ClusKeyVec.push_back(*cluskey);
				//	ClusIdVec.push_back(*cluskey);

			}


		//	std::cout << "Si - Pass 2" << std::endl;

			if (TrkrDefs::getTrkrId(*cluskey) == TrkrDefs::mvtxId){

				layer =  TrkrDefs::getLayer(*cluskey);
				int stave =  MvtxDefs::getStaveId(*cluskey);
				int chip =  MvtxDefs::getChipId(*cluskey);

				//std::cout << "layer = " << layer << std::endl;
				TrkrCluster* cluster = clustermap->findCluster(*cluskey);

				Acts::Vector3 global = geometry->getGlobalPosition(*cluskey, cluster);  
				TotalClusAssoc++;




				nmaps++;
				seedlayers.push_back(layer);
				seedstave.push_back(stave);
				seedchip.push_back(chip);

				CylinderGeom_Mvtx* layergeom = dynamic_cast<CylinderGeom_Mvtx*>(geantGeom_MVTX->GetLayerGeom(layer));
				auto surface = actsGeom->maps().getSiliconSurface(*cluskey);


				mvtxlaycount.push_back(layer);


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




				//std::cout << "x = " << x << "  y = " << y << std::endl;
			}



	//		std::cout << "Si - Pass 3" << std::endl;

			if (TrkrDefs::getTrkrId(*cluskey) == TrkrDefs::inttId){
				nintt++;
				layer =  TrkrDefs::getLayer(*cluskey);
				inttlaycount.push_back(layer);

			}





			if (TrkrDefs::getTrkrId(*cluskey) == TrkrDefs::mvtxId || TrkrDefs::getTrkrId(*cluskey) == TrkrDefs::inttId ){

				TrkrCluster* cluster = clustermap->findCluster(*cluskey);
				Acts::Vector3 global = geometry->getGlobalPosition(*cluskey, cluster);  

				int INTTTimeBucket = 0;

				if(TrkrDefs::getTrkrId(*cluskey) == TrkrDefs::inttId) INTTTimeBucket = InttDefs::getTimeBucketId(*cluskey);

				INTTTimeBucketVec.push_back(INTTTimeBucket);

				float x = global[0];
				float y = global[1];
				float z = global[2];


				ClusPosX.push_back(x);
				ClusPosY.push_back(y);
				ClusPosZ.push_back(z);

				SiClusHits->Fill(x,y);


			}
			//		std::cout << "missinglayer = " << missinglayer << std::endl;

	//		std::cout << "Si - Pass 4" << std::endl;

		}
	

		std::cout << "Now Looping TPC stuffs" << std::endl;


		//TPC Seed Loop

		TrackSeed* tpcseed = track->get_tpc_seed();		
		

		if(!tpcseed){

			std::cout << "No TPC Seed" << std::endl;
			continue;
		}



		TrackSeed::ClusterKeyIter tpccluskeybegin = tpcseed->begin_cluster_keys();
		TrackSeed::ClusterKeyIter tpccluskeyend = tpcseed->end_cluster_keys();



		tpcx = tpcseed->get_x();
		tpcy = tpcseed->get_y();
		tpcz = tpcseed->get_z();

		for (TrackSeed::ClusterKeyIter tpccluskey = tpccluskeybegin; tpccluskey != tpccluskeyend; ++tpccluskey){ //looping through silicon clusters


			if (TrkrDefs::getTrkrId(*tpccluskey) == TrkrDefs::tpcId){
				ntpc++;
				layer =  TrkrDefs::getLayer(*tpccluskey);
				tpclaycount.push_back(layer);

			}


		}


		



		//Count MVTX, INTT, and TPC Unique Layers//

		std::sort(mvtxlaycount.begin(), mvtxlaycount.end());
		auto mvtxuniq = std::unique(mvtxlaycount.begin(), mvtxlaycount.end());
		nmapslayer = std::distance(mvtxlaycount.begin(), mvtxuniq);

		std::sort(inttlaycount.begin(), inttlaycount.end());
		auto inttuniq = std::unique(inttlaycount.begin(), inttlaycount.end());
		ninttlayer = std::distance(inttlaycount.begin(), inttuniq);

		std::sort(tpclaycount.begin(), tpclaycount.end());
		auto tpcuniq = std::unique(tpclaycount.begin(), tpclaycount.end());
		ntpclayer = std::distance(tpclaycount.begin(), tpcuniq);


		

		//	std::cout << "nmaps = " << nmaps << "   TotalTracklet = " <<  TotalTracklets << std::endl;
		NHitStat->Fill(nmaps);



		//if(!goodclussize) continue;
		//if(quality > 100) continue;





	//	if(nmaps== 2 && nintt == 2){
		if(nmaps== 2){


			for(int i = 0; i < NMVTXLayer; i++){

				// Use std::find to search for the target element
				auto it = std::find(seedlayers.begin(), seedlayers.end(), i);

				if (it == seedlayers.end()) {

					missinglayer = i;
				} 		
			}


		}


//		if(nmaps > 1 && nintt == 2){
		CylinderGeom_Mvtx* layergeom = NULL;
	
		if(nmaps > 1){

			int mvtxclusize = seedlayers.size();

			for(int i = 0; i < mvtxclusize; i++){

				layerfired = seedlayers[i];
				stavefired = seedstave[i];
				chipfired = seedchip[i];

				layergeom = dynamic_cast<CylinderGeom_Mvtx*>(geantGeom_MVTX->GetLayerGeom(layerfired));
				

				auto hitsetkeyfired = MvtxDefs::genHitSetKey(layerfired, stavefired, chipfired, 0);
				
				auto surface = actsGeom->maps().getSiliconSurface(hitsetkeyfired);

				double mvtxfiredchiploc[3] = {0,0,0};

				layergeom->find_sensor_center(surface, actsGeom, mvtxfiredchiploc);

				chipphifired = TMath::ATan2(mvtxfiredchiploc[1],mvtxfiredchiploc[0]);
				chipzfired = mvtxfiredchiploc[2];

				TotalChip->Fill();
			}


		}




		bool MissingOneLayer = false;
		float radius_miss = -1;
		int nskipped = 0;

		if(missinglayer != -1) MissingOneLayer = true;

		//	MissingOneLayer = false;
		std::cout << "Missing layer here: " << MissingOneLayer << std::endl;
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

			x_miss = (posx1-posx0) * time + posx0;
			y_miss = (posy1-posy0) * time + posy0; 
			z_miss = (posz1-posz0) * time + posz0; 

			unsigned int missingstave = 1;
			unsigned int missingchip = 1;

			//std::cout << "x_miss = " << x_miss << "   y_miss = " << y_miss << "   z_miss = " << z_miss << std::endl;


			std::vector<double> world_inner_vec = {x_miss, y_miss, z_miss};
			layergeom->get_sensor_indices_from_world_coords(world_inner_vec, missingstave, missingchip);

			if(missingchip > 8){ 
				nskipped++;
				continue;
			}

			//	MvtxDefs * mvtxdef = new MvtxDefs();
			//	auto missinghitsetkey = mvtxdef->GenHitSetKey(missinglayer, missingstave, missingchip, 0);
			auto missinghitsetkey = MvtxDefs::genHitSetKey(missinglayer, missingstave, missingchip, 0);

			//std::cout << "missinglayer = " << missinglayer << "   missingstave = " << missingstave << "   missingchip =  " << missingchip << std::endl;


			auto surface = actsGeom->maps().getSiliconSurface(missinghitsetkey);

			double mvtxmissingchiploc[3] = {0,0,0};

			layergeom->find_sensor_center(surface, actsGeom, mvtxmissingchiploc);

			layerindex = missinglayer; 
			staveindex = missingstave;
			chipindex = missingchip;

			chipphi = TMath::ATan2(mvtxmissingchiploc[1],mvtxmissingchiploc[0]);
			chipz = mvtxmissingchiploc[2];

			MissingChip->Fill();
			MissingClus->Fill(x_miss,y_miss);

			MissingClus3D->Fill(z_miss,y_miss,x_miss);



			//std::cout << "MVTX Layer: " <<  missinglayer << "   radius = " << radius << std::endl;
			//		std::cout << "trackseed works" << std::endl;

			//		std::cout << "MVTX Missing Sensor: Layer - " <<  missinglayer << "   missingstave = " << missingstave << "   missingchip = " << missingchip << std::endl;


			layerfired = layerindex;
			stavefired = staveindex;
			chipfired = chipindex;

			chipzfired = chipz;
			chipphifired = chipphi;


			TotalChip->Fill();

		}




		SiSeedSize = ClusPosX.size();
		SiSeedAna->Fill();
		ClusPosX.clear();
		ClusPosY.clear();
		ClusPosZ.clear();
		INTTTimeBucketVec.clear();
		//++itrack;


	}

	//}


	if(Event == 10){




		gStyle->SetOptStat(0);
		TCanvas * c = new TCanvas("c","c",600,600);
		c->cd();




		ClusHits->Draw("p");
		MissingClus->Draw("pSAME");


		c->SaveAs("EvtDisplay.png");



		ClusHits3D->Draw("p");
		MissingClus3D->Draw("pSAME");


		c->SaveAs("EvtDisplay3D.png");


		SiClusHits->Draw("p");
		c->SaveAs("MVTX+INTT.png");

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

	TotalChip->Write();
	MissingChip->Write();
	NHitStat->Write();
	SiSeedAna->Write();
	VertexTree->Write();
	MVTXClusSize->Write();
	INTTClusSize->Write();

	fout->Close();

	std::cout  << "Total Tracklets: " << TotalTracklets << "   TwoClusTracklets: " << TwoClusTracklets << std::endl;
	return Fun4AllReturnCodes::EVENT_OK; 
}




