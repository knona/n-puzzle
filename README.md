# N-puzzle

## Dépendences

Les bibliothèques **SFML** et **Boost** sont utilisées pour ce projet. Elles sont construites à partir de leurs fichiers sources qui sont téléchargés.

Cependant, la bilbliothèque **SFML** possède des dépendences que vous pouvez retrouver [ici](https://www.sfml-dev.org/faq.php#grl-dependencies).
Pour les distributions basées sur debian vous pouvez les installer via cette commande :
```bash
apt install libpthread-stubs0-dev libopengl-dev libx11-dev libxrandr-dev libudev-dev libfreetype-dev
```

Vous pouvez construire et compiler le projet avec les commandes suivantes (contenues dans build.bash) :
```bash
./scripts/install-sfml.bash
./scripts/install-boost.bash
cmake .
make
```

## Tests

Des tests sur le parsing sont fournis.

À la racine du projet :

```bash
./tests/parsing/parsing.bash
```

Les tests s'effectuent un à un, en affichant le fichier avant. Les options suivantes sont disponibles :

- `--no-waiting` : permet de ne pas attendre l'input utilisateur entre chacun des tests
- `--valgrind` : lance le programme avec valgrind et les options **-q** et **--leak-check=full** qui permettent d'afficher un message seulement si des memory leaks sont présents
