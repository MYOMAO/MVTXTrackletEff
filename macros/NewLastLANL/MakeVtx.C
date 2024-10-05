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

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

#include "/direct/sphenix+tg+tg01/hf/zshi/TrackletEff/coresoftware/offline/QA/MvtxEff/MvtxHitEff.h"

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
	
void MakeVtx(int Run, int Seg)
{

  std::string infile = Form("/direct/sphenix+tg+tg01/hf/zshi/SeedMatchFileAllNew/DST/DSTSeedSofar_%d_%d.root",Run,Seg);

  int runnumber = Run;
  

  auto se = Fun4AllServer::instance();
  se->Verbosity(2);
  auto rc = recoConsts::instance();
  rc->set_IntFlag("RUNNUMBER", runnumber);

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
  hitsin->fileopen(infile);
  // hitsin->AddFile(inputMbd);
  se->registerInputManager(hitsin);

  auto finder = new PHSimpleVertexFinder;
  finder->Verbosity(0);
  finder->setDcaCut(0.5);
  finder->setTrackPtCut(-99999.);
  finder->setBeamLineCut(1);
  finder->setTrackQualityCut(1000000000);
  finder->setNmvtxRequired(3);
  finder->setOutlierPairCut(0.1);
  se->registerSubsystem(finder);

  std::cout << "MVTXHitAna - Use My Own Ana New Should worlk now" << std::endl; 

  Fun4AllOutputManager *out = new Fun4AllDstOutputManager("out", "Vtx.root");
  se->registerOutputManager(out);

 // MvtxHitEff * MVTXHitAna =  new MvtxHitEff("MVTXHitAna");
 // se->registerSubsystem(MVTXHitAna);

  int nEvents = -1;

  se->run(nEvents);
  se->End();
  se->PrintTimer();

  delete se;
  std::cout << "Finished" << std::endl;
  gSystem->Exit(0);
}
