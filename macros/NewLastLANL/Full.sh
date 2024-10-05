#!/bin/csh
setenv HOME /star/u/$LOGNAME
#setenv HOME /sphenix/user/$LOGNAME

source /etc/csh.login
foreach i (/etc/profile.d/*.csh)
 source $i
end

source $HOME/.login
#source /direct/star+u/zshi/.login

#source /cvmfs/sphenix.sdcc.bnl.gov/x8664_sl7/opt/sphenix/core/bin/sphenix_setup.csh -n ana.141

#source /cvmfs/sphenix.sdcc.bnl.gov/x8664_sl7/opt/sphenix/core/bin/sphenix_setup.csh -n
#source /cvmfs/sphenix.sdcc.bnl.gov/x8664_sl7/opt/sphenix/core/bin/setup_root6.csh

#source /opt/sphenix/core/bin/sphenix_setup.csh -n
#source /opt/sphenix/core/bin/setup_root6.csh

#source /opt/sphenix/core/bin/setup_root6_include_path.csh


echo "START PRINT ENV"

#printenv


echo "DONE PRINt ENV"


set Name=$argv[1]
set Run=$argv[2]
set Seg=$argv[3]



#source /opt/sphenix/core/bin/sphenix_setup.csh -n





#source Build.sh


echo "Now PWD"

pwd

ls

echo "DONE CHECK"

cd workdir

mkdir ${Name}

#source Reconnect.sh


cp -r ../macros ${Name}/ 

cp ../Link.sh ${Name}/ 


cd ${Name}

#source /opt/sphenix/core/bin/sphenix_setup.csh -n new

source Link.sh
echo "NowList"


#mkdir -p macros/detectors/sPHENIX

#mv Simple.C macros/detectors/sPHENIX

cd  macros/MVTXTrackletAna/



#root -b -l -q Simple.C'('${Run}','${Seg}')'

csh runit_beam2.csh ${Run} ${Seg}
 
echo "Now LS see what we have"

ls *root
#mv  track_match.root  /direct/sphenix+tg+tg01/hf/zshi/Fitted/Match/track_match_${Run}_${Seg}.root

mv DSTSeedSofar.root ../detectors/sPHENIX/

cd ../../


cd macros/Fitter/

echo "See what we have here"

ls *

echo "OK Good -> Make Fit"

root -b -l -q MakeFitJob.C'('${Run},${Seg}')'


echo "See what we have next"

ls *


echo "OK Good"

mv  DSTACTSFitted_NoClean_Vertex.root /direct/sphenix+tg+tg01/hf/zshi/Fitted/DST/DSTACTSFitted_NoClean_Vertex_${Run}_${Seg}.root
mv  SeedAna_Svtx.root  /direct/sphenix+tg+tg01/hf/zshi/Fitted/Ntuple/SeedAna_Svtx_${Run}_${Seg}.root
mv  EvtAna_Svtx.root  /direct/sphenix+tg+tg01/hf/zshi/Fitted/EventFile/EvtAna_Svtx_${Run}_${Seg}.root


cd ../../


cd ../../

rm -rf workdir/${Name}


