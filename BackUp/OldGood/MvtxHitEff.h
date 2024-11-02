// Tell emacs that this is a C++ source
//  -*- C++ -*-.
#ifndef MVTXRAWHITQA_H
#define MVTXRAWHITQA_H

#include <fun4all/SubsysReco.h>
#include <trackbase/TrkrDefs.h>

#include <ffarawobjects/MvtxRawHitContainer.h>
#include <ffarawobjects/MvtxRawHit.h>

#include <TH1.h>
#include <TH2.h>
#include <TProfile2D.h>
#include <TH3.h>

#include <string>
#include <vector>

class MvtxHitEff : public SubsysReco
{
 public:
  MvtxHitEff(const std::string& name = "MvtxHitEff");

  ~MvtxHitEff() override = default;

  int InitRun(PHCompositeNode* topNode) override;
  int process_event(PHCompositeNode* topNode) override;
  int EndRun();

  int End(PHCompositeNode *topNode) override;
  
 private:

  PHG4CylinderGeomContainer * geantGeom_MVTX = NULL;
  ActsGeometry * actsGeom = NULL;
  int NMVTXLayer = 3;	
  TFile * fout = NULL;  
  TTree * MissingChip = NULL;
  int layerindex;
  int staveindex;
  int chipindex;
  int Event;
  int TrackletID;
  float SiSeedEta;
  float SiSeedPt;

  TH1D * MVTXClusSize;
  TH1D * INTTClusSize;



  TH2D * ClusHits;
  TH2D * SiClusHits;
 
  TH2D * MissingClus;

  TH3D * MissingClus3D;
  TH3D * ClusHits3D;


  TTree * SiSeedAna = NULL;
  TTree * VertexTree = NULL;

  TTree * TotalChip = NULL;

  int TotalTracklets;
  int TwoClusTracklets;

  int TotalClus;
  int TotalClusAssoc;
 
  int nmaps;
  int nintt;
  int SiSeedSize;
  int SeedID;
  int Crossing;

  float chipphi;
  float chipz;

  float chisq;
  int ndf;
  float quality;
  
  int VertexCrossing;

  std::vector<float> ClusPosX;
  std::vector<float> ClusPosY;
  std::vector<float> ClusPosZ;

  std::vector<int> INTTTimeBucketVec;

  int Ntracks;
  int Vx;
  int Vy;
  int Vz;
  int m_nvertices;

  double x_miss;
  double y_miss;
  double z_miss;
	
  int layerfired;
  int stavefired;
  int chipfired;
  float chipphifired;
  float chipzfired;

  int vtxId;
 
  float vtxX;
  float vtxY;
  float vtxZ;

  int nmaps_uniqlay;
  int intt_uniqlay;

  int mvtxclusmax = 4;
  int inttclusmax = 10;

//  std::vector<int> ClusIdVec; 
  std::vector<uint64_t> ClusKeyVec; 

};

#endif  // MVTXRAWHITQA_H
