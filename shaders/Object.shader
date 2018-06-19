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
out float col;
out vec4 coloor;
  
// Values that stay constant for the whole mesh.
uniform mat4 u_MVP;
uniform mat4 u_VP;
uniform mat4 u_M;
uniform mat4 u_V;
uniform sampler2D u_DefaultSampler;
uniform ivec2 base;
uniform ivec2 torBase;
  
void main(){
  vec4 p = u_M * vec4(vPos,1);
  pos = (u_M*vec4(vPos,1)).xz;
  gl_Position =  u_VP * p;
  vec4 pos = u_M * vec4(vPos,1.0);
  vec4 posV = u_V * pos;
  float texsize = textureSize(u_DefaultSampler, 0).x;
  ivec2 wPos = ivec2(pos.x,pos.z);
  ivec2 tPos = ivec2(mod((torBase + (wPos - base)),texsize)); 
  uv = (vec2(tPos)/(texsize));
  pos.y =dot( texture(u_DefaultSampler, uv), vec4(1.0, 1/255.0, 1/65025.0, 1/16581375.0) )*64-32;//*8000-4000;
  gl_Position =  u_VP * pos;
  //gl_Position =  u_MVP * vec4(vPos,1);

  uv = uv_coords;

  vec3 LightPosition_worldspace = vec3(-10.0f,40.0f,-10.0f);
  vec3 Position_worldspace = (u_M * vec4(vPos,1)).xyz;

  vec3 vertexPosition_cameraspace = ( u_V * u_M * vec4(vPos,1)).xyz;
  vec3 eyedir = vec3(0,0,0) - vertexPosition_cameraspace;
  vec3 LightPosition_cameraspace = ( u_V * vec4(LightPosition_worldspace,1)).xyz;
  lightdir = LightPosition_cameraspace + eyedir;

  normal = ( u_V * u_M * vec4(normals,0)).xyz; 
  if(fract(p.x) == 0.0f)
    col = 1.0;
  else
    col = 0.0;

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
in float col;

void main(){
  vec3 lightColor = vec3(0.8f);
  vec3 n = normalize( normal );
  vec3 l = normalize( lightdir );
  vec3 e = normalize(eyedir);
  vec3 r = reflect(-l,n);

  float cost = clamp( dot( n,l ), 0,1 );
  float cosAlpha = clamp( dot( e,r ), 0,1 );
//  color = vec4(u_Ka * u_Kd + vec3(u_D) * u_Kd * lightColor * vec3(u_Ni) * cost + vec3(u_D) * u_Ks * lightColor * vec3(u_Ni) * pow(cosAlpha, 5), 1.0f);
  //color = vec4(uv.y,uv.x,0,1.0);
  color = vec4(u_Ka.x,u_Ka.y,u_Ka.z,1.0);
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
