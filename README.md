# N-puzzle

## Parsing

Des tests sur le parsing sont fournis.

À la racine du projet :

```bash
./tests/parsing/parsing.bash
```

Les tests s'effectuent un à un, en affichant le fichier avant. Les options suivantes sont disponibles :

- `--no-waiting` : permet de ne pas attendre l'input utilisateur entre chacun des tests
- `--valgrind` : lance le programme avec valgrind et les options **-q** et **--leak-check=full** qui permettent d'afficher un message seulement si des memory leaks sont présents
