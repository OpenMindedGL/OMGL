## Document post-mortem 
### 1)	Comment on build
Il faut installer toutes les librairies nécessaires pour le programme, glew, glm, glfw, stbi et fastnoise.
Sur visual studio il faut donc aussi configurer ces lib dans les propriétés.
Le code nécessaire pour faire fonctionner l’application est déjà présent dans le main, il suffit donc de l’exécuter.

### 2)	Comment utiliser les fonctionnalités principales (terrain, objet..)

Pour pouvoir dessiner quoi que ce soit, il faut créer une instance de la classe Renderer, qui permet de dessiner les différents modèles : 
-	Renderer renderer(*window, WIDTH, HEIGHT);

Pour générer un terrain aléatoire : 
Il faut choisir un/des biome(s), soit parmi ceux déjà existant ou en faire un soit même, en héritant de la classe BIOME. 
Exemple avec un biome existant : 
-	std ::vercot<Biome*> bs = {new OMGLMountains()};

Il suffit ensuite de passer le passer à un terrain dans le constructeur : 
-	Terrain t(&bs) ;

Pour l’afficher, dans la boucle il faut l’update puis appeler renderer.Draw() : 
-	t.Update() ;
-	renderer.Draw(t) ;

Pour générer des objets : 
On peut générer des objets de différentes façons. 
On peut passer au constructeur un Mesh, ou un chemin vers un .obj, en suite on peut lui passer une couleur 
(ex : OMGL_RED), ou le chemin vers le fichier .mtl ou encore un shader : 
-	Cube cube;
	Object o(&cube, OMGL_BLUE); (Un mesh + une couleur)
-	Object o( "res/objects/dodge/CHALLENGER71.obj",
		"res/objects/dodge/CHALLENGER71.mtl",
		 true, "shaders/DynamicShader",
		 "shaders/DynamicShader/Basic.genshader",
               "res/objects/dodge/");
             (.obj + .mtl)

On peut voir qu’il y a d’autre paramètres à définir, 
comme préciser si les indices y et z doivent être inversés, le fichier où se trouvent les textures… 
De plus, si on ne renseigne aucune couleur, aucun Material ni aucun shader, 
le constructeur va lui-même définir un shader par défaut. 

Pour l’afficher, dans la boucle il suffit d'appeler renderer.Draw() : 
-	renderer.Draw(o) ;

Pour générer les nuages : 
Il faut d’abord créer une instance de la classe Sky : 
-	Vec2 center;
-	Sky s(center);

Puis il suffit d’appeler renderer.Draw() pour le dessine: 
-	Renderer.Draw(s);


### 3)	Ce qu’il reste à faire 
Dans un projet avec un sujet aussi vaste que celui-ci il y aura toujours de nouvelles fonctionnalités à implémenter, 
parmi celles que nous aurions aimer ajouter mais dont nous n’avons pas eu le temps il y a : 
-	un cycle jour/nuit, 
-	l’eau,
-	un système de physique plus élaboré,
-	un système qui gère les animations de personnages, 
-	le shadow mapping (ombrage dynamique).
