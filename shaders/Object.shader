#shader vertex
#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 uv_coords;
layout(location = 2) in vec3 normals;

out vec2 uv;
out vec3 normal;
out vec3 lightdir;
out vec2 pos;
out vec3 eyedir;
  
// Values that stay constant for the whole mesh.
uniform mat4 u_MVP;
uniform mat4 u_M;
uniform mat4 u_V;
  
void main(){
  pos = vPos.xz;
  gl_Position =  u_MVP * vec4(vPos,1);
  uv = uv_coords;

  vec3 LightPosition_worldspace = vec3(-10.0f,40.0f,-10.0f);
  vec3 Position_worldspace = (u_M * vec4(vPos,1)).xyz;

  vec3 vertexPosition_cameraspace = ( u_V * u_M * vec4(vPos,1)).xyz;
  vec3 eyedir = vec3(0,0,0) - vertexPosition_cameraspace;
  vec3 LightPosition_cameraspace = ( u_V * vec4(LightPosition_worldspace,1)).xyz;
  lightdir = LightPosition_cameraspace + eyedir;

  normal = ( u_V * u_M * vec4(normals,0)).xyz; 


}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform float u_Ns;
uniform float u_Ni;
uniform float u_D;
uniform vec3 u_Ka;
uniform vec3 u_Kd;
uniform vec3 u_Ks;
uniform vec3 u_Ke;
uniform int u_Illum;


in vec2 uv;
in vec3 normal;
in vec3 lightdir;
in vec2 pos;
in vec3 eyedir;

void main(){
  vec3 lightColor = vec3(0.8f);
  vec3 n = normalize( normal );
  vec3 l = normalize( lightdir );
  vec3 e = normalize(eyedir);
  vec3 r = reflect(-l,n);

  float cost = clamp( dot( n,l ), 0,1 );
  float cosAlpha = clamp( dot( e,r ), 0,1 );
  color = vec4(u_Ka * u_Kd + vec3(u_D) * u_Kd * lightColor * vec3(u_Ni) * cost + vec3(u_D) * u_Ks * lightColor * vec3(u_Ni) * pow(cosAlpha, 5), 1.0f);
}

/*

color =
   // Ambient : simulates indirect lighting
   MaterialAmbientColor +
   // Diffuse : "color" of the object
   visibility * MaterialDiffuseColor * LightColor * LightPower * cosTheta+
   // Specular : reflective highlight, like a mirror
   visibility * MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5);

 */

/*

"Ka" nous donne la couleur ambiante (la couleur de l'objet sans lumière directe), RVB entre 0 (Min) et 1 (Max)
"Kd" est utilisé pour la couleur diffuse (la couleur de l'objet sous lumière blanche)
"Ks" pour la couleur spéculaire (specular)
"Ke" pour la couleur émissive (emissive)
"Ni" pour la densité optique
"Ns" pour le specular exponent entre 0 et 100 (0)
"d" pour la transparence entre 0 et 1 (aucune transparence)
"illum" pour les paramètres de lumières
"map_kd" (ks, ka) pour la texture utilisé diffuse (specular, ambiante)

*/