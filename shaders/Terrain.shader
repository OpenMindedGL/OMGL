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
out float biom;
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
  
  biom = floor(tex.w*255.0) ;
  height = (dot(d1*255.0f, vec3(256.0 * 256.0, 256.0, 1.0)) / max24int );
  height = clamp(height,0.5,1);
  pos.y = height*(u_MaxHeight-u_MinHeight)+u_MinHeight;
  //pos.y = clamp(pos.y,0,u_MaxHeight);
  gl_Position =  u_VP * pos;


  vec3 Position_worldspace = pos.xyz;

  vec3 vPos_cameraspace = (pos).xyz;

  

}

#shader fragment
#version 330 core

#define MAX_BIOMES_MAT  3
#define MAX_ALT_BIOMES  5
#define MAX_BIOMES  20

struct BiomeMaterial {
  float mincost;
  int tex;
  int texspread;
  float Ns;
  vec3 Ka;
  vec3 Kd;
  vec3 Ks;
};
struct AltitudeBiome {
  float minaltitude;
  int nbBiomesMat;
  BiomeMaterial biomemats[MAX_BIOMES_MAT];
};
struct Biome {
  int minheight;
  int maxheight;
  int nbAltBiomes;
  AltitudeBiome altbiomes[MAX_ALT_BIOMES];
};

layout(location = 0) out vec4 color;

layout (std140) uniform ub_Biomes
{ 
  Biome biomes[MAX_BIOMES];
}; 




//uniform mat4 u_M;
//uniform mat4 u_V;
in vec2 uv;
//in vec3 normal;
in vec3 lightdir;
in vec3 pp;
in vec3 d1;
in float height;
in float rd;
in float biom;
uniform int u_UnitSize;
uniform ivec2 base;
uniform ivec2 torBase;
uniform ivec2 torBegin;
uniform int u_MaxHeight;
uniform int u_MinHeight;
uniform vec3 u_ViewerPos;
//uniform int nb_biomes;


//uniform sampler2D u_NormalMap;
//uniform sampler2D u_DefaultSampler;
uniform sampler2D u_HeightMapLinear;
//out vec3 color;
int biome = int(biom);

//----------------------------------------------------//
// Simplex 2D noise
// https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83
//

vec3 permute(vec3 x) { return mod(((x*34.0)+1.0)*x, 289.0); }

float snoise(vec2 v){
  const vec4 C = vec4(0.211324865405187, 0.366025403784439,
           -0.577350269189626, 0.024390243902439);
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);
  vec2 i1;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;
  i = mod(i, 289.0);
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
  + i.x + vec3(0.0, i1.x, 1.0 ));
  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy),
    dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;
  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}
//
//----------------------------------------------------//

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

int getAltBiome(float height){
  int nb = biomes[biome].nbAltBiomes;
  int res = 0;
  float limit;
  for(int i = 0;i<nb;i++){
    limit = biomes[biome].altbiomes[i].minaltitude;
    res += pos_sign(height-limit);
  }
  return clamp(res-1,0,nb);
}

int getBiomeMat(const int altbiome, float cost){
  int nb = biomes[biome].altbiomes[altbiome].nbBiomesMat;
  int res = 0;
  float limit;
  for(int i = 0;i<nb;i++){
    limit = biomes[biome].altbiomes[altbiome].biomemats[i].mincost;
    res += pos_sign(cost-limit);
  }
  return clamp(res-1,0,nb);
}

void main(){
  vec3 objColor = vec3(1);
  int iswater = pos_sign(height-0.5f);
  int isnotwater = int(mod(iswater+1,2));
  vec3 n = normalize(getnormals(u_HeightMapLinear, pp.xz).xzy)*iswater+isnotwater*vec3(0,1,0);
  vec3 sunpos = vec3(4,10.0f,.0);
  float noiseinfl = 0.01f;
  float noise = snoise(pp.xz*0.05f) * noiseinfl +1 - noiseinfl/2.0f;
  float cost = dot(n,normalize(vec3(n.x,0.0f,n.z)));
  int altbiome = getAltBiome(height*noise);
  int biomemat = getBiomeMat(altbiome,cost*noise);

  float Ns = biomes[biome].altbiomes[altbiome].biomemats[biomemat].Ns;
  vec3 Ka = biomes[biome].altbiomes[altbiome].biomemats[biomemat].Ka;
  vec3 Kd = biomes[biome].altbiomes[altbiome].biomemats[biomemat].Kd;
  vec3 Ks = biomes[biome].altbiomes[altbiome].biomemats[biomemat].Ks;

  vec3 l = normalize(sunpos) ;

  // ambient
  vec3 ambient = Ka;

  // diffuse 
  float diff = max(dot(n, l), 0.0);
  vec3 diffuse = (diff * Kd);

  // specular
  vec3 viewDir = normalize(u_ViewerPos - pp);
  vec3 reflectDir = reflect(-l, n);  
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), Ns);
  vec3 specular = (spec * Ks);  

  vec3 result = ( ambient + diffuse + specular ) * objColor;

  color = vec4(result ,1.0f);
}
  /*float rr[3];
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

*/
