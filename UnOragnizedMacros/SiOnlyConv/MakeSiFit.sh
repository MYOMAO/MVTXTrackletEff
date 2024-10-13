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

cd macros/SiOnlyConv

echo "See what we have here"

ls *

echo "OK Good"

root -b -l -q MakeSiFit.C'('$Run,$Seg')'


echo "See what we have next"

ls *

echo "OK Good"


#root -b -l -q JobAna.C

echo "See what we have last"

ls *

echo "OK Good"

mv  DSTSiFit.root /direct/sphenix+tg+tg01/hf/zshi/MVTXFinalStudy/SiOnlyDST/DSTSiFit_${Run}_${Seg}.root
cd ../../


cd ../../

rm -rf workdir/${Name}


