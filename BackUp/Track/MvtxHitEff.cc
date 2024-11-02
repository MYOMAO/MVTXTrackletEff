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
#include <trackbase_historic/TrackAnalysisUtils.h>
#include <limits>
//____________________________________________________________________________..
MvtxHitEff::MvtxHitEff(const std::string &name)
	: SubsysReco(name)
{
	std::cout << "Declared" << std::endl;
	fout->cd();
	MissingChip = new TTree("MissingChip","MissingChip");

	MissingChip->SetBranchAddress("layerindex",&layerindex);
	MissingChip->SetBranchAddress("staveindex",&staveindex);
	MissingChip->SetBranchAddress("chipindex",&chipindex);
	
}

//____________________________________________________________________________..
int MvtxHitEff::InitRun(PHCompositeNode *topNode)
{
	std::cout << "I am using my own module" << std::endl; 	

	topNode->print();


	return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int MvtxHitEff::process_event(PHCompositeNode *topNode)
{
	std::cout << "Now processing - ZZ MvtxHitEff -> Track" << std::endl; 	




	//	topNode->print();
	//	auto trackmap = findNode::getClass<SvtxTrackSeed_v1>(topNode, "SvtxTrackSeed_v1");
	auto trackmap = findNode::getClass<SvtxTrackMap>(topNode, "SvtxTrackMap");
	
	int nmaps = 0;

	int trackmapsize = trackmap->size();

	std::cout << "trackmapsize = "  << trackmapsize << std::endl; 

	for (const auto &[key, track] : *trackmap)
	{
		if (!track)
		{
			std::cout << "No Tracks" << std::endl;
			continue;
	
		}

		nmaps = 0;

		auto ckeys = TrackAnalysisUtils::get_cluster_keys(track);
//		int trkcrossing = track->get_crossing();
		// std::cout << "track crossing: " << trkcrossing << std::endl;


		for (auto &ckey : ckeys)
		{

			if (TrkrDefs::getTrkrId(ckey) == TrkrDefs::mvtxId){


				nmaps++;
				int layer = TrkrDefs::getLayer(ckey);


				std::cout << "layer: " << layer << std::endl;
			}

		}

		if(nmaps == 2) std::cout << "No 3 MVTX Hits -> Actually 2" << std::endl;
		
	}





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
	return Fun4AllReturnCodes::EVENT_OK; 
}




