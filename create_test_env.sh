#!/bin/bash

# Nettoyage avant création
rm -rf test_env regular_file setuid_file setgid_file sticky_file exec_setuid exec_setgid exec_sticky sticky_dir full_special

echo "Création de l'environnement de test..."

# Création dossiers et fichiers basiques
mkdir -p test_env/{dir1,dir2,dir3/subdir}

touch test_env/-weirdfile
touch test_env/file1.txt
touch test_env/dir1/file2.log
echo "Contenu test" > test_env/dir2/file3.txt

# Liens
ln -s ../dir1/file2.log test_env/dir3/link_to_file2.log
ln test_env/file1.txt test_env/dir2/hardlink_to_file1.txt

# FIFO
mkfifo test_env/dir3/fifo_test

# Permissions variées
chmod 777 test_env/file1.txt
chmod 644 test_env/dir2/file3.txt
chmod 000 test_env/dir3/subdir

# Fichiers spéciaux (hors test_env)
touch regular_file

touch setuid_file
chmod u+s setuid_file

touch setgid_file
chmod g+s setgid_file

touch sticky_file
chmod +t sticky_file

touch exec_setuid
chmod u+s exec_setuid
chmod u+x exec_setuid

touch exec_setgid
chmod g+s exec_setgid
chmod g+x exec_setgid

touch exec_sticky
chmod +t exec_sticky
chmod o+x exec_sticky

mkdir sticky_dir
chmod +t sticky_dir

touch full_special
chmod u+s,g+s,+t full_special
chmod +x full_special

echo "✅ Environnement de test complet créé."

echo
echo "Liste des fichiers/dossiers avec détails :"
ls -l test_env
ls -l regular_file setuid_file setgid_file sticky_file exec_setuid exec_setgid exec_sticky sticky_dir full_special
