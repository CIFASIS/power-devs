#!/bin/bash
if [ ! -d $HOME/powerdevs ]
then
  # No previous instalation and first run
	echo "Installing PowerDEVS..."
	cp -a /opt/powerdevs $HOME/powerdevs
	rm $HOME/powerdevs/bin/run.sh
  cd $HOME/powerdevs/bin
  ./pdme
else
  VEROPT=`cat /opt/powerdevs/version`
  VERHOME=`cat $HOME/powerdevs/version`
  if [  "$VEROPT" == "$VERHOME" ];
  then
    # Previous instalation and same version
    cd $HOME/powerdevs/bin
    ./pdme
  else
    # Previous instalation and different version
    OW=`zenity --question --text "There is a different version of PowerDEVS on your home folder (ver. $VERHOME). Do you wish to overwrite it with ver. $VEROPT?\nNOTE: you will not lose your models."; echo $?`
    if [ $OW == 0 ];
    then 
	    cp -a -f /opt/powerdevs/* $HOME/powerdevs
    	rm $HOME/powerdevs/bin/BackDoor/loader.sce
    	rm $HOME/powerdevs/build/lib/*
    	rm $HOME/powerdevs/build/objs/*
    	rm $HOME/powerdevs/bin/run.sh
      cd $HOME/powerdevs/bin
      ./pdme
    else
      cd $HOME/powerdevs/bin
      ./pdme
    fi
  fi
fi
