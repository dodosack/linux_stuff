#!/bin/bash

# $0 ist  der name  $ option unsere lokale var
option=$1

# Falls -h oder --help übergeben wurde, Hilfe anzeigen
if [[ "$option" == "-h" || "$option" == "--help" ]]; then
    echo "-c, --command  : Zeigt den Prozessnamen an, der den Port nutzt (Standard)"
    echo "-u, --user     : Zeigt die User-ID des Prozesses an"
    echo "-h, --help     : Zeigt diese Hilfe an"
    exit 0
fi

# 2te prüfung um zu schauen ob mehr  1 argument
# Falls mehr als ein Argument übergeben wurde, Fehler ausgeben
if [[ $# -gt 1 ]]; then
# greaterthen  und exit 1 ist wie c fehlermeldung
    echo "Fehler: Bitte nur einen Parameter angeben (-c oder -u)."
    exit 1
fi

# Liste der offenen TCP-Ports holen

# ss   tcp  ports die listen  und nur nummer
#  NR>1 erste zeile niht einscannen und nur 4te spalte  argument eins>
#  awk f: ist das trenzeichen der spalten   sort u ist neu  und nimmt redundanz raus  und sortiert
sudo ss -tln | awk 'NR>1 {print $4}' | awk -F: '{print $NF}' | sort -u | while read port; do  # port var mit while do

    # PID des ersten Prozesses auf diesem Port holen
    pid=$(sudo lsof -iTCP:$port -sTCP:LISTEN -P -n | awk 'NR==2 {print $2}' | head -n 1)
    # pid var  zeigt alles offene an tcp ports von  port var
    # dann scokets die lsiten und wieder Portnummr und als nummer

    # Falls kein Prozess gefunden, nächste Iteration
    # falls pid var leer ist  dann nächste durhclauf
    if [[ -z "$pid" ]]; then
        continue
    fi
    # sosnt ausgabe mit den zwischenspeihcer var cmd
    # die  die prozesse der bestimmten pid asugibt und dann diese auseben>
    if [[ "$option" == "-c" || "$option" == "--command" || -z "$option" ]]; then
        cmd=$(ps -p $pid -o cmd=)
        echo "$port $cmd"
    elif [[ "$option" == "-u" || "$option" == "--user" ]]; then
        user=$(ps -o uid= -p $pid)
        echo "$port $user"
    fi
done
