source /opt/sphenix/core/bin/sphenix_setup.csh -n new

#setenv MYINSTALL /sphenix/user/zshi/FastMLUConn/HFMLTriggerCodes/install/
#setenv LD_LIBRARY_PATH $MYINSTALL/lib:$LD_LIBRARY_PATH
#set path = ( $MYINSTALL/bin $path )

rm -r install

mkdir install
setenv MYINSTALL $PWD/install/
setenv LD_LIBRARY_PATH $MYINSTALL/lib:$LD_LIBRARY_PATH
set path = ( $MYINSTALL/bin $path )


cd coresoftware/offline/QA/MvtxEff/ 
make clean
autogen.sh --prefix=$MYINSTALL
make -j20 install
cd  ../../../../



#setenv ROOT_INCLUDE_PATH /sphenix/user/zshi/EvtGenPullRequest/macros/common:$ROOT_INCLUDE_PATH
#setenv ROOT_INCLUDE_PATH /sphenix/user/zshi/FastMLLatest/JobSub/macros/common:$ROOT_INCLUDE_PATH

setenv ROOT_INCLUDE_PATH ${PWD}/macros/common:$ROOT_INCLUDE_PATH
