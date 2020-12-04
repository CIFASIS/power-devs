param (
    [switch]$Install 
)

# Sanity check before starting PowerDEVS installation.

Write-Host Test that WSL works correctly. 

$test_wsl=wsl -l

if (-not [string]::isNullOrEmpty($test_wsl)) {
    Write-Host WSL enabled and working correctly!
    if ($Install) {
        New-Item -Path HKCU:\Software\PowerDEVS -Name WSLEnabled –Force
    }
} else {
    Write-Host WSL is not enabled on this machine!
}
