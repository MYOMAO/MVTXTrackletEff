/*
 * This macro shows a minimum working example of running the tracking
 * hit unpackers with some basic seeding algorithms to try to put together
 * tracks. There are some analysis modules run at the end which package
 * hits, clusters, and clusters on tracks into trees for analysis.
 */

#include <fun4all/Fun4AllUtils.h>
#include <G4_ActsGeom.C>
#include <G4_Global.C>
#include <G4_Magnet.C>
#include <G4_Mbd.C>
#include <GlobalVariables.C>
#include <QA.C>
#include <Trkr_Clustering.C>
#include <Trkr_Reco.C>
#include <Trkr_RecoInit.C>
#include <Trkr_TpcReadoutInit.C>

#include <ffamodules/CDBInterface.h>
#include <Trkr_LaserClustering.C>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllServer.h>

#include <phool/recoConsts.h>

#include <cdbobjects/CDBTTree.h>

#include <tpccalib/PHTpcResiduals.h>

#include <trackingqa/InttClusterQA.h>
#include <trackingqa/MicromegasClusterQA.h>
#include <trackingqa/MvtxClusterQA.h>
#include <trackingqa/TpcClusterQA.h>

#include <trackingdiagnostics/TrackResiduals.h>
#include <trackingdiagnostics/TrkrNtuplizer.h>
#include <trackingdiagnostics/KshortReconstruction.h>

#include <trackermillepedealignment/AlignmentDefs.h>
#include <trackermillepedealignment/HelicalFitter.h>


#include <stdio.h>

	R__LOAD_LIBRARY(libfun4all.so)
	R__LOAD_LIBRARY(libffamodules.so)
	R__LOAD_LIBRARY(libphool.so)
	R__LOAD_LIBRARY(libcdbobjects.so)
	R__LOAD_LIBRARY(libmvtx.so)
	R__LOAD_LIBRARY(libintt.so)
	R__LOAD_LIBRARY(libtpc.so)
	R__LOAD_LIBRARY(libmicromegas.so)
	R__LOAD_LIBRARY(libTrackingDiagnostics.so)
	R__LOAD_LIBRARY(libtrackingqa.so)


void MakeSiFit(int Run, int Seg)
{


	std::cout << "Now I also go with the Silicon Seeds -> Worked?" << std::endl;

	std::string infile = Form("/direct/sphenix+tg+tg01/hf/zshi/MVTXFinalStudy/DST/DSTACTSFitted_NoClean_Vertex_%d_%d.root",Run,Seg);


	int runnumber = Run;
	int segment = Seg;

	TRACKING::pp_mode = true;

	ACTSGEOM::mvtxMisalignment = 100;
	ACTSGEOM::inttMisalignment = 100.;
	ACTSGEOM::tpotMisalignment = 100.;

	auto se = Fun4AllServer::instance();
	se->Verbosity(2);
	auto rc = recoConsts::instance();
	rc->set_IntFlag("RUNNUMBER", runnumber);
	rc->set_IntFlag("RUNSEGMENT", segment);

	Enable::CDB = true;
	rc->set_StringFlag("CDB_GLOBALTAG", "ProdA_2024");
	rc->set_uint64Flag("TIMESTAMP", runnumber);
	std::string geofile = CDBInterface::instance()->getUrl("Tracking_Geometry");

	Fun4AllRunNodeInputManager *ingeo = new Fun4AllRunNodeInputManager("GeoIn");
	ingeo->AddFile(geofile);
	se->registerInputManager(ingeo);

	std::cout << "Using infile: " << infile  << std::endl;

	auto hitsin = new Fun4AllDstInputManager("InputManager");
	hitsin->fileopen(infile);
	// hitsin->AddFile(inputMbd);
	se->registerInputManager(hitsin);

	std::cout << "Re-initialize Tracking" << std::endl;
	TrackingInit();
	std::cout << "Done Initialization" << std::endl;
	

	std::cout << "Just Fit to Silicon Seeds wit Helix for Quality Extraction" << std::endl;

    auto converter = new TrackSeedTrackMapConverter;
    // Default set to full SvtxTrackSeeds. Can be set to
    // SiliconTrackSeedContainer or TpcTrackSeedContainer
    converter->setTrackSeedName("SiliconTrackSeedContainer");
    converter->setFieldMap(G4MAGNET::magfield_tracking);
    converter->Verbosity(0);
    se->registerSubsystem(converter);

	Fun4AllOutputManager *out = new Fun4AllDstOutputManager("out", "DSTSilicon.root");
	se->registerOutputManager(out);

	int nEvents = 10;

	se->run(nEvents);
	se->End();
	se->PrintTimer();

	delete se;
	std::cout << "Finished" << std::endl;
	gSystem->Exit(0);
}
