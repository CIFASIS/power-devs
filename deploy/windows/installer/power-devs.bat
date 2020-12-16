powershell.exe -ExecutionPolicy ByPass -File ../scripts/check-vcxsrv.ps1
cd C:\Users\%USERNAME%\PowerDEVS
wsl -u powerdevs -d Ubuntu-20.04 bash -ic "/opt/powerdevs/bin/run_win.sh"
