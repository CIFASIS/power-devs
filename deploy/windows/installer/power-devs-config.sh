echo "Adding X server display environment variables"
echo 'export DISPLAY=:0.0' >> ~/.bashrc
echo 'export LIBGL_ALWAYS_INDIRECT=1' >> ~/.bashrc

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
