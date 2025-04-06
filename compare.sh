#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 fichier1 fichier2"
    exit 1
fi

fichier1="$1"
fichier2="$2"

# Normaliser les fichiers : supprimer les espaces en trop et les lignes vides
#sed 's/[[:space:]]\+/ /g; /^$/d' "$fichier1" > /tmp/f1_normalized
#sed 's/[[:space:]]\+/ /g; /^$/d' "$fichier2" > /tmp/f2_normalized

awk '{$1=$1} NF' "$fichier1" > /tmp/f1_normalized
awk '{$1=$1} NF' "$fichier2" > /tmp/f2_normalized

# Comparaison avec diff
diff -u /tmp/f1_normalized /tmp/f2_normalized

# Nettoyage des fichiers temporaires
rm /tmp/f1_normalized /tmp/f2_normalized
