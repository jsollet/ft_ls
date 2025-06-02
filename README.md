# ft_ls
Une réimplémentation de la commande `ls` sous Linux, écrite en C.

## 🔧 Compilation
`builds.sh` : construit la librairie `libacl` dans le dossier `/local`
`make`      : compile `ft_ls`
`./ft_ls`   : exécute le programme

## 🔧 Scripts utils
`create_test_env.sh`    : crée un dossier `test_env` servant à tester `ft_ls`
`test_acl_xattr.sh`     : crée des fichiers avec des attributs étendu
`compare.sh`            : compare les sorties de `ft_ls` et `ls`, en ignorant la pagination

## 🚀 Utilisation
usage: `./ft_ls [-@RUalrtufgdeU1] [--color --acl --attr --extended][file ...]`

## 🧩 Options implémentées

# Options de base (comme ls GNU)
-l : affichage long
-R : récursif
-a : inclut les fichiers cachés
-t : trie par date de modification
-r : inverse l’ordre de tri
-u : trie par date d'accès (avec -t)
-f : pas de tri, équivalent à -aU (selon GNU ls)
-g : affichage long sans le nom de l'utilisateur
-d : affiche les dossiers sans les lister
-1 : une entrée par ligne
-U : tri dans l’ordre physique du disque (inode)

## Extensions par rapport à `ls`

Certaines options ne sont pas présentes dans la version standard de `ls`, mais ont été ajoutées pour les besoins des bonus du projet `ft_ls` :

- `@` : affiche les attributs étendus (xattr), comme `ls -@` sur macOS.
- `e` : affiche les ACL (similaire à `ls -e` sur macOS).
- `--acl`, `--attr`, `--extended` : options longues ajoutées pour activer plus explicitement certaines fonctionnalités bonus.

Ces options sont propres à ce projet et peuvent ne pas exister dans `ls` GNU.

## ❗Limitation connue
Certaines options GNU (ex: --sort, --group-directories-first, etc.) ne sont pas implémentées.
Couleurs partiellement compatibles avec ls --color.
Note : L'affichage des ACL et attributs étendus peut ne pas fonctionner si le projet est exécuté depuis un répertoire temporaire (ex. /tmp) ne supportant pas ces fonctionnalités. Tester dans un répertoire utilisateur standard est recommandé.


## 📘 Page man

Une page man personnalisée est disponible dans `ft_ls.1`.

Pour la consulter localement :

```bash
man -l ft_ls.1
```

## 👤 Auteur

Jean-Marie Solletti – 42 project – 2025
