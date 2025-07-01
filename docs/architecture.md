# Architecture du serveur Zappy

## Comment c'est organisé

Le serveur, c'est basiquement 4 gros morceaux qui bossent ensemble :

```
[Clients] → [Réseau] → [Commandes] → [Jeu]
```

### Le truc réseau
- Accepte les connexions (bots et GUI)
- Reçoit/envoie les messages
- Dispatche vers les bons modules

### Le parser de commandes  
- Prend les messages reçus
- Les transforme en actions sur le jeu
- Renvoie les réponses

### Le moteur de jeu
- L'état du monde (carte, joueurs, ressources)
- Fait évoluer tout ça dans le temps
- Applique les règles

### Les données
- Structures qui stockent tout (serveur, jeu, joueurs, etc.)

## Structure des fichiers

J'ai essayé d'organiser ça proprement :

```
server/src/
├── main.c              # Point d'entrée
├── server/             # Gestion du serveur principal
├── network/            # Tout ce qui touche au réseau
├── game/               # Logique du jeu
├── commands/           # Traitement des commandes
├── gui/                # Interface pour les observateurs  
├── parser/             # Parsing des arguments
└── utils/              # Fonctions utiles
```

### Serveur (server/)
- `server.c` : Création et destruction du serveur
- `server_run.c` : La boucle principale avec poll()
- `server_accept.c` : Accepter/déconnecter les clients
- `server_events.c` : Traiter les événements poll

### Réseau (network/) 
- `network.c` : Envoyer/recevoir des messages
- `network_handler.c` : Dispatcher les messages selon le type de client
- `network_client.c` : Gestion des nouveaux clients
- `network_player.c` : Logique spécifique aux bots
- `network_gui.c` : Logique spécifique aux GUI
- `network_commands.c` : File d'attente des commandes

### Jeu (game/)
- `game.c` : Fonctions principales du jeu
- `game_create.c` : Créer le monde et l'initialiser
- `game_teams.c` : Gestion des équipes et œufs
- `game_update.c` : Mise à jour à chaque cycle
- `game_resources.c` : Spawn des ressources
- `player.c` : Création/destruction des joueurs

## Comment ça marche

### Démarrage
1. `main()` parse les arguments
2. Crée le serveur avec `server_create()`
3. Initialise le jeu avec `game_create()`
4. Lance la boucle principale `server_run()`

### Boucle principale
```c
while (1) {
    // Attendre des événements avec poll()
    poll(fds, count, timeout);
    
    // Traiter les nouveaux clients / messages
    handle_poll_events();
    
    // Mettre à jour le jeu si c'est le moment
    if (temps_écoulé) {
        game_update();
    }
}
```

### Quand un client se connecte
1. `accept()` dans `server_accept.c`
2. Nouveau client créé dans la liste
3. Le client dit son type (nom équipe ou "GRAPHIC")
4. Dispatch vers `network_player.c` ou `network_gui.c`

### Quand un bot envoie une commande
1. Message reçu dans `network.c`
2. Parsé dans `network_handler.c`
3. Ajouté à la queue dans `network_commands.c`
4. Exécuté dans `commands/` quand c'est le moment
5. Réponse renvoyée au client

## Structures de données importantes

### Le serveur
```c
typedef struct s_server {
    int socket_fd;              // Socket principal
    struct pollfd *poll_fds;    // Pour poll()
    client_t *clients;          // Liste des clients connectés
    game_t *game;               // État du jeu
} server_t;
```

### Le jeu
```c
typedef struct s_game {
    int width, height;          // Taille de la carte
    tile_t **map;              // La carte elle-même
    team_t *teams;             // Liste des équipes
    int time_unit;             // Fréquence
} game_t;
```

### Un client
```c
typedef struct s_client {
    int fd;                     // File descriptor
    client_type_t type;         // PLAYER ou GRAPHIC
    char read_buffer[BUFFER_SIZE];
    player_t *player;           // Si c'est un bot
    command_t *cmd_queue;       // File des commandes
} client_t;
```

## Trucs à retenir

### Gestion mémoire
- Tout est alloué dynamiquement
- `server_destroy()` nettoie tout en cascade
- Les clients sont dans une liste chaînée
- Attention aux fuites dans la queue de commandes

### Concurrence
- Tout est single-thread
- `poll()` pour gérer plein de clients sans threads
- Les commandes des bots sont mises en queue
- Chaque bot a un timer pour ses actions

### Points sensibles
- La boucle principale peut ramer si trop de clients
- Le parsing des commandes doit être robuste
- Les broadcasts GUI peuvent prendre du temps
- La gestion des déconnexions brutales

### Ajouts possibles
- Pour ajouter une commande : modifier `COMMANDS[]` et créer `cmd_nouvelle()`
- Pour changer le protocole : adapter les structures et les parsers
- Pour optimiser : regarder du côté des broadcasts et du parsing

## Schéma global

```
main()
└── server_create()
    ├── game_create()
    │   ├── create_map()
    │   └── init_teams()
    └── setup socket + poll
└── server_run()
    └── while(1)
        ├── poll()
        ├── handle_events()
        │   ├── accept clients
        │   └── process messages
        └── game_update()
            ├── process command queues
            ├── update players
            └── spawn resources
```

Voilà, c'est pas parfait mais ça donne une idée de comment tout s'articule !