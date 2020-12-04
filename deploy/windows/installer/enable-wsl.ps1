param (
    [switch]$Install 
)

Write-Host Enable WSL for Windows 10, computer restart required.
Enable-WindowsOptionalFeature -Online -NoRestart -FeatureName Microsoft-Windows-Subsystem-Linux
if ($Install) {  
    Write-Host Adding PowerDEVS install key.  
    New-Item -Path HKCU:\Software\PowerDEVS -Name WSLRestartNeeded -Force
}
