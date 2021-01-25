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

## Lancer le projet

Voici les options supportées par le programme :

```
Usage: ./n-puzzle [options]
Options:
  -h [ --help ]             Produce help message
  -f [ --file ] arg         Path to the puzzle file (default stdin)
  --parse-only              Parse input and display the puzzle
  -q [ --quiet ]            Do not display solution on stdout
  --heuristic arg (=linear) Heuristic to use :[manhattan/linear/hamming] (default linear)
  --algorithm arg (=astar)  Algorithm to use :[astar/greedy/uniform] (default astar)
  -g [ --gui ]              Enable the gui
```

Un générateur de puzzle aléatoire est fourni :

```
usage: npuzzle-gen.py [-h] [-s] [-u] [-i ITERATIONS] size

positional arguments:
  size                  Size of the puzzle's side. Must be >3.

optional arguments:
  -h, --help                   show this help message and exit
  -s, --solvable               Forces generation of a solvable puzzle. Overrides -u.
  -u, --unsolvable             Forces generation of an unsolvable puzzle
  -i, --iterations ITERATIONS  Number of passes
```

Vous pouvez utiliser la commande suivante pour exécuter le solveur avec un puzzle généré aléatoirement :

```bash
./resources/npuzzle-gen.py -s 3 | ./n-puzzle
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

##Détails sur le projet

N-Puzzle est le premier projet avancé de parcours de graphe de la branche `Algorithmique` de 42. L'algorithme principal imposé est l'A*, de type parcours en largeur, que nous utilisons de concert avec trois heuristiques, au choix :

1. Manhattan distance
2. Hamming distance
3. Linear Conflict Distance

Une heuristique n'est admissible que si elle ne surestime jamais le coût pour atteindre l'objectif. Ici, l'objectif est de réussir à atteindre l'état final du puzzle. Les trois heuristiques présentées ci-dessus sont toutes les trois admissibles, ce que nous allons essayer d'expliciter ci-après.

####Manhattan Distance

Aussi appelée taxi-distance, nous utilisons la somme des distances de Manhattan de chaque pièce du puzzle comme heuristique.

*Formule de la distance de Manhattan:*
$$
d(A,B) = |Xb - Xa| + |Yb - Ya|
$$


*Définition de notre heuristique :*
$$
\begin{align*}
h(puzzle) &= \sum(d(A,B))\  \forall\ piece\ in\ Puzzle \\
&\ with\ \\
&A =position\ actual\ piece \\
&B = expected\ position\ actual\ piece
\end{align*}
$$
L'utilisation d'une heuristique en tant qu'estimation locale du choix optimal permet de parcourir le graphe de façon plus rapide et, dans notre cas, grâce à son admissibilité, de ressortir le plus court chemin amenant à la solution.

