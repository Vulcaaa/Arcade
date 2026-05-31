#!/bin/bash

set -ex 

echo "Installing dependencies for Arcade..."

sudo apt-get update

sudo apt-get install -y \
    build-essential cmake clang-format pkg-config \
    libfreetype-dev \
    libwayland-dev wayland-protocols libxkbcommon-dev \
    libx11-dev libxext-dev libgl1-mesa-dev \
    libasound2-dev libpulse-dev \
    libncurses5-dev libncursesw5-dev \
    liballegro5-dev liballegro-image5-dev liballegro-ttf5-dev

echo "Installing Criterion..."

wget -q https://github.com/Snaipe/Criterion/releases/download/v2.4.2/criterion-2.4.2-linux-x86_64.tar.xz
tar xf criterion-2.4.2-linux-x86_64.tar.xz
sudo cp -r criterion-2.4.2/* /usr/local/
sudo ldconfig

echo "Dependencies installed successfully"