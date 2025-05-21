#!/usr/bin/env bash

# Pfade zu deinen Binaries
SERVER_PATH="./server_pipes"
CLIENT_PATH="./client_pipes"

# Prüfen, ob Server und Client ausführbar sind
for PROG in "$SERVER_PATH" "$CLIENT_PATH"; do
  if [[ ! -x "$PROG" ]]; then
    echo "Error: $PROG nicht gefunden oder nicht ausführbar."
    exit 1
  fi
done

open_two_windows() {
  local cmd1="$1"
  local cmd2="$2"

  if command -v gnome-terminal >/dev/null 2>&1; then
    # Server-Fenster: startet sofort, kill nach 6s
    gnome-terminal --window -- bash -c "\
      $cmd1 &                                  \
      pid=\$!;                                 \
      sleep 6;                                 \
      kill \$pid 2>/dev/null;                  \
      exit" &

    # Client-Fenster: startet nach 3s, kill 3s später (=6s Gesamt)
    gnome-terminal --window -- bash -c "\
      sleep 3;                                 \
      $cmd2 &                                  \
      pid=\$!;                                 \
      sleep 3;                                 \
      kill \$pid 2>/dev/null;                  \
      exit" &
  
  elif command -v xterm >/dev/null 2>&1; then
    # xterm-Variante ohne -hold, damit Fenster nach exit schließt
    xterm -e bash -c "\
      $cmd1 &                                  \
      pid=\$!;                                 \
      sleep 6;                                 \
      kill \$pid 2>/dev/null;                  \
      exit" &

    xterm -e bash -c "\
      sleep 3;                                 \
      $cmd2 &                                  \
      pid=\$!;                                 \
      sleep 3;                                 \
      kill \$pid 2>/dev/null;                  \
      exit" &
  else
    echo "Kein unterstützter Terminal-Emulator gefunden (gnome-terminal oder xterm)." >&2
    exit 1
  fi
}

# Befehle definieren
SERVER_CMD="$SERVER_PATH"
CLIENT_CMD="$CLIENT_PATH"

# Starte beide Fenster
open_two_windows "$SERVER_CMD" "$CLIENT_CMD"
