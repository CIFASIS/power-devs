param (
    [string]$InstallDir,
    [string]$TmpDir
     )


Write-Host PowerDEVS initial setup...
Write-Host $TmpDir
cd $TmpDir

# Check initial configuration for required dependencies.
$ubuntu_installed = Get-AppxPackage -Name  CanonicalGroupLimited.Ubuntu20.04onWindows
if ($ubuntu_installed.Status -eq "Ok") {
    Write-Host Ubuntu 20.04 for Windows already installed, continue PowerDEVS setup.
} else {
    Write-Host Preparing Ubuntu 20.04 package...
    Add-AppxPackage .\Ubuntu_2004.appx
    
    Write-Host Installing Ubuntu 20.04...
    ubuntu2004 install --root
}

cp .\powerdevs.deb $InstallDir\scripts\

cd $InstallDir

# Remove Windows EOF extra characters, needed by Jenkins installer.

$original_file = Convert-Path '.\scripts\power-devs-setup.sh'
$text = [IO.File]::ReadAllText($original_file) -replace "`r`n", "`n"
[IO.File]::WriteAllText($original_file, $text)

$original_file = Convert-Path '.\scripts\power-devs-config.sh'
$text = [IO.File]::ReadAllText($original_file) -replace "`r`n", "`n"
[IO.File]::WriteAllText($original_file, $text)

$original_file = Convert-Path '.\scripts\export-vcxsrv-vars.sh'
$text = [IO.File]::ReadAllText($original_file) -replace "`r`n", "`n"
[IO.File]::WriteAllText($original_file, $text)

Write-Host Install PowerDEVS
bash.exe ./scripts/power-devs-setup.sh

ubuntu2004.exe config --default-user powerdevs

Write-Host Configure PowerDEVS
bash.exe ./scripts/power-devs-config.sh