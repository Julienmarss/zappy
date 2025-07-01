# API du serveur Zappy

## Se connecter

### Pour un bot
1. Connexion TCP sur le port du serveur
2. Le serveur dit `WELCOME`
3. Tu réponds avec ton nom d'équipe, ex: `team1`
4. Le serveur te donne le nombre de places dispo et la taille de la carte

### Pour observer (GUI)
1. Pareil, connexion TCP
2. Le serveur dit `WELCOME`  
3. Tu réponds `GRAPHIC`
4. Le serveur t'envoie plein d'infos sur l'état actuel

## Commandes pour les bots

Toutes les commandes se terminent par `\n`. Les réponses aussi.

### Bouger

**`Forward`** - Avancer d'une case
- Temps: 7/f (f = fréquence du serveur)
- Réponse: `ok` ou `ko`

**`Right`** - Tourner à droite
- Temps: 7/f  
- Réponse: `ok`

**`Left`** - Tourner à gauche
- Temps: 7/f
- Réponse: `ok`

### Regarder autour

**`Look`** - Voir ce qu'il y a autour
- Temps: 7/f
- Réponse: `[case1,case2,case3,...]`
- Dans chaque case tu peux avoir: `player`, `food`, `linemate`, `deraumere`, `sibur`, `mendiane`, `phiras`, `thystame`
- La vision dépend de ton niveau (niveau 1 = 3 cases, niveau 2 = 8 cases, etc.)

Exemple:
```
> Look
< [player food,linemate,food player,sibur]
```

**`Inventory`** - Voir ton inventaire
- Temps: 1/f
- Réponse: `[food n, linemate n, deraumere n, ...]`

Exemple:
```
> Inventory  
< [food 10, linemate 2, deraumere 0, sibur 1, mendiane 0, phiras 0, thystame 0]
```

### Ramasser/poser des trucs

**`Take [objet]`** - Ramasser un objet sur ta case
- Objets possibles: `food`, `linemate`, `deraumere`, `sibur`, `mendiane`, `phiras`, `thystame`
- Temps: 7/f
- Réponse: `ok` si l'objet est là, `ko` sinon

**`Set [objet]`** - Poser un objet de ton inventaire
- Temps: 7/f  
- Réponse: `ok` si tu as l'objet, `ko` sinon

### Autres actions

**`Eject`** - Virer tous les autres joueurs de ta case
- Temps: 7/f
- Réponse: `ok`
- Les joueurs virés reçoivent `eject: [direction]` et bougent d'une case

**`Broadcast [message]`** - Envoyer un message à tout le monde
- Temps: 7/f
- Réponse: `ok`
- Les autres reçoivent: `message [direction], [message]`
- Direction de 0 à 8 (0 = c'est toi, 1-8 = d'où ça vient)

**`Fork`** - Pondre un œuf pour ton équipe
- Temps: 42/f
- Réponse: `ok`

**`Connect_nbr`** - Combien de places libres dans ton équipe
- Temps: 0/f (instantané)
- Réponse: un nombre

### Évoluer

**`Incantation`** - Passer au niveau suivant
- Temps: 300/f (long!)
- Conditions compliquées selon le niveau (nombre de joueurs + ressources)
- Réponse: `Elevation underway\nCurrent level: [nouveau_niveau]` ou `ko`

Les conditions pour évoluer:
- Niveau 1→2: 1 joueur niveau 1, 1 linemate
- Niveau 2→3: 2 joueurs niveau 2, 1 linemate + 1 deraumere + 1 sibur
- Niveau 3→4: 2 joueurs niveau 3, 2 linemate + 1 sibur + 2 phiras
- etc. (ça devient compliqué après)

## Commandes pour les GUI

### Infos sur la carte

**`msz`** - Taille de la carte
- Réponse: `msz [largeur] [hauteur]`

**`bct [x] [y]`** - Contenu d'une case
- Réponse: `bct [x] [y] [food] [linemate] [deraumere] [sibur] [mendiane] [phiras] [thystame]`

**`mct`** - Contenu de toute la carte
- Réponse: un `bct` pour chaque case

**`tna`** - Liste des équipes
- Réponse: un `tna [nom]` pour chaque équipe

### Infos sur les joueurs

**`ppo #[id]`** - Position d'un joueur
- Réponse: `ppo #[id] [x] [y] [orientation]` ou `sbp` si le joueur existe pas
- Orientation: 1=Nord, 2=Est, 3=Sud, 4=Ouest

**`plv #[id]`** - Niveau d'un joueur  
- Réponse: `plv #[id] [niveau]` ou `sbp`

**`pin #[id]`** - Position et inventaire d'un joueur
- Réponse: `pin #[id] [x] [y] [food] [linemate] ...` ou `sbp`

### Contrôler la vitesse

**`sgt`** - Vitesse actuelle
- Réponse: `sgt [fréquence]`

**`sst [fréquence]`** - Changer la vitesse
- Réponse: `sst [nouvelle_fréquence]` ou `sbp` si invalide
- Fréquence entre 2 et 10000

## Messages automatiques pour les GUI

Si tu es connecté en GUI, le serveur t'envoie automatiquement des trucs quand ça bouge:

### Joueurs
- `pnw #[id] [x] [y] [orientation] [level] [team]` - Nouveau joueur
- `ppo #[id] [x] [y] [orientation]` - Joueur qui bouge
- `plv #[id] [level]` - Joueur qui change de niveau  
- `pin #[id] [x] [y] [food] [linemate] ...` - Inventaire qui change
- `pdi #[id]` - Joueur qui meurt

### Actions
- `pex #[id]` - Joueur qui en éjecte d'autres
- `pbc #[id] [message]` - Joueur qui broadcast
- `pfk #[id]` - Joueur qui pond un œuf
- `pdr #[id] [resource]` - Joueur qui pose un objet
- `pgt #[id] [resource]` - Joueur qui ramasse un objet

### Œufs
- `enw #[egg_id] #[player_id] [x] [y]` - Œuf pondu
- `ebo #[egg_id]` - Œuf qui éclot
- `edi #[egg_id]` - Œuf qui meurt

### Incantations
- `pic [x] [y] [level] #[id1] #[id2] ...` - Début d'incantation
- `pie [x] [y] [result]` - Fin d'incantation (0=échec, 1=succès)

### Fin de partie
- `seg [team]` - Une équipe a gagné
- `smg [message]` - Message du serveur

## Codes d'erreur

### Pour les bots
- `ok` - Ça a marché
- `ko` - Ça a pas marché (objet pas là, conditions pas remplies, etc.)
- `dead` - Tu es mort

### Pour les GUI  
- `suc` - Commande inconnue ("Syntax Unknown Command")
- `sbp` - Mauvais paramètres ("Syntax Bad Parameters")

## Exemples concrets

### Session bot simple
```
# Connexion
> team1
< 3
< 10 10

# Regarder et bouger
> Look
< [food,player,linemate]

> Take food
< ok

> Forward  
< ok

> Inventory
< [food 11, linemate 0, deraumere 0, sibur 0, mendiane 0, phiras 0, thystame 0]
```

### Session GUI simple
```
# Connexion
> GRAPHIC
< msz 10 10
< tna team1
< tna team2
< bct 0 0 1 0 0 0 0 0 0
...

# Requêtes
> ppo #1
< ppo #1 5 5 1

> sst 200
< sst 200
```

## Notes importantes

### Timing
- Toutes les actions des bots prennent du temps basé sur la fréquence
- Temps réel = (temps de la commande) / (fréquence du serveur)
- Ex: `Forward` à 100 Hz = 7/100 = 0.07 seconde

### Limitations
- Max 10 commandes en queue par bot
- Max 1024 clients total
- Pas de timeout côté serveur

### Trucs à savoir
- Le monde est toroïdal (les bords se rejoignent)
- Les bots mangent automatiquement de la food pour survivre
- Si plus de food, ils meurent
- Pour gagner: 6 joueurs niveau 8 dans la même équipe

C'est pas exhaustif mais ça couvre l'essentiel !