## Document post-mortem 
### 1)	Comment on build
Il faut installer toutes les librairies n�cessaires pour le programme, glew, glm, glfw, stbi et fastnoise.
Sur visual studio il faut donc aussi configurer ces lib dans les propri�t�s.
Le code n�cessaire pour faire fonctionner l�application est d�j� pr�sent dans le main, il suffit donc de l�ex�cuter.

### 2)	Comment utiliser les fonctionnalit�s principales (terrain, objet..)

Pour pouvoir dessiner quoi que ce soit, il faut cr�er une instance de la classe Renderer, qui permet de dessiner les diff�rents mod�les : 
-	Renderer renderer(*window, WIDTH, HEIGHT);

Pour g�n�rer un terrain al�atoire : 
Il faut choisir un/des biome(s), soit parmi ceux d�j� existant ou en faire un soit m�me, en h�ritant de la classe BIOME. 
Exemple avec un biome existant : 
-	std ::vercot<Biome*> bs = {new OMGLMountains()};

Il suffit ensuite de passer le passer � un terrain dans le constructeur : 
-	Terrain t(&bs) ;

Pour l�afficher, dans la boucle il faut l�update puis appeler renderer.Draw() : 
-	t.Update() ;
-	renderer.Draw(t) ;

Pour g�n�rer des objets : 
On peut g�n�rer des objets de diff�rentes fa�ons. 
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

On peut voir qu�il y a d�autre param�tres � d�finir, 
comme pr�ciser si les indices y et z doivent �tre invers�s, le fichier o� se trouvent les textures� 
De plus, si on ne renseigne aucune couleur, aucun Material ni aucun shader, 
le constructeur va lui-m�me d�finir un shader par d�faut. 

Pour l�afficher, dans la boucle il suffit d'appeler renderer.Draw() : 
-	renderer.Draw(o) ;

Pour g�n�rer les nuages : 
Il faut d�abord cr�er une instance de la classe Sky : 
-	Vec2 center;
-	Sky s(center);

Puis il suffit d�appeler renderer.Draw() pour le dessine: 
-	Renderer.Draw(s);


### 3)	Ce qu�il reste � faire 
Dans un projet avec un sujet aussi vaste que celui-ci il y aura toujours de nouvelles fonctionnalit�s � impl�menter, 
parmi celles que nous aurions aimer ajouter mais dont nous n�avons pas eu le temps il y a : 
-	un cycle jour/nuit, 
-	l�eau,
-	un syst�me de physique plus �labor�,
-	un syst�me qui g�re les animations de personnages, 
-	le shadow mapping (ombrage dynamique).
