# Exercice 15

## Énoncé

Fabriquez une clé de signature et notez son emplacement et son mot de passe ailleurs que dans le dépôt.

Rendez la commande employée, sans le mot de passe, et dites où vous avez rangé celui-ci.

## Solution

### Commande utilisée

J'ai utilisé la commande suivante pour générer la clé de signature Android :

```powershell
jenga keygen --alias monapp-key --output monapp.keystore
```

L'option `--output` permet de choisir le fichier dans lequel la clé est enregistrée.

### Emplacement de la clé

La clé de signature est enregistrée dans le fichier :

```text
monapp.keystore
```

Le fichier se trouve à la **racine de mon projet**.

### Protection de la clé

Pour éviter d'envoyer la clé dans le dépôt Git, j'ai ajouté le fichier dans `.gitignore` :

```gitignore
monapp.keystore
```

Ainsi, le fichier de clé reste uniquement sur ma machine et n'est pas versionné dans le dépôt.

### Mot de passe

Le mot de passe n'est pas indiqué ici. Je l'ai conservé séparément, dans un emplacement personnel et sécurisé.

## Conclusion

La clé de signature Android a été générée avec `jenga keygen`. La commande utilisée et l'emplacement du fichier sont indiqués dans ce rapport. Le fichier `monapp.keystore` est placé à la racine du projet et ajouté au `.gitignore` afin qu'il ne soit pas envoyé dans le dépôt. Le mot de passe est conservé en dehors du dépôt.
