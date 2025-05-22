#!/bin/bash

# Crée un dossier temporaire de test
mkdir -p test_acl_xattr
cd test_acl_xattr

echo "Création des fichiers de test..."

# Fichier simple
touch file_plain.txt

# Fichier avec xattr (macOS ou Linux avec attr)
touch file_xattr.txt
if command -v xattr >/dev/null 2>&1; then
    xattr -w user.comment "ceci est un attribut étendu" file_xattr.txt
elif command -v setfattr >/dev/null 2>&1; then
    setfattr -n user.comment -v "ceci est un attribut étendu" file_xattr.txt
fi

# Fichier avec ACL
touch file_acl.txt
if [[ "$OSTYPE" == "darwin"* ]]; then
    chmod +a "everyone deny write" file_acl.txt
elif command -v setfacl >/dev/null 2>&1; then
    setfacl -m u:$(whoami):rw file_acl.txt
fi

# Fichier avec xattr + ACL
touch file_both.txt
if command -v xattr >/dev/null 2>&1; then
    xattr -w user.test "xattr+acl" file_both.txt
elif command -v setfattr >/dev/null 2>&1; then
    setfattr -n user.test -v "xattr+acl" file_both.txt
fi
if [[ "$OSTYPE" == "darwin"* ]]; then
    chmod +a "everyone deny read" file_both.txt
elif command -v setfacl >/dev/null 2>&1; then
    setfacl -m u:$(whoami):r file_both.txt
fi

# Résumé des permissions
echo -e "\nAffichage avec ls -le (macOS) ou ls -l (Linux) :"
ls -le || ls -l

echo -e "\nVérification des xattr (si disponibles) :"
if command -v xattr >/dev/null 2>&1; then
    echo "macOS xattr:"
    xattr -l file_xattr.txt file_both.txt
elif command -v getfattr >/dev/null 2>&1; then
    echo "Linux getfattr:"
    getfattr -d -m . file_xattr.txt file_both.txt 2>/dev/null
fi

echo -e "\nVérification des ACL (si disponibles) :"
if command -v getfacl >/dev/null 2>&1; then
    getfacl file_acl.txt file_both.txt
elif [[ "$OSTYPE" == "darwin"* ]]; then
    ls -le file_acl.txt file_both.txt
fi

echo -e "\n✅ Tests prêts dans $(pwd)"
