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
	std::cout << "Use Total Track Write" << std::endl;
	std::cout << "Additional Info + Silicon Surface Protection" << std::endl;
	std::cout << "Include TPC Clusters for Demo -> Propered -> Uodated" << std::endl;
	std::cout << "Improve Missing" << std::endl;
	std::cout << "Fix Missing" << std::endl;
	std::cout << "Include Si Fit Data" << std::endl;
	std::cout << "Make Use TPC Method" << std::endl;
	std::cout << "Make 1 Single Ntuple" << std::endl;
	std::cout << "Keep 1 Fill Only" << std::endl;
	std::cout << "Updated with Unuse" << std::endl;

	fout = new TFile("MVTXEffAnaNew.root","RECREATE");
	fout->cd();
	Event = 0;







	//Enable Clusters




	TotalTrack = new TTree("TotalTrack","TotalTrack");
	TotalTrack->Branch("Event",&Event);	
	TotalTrack->Branch("CrossingID",&Crossing);				
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
	TotalTrack->Branch("trackpt",&trackpt);	
	TotalTrack->Branch("tracketa",&tracketa);	
	TotalTrack->Branch("trackphi",&trackphi);	
	TotalTrack->Branch("trackpx",&trackpx);	
	TotalTrack->Branch("trackpy",&trackpy);	
	TotalTrack->Branch("trackpz",&trackpz);	
	TotalTrack->Branch("trackq",&trackq);	

	TotalTrack->Branch("tpcseedpt",&tpcseedpt);	
	TotalTrack->Branch("tpcseedeta",&tpcseedeta);	
	TotalTrack->Branch("tpcseedphi",&tpcseedphi);	
	TotalTrack->Branch("tpcseedpx",&tpcseedpx);	
	TotalTrack->Branch("tpcseedpy",&tpcseedpy);	
	TotalTrack->Branch("tpcseedpz",&tpcseedpz);	
	TotalTrack->Branch("tpcseedq",&tpcseedq);	

	TotalTrack->Branch("siseedpt",&siseedpt);	
	TotalTrack->Branch("siseedeta",&siseedeta);	
	TotalTrack->Branch("siseedphi",&siseedphi);	
	TotalTrack->Branch("siseedpx",&siseedpx);	
	TotalTrack->Branch("siseedpy",&siseedpy);	
	TotalTrack->Branch("siseedpz",&siseedpz);	
	TotalTrack->Branch("siseedq",&siseedq);	
	TotalTrack->Branch("CosTheta",&CosTheta);	




	//Clus Info
	TotalTrack->Branch("TotalClusSize",&TotalClusSize);
	TotalTrack->Branch("ClusPosX",&ClusPosX);
	TotalTrack->Branch("ClusPosY",&ClusPosY);
	TotalTrack->Branch("ClusPosZ",&ClusPosZ);


	TotalTrack->Branch("MVTXClusArea",&MVTXClusArea);
	TotalTrack->Branch("MVTXClusX",&MVTXClusX);
	TotalTrack->Branch("MVTXClusY",&MVTXClusY);
	TotalTrack->Branch("MVTXClusZ",&MVTXClusZ);

	TotalTrack->Branch("MVTXClusLayer",&MVTXClusLayer);
	TotalTrack->Branch("MVTXClusStave",&MVTXClusStave);
	TotalTrack->Branch("MVTXClusChip",&MVTXClusChip);

	TotalTrack->Branch("MVTXChipZ",&MVTXChipZ);
	TotalTrack->Branch("MVTXChipPhi",&MVTXChipPhi);



	TotalTrack->Branch("x_miss",&x_miss);
	TotalTrack->Branch("y_miss",&y_miss);
	TotalTrack->Branch("z_miss",&z_miss);

	TotalTrack->Branch("surfacegood",&surfacegood);	
	TotalTrack->Branch("layerindex_miss",&layerindex);
	TotalTrack->Branch("staveindex_miss",&staveindex);
	TotalTrack->Branch("chipindex_miss",&chipindex);

	TotalTrack->Branch("chipphi_miss",&chipphi);
	TotalTrack->Branch("chipz_miss",&chipz);


	//


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


void MvtxHitEff::SetUseTPC(bool WithTPC){

	UseTPC = WithTPC;
}



//____________________________________________________________________________..
int MvtxHitEff::process_event(PHCompositeNode *topNode)
{
	std::cout << "Now processing - ZZ MvtxHitEff - use track map -> Find nskip -> Changed ZZ -> Vertex -> Updated 10/04 New -> More Print Out -> Add Use TPC Flag for Si Only Fit"  << std::endl; 	


	std::cout << "Are We using TPC? " << UseTPC << std::endl;


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

		trackpt =  track->get_pt();
		tracketa =  track->get_eta();
		trackphi =  track->get_phi();
		trackpx =  track->get_px();
		trackpy =  track->get_py();
		trackpz =  track->get_pz();
		trackq =  track->get_charge();
		trackx = track->get_x();
		tracky = track->get_y();
		trackz = track->get_z();



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

		Crossing = siseed->get_crossing();

		siseedpt = siseed->get_pt();
		siseedeta = siseed->get_eta();
		siseedphi = siseed->get_phi();
		siseedpx = siseed->get_px();
		siseedpy = siseed->get_py();
		siseedpz = siseed->get_pz();
		siseedq = siseed->get_charge();

		CosTheta = (tpcseedpx * siseedpx  + tpcseedpy  * siseedpy  + tpcseedpz  * siseedpz )/(sqrt(tpcseedpx  * tpcseedpx  + tpcseedpy  *tpcseedpy  + tpcseedpz  * tpcseedpz ) * sqrt(siseedpx  * siseedpx  + siseedpy  * siseedpy  + siseedpz  * siseedpz ));


		//if(Crossing != 0 || SiSeedPt < 0.5) continue;  //Apply Cuts

		//if(Crossing != 0) continue;   //Do Reject Crossing for now

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
			//	Acts::Vector3 global = geometry->getGlobalPosition(*cluskey, cluster);  
/*
				float x = global[0];
				float y = global[1];
				float z = global[2];
*/
			//	ClusHits->Fill(x,y);
			//	ClusHits3D->Fill(z,y,x);

				int size = cluster->getSize();

				if (TrkrDefs::getTrkrId(*cluskey) == TrkrDefs::mvtxId){
				//	MVTXClusSize->Fill(size);
					if(size > mvtxclusmax){
						//		goodclussize = false;
						poormvtxclus++;
					}

				}

				if (TrkrDefs::getTrkrId(*cluskey) == TrkrDefs::inttId){				
			//		INTTClusSize->Fill(size);	
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


				MVTXClusArea.push_back(cluster->getSize());
				MVTXClusX.push_back(x);
				MVTXClusY.push_back(y);
				MVTXClusZ.push_back(z);

				MVTXClusLayer.push_back(layer);
				MVTXClusStave.push_back(stave);
				MVTXClusChip.push_back(chip);

				double mvtxfiredchiploc[3] = {0,0,0};

				layergeom->find_sensor_center(surface, actsGeom, mvtxfiredchiploc);


				float chipphi_temp = TMath::ATan2(mvtxfiredchiploc[1],mvtxfiredchiploc[0]);
				float chipz_temp = mvtxfiredchiploc[2];


				MVTXChipPhi.push_back(chipphi_temp);
				MVTXChipZ.push_back(chipz_temp);

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



				
			//	SiClusHits->Fill(x,y);

			//	FullClusHits->Fill(x,y);

			}
			//		std::cout << "missinglayer = " << missinglayer << std::endl;

			//		std::cout << "Si - Pass 4" << std::endl;

		}


		//std::cout << "Now Looping TPC stuffs" << std::endl;


		//TPC Seed Loop
		if(UseTPC){
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

			tpcseedpt =  tpcseed->get_pt();
			tpcseedeta =  tpcseed->get_eta();
			tpcseedphi =  tpcseed->get_phi();
			tpcseedpx =  tpcseed->get_px();
			tpcseedpy =  tpcseed->get_py();
			tpcseedpz =  tpcseed->get_pz();
			tpcseedq =  tpcseed->get_charge();


			for (TrackSeed::ClusterKeyIter tpccluskey = tpccluskeybegin; tpccluskey != tpccluskeyend; ++tpccluskey){ //looping through silicon clusters


				if (TrkrDefs::getTrkrId(*tpccluskey) == TrkrDefs::tpcId){
					ntpc++;
					layer =  TrkrDefs::getLayer(*tpccluskey);
					tpclaycount.push_back(layer);

					TrkrCluster* cluster = clustermap->findCluster(*tpccluskey);
					Acts::Vector3 global = geometry->getGlobalPosition(*tpccluskey, cluster);  

					float x = global[0];
					float y = global[1];
					float z = global[2];



					ClusPosX.push_back(x);
					ClusPosY.push_back(y);
					ClusPosZ.push_back(z);




				}


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
	//	NHitStat->Fill(nmaps);



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

				if(!surface){
					std::cout << "No Surface for All, skip for now" << std::endl;
					continue;			
				}
				double mvtxfiredchiploc[3] = {0,0,0};

				layergeom->find_sensor_center(surface, actsGeom, mvtxfiredchiploc);

				chipphifired = TMath::ATan2(mvtxfiredchiploc[1],mvtxfiredchiploc[0]);
				chipzfired = mvtxfiredchiploc[2];

		//		TotalChip->Fill();
			}


		}




		bool MissingOneLayer = false;
		float radius_miss = -1;
		int nskipped = 0;

		if(missinglayer != -1) MissingOneLayer = true;

		//	MissingOneLayer = false;
		//	std::cout << "Missing layer here: " << MissingOneLayer << std::endl;
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


			//	MvtxDefs * mvtxdef = new MvtxDefs();
			//	auto missinghitsetkey = mvtxdef->GenHitSetKey(missinglayer, missingstave, missingchip, 0);
			auto missinghitsetkey = MvtxDefs::genHitSetKey(missinglayer, missingstave, missingchip, 0);

			//std::cout << "missinglayer = " << missinglayer << "   missingstave = " << missingstave << "   missingchip =  " << missingchip << std::endl;


			auto surface = actsGeom->maps().getSiliconSurface(missinghitsetkey);


			if(!surface){
				std::cout << "No Surface for this Missing Chip, skip for now" << std::endl;
				std::cout << "Missing Chip Info: " << "  Layer: " << missinglayer <<  "   missingstave = " << missingstave  << "   missingchip = " << missingchip << std::endl;
				std::cout << "Missing Chip Coordinates: " << "  x = " << x_miss << "  y = " << y_miss << "  z = " << z_miss  << std::endl;
				nskipped++;				

				layerindex = missinglayer; 
				staveindex = -1;
				chipindex = -1;


				chipphi = 9999;
				chipz = 9999;

				surfacegood = false;

				//continue;
			}else{

				double mvtxmissingchiploc[3] = {0,0,0};

				layergeom->find_sensor_center(surface, actsGeom, mvtxmissingchiploc);

				layerindex = missinglayer; 
				staveindex = missingstave;
				chipindex = missingchip;

				chipphi = TMath::ATan2(mvtxmissingchiploc[1],mvtxmissingchiploc[0]);
				chipz = mvtxmissingchiploc[2];
				surfacegood = true;


			}
		//	MissingChip->Fill();

		//	MissingClus->Fill(x_miss,y_miss);
		//	MissingClusFull->Fill(x_miss,y_miss);

		//	MissingClus3D->Fill(z_miss,y_miss,x_miss);



			//std::cout << "MVTX Layer: " <<  missinglayer << "   radius = " << radius << std::endl;
			//		std::cout << "trackseed works" << std::endl;

			//		std::cout << "MVTX Missing Sensor: Layer - " <<  missinglayer << "   missingstave = " << missingstave << "   missingchip = " << missingchip << std::endl;


			layerfired = layerindex;
			stavefired = staveindex;
			chipfired = chipindex;

			chipzfired = chipz;
			chipphifired = chipphi;


		//	TotalChip->Fill();

		}else{

				//Set Unused


				
				x_miss = -1;
				y_miss = -1; 
				z_miss = -1; 

				layerindex = -1; 
				staveindex = -1;
				chipindex = -1;

				chipphi = -1;
				chipz = -1;
				surfacegood = false;


				
			}






		TotalClusSize = ClusPosX.size();

		TotalTrack->Fill();



		SiSeedSize = ClusPosX.size();
//		SiSeedAna->Fill();

		ClusPosX.clear();
		ClusPosY.clear();
		ClusPosZ.clear();
		INTTTimeBucketVec.clear();
		//++itrack;

		MVTXClusArea.clear();
		MVTXClusX.clear();
		MVTXClusY.clear();
		MVTXClusZ.clear();

		MVTXClusLayer.clear();
		MVTXClusStave.clear();
		MVTXClusChip.clear();

		MVTXChipZ.clear();
		MVTXChipPhi.clear();


	}

	//}

/*
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
		MissingClus->Draw("pSAME");		
		c->SaveAs("MVTX+INTT.png");

	}


	Event++;

	ClusHits->Reset();
	MissingClus->Reset();
*/
	Event++;

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
		TotalTrack->Write();
/*
		TotalChip->Write();
		MissingChip->Write();
		NHitStat->Write();
		SiSeedAna->Write();
		VertexTree->Write();
		MVTXClusSize->Write();
		INTTClusSize->Write();
*/
		fout->Close();

		std::cout  << "Total Tracklets: " << TotalTracklets << "   TwoClusTracklets: " << TwoClusTracklets << std::endl;
		return Fun4AllReturnCodes::EVENT_OK; 
	}




