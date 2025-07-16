#!/bin/bash

# Exit on error
set -e

echo "📦 Updating package index..."
sudo apt update

echo "📦 Installing dependencies..."
sudo apt install -y \
    git \
    make \
    gnome-shell \
    gnome-shell-extensions \
    gnome-tweaks \
    nodejs \
    npm

echo "📦 Installing TypeScript (latest)..."
sudo npm install -g typescript@latest

echo "📥 Cloning Pop Shell..."
git clone https://github.com/pop-os/shell.git ~/pop-shell
cd ~/pop-shell

echo "⚙️ Building and installing Pop Shell locally..."
make local-install

echo "✅ Done! Restarting GNOME Shell (Alt+F2 → r → Enter) or re-login to apply changes."
echo "👉 Activate Pop Shell via 'GNOME Tweaks' under Extensions."
echo "🧠 Use Super + Y to enable tiling, Super + H/J/K/L to move, Super + Enter to open terminal."
