#shader vertex
#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 uv_coords;
layout(location = 2) in vec3 normals;

out vec2 uv;
out vec3 normal;
out vec3 lightdir;
out mat4 MV;
out vec3 pp;
out float biome;
out float height;
out float rd;

out vec3 d1;

  
// Values that stay constant for the whole mesh.
uniform mat4 u_MVP;
uniform mat4 u_VP;
uniform mat4 u_M;
uniform mat4 u_V;
//uniform sampler2D u_HeightMap;
uniform sampler2D u_DefaultSampler;
uniform sampler2D u_NormalMap;
uniform ivec2 base;
uniform ivec2 torBase;
uniform ivec2 torBegin;
uniform int u_UnitSize;
uniform int u_MaxHeight;
uniform int u_MinHeight;

  
void main(){
  vec4 pos = round(u_M * vec4(vPos,1.0));
  vec4 posV = u_V * pos;
  pp = pos.xyz;
  float texsize = textureSize(u_DefaultSampler, 0).x;
  ivec2 wPos = ivec2(pos.x,pos.z);
  ivec2 tPos = ivec2(mod((torBase + (wPos - base))/u_UnitSize,texsize)); 
  uv = (vec2(tPos)/(texsize))+vec2(0.00001,0.00001);
  //pos.y =dot( texture(u_DefaultSampler, uv), vec4(1.0, 1/255.0, 1/65025.0, 1/16581375.0) ) *(u_MaxHeight-u_MinHeight)+u_MinHeight;
  const float max24int = 256.0 * 256.0 * 256.0 - 1.0;
  vec4 tex = texture(u_DefaultSampler, uv);
  d1 = tex.xyz;
  rd = 1-tex.y;
  
  biome = floor(tex.w*255.0) ;
  height = (dot(d1*255.0f, vec3(256.0 * 256.0, 256.0, 1.0)) / max24int );
  pos.y = height*(u_MaxHeight-u_MinHeight)+u_MinHeight;
  pos.y = clamp(pos.y,0,u_MaxHeight);
  gl_Position =  u_VP * pos;


  vec3 Position_worldspace = pos.xyz;

  vec3 vPos_cameraspace = (pos).xyz;

  

}

#shader fragment
#version 330 core

#define MAX_BIOMES_MAT  5;


layout (std140) uniform ub_Biomes
{ 
  //Material u_Mat[1];
  float u_m;
}; 

/*struct AltitudeBiome {
  float minaltitude;
  int nbBiomesMaterials;
  BiomeMaterials[MAX_BIOMES_MAT];
};
*/
struct BiomeMaterial {
  float mincost;
  vec3 Ka;
  vec3 Kd;
  vec3 Ks;
  float Ns;
  sampler2D map_Kd;
};

layout(location = 0) out vec4 color;
//uniform mat4 u_M;
//uniform mat4 u_V;
in vec2 uv;
//in vec3 normal;
in vec3 lightdir;
in vec3 pp;
in vec3 d1;
in float height;
in float rd;
in float biome;
uniform ivec2 base;
uniform ivec2 torBase;
uniform ivec2 torBegin;
uniform int u_UnitSize;
uniform int u_MaxHeight;
uniform int u_MinHeight;
uniform vec3 u_ViewerPos;
//uniform int nb_biomes;
uniform BiomeMaterial u_Mat[1];


//uniform sampler2D u_NormalMap;
//uniform sampler2D u_DefaultSampler;
uniform sampler2D u_HeightMapLinear;
//out vec3 color;

vec4 getnormals(sampler2D s, vec2 pos){
  vec2 size = vec2(u_UnitSize,0.0);
  vec3 off = vec3(-u_UnitSize/2.0f,0,u_UnitSize/2.0f);
  const float max24int = 256.0 * 256.0 * 256.0 - 1.0;

  float texsize = textureSize(s, 0).x;

  vec2 wPos = vec2(pos.x,pos.y);
  vec2 tPos = vec2(mod((torBase + (wPos - base))/u_UnitSize,texsize)); 
  vec2 uv = (vec2(tPos)/(texsize))+vec2(0.00001,0.00001);
  float s11 = (dot(texture(s,uv).xyz*255.0f, vec3(256.0 * 256.0, 256.0, 1.0)) / max24int )*(u_MaxHeight-u_MinHeight)+u_MinHeight;

  wPos = vec2(pos.x,pos.y)+off.xy;
  tPos = vec2(mod((torBase + (wPos - base))/u_UnitSize,texsize)); 
  uv = (vec2(tPos)/(texsize))+vec2(0.00001,0.00001);
  float s01 = (dot(texture(s,uv).xyz*255.0f, vec3(256.0 * 256.0, 256.0, 1.0)) / max24int )*(u_MaxHeight-u_MinHeight)+u_MinHeight;

  wPos = vec2(pos.x,pos.y)+off.zy;
  tPos = vec2(mod((torBase + (wPos - base))/u_UnitSize,texsize)); 
  uv = (vec2(tPos)/(texsize))+vec2(0.00001,0.00001);
  float s21 = (dot(texture(s,uv).xyz*255.0f, vec3(256.0 * 256.0, 256.0, 1.0)) / max24int )*(u_MaxHeight-u_MinHeight)+u_MinHeight;

  wPos = vec2(pos.x,pos.y)+off.yx;
  tPos = vec2(mod((torBase + (wPos - base))/u_UnitSize,texsize)); 
  uv = (vec2(tPos)/(texsize))+vec2(0.00001,0.00001);
  float s10 =(dot(texture(s,uv).xyz*255.0f, vec3(256.0 * 256.0, 256.0, 1.0)) / max24int )*(u_MaxHeight-u_MinHeight)+u_MinHeight;

  wPos = vec2(pos.x,pos.y)+off.yz;
  tPos = vec2(mod((torBase + (wPos - base))/u_UnitSize,texsize)); 
  uv = (vec2(tPos)/(texsize))+vec2(0.00001,0.00001);
  float s12 =(dot(texture(s,uv).xyz*255.0f, vec3(256.0 * 256.0, 256.0, 1.0)) / max24int )*(u_MaxHeight-u_MinHeight)+u_MinHeight;

  vec3 va = normalize(vec3(size.xy,s21-s01));
  vec3 vb = normalize(vec3(size.yx,s12-s10));
  return vec4( cross(va,vb), s11 ); 
}
int pos_sign(float a){
  return (int(sign(a))+1)/2;
}

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main(){
  vec3 objColor;
  vec3 n = normalize(getnormals(u_HeightMapLinear, pp.xz).xzy);
  float rr[3];
  rr[0] = 0.3f;
  rr[1] = 0.45f;
  rr[2] = 0.65f;
  float r[3];
  r[0] = 0.3f;
  r[1] = 0.5f;
  r[2] = 0.75f;
  vec3 m[3];
  //float rd = rand(pp.xz)*2-1;
  if(biome == 0){
    float hh = height;
    float dd = dot(n,normalize(vec3(n.x,0.0f,n.z)));
    hh=height+(height*rd)/50;
    int g = clamp(pos_sign(hh-r[0])+pos_sign(hh-r[1])+pos_sign(hh-r[2])-1,0,2);
    int gg = clamp(pos_sign(dd-rr[0])+pos_sign(dd-rr[1])+pos_sign(dd-rr[2])-1,0,2);
    float h = float(g)/2.0f;
    m[0] = vec3(0.10f,0.1f,0.25f);
    m[1] = vec3(0.10f,0.30f*h,0.10f);
    m[2] = vec3(0.40f,0.40f,0.40f);
    vec3 mm[3];
    mm[0] = vec3(0.10f,0.3f,0.15f);
    mm[1] =  mm[0];vec3(0.3f,0.1f,0.3f);
    mm[2] =vec3(0.30f,0.30f,0.30f);
    if(g == 1){
      objColor = mm[gg];
    }
    else{
      objColor = m[g];
    }
    if(g == 0){
      n = vec3(1);
    }
  }
  else{
    objColor = vec3(0.30f,0.30f,0.10f);
  }
  //vec3 objColor = vec3(0.10f,0.30f*biome*255.0f,0.10f);
  //vec3 objColor = d1;
//int b = int(biome*255);
int b = 0;

//vec3 n =vec3(0);

  vec3 l = normalize(vec3(4,10.0f,.0)) ;

  // ambient
  vec3 ambient = u_Mat[b].Ka;

  // diffuse 
  float diff = max(dot(n, l), 0.0);
  vec3 diffuse = (diff * u_Mat[b].Kd);

  // specular
  vec3 viewDir = normalize(u_ViewerPos - pp);
  vec3 reflectDir = reflect(-l, n);  
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Mat[b].Ns);
  vec3 specular = (spec * u_Mat[b].Ks);  

  vec3 result = ( ambient + diffuse + specular ) * objColor;

  /*vec3 n = normalize( normal );
  float cost = clamp( dot( n,l ), 0,1 );
  float ambient = 0.5f;*/
  
  //color = vec4(Ka*Kd,1);
  //vec3 col = (Ka * Kd + Kd * cost) * green;
  color = vec4(result ,1.0f);
}
