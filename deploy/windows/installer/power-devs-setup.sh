#!/bin/bash

sudo apt-get update

echo "Installing PowerDEVS Dependencies"
sudo apt-get install gnuplot-x11
sudo apt-get install build-essential
sudo apt-get install libc6
sudo apt-get install qt5-default
sudo apt-get install libqt5svg5
sudo apt-get install zenity
sudo apt-get install libhdf5-dev

echo "Installing PowerDEVS"
sudo dpkg -i -y ./powerdevs.deb

# Packages needed to open user folder browser.
sudo apt-get install -y konqueror
sudo apt-get install -y nautilus

sudo useradd -m -s /bin/bash -G sudo,dialout -p $(openssl passwd powerdevs) powerdevs
