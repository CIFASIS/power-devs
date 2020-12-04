@echo off
set PATH=%PATH%;%SystemRoot%\System32
set PATH=%PATH%;C:\Program Files\Subversion\bin
set PATH=%PATH%;C:\Program Files\Ethalone\Ghost Installer\Bin\
set PATH=%PATH%;C:\Program Files\GnuWin32\bin
set QTDIR=C:\Qt\5.4\mingw491_32
echo "Retrieving latest from SVN"
svn up
svnversion >rev1
tr -d '\n' <rev1 >rev2
tr -d '\r' <rev2 >rev
set /p version= <rev
copy version.major+rev version /Y /b
rmdir /s /q tmp_win_installer 
svn export win_installer tmp_win_installer > tmp
svn export atomics tmp_win_installer\powerdevs\atomics > tmp
svn export build tmp_win_installer\powerdevs\build > tmp
svn export engine tmp_win_installer\powerdevs\engine > tmp
xcopy /s /i win_installer\hdf5\* tmp_win_installer\powerdevs\engine\hdf5
svn export examples tmp_win_installer\powerdevs\examples> tmp
svn export library tmp_win_installer\powerdevs\library> tmp
svn export output tmp_win_installer\powerdevs\output> tmp
copy win_installer\hdf5\bin\*.dll tmp_win_installer\powerdevs\output
copy bin\*.sce tmp_win_installer\powerdevs\bin
copy version tmp_win_installer\powerdevs
mkdir tmp_win_installer\powerdevs\doc\
copy doc\PD_UserGuide.pdf tmp_win_installer\powerdevs\doc\
copy COPYING tmp_win_installer\powerdevs\COPYING
echo "Building Binaries";
cd src/uname
qmake -r -spec win32-g++ "CONFIG+=release"
mingw32-make clean
mingw32-make 
cd ../pdae
qmake -r -spec win32-g++ "CONFIG+=release"
mingw32-make clean
mingw32-make 
cd ../pdif
qmake -r -spec win32-g++ "CONFIG+=release"
mingw32-make clean
mingw32-make 
cd ../pdme_v2
qmake -r -spec win32-g++ "CONFIG+=release"
mingw32-make clean
mingw32-make 
cd ../pdppt
qmake -r -spec win32-g++ "CONFIG+=release"
mingw32-make clean
mingw32-make
copy %QTDIR%\bin\Qt5Core.dll ..\..\tmp_win_installer\powerdevs\bin
copy %QTDIR%\bin\Qt5Gui.dll ..\..\tmp_win_installer\powerdevs\bin
copy %QTDIR%\bin\Qt5Svg.dll  ..\..\tmp_win_installer\powerdevs\bin
copy %QTDIR%\bin\Qt5Widgets.dll  ..\..\tmp_win_installer\powerdevs\bin
copy %QTDIR%\bin\libgcc_s_dw2-1.dll  ..\..\tmp_win_installer\powerdevs\bin
copy %QTDIR%\bin\libstdc*.dll ..\..\tmp_win_installer\powerdevs\bin
copy %QTDIR%\bin\ic*.dll ..\..\tmp_win_installer\powerdevs\bin
copy %QTDIR%\bin\libwinpthread*.dll ..\..\tmp_win_installer\powerdevs\bin
cd ../../tmp_win_installer
gibuild PowerDEVS.gpr
cd ../
copy /Y tmp_win_installer\Output\* .
copy /Y powerdevs_install.exe powerdevs_install_%version%.exe
del powerdevs_install.exe
rmdir /s /q tmp_win_installer 
