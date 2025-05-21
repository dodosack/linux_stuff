#!/usr/bin/env bash

# Pfade zu deinen Binaries
SERVER_PATH="./server_socket"
CLIENT_PATH="./client_socket"

# Standard-Nachricht, kann per Argument überschrieben werden
MESSAGE="${1:-"Hello from automated client!"}"

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

  if command -v gnome-terminal &>/dev/null; then
    gnome-terminal --window -- bash -c "\
      $cmd1 & pid=\$!; sleep 6; kill \$pid 2>/dev/null; exit" &

    gnome-terminal --window -- bash -c "\
      sleep 3; $cmd2 & pid=\$!; sleep 3; kill \$pid 2>/dev/null; exit" &
  elif command -v xterm &>/dev/null; then
    xterm -e bash -c "\
      $cmd1 & pid=\$!; sleep 6; kill \$pid 2>/dev/null; exit" &
    xterm -e bash -c "\
      sleep 3; $cmd2 & pid=\$!; sleep 3; kill \$pid 2>/dev/null; exit" &
  else
    echo "Kein unterstützter Terminal-Emulator gefunden." >&2
    exit 1
  fi
}

# Befehle definieren
SERVER_CMD="$SERVER_PATH"
# hier wird das -m flag dynamisch mit $MESSAGE gesetzt:
CLIENT_CMD="$CLIENT_PATH -m \"$MESSAGE\""

# Starte beide Fenster
open_two_windows "$SERVER_CMD" "$CLIENT_CMD"
