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

#include <string>
#include <vector>

class MvtxHitEff : public SubsysReco
{
 public:
  MvtxHitEff(const std::string& name = "MvtxRawHitQA");

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
	
  TH2D * ClusHits;
  TH2D * MissingClus;
	

  int TotalTracklets;
  int TwoClusTracklets;


};

#endif  // MVTXRAWHITQA_H
