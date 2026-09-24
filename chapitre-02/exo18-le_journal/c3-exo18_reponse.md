# Exercice 18

## Enoncer

Faites afficher trois lignes par votre programme au démarrage, puis lisez-les dans le journal du système. Rendez la commande de filtrage que vous avez employée pour ne voir que les vôtres, au milieu de tout le reste.

## Solution

Pour cet exercice, j'ai utilisé le journal système Linux avec `syslog`.
Le programme envoie trois messages au démarrage.

### Code de [main.cpp](main.cpp) et [salle.jenga](salle.jenga)

```cpp
#include <syslog.h>

int main() {
    openlog("MaSalle", LOG_PID, LOG_USER);

    syslog(LOG_INFO, "Demarrage du programme");
    syslog(LOG_INFO, "Programme initialise");
    syslog(LOG_INFO, "Programme pret");

    closelog();

    return 0;
}
```

### Construction du programme

* J'ai d'abord nettoyé l'ancien build :

```bash
jenga clean
```

* Ensuite, j'ai construit le projet :

```bash
jenga build
```

* Le projet a été compilé avec :

```text
Configuration: Debug
Target:        Linux x86_64
Toolchain:     host-gcc
```

### Exécution

J'ai exécuté le programme avec :

```bash
jenga run
```

Le programme s'est terminé normalement sans rine afficher.

### Lecture du journal

Pour afficher uniquement les messages envoyés par mon programme, j'ai utilisé :

```bash
journalctl -t MaSalle
```

Le résultat obtenu est :

```text
Sep 24 20:59:00 SONWA MaSalle[459]: Demarrage du programme
Sep 24 20:59:00 SONWA MaSalle[459]: Programme initialise
Sep 24 20:59:00 SONWA MaSalle[459]: Programme pret
```

## Commande de filtrage

La commande utilisée pour filtrer les messages de mon programme est :

```bash
journalctl -t MaSalle
```

L'option `-t MaSalle` permet de filtrer les messages associés à l'identifiant `MaSalle`.

## Conclusion

Dans cet exercice, j'ai utilisé `syslog` pour envoyer trois messages dans le journal système Linux. Ensuite, avec `journalctl -t MaSalle`, j'ai pu retrouver uniquement les messages de mon programme parmi les autres journaux du système.
