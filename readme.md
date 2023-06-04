# README du projet Chat Client/Serveur en C 📝

Ce projet est un système de chat client/serveur simple, léger et convivial, permettant aux utilisateurs de communiquer entre eux à travers une interface en ligne de commande.

## Table des Matières 📚

- [Fonctionnalités](#fonctionnalités)
- [Prérequis](#prérequis)
- [Configuration du serveur et du client](#configuration-du-serveur-et-du-client)
- [Compilation et Lancement](#compilation-et-lancement)
- [Commandes Disponibles](#commandes-disponibles)
- [Auteurs](#auteurs)
- [Licence](#licence)

## Fonctionnalités 💡

- Connexion de plusieurs clients simultanément.
- Envoi de messages publics et privés.
- Gestion des channels.
- Mention des utilisateurs via `@pseudo`.
- Envoi et réception de fichiers.
- Commandes pour gérer les clients et obtenir de l'aide.
- Affichage d'un manuel d'utilisation intégré.

## Prérequis 📋

- Un environnement Unix (Linux, MacOS).
- Le compilateur GCC.
- `make` installé sur votre système.

## Configuration du serveur et du client 🛠️

Le fichier `common.h` est utilisé pour configurer le serveur et le client. Il définit les constantes utilisées dans le programme, comme l'adresse IP du serveur, le nombre maximum de clients, la taille du buffer, le port d'écoute du serveur et les dossiers pour le transfert de fichiers.

Voici un aperçu de `common.h` :

```c
#ifndef COMMON_H
#define COMMON_H

#define MAX_CLIENTS 10
#define MAX_CHANNELS 10
#define MAX_CLIENT_CHANNEL 10
#define BUFFER_SIZE 2048
#define MAX_PSEUDO_LENGTH 20
#define PORT 8080          
#define SERVER_PORT_FILE 8081 
#define BACKLOG 5 
#define SERVER_IP "127.0.0.1"  
#define MAX_MESSAGE_LENGTH 1000 
#define FILE_BUFFER_SIZE 1024
#define PATCH_CLIENT_FILE "client_files"
#define PATCH_SERVEUR_FILE "serveur_files"
#define CHANNEL_FILE "channel.txt"
#define MAX_FILES_LIST 100
#define MAX_FILES_NAME 256
#define MAX_FILES_SEND 10

#define min(a,b) ((a) < (b) ? (a) : (b)) // Macro pour retourner le minimum entre deux valeurs

#endif // COMMON_H
```
## Compilation et Lancement 🚀

1. Clonez le dépôt GitHub dans un répertoire local de votre choix.
2. Ouvrez un terminal et naviguez vers le répertoire du projet.
3. Compilez le projet en exécutant la commande `make`. Cela générera deux exécutables : `client` et `serveur`.
4. Lancez le serveur en exécutant `./serveur` dans le terminal.
5. Lancez le client dans un autre terminal en exécutant `./client`.
6. Entrez votre pseudo pour rejoindre le chat.
7. Utilisez les commandes disponibles pour interagir avec les autres clients.

## Commandes Disponibles 🎮

- `/list` : Affiche la liste des clients connectés au serveur.
- `/quit` : Déconnecte le client du serveur et ferme l'application.
- `/help` : Affiche le manuel d'utilisation.
- `/mp <pseudo> <message>` : Envoie un message privé au client spécifié par `<pseudo>`.
- `@<pseudo>` : Mentionne un client spécifié par `<pseudo>` dans un message public.
- `/sendFiles` : Envoie un fichier au serveur.
- `/getFiles` : Récupère un fichier du serveur.
- `/set <nom channel>` : Permet de changer la conversation par défaut.
- `/create <nom channel> <description>` : Crée un nouveau channel.
- `/join <nom channel>` : Rejoint un channel existant.
- `/leave <nom channel>` : Quitte un channel.
- `/delete <nom channel>` : Supprime un channel.
- `/getChannels` : Obtient la liste des channels disponibles avec le nombre de clients connectés.

## Auteurs 👥

- Vincent Dubuc
- Lucas Leroy
- Alexandre Lagorce

## Licence 📄

Ce projet a été réalisé dans le cadre d'un projet universitaire pour polytech Montpellier.
Ce projet est sous licence open-source .
