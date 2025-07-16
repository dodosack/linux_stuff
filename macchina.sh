#!/bin/bash

set -e

echo "🔧 Schritt 1: Abhängigkeiten installieren..."
sudo apt update
sudo apt install -y cargo zsh

echo "📦 Schritt 2: macchina installieren (wenn nicht vorhanden)..."
if [ ! -f "$HOME/.cargo/bin/macchina" ]; then
  cargo install macchina
else
  echo "✅ macchina ist bereits installiert."
fi

ZSHRC="$HOME/.zshrc"

# PATH eintragen, falls noch nicht vorhanden
if ! grep -q 'export PATH="$HOME/.cargo/bin:$PATH"' "$ZSHRC"; then
  echo "➕ Eintrag für \$PATH wird in .zshrc hinzugefügt..."
  echo 'export PATH="$HOME/.cargo/bin:$PATH"' >> "$ZSHRC"
fi

# macchina-Start nur bei interaktivem Terminal
if ! grep -q 'macchina' "$ZSHRC"; then
  echo "🧠 Autostart-Bedingung für macchina wird hinzugefügt..."
  cat << 'EOF' >> "$ZSHRC"

# macchina automatisch anzeigen bei interaktivem Terminal
if [[ $- == *i* ]] && [ -t 1 ]; then
  macchina
fi
EOF
fi

echo "✅ Fertig! Änderungen aktiv. Starte ein neues Terminal oder führe aus:"
echo "   source ~/.zshrc"
