#!/bin/bash
if [ ! -d $HOME/powerdevs ]
then
  # No previous instalation and first run
  echo "Linking PowerDEVS user folder"

  cmd.exe /c "echo %USERNAME%" > user.txt
  WINDOWS_USER=`tr -d '\r' < user.txt`
  rm user.txt

  echo "Found Windows user: " $WINDOWS_USER

  LINUX_POWERDEVS=~/powerdevs
  LINUX_POWERDEVS_TMP=${LINUX_POWERDEVS}-tmp
  WINDOWS_USER_FOLDER=/mnt/c/Users/${WINDOWS_USER}
  WINDOWS_POWERDEVS=${WINDOWS_USER_FOLDER}/PowerDEVS

  echo Temp folders:
  echo $LINUX_POWERDEVS
  echo $LINUX_POWERDEVS_TMP
  echo $WINDOWS_POWERDEVS

  if [[ -L "$LINUX_POWERDEVS" && -d "$LINUX_POWERDEVS" ]]
  then
  echo "Removing previous symbolic link to Windows user folder"
  rm $LINUX_POWERDEVS 
  fi

  if [ -d "$LINUX_POWERDEVS" ]
  then
  mv $LINUX_POWERDEVS $LINUX_POWERDEVS_TMP
  fi

  mkdir -p $WINDOWS_POWERDEVS

  ln -s $WINDOWS_POWERDEVS/ $LINUX_POWERDEVS
  ln -s $WINDOWS_USER_FOLDER/ ~/$WINDOWS_USER 
  ln -s /mnt/c/ ~/C 

  if [ -d "$LINUX_POWERDEVS_TMP" ]
  then
  cp -r $LINUX_POWERDEVS_TMP/* $LINUX_POWERDEVS/
  rm -rf $LINUX_POWERDEVS_TMP
  fi

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
