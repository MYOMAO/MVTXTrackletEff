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
#include <Trkr_RecoInit.C>
#include <Trkr_Clustering.C>
#include <Trkr_Reco.C>

#include <ffamodules/CDBInterface.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllServer.h>

#include <eventdisplay/TrackerEventDisplay.h>
#include <phool/recoConsts.h>
#include <trackingqa/InttClusterQA.h>

#include <trackingqa/MicromegasClusterQA.h>

#include <trackingqa/MvtxClusterQA.h>

#include <trackingdiagnostics/TrackResiduals.h>
#include <trackingdiagnostics/TrkrNtuplizer.h>
#include <trackingqa/TpcClusterQA.h>
#include <trackreco/AzimuthalSeeder.h>

#include "/direct/sphenix+tg+tg01/hf/zshi/TrackletEff/coresoftware/offline/QA/MvtxEff/MvtxHitEff.h"


#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libmvtx.so)
R__LOAD_LIBRARY(libintt.so)
R__LOAD_LIBRARY(libtpc.so)
R__LOAD_LIBRARY(libmicromegas.so)
R__LOAD_LIBRARY(libTrackingDiagnostics.so)
R__LOAD_LIBRARY(libtrackingqa.so)
R__LOAD_LIBRARY(libEventDisplay.so)
R__LOAD_LIBRARY(libmvtxhiteff.so)

void QuickSeeding(std::string infile = "/sphenix/lustre01/sphnxpro/physics/slurp/tracking/run_00048000_00048100/DST_TRKR_CLUSTER_run2pp_new_2024p004-00048002-00000.root")
{

 // std::string infile = "/sphenix/lustre01/sphnxpro/physics/slurp/tracking/run_00048000_00048100/DST_TRKR_SEED_run2pp_new_2024p004-00048002-00000.root";
  
  auto se = Fun4AllServer::instance();
  se->Verbosity(2);
  auto rc = recoConsts::instance();
//  rc->set_IntFlag("RUNNUMBER", 47502);

  Enable::CDB = true;
  rc->set_StringFlag("CDB_GLOBALTAG", "ProdA_2024");
  rc->set_uint64Flag("TIMESTAMP", 6);


  //ACTSGEOM::tpotMisalignment = 100.;

  std::string geofile = CDBInterface::instance()->getUrl("Tracking_Geometry");
  Fun4AllRunNodeInputManager *ingeo = new Fun4AllRunNodeInputManager("GeoIn");
  ingeo->AddFile(geofile);
  se->registerInputManager(ingeo);

  G4MAGNET::magfield_rescale = 1;

  std::cout << "Inite Bro" << std::endl;
 // Tracking_Reco();

  ACTSGEOM::ActsGeomInit();

  ACTSGEOM::mvtxMisalignment = 100;
  ACTSGEOM::inttMisalignment = 100.;

  auto hitsin = new Fun4AllDstInputManager("InputManager");
//  Fun4AllInputManager *infile = new Fun4AllDstInputManager("Infile");
  hitsin->fileopen(infile);
 // hitsin->AddListFile("FileList.txt");
  // hitsin->AddFile(inputMbd);

	
//  MVTXHitAna->InitRun();
//  MVTXHitAna->process_event();
  se->registerInputManager(hitsin);

 // auto hitsin2 = new Fun4AllDstInputManager("InputManager2");

//  hitsin2->fileopen(infile2);
 // se->registerInputManager(hitsin2);


  // Assemble silicon clusters into track stubs

  auto silicon_Seeding = new PHActsSiliconSeeding;
  silicon_Seeding->Verbosity(1);
  se->registerSubsystem(silicon_Seeding);

  auto merger = new PHSiliconSeedMerger;
  merger->Verbosity(1);
  se->registerSubsystem(merger);


  Fun4AllOutputManager *out = new Fun4AllDstOutputManager("out", "SiSeeding.root");
  se->registerOutputManager(out);


//  MvtxHitEff * MVTXHitAna = new MvtxHitEff("MVTXHitAna");

  //se->registerSubsystem(MVTXHitAna);
  

  int nEvents = 1000;

  se->run(nEvents);
  se->End();
  se->PrintTimer();

  delete se;
  std::cout << "Finished" << std::endl;
  gSystem->Exit(0);
}
