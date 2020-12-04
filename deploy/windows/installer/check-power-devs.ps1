param (
    [switch]$Install 
)

Write-Host Checking PowerDEVS installation

$qss_solver_installed = bash.exe -c "command -v qss-solver"

if ([string]::IsNullOrEmpty($qss_solver_installed)) {
    Write-Host PowerDEVS installation failed
    if ($Install) {  
        Write-Host Adding PowerDEVS install key.  
        New-Item -Path HKCU:\Software\PowerDEVS -Name PowerDEVSInstallFailed –Force
    }
} else {
    Write-Host PowerDEVS successfully installed!
}
