# N-puzzle

## Dépendences

### Boost program_options

Facilite le parsing des différents arguments passés en ligne de commande.
[Documentation](https://www.boost.org/doc/libs/1_75_0/doc/html/program_options.html)

- **Debian-based distribution**: `apt install libboost-program-options-dev`

### Sfml

Cette bibliothèque graphique permet l'affichage du puzzle et de sa solution.
[Site web](https://www.sfml-dev.org/index.php)

- **Debian-based distribution**: `apt install libsfml-dev`

## Tests

Des tests sur le parsing sont fournis.

À la racine du projet :

```bash
./tests/parsing/parsing.bash
```

Les tests s'effectuent un à un, en affichant le fichier avant. Les options suivantes sont disponibles :

- `--no-waiting` : permet de ne pas attendre l'input utilisateur entre chacun des tests
- `--valgrind` : lance le programme avec valgrind et les options **-q** et **--leak-check=full** qui permettent d'afficher un message seulement si des memory leaks sont présents
