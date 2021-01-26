# N-puzzle



N-Puzzle est le premier projet avancé de parcours de graphe de la branche `Algorithmique` de 42. L'algorithme principal imposé est l'A*, de type parcours en largeur, que nous utilisons de concert avec trois heuristiques, à savoir:

1. *Manhattan distance*
2. *Hamming distance*
3. *Linear Conflict Distance*

Une heuristique n'est admissible que si elle ne surestime jamais le coût pour atteindre l'objectif. Ici, l'objectif est de réussir à atteindre l'état final du puzzle. Les trois heuristiques présentées ci-dessus sont toutes admissibles, et nous allons essayer de les détailler ci-après.

L'utilisation d'une heuristique en tant qu'estimation locale du choix optimal permet de parcourir le graphe de façon plus rapide et, dans notre cas, grâce à son admissibilité, de ressortir le plus court chemin amenant à la solution.

####Manhattan Distance

Aussi appelée taxi-distance, nous utilisons la somme des distances de Manhattan de chaque pièce du puzzle comme heuristique.

*Formule de la distance de Manhattan:*

<img src="https://render.githubusercontent.com/render/math?math=Md(A%2C%20A')%20%3D%20%7CXa'%20-%20Xa%7C%20%2B%20%20%7CYa'%20-%20Ya%7C">



*Définition de notre heuristique :*

<img src="https://render.githubusercontent.com/render/math?math=%5Cbegin%7Balign*%7D%0Ah(puzzle)%20%26%3D%20%5Csum(Md(A%2CA'))%5C%20%5C%5C%0A%26%5Cforall%5C%20piece%5C%20in%5C%20Puzzle%5C%20with%5C%20%5C%5C%0A%26A%20%3Dposition%5C%20actual%5C%20piece%20%5C%5C%0A%26A'%20%3D%20expected%5C%20position%5C%20actual%5C%20piece%0A%5Cend%7Balign*%7D">



Cette heuristique est bel-et-bien admissible car elle aurait, au maximum et dans le cas d'une pièce présente à côté de sa place finale, une distance de 1 équivalente à la distance réelle. Sur une configuration de puzzle où la pièce serait à l'opposée de sa destination finale, placée à l'autre extrémité, elle serait de-facto inférieure à la distance réelle nécessaire.

#### Hamming Distance

La distance de Hamming s'appuie, plus simplement, sur la présence effective de la pièce actuelle sur sa destination finale ou non. 

*Formule de la distance de Hamming:*

<img src="https://render.githubusercontent.com/render/math?math=Hd(A%2C%20A')%20%3D%20A(Xa%2C%20Ya)%20%5Cneq%20A'(Xa'%2C%20Ya')">

*Définition de notre heuristique :*

<img src="https://render.githubusercontent.com/render/math?math=%5Cbegin%7Balign*%7D%0Ah(puzzle)%20%26%3D%20%5Csum(Hd(A%2CA'))%5C%20%5C%5C%0A%26%5Cforall%5C%20piece%5C%20in%5C%20Puzzle%5C%20with%5C%20%5C%5C%0A%26A%20%3Dposition%5C%20actual%5C%20piece%20%5C%5C%0A%26A'%20%3D%20expected%5C%20position%5C%20actual%5C%20piece%0A%5Cend%7Balign*%7D">

Cette heuristique est elle-aussi admissible et facilement appréhendable car elle ne dépasse jamais le nombre de pièces mal placées. Elle reste cependant moins précise que la précédente, et son utilisation ne permet pas un parcours optimal.

#### Linear Conflict Distance

La Linear Conflict Distance, quant-à-elle, est plus difficile à appréhender. Cette distance se décompose en 2 sous-parties :

1. Une Manhattan distance
2. Une comptabilisation du nombre de pièces minimales à déplacer pour réduire les conflits existants sur le puzzle, le tout multiplié par deux

> *Définition d'un conflit* *:* Deux pièces, Tj et Tk, sont en conflit linéaire si et seulement si Tj et Tk sont sur la même ligne (ou colonne), les positions attendues de Tj et Tk sont toutes les deux sur cette même ligne (ou colonne), et que si Tj est à droite de Tk, la position attendue de Tj doit se trouver à gauche de la position attendue de Tk (ou inversement)

*Formule de la distance par conflits linéaires entre 2 pièces:*

<img src="https://render.githubusercontent.com/render/math?math=%5Cbegin%7Balign*%7D%0ALCd(A%2CB)%20%26%3D%20Md(A%2C%20A')%20%2B%20Md(B%2C%20B')%20%2B%202%20*%20conflict(A%2CB)%5C%20%5C%5C%0A%26with%5C%20%5C%5C%0A%26conflict(A%2CB)%20%3D%20presence%5C%20of%5C%20linear%5C%20conflict%5C%20or%5C%20not%5C%20%5B1%2F0%5D%2C%20%5C%5C%0A%26A%5C%20and%5C%20B%3D%20position%5C%20of%5C%202%5C%20pieces%5C%20in%5C%20puzzle%2C%20%5C%5C%0A%26A'%5C%20and%5C%20B'%3D%20respective%5C%20expected%5C%20position%5C%20of%5C%20pieces%0A%5Cend%7Balign*%7D">



Seulement, notre heuristique diffère un peu. Afin de garder son admissibilité, nous ne calculons pas la somme des conflits linéaires tels quels, au risque de nous retrouver avec des doublons au niveau des calculs de distances de Manhattan. 

Notre heuristique se base plutôt sur la Manhattan distance totale du Puzzle, à laquelle nous ajoutons le nombre minimal de pièces à déplacer afin de ne plus avoir de conflits linéaires entre les pièces d'une colonne/ ligne, et ce pour chaque paire de pièces de cette même colonne/ ligne.

*Définition de notre heuristique :*

<img src="https://render.githubusercontent.com/render/math?math=%5Cbegin%7Balign*%7D%0Ah(puzzle)%20%26%3D%20Md(puzzle)%20%2B%202*%20%5Cmin%5C%7B%5C%20conflict(row%7Ccolumn)%5C%7D%5C%20%5C%5C%0A%26%5Cforall%5C%20row%7Ccolumn%5C%C2%A0in%5C%20Puzzle%5C%0A%5Cend%7Balign*%7D">

Cette heuristique est elle-aussi admissible car elle ne surestime jamais la distance entre deux pièces. En effet, nous ne cherchons pas à résoudre les conflits de manière optimale, mais à déplacer les pièces contenant le plus de conflits en priorité. De ce fait, nous nous retrouvons toujours avec le nombre minimal de pièces à déplacer sur un puzzle donné, afin de ne plus avoir les conflits actuels. 

Qui plus est, comme chaque conflit oblige à minima deux mouvements (changer de ligne, puis y revenir), nous pouvons multiplier ce score par deux. La Manhattan distance étant admissible, nous pouvons bien entendu la rajouter dans le calcul de l'heuristique.

Cette heuristique est bien plus efficace et nous permet de calculer aisément des puzzles de taille 4x4.

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
