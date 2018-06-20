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
out vec2 pp;

out float d1;

  
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

vec3 FindNormal(sampler2D tex, vec2 uv, float u)
{
  //u is one uint size, ie 1.0/texture size
  vec2 offsets[4];
  offsets[0] = uv + vec2(-u, 0);
  offsets[1] = uv + vec2(u, 0);
  offsets[2] = uv + vec2(0, -u);
  offsets[3] = uv + vec2(0, u);

  float hts[4];
  for(int i = 0; i < 4; i++)
  {
    hts[i] = dot( texture(u_DefaultSampler, offsets[i]), vec4(1.0, 1/255.0, 1/65025.0, 1/16581375.0) )*8000-4000;
  }

  vec2 _step = vec2(1.0, 0.0);

  vec3 va = normalize( vec3(_step.xy, hts[1]-hts[0]) );
  vec3 vb = normalize( vec3(_step.yx, hts[3]-hts[2]) );

  return cross(va,vb).rbg; //you may not need to swizzle the normal

}
  
void main(){
  //vec4 vPosh = vPos;
  vec4 pos = round(u_M * vec4(vPos,1.0));
  vec4 posV = u_V * pos;
  float texsize = textureSize(u_DefaultSampler, 0).x;
  ivec2 wPos = ivec2(pos.x,pos.z);
  ivec2 tPos = ivec2(mod((torBase + (wPos - base))/u_UnitSize,texsize)); 
  uv = (vec2(tPos)/(texsize))+vec2(0.001,0.001);
  //uv = (pos.xz+1024)/(2048);
  pos.y =dot( texture(u_DefaultSampler, uv), vec4(1.0, 1/255.0, 1/65025.0, 1/16581375.0) ) *64-32;//*8000-4000;
//*64-32;//*8000-4000;
  //vec2 a = texture(u_DefaultSampler, uv).xy;
  //pos.y = a.x*500;// * 256 + a.y;
 // gl_Position =  u_MVP * vPos;
  gl_Position =  u_VP * pos;
  vec3 LightPosition_worldspace = vec3(100.0f,500.0f,.0f);


  vec3 Position_worldspace = pos.xyz;

  vec3 vPos_cameraspace = (pos).xyz;
  //vec3 EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

  vec3 lightpos_cameraspace = ( vec4(LightPosition_worldspace,1)).xyz;
  lightdir = lightpos_cameraspace - vPos_cameraspace.xyz;
  
 // normal = FindNormal(u_DefaultSampler,uv, 1.0f/8192/2);

  //uv = vPos.xz;
/*
  // Position of the vertex, in worldspace : M * position
  Position_worldspace = (u_M * vPos).xyz;
  
  // Vector that goes from the vertex to the camera, in camera space.
  // In camera space, the camera is at the origin (0,0,0).
  vec3 vertexPosition_cameraspace = ( u_V * u_M * vPos).xyz;
  EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

  // Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
  vec3 LightPosition_cameraspace = ( u_V * vec4(LightPosition_worldspace,1)).xyz;
  LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
  
  // UV of the vertex. No special space for this one.
  UV = vertexUV;
  
  // model to camera = ModelView
  vec3 vertexTangent_cameraspace = MV3x3 * vertexTangent_modelspace;
  vec3 vertexBitangent_cameraspace = MV3x3 * vertexBitangent_modelspace;
  vec3 vertexNormal_cameraspace = MV3x3 * vertexNormal_modelspace;
  
  mat3 TBN = transpose(mat3(
    vertexTangent_cameraspace,
    vertexBitangent_cameraspace,
    vertexNormal_cameraspace    
  )); // You can use dot products instead of building this matrix and transposing it. See References for details.

  LightDirection_tangentspace = TBN * LightDirection_cameraspace;
EyeDirection_tangentspace = TBN * EyeDirection_cameraspace; ) )
*/
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
//uniform mat4 u_M;
//uniform mat4 u_V;
in vec2 uv;
//in vec3 normal;
in vec3 lightdir;
in vec2 pos;
in float d1;

//uniform sampler2D u_NormalMap;
uniform sampler2D u_DefaultSampler;
//out vec3 color;

vec4 getnormals(sampler2D s, vec2 uv){
  const vec2 size = vec2(2,0.0);
  const ivec3 off = ivec3(-1,0,1);

  
  float s11 = dot( texture(s, uv), vec4(1.0, 1/255.0, 1/65025.0, 1/16581375.0) )*8000-4000;
  float s01 = dot( textureOffset(s, uv, off.xy), vec4(1.0, 1/255.0, 1/65025.0, 1/16581375.0) )*8000-4000;
  float s21 = dot( textureOffset(s, uv, off.zy), vec4(1.0, 1/255.0, 1/65025.0, 1/16581375.0) )*8000-4000;
  float s10 = dot( textureOffset(s, uv, off.yx), vec4(1.0, 1/255.0, 1/65025.0, 1/16581375.0) )*8000-4000;
  float s12 = dot( textureOffset(s, uv, off.yz), vec4(1.0, 1/255.0, 1/65025.0, 1/16581375.0) )*8000-4000;
  vec3 va = normalize(vec3(size.xy,s21-s01));
  vec3 vb = normalize(vec3(size.yx,s12-s10));
  return vec4( cross(va,vb), s11 ); 
}

void main(){
  /*  if(0.01f  > mod(pos.y,31.0f) || 0.01f  > mod(pos.x,31.0f)){
      color = vec4(0.0f,1.0f,0.0f,1.0f);
      }
      else{
      color = vec4(0.0f,0.0f,1.0f,1.0f);
   */
  //color = vec4(0.0f,mod(pos.y,2.0f),mod(pos.x,2.0f),1.0f);
  vec3 blue = vec3(0.1f,0.3f,0.1f);
  //vec3 norm = texture(u_NormalMap, uv).rgb;

  //vec3 normal = ( vec4(norm,0)).xzy; 
  vec3 normal = getnormals(u_DefaultSampler, uv).xzy;


  vec3 n = normalize( normal );
  vec3 l = normalize( lightdir );
  float cost = clamp( dot( n,l ), 0,1 );
  float ambient = 0.15f;

  /*if(uv.x > 1.01f || uv.y > 1.01f)
    color = vec4(1.0,1.0,0.0,1.0f);
  else if(0.01f > n.y || 0.01f > l.y)
    color = vec4(1.0,1.0,0.0,1.0f);
  else
    color = vec4(uv.x,0.0f,uv.y,1.0f);
*/  
color = vec4(vec3(ambient) + blue * cost,1.0f);
    //color = vec4(normal.x,normal.y,normal.z,1.0f);
  //l = clamp(l,0,1);
    //color = vec4(uv.x,uv.y,0.0f,1.0f);
    //color = vec4((l.x+1)/2,(l.y+1)/2,(l.z+1)/2,1.0f);

 // color = texture(u_NormalMap, uv/16);
 // }
}
