Crossing=$2
Run=$1

echo "Now Run the Cleaning Work Flow - Run ID: "  $Run "   Crossing Focus = " ${Crossing} 


echo "Step 0: Select Crossing " ${Crossing} 

root -b -l -q Step0.C'('${Crossing},${Run}')'



echo "Step 1: Clean Repeat Silicon Seeds" 
root -b -l -q Step1.C'('${Run}')'



echo "Step 2: Clean ReFit and Select Best Silicon Seeds" 
root -b -l -q Step2.C'('${Run}')'


echo "Step 3: Merge Two Tree to Have In Comment Tracklets -> No Nee Now" 
#root -b -l -q Step3.C'('${Run}')'



echo "DONE"
