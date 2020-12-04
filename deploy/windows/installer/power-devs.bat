powershell.exe -ExecutionPolicy ByPass -File ../scripts/check-vcxsrv.ps1
cd C:\Users\%USERNAME%\PowerDEVS
wsl -d Ubuntu-20.04 bash -ic "qss-solver"
