#!/bin/bash
PDMO=/home/fbergero/PHD/pd2mo
MOCC=/home/fbergero/PHD/branches/new_parser/bin
QSS=/home/fbergero/PHD/qssengine
fbname=$(basename "$1" .pdm)
filename="${1%.*}"
$PDMO/pd2mo $filename.pds
$MOCC/causalize <$fbname.mo | $MOCC/mmo >qsst.mo
TF=`cat $filename.stm | tail -n 1`
head -n-1 qsst.mo > qss.mo
echo "  annotation(
  experiment(
    MMO_Description=\"\",
    MMO_Solver=QSS3,
    MMO_Output={QSSIntegrator_1_y[1]},
    StartTime=0.0,
    StopTime=$TF,
    Tolerance={1e-3},
    AbsTolerance={1e-3}
  ));" >> qss.mo
tail -n 1 qsst.mo >> qss.mo
rm -rf $QSS/build/qss
mkdir -p $QSS/build/qss
$QSS/bin/mmoc.sh -o$QSS/build/qss/qss `pwd`/qss.mo
make -C $QSS/build/qss/ -f $QSS/build/qss/qss.makefile
pushd .
cd $QSS/build/qss/
$QSS/build/qss/qss
gnuplot qss.plt
popd
