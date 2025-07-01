# Guide d'utilisation - Serveur Zappy

## Comment lancer le serveur

### Commande de base
```bash
./zappy_server -p 4242 -x 10 -y 10 -n team1 team2 -c 5 -f 100
```

### Paramètres obligatoires
- **-n [équipes]** : Noms des équipes (au moins une)
  ```bash
  -n team1 team2 team3
  ```

### Paramètres optionnels
- **-p [port]** : Port d'écoute (défaut: 4242)
- **-x [largeur]** : Largeur du monde (défaut: 10, min: 10, max: 30)
- **-y [hauteur]** : Hauteur du monde (défaut: 10, min: 10, max: 30)  
- **-c [clients]** : Nombre max de bots par équipe (défaut: 3)
- **-f [fréquence]** : Vitesse de simulation (défaut: 100, min: 2, max: 10000)

### Exemples de lancement

**Partie rapide à 2 équipes :**
```bash
./zappy_server -n alpha beta
```

**Partie personnalisée :**
```bash
./zappy_server -p 8080 -x 20 -y 15 -n red blue green -c 10 -f 200
```

**Partie lente pour observation :**
```bash
./zappy_server -x 15 -y 15 -n team1 team2 -c 3 -f 10
```

## Comment connecter des clients

### Connecter un bot/IA
1. Connexion TCP sur le port du serveur
2. Le serveur envoie `WELCOME`
3. Tu réponds avec le nom de l'équipe : `team1`
4. Le serveur répond le nombre de places disponibles et la taille du monde
5. Ton bot peut maintenant envoyer des commandes

### Connecter un observateur graphique
1. Connexion TCP sur le port du serveur  
2. Le serveur envoie `WELCOME`
3. Tu réponds : `GRAPHIC`
4. Tu peux maintenant envoyer des commandes de visualisation

## Commandes pour les bots

### Déplacement
- `Forward` : Avancer d'une case
- `Right` : Tourner à droite
- `Left` : Tourner à gauche

### Information
- `Look` : Voir ce qui est autour (vision en cône selon le niveau)
- `Inventory` : Voir son inventaire
- `Connect_nbr` : Nombre de places restantes dans l'équipe

### Actions
- `Take [objet]` : Ramasser un objet (`food`, `linemate`, etc.)
- `Set [objet]` : Poser un objet
- `Eject` : Pousser les autres joueurs de la case

### Communication et évolution
- `Broadcast [message]` : Envoyer un message à tous
- `Fork` : Pondre un œuf
- `Incantation` : Évoluer au niveau suivant

**Réponses du serveur :**
- `ok` : Action réussie
- `ko` : Action échouée  
- `[données]` : Informations demandées

## Commandes pour l'observateur

### Informations globales
- `msz` : Taille de la carte
- `mct` : Contenu de toute la carte
- `tna` : Liste des équipes

### Informations sur une case
- `bct X Y` : Contenu de la case (X, Y)

### Informations sur un joueur
- `ppo #N` : Position du joueur N
- `plv #N` : Niveau du joueur N
- `pin #N` : Inventaire du joueur N

### Contrôle de la simulation
- `sgt` : Vitesse actuelle
- `sst [vitesse]` : Changer la vitesse (2-10000)

## Réglages et optimisation

### Pour une partie équilibrée
- **Ratio joueurs/taille** : 5-10 joueurs sur une carte 15x15
- **Fréquence** : 100 pour un rythme normal
- **Équipes** : 2-4 équipes pour une bonne compétition

### Pour débugger ton IA
- **Carte petite** : 10x10 pour voir rapidement
- **Peu de joueurs** : 2-3 par équipe max
- **Lent** : fréquence 10-20 pour suivre les actions

### Pour une démo spectaculaire  
- **Grande carte** : 25x25 ou 30x30
- **Beaucoup de joueurs** : 15-20 par équipe
- **Rythme soutenu** : fréquence 200-500

## Messages d'erreur courants

### Au lancement
- **"Invalid world dimensions"** : Taille doit être entre 10 et 30
- **"Invalid frequency"** : Fréquence doit être entre 2 et 10000
- **"No teams specified"** : Il faut au moins une équipe avec -n

### En cours de partie
- **"ko" sur connexion** : Équipe pleine ou nom d'équipe invalide
- **"ko" sur action** : Action impossible (pas de ressource, mauvais niveau, etc.)

## Arrêter le serveur

- **Ctrl+C** : Arrêt propre du serveur
- Les clients connectés sont déconnectés automatiquement
- Aucune sauvegarde automatique (redémarrage = nouvelle partie)

## Fichiers et dossiers

### Structure après compilation
```
zappy_server          # Exécutable du serveur
├── server/          # Code source du serveur
├── gui/             # Client graphique (si implémenté)
└── ai/              # Exemples d'IA (si fournis)
```

### Logs
Le serveur affiche sur la console :
- Connexions/déconnexions des clients
- Actions importantes (évolutions, morts, victoire)
- Erreurs de fonctionnement

## Aide rapide

```bash
# Voir l'aide
./zappy_server -h

# Lancement minimal
./zappy_server -n team1 team2

# Test rapide avec observateur
./zappy_server -n test &
telnet localhost 4242
> GRAPHIC
> msz
```