"God clicks" de Thibaut Fayard
Temps de travail estimé: 20h

God Clicks est votre bac à sable personnalisé.
Sélectionnez un élément du menu pour le placer dans le monde avec le clic gauche.
Créez des cascades d'eau, des tuyaux en roche, des échangeurs thermiques dans ce monde à possibilité infinie!
Appuyez sur R pour remettre à zéro la simulation, clic gauche pour poser un bloc, et les menus en bas à gauche pour changer de bloc actif.

Le programme implémente une simulation PIC, "Particle in Cell", où chaque particule individuelle est son propre objet mais est aussi classée dans un tableau 2D aux dimensions de l'écran. Cela permet à chaque particule de savoir où elle est (en stockant ses coordonnées dans sa structure) et d'avoir un accès facile au type et à la chaleur de ses voisins pour décider de son comportement à la prochaine itération.

La gestion du menu s'effectue avec un switch sur la valeur sélectionnée, on appelle ensuite les fonctions de création ou de mise à jour de particule selon des paramètres propres à chaque type.

Les "équations de fluide" sont relativement simplistes: en deux mots, s'il y a de la place en-dessous, l'eau se déplace. Sinon, elle a une chance de se déplacer sur les côtés. Il est plus intéressant de se pencher sur la conduction: on fait la moyenne de la chaleur des voisins adjacents et diagonaux de la particule (elle-même y compris), puis on interpole linéairement entre sa chaleur et le résultat de ce calcul en fonction d'un facteur d'interpolation, qui est inversement proportionnel à un facteur de conductivité thermique.

La chaleur étant stockée en degrés celsius, l'eau gèle quand sa température passe à 0 ou moins, s'évapore à 100°C ou plus, etc.

L'affichage est très simple, on peint un pixel par particule à l'aide des valeurs r, g, b stokées dans la structure.

Dans la boucle du main(), on calcule l'écart entre le temps que prend une image à rendre et le nombre d'images par seconde souhaitées, ce qui permet de fixer les FPS (frames per second) et de limiter l'utilisation du processeur / de la carte graphique.
