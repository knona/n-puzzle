# N-puzzle

## Dépendences

### Boost program_options

Facilite le parsing des différents arguments passés en ligne de commande.
[Doc](https://www.boost.org/doc/libs/1_75_0/doc/html/program_options.html)

- **MacOs**: `brew install boost`
- **Debian-based distribution**: `apt install libboost-program-options-dev`

## Parsing

Des tests sur le parsing sont fournis.

À la racine du projet :

```bash
./tests/parsing/parsing.bash
```

Les tests s'effectuent un à un, en affichant le fichier avant. Les options suivantes sont disponibles :

- `--no-waiting` : permet de ne pas attendre l'input utilisateur entre chacun des tests
- `--valgrind` : lance le programme avec valgrind et les options **-q** et **--leak-check=full** qui permettent d'afficher un message seulement si des memory leaks sont présents
