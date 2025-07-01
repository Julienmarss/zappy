# 🛠 ZAPPY – A Tribute to Zaphod Beeblebrox

Zappy est un projet de jeu en réseau dans lequel plusieurs équipes s’affrontent dans un monde 2D, en collectant des ressources et en effectuant des rituels d’élévation. Le premier objectif est de faire évoluer au moins 6 membres d’une même équipe au niveau maximum (niveau 8).

## 🔧 Technologies utilisées

- **Serveur** – C
- **Client graphique (GUI)** – C++
- **Intelligence Artificielle (AI client)** – Langage libre (ici C++)
- **Communication** – TCP sockets + `poll` pour la gestion des connexions

---

## 🎮 Objectif du jeu

Chaque équipe doit :

- Collecter des ressources (nourriture + 6 types de pierres)
- Se nourrir pour survivre
- Coopérer pour atteindre les conditions d'incantation et monter en niveau
- Être la première équipe à avoir 6 joueurs au niveau 8

---

## 📺 Monde et Ressources

Le monde est une grille torique (sortir à droite = revenir à gauche).

### Ressources disponibles :

| Ressource | Densité |
| --------- | ------- |
| Food      | 0.5     |
| Linemate  | 0.3     |
| Deraumere | 0.15    |
| Sibur     | 0.10    |
| Mendiane  | 0.10    |
| Phiras    | 0.08    |
| Thystame  | 0.05    |

> Exemple : sur une map 10x10 → 50 food, 5 thystame

---

## ⚙️ Compilation

Utiliser `make` à la racine du projet. Le Makefile doit contenir les règles suivantes :

```bash
make zappy_server
make zappy_gui
make zappy_ai
```

---

## 🚀 Lancement des exécutables

### 🔵 Serveur

```bash
./zappy_server -p <port> -x <width> -y <height> -n <team1> <team2> ... -c <clientsNb> -f <freq>
```

**Options :**

- `-p` : port du serveur
- `-x`, `-y` : dimensions de la map
- `-n` : noms des équipes
- `-c` : nombre de slots (clients) par équipe
- `-f` : fréquence (inverse du temps d’unité)

Exemple :

```bash
./zappy_server -p 4242 -x 10 -y 10 -n team1 team2 -c 6 -f 100
```

---

### 🔑 Interface graphique (GUI)

```bash
./zappy_gui -p <port> -h <machine>
```

**Options :**

- `-p` : port du serveur
- `-h` : adresse IP ou nom d’hôte

Exemple :

```bash
./zappy_gui -p 4242 -h localhost
```

---

### 🔴 Client IA

```bash
./zappy_ai -p <port> -n <team_name> -h <machine>
```

**Options :**

- `-p` : port du serveur
- `-n` : nom de l’équipe
- `-h` : adresse IP ou nom d’hôte

Exemple :

```bash
./zappy_ai -p 4242 -n team1 -h 127.0.0.1
```

---

## 📈 Élévations (Incantations)

Pour monter de niveau, un joueur doit réunir :

- Les **bons objets**
- Le **bon nombre de joueurs** du même niveau (équipe peu importe)

| Niveau | Joueurs | Linemate | Deraumere | Sibur | Mendiane | Phiras | Thystame |
| ------ | ------- | -------- | --------- | ----- | -------- | ------ | -------- |
| 1 → 2  | 1       | 1        | 0         | 0     | 0        | 0      | 0        |
| 2 → 3  | 2       | 1        | 1         | 1     | 0        | 0      | 0        |
| 3 → 4  | 2       | 2        | 0         | 1     | 0        | 2      | 0        |
| 4 → 5  | 4       | 1        | 1         | 2     | 0        | 1      | 0        |
| 5 → 6  | 4       | 1        | 2         | 1     | 3        | 0      | 0        |
| 6 → 7  | 6       | 1        | 2         | 3     | 0        | 1      | 0        |
| 7 → 8  | 6       | 2        | 2         | 2     | 2        | 2      | 1        |

---

## 📱 Communication réseau

- Protocoles : **TCP socket**
- Utilisation de `poll` pour la gestion non-bloquante
- Jusqu’à **10 requêtes en file d’attente** par client
- Commandes principales :
  - `Forward`, `Right`, `Left`
  - `Inventory`, `Look`, `Broadcast`
  - `Take <object>`, `Set <object>`
  - `Fork`, `Incantation`, `Connect_nbr`

---

## 🎨 Interface Graphique

La GUI affiche :

- La map 2D (avec SFML recommandé)
- Les ressources sur chaque case
- Les joueurs avec leur équipe, orientation, et niveau
- Les incantations en cours
- Les naissances / œufs

Elle se connecte avec la commande `GRAPHIC` au lieu du nom d’équipe.

---

## 🔀 Reproduction

- Un joueur peut `Fork` → un œuf est pondu
- Lorsqu’un joueur se connecte, il **éclot** aléatoirement à l’un des œufs disponibles
- L’IA doit gérer la reproduction pour augmenter la population de l’équipe

---

## 🥾 Tests et développement

- Le serveur officiel de référence peut être utilisé pour tester l’IA ou la GUI
- La communication suit un **protocole strict**
- Utiliser `strace` pour vérifier que le `poll` fonctionne correctement

---

## 🏆 Condition de victoire

Une équipe gagne dès que **6 de ses joueurs** atteignent le **niveau 8**.

---

## 📜 Licence

Projet pédagogique – EPITECH – Promotion 2025

