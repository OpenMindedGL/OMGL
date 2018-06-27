#shader vertex
#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 uv_coords;
layout(location = 2) in vec3 normals;

out vec2 uv;
out vec3 v_normal;
out vec3 lightdir;
out mat4 MV;
out vec2 pp;
out vec3 v_vertex;

out float d1;

  
// Values that stay constant for the whole mesh.
uniform mat4 u_MVP;
uniform mat4 u_VP;
uniform mat4 u_M;
uniform mat4 u_V;
//uniform sampler2D u_HeightMap;
uniform sampler2D u_HeightMapLinear;
uniform sampler2D u_DefaultSampler;
uniform sampler2D u_NormalMap;
uniform ivec2 base;
uniform ivec2 torBase;
uniform ivec2 torBegin;
uniform int u_UnitSize;
uniform int u_MaxHeight;
uniform int u_MinHeight;

vec4 getnormals(sampler2D s, vec2 pos){
  vec2 size = vec2(u_UnitSize,0.0);
  vec3 off = vec3(-u_UnitSize/2,0,u_UnitSize/2);

  float texsize = textureSize(s, 0).x;

  vec2 wPos = vec2(pos.x,pos.y);
  vec2 tPos = vec2(mod((torBase + (wPos - base))/u_UnitSize,texsize)); 
  vec2 uv = (vec2(tPos)/(texsize))+vec2(0.00001,0.00001);
  float s11 = dot( texture(s, uv), vec4(1.0, 1/255.0, 1/65025.0, 1/16581375.0) )*(u_MaxHeight-u_MinHeight)+u_MinHeight;

  wPos = vec2(pos.x,pos.y)+off.xy;
  tPos = vec2(mod((torBase + (wPos - base))/u_UnitSize,texsize)); 
  uv = (vec2(tPos)/(texsize))+vec2(0.00001,0.00001);
  float s01 = dot( texture(s, uv), vec4(1.0, 1/255.0, 1/65025.0, 1/16581375.0) )*(u_MaxHeight-u_MinHeight)+u_MinHeight;

  wPos = vec2(pos.x,pos.y)+off.zy;
  tPos = vec2(mod((torBase + (wPos - base))/u_UnitSize,texsize)); 
  uv = (vec2(tPos)/(texsize))+vec2(0.00001,0.00001);
  float s21 = dot( texture(s, uv), vec4(1.0, 1/255.0, 1/65025.0, 1/16581375.0) )*(u_MaxHeight-u_MinHeight)+u_MinHeight;

  wPos = vec2(pos.x,pos.y)+off.yx;
  tPos = vec2(mod((torBase + (wPos - base))/u_UnitSize,texsize)); 
  uv = (vec2(tPos)/(texsize))+vec2(0.00001,0.00001);
  float s10 = dot( texture(s, uv), vec4(1.0, 1/255.0, 1/65025.0, 1/16581375.0) )*(u_MaxHeight-u_MinHeight)+u_MinHeight;

  wPos = vec2(pos.x,pos.y)+off.yz;
  tPos = vec2(mod((torBase + (wPos - base))/u_UnitSize,texsize)); 
  uv = (vec2(tPos)/(texsize))+vec2(0.00001,0.00001);
  float s12 = dot( texture(s, uv), vec4(1.0, 1/255.0, 1/65025.0, 1/16581375.0) )*(u_MaxHeight-u_MinHeight)+u_MinHeight;

  vec3 va = normalize(vec3(size.xy,s21-s01));
  vec3 vb = normalize(vec3(size.yx,s12-s10));
  return vec4( cross(va,vb), s11 ); 
}


void main(){
  vec4 pos = round(u_M * vec4(vPos,1.0));
  vec4 posV = u_V * pos;
  pp = pos.xz;
  float texsize = textureSize(u_DefaultSampler, 0).x;
  ivec2 wPos = ivec2(pos.x,pos.z);
  ivec2 tPos = ivec2(mod((torBase + (wPos - base))/u_UnitSize,texsize)); 
  uv = (vec2(tPos)/(texsize))+vec2(0.00001,0.00001);
  pos.y =dot( texture(u_DefaultSampler, uv), vec4(1.0, 1/255.0, 1/65025.0, 1/16581375.0) ) *(u_MaxHeight-u_MinHeight)+u_MinHeight;
  gl_Position =  u_VP * pos;
  v_vertex = (u_V * pos).xyz;
  vec3 LightPosition_worldspace = vec3(100.0f,500.0f,.0f);


  vec3 Position_worldspace = pos.xyz;

  vec3 vPos_cameraspace = (pos).xyz;

  vec3 lightpos_cameraspace = ( vec4(LightPosition_worldspace,1)).xyz;
  lightdir = lightpos_cameraspace - vPos_cameraspace.xyz;
  v_normal = (u_V * vec4(getnormals(u_HeightMapLinear, pp).xzy,0.0f)).xyz;
  

}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec2 uv;
in vec3 lightdir;
in vec3 v_normal;
in vec2 pp;
in float d1;
in vec3 v_vertex;
uniform ivec2 base;
uniform ivec2 torBase;
uniform ivec2 torBegin;
uniform int u_UnitSize;
uniform int u_MaxHeight;
uniform int u_MinHeight;
uniform mat4 u_V;

uniform sampler2D u_HeightMapLinear;

vec4 getnormals(sampler2D s, vec2 pos){
  vec2 size = vec2(u_UnitSize,0.0);
  vec3 off = vec3(-u_UnitSize/2,0,u_UnitSize/2);

  float texsize = textureSize(s, 0).x;

  vec2 wPos = vec2(pos.x,pos.y);
  vec2 tPos = vec2(mod((torBase + (wPos - base))/u_UnitSize,texsize)); 
  vec2 uv = (vec2(tPos)/(texsize))+vec2(0.00001,0.00001);
  float s11 = dot( texture(s, uv), vec4(1.0, 1/255.0, 1/65025.0, 1/16581375.0) )*(u_MaxHeight-u_MinHeight)+u_MinHeight;

  wPos = vec2(pos.x,pos.y)+off.xy;
  tPos = vec2(mod((torBase + (wPos - base))/u_UnitSize,texsize)); 
  uv = (vec2(tPos)/(texsize))+vec2(0.00001,0.00001);
  float s01 = dot( texture(s, uv), vec4(1.0, 1/255.0, 1/65025.0, 1/16581375.0) )*(u_MaxHeight-u_MinHeight)+u_MinHeight;

  wPos = vec2(pos.x,pos.y)+off.zy;
  tPos = vec2(mod((torBase + (wPos - base))/u_UnitSize,texsize)); 
  uv = (vec2(tPos)/(texsize))+vec2(0.00001,0.00001);
  float s21 = dot( texture(s, uv), vec4(1.0, 1/255.0, 1/65025.0, 1/16581375.0) )*(u_MaxHeight-u_MinHeight)+u_MinHeight;

  wPos = vec2(pos.x,pos.y)+off.yx;
  tPos = vec2(mod((torBase + (wPos - base))/u_UnitSize,texsize)); 
  uv = (vec2(tPos)/(texsize))+vec2(0.00001,0.00001);
  float s10 = dot( texture(s, uv), vec4(1.0, 1/255.0, 1/65025.0, 1/16581375.0) )*(u_MaxHeight-u_MinHeight)+u_MinHeight;

  wPos = vec2(pos.x,pos.y)+off.yz;
  tPos = vec2(mod((torBase + (wPos - base))/u_UnitSize,texsize)); 
  uv = (vec2(tPos)/(texsize))+vec2(0.00001,0.00001);
  float s12 = dot( texture(s, uv), vec4(1.0, 1/255.0, 1/65025.0, 1/16581375.0) )*(u_MaxHeight-u_MinHeight)+u_MinHeight;

  vec3 va = normalize(vec3(size.xy,s21-s01));
  vec3 vb = normalize(vec3(size.yx,s12-s10));
  return vec4( cross(va,vb), s11 ); 
}

void main(){
  vec3 u_Kd = vec3(0.1f,0.3f,0.1f);
  vec3 u_Ka = vec3(0.15f);
  float u_Ns = 1.0f;
  vec3 u_Ks = vec3(0.0f);
  //vec3 norm = texture(u_NormalMap, uv).rgb;

  //vec3 v_normal = (u_V * vec4(getnormals(u_HeightMapLinear, pp).xzy,0.0f)).xyz;
  vec3 n = normalize( v_normal );

  vec3 lightPos = vec3(0.0f,10000.0f,0.0f);
  vec3 to_light = lightPos - v_vertex;
  to_light = normalize(to_light);
  float cos_angle = dot(n, to_light);
  cos_angle = clamp(cos_angle, 0.0, 1.0);

  vec3 lightColor = vec3(0.8f);
  vec3 ambient_color = u_Ka * u_Kd;
  vec3 diffuse_color = u_Kd * cos_angle;
  vec3 reflection = 2.0 * dot(n, to_light) * n - to_light;

  vec3 to_camera = 1.0 * v_vertex;
  reflection = normalize( reflection );
  to_camera = normalize( to_camera );
  cos_angle = dot(reflection, to_camera);
  cos_angle = clamp(cos_angle, 0.0, 1.0);
  cos_angle = pow(cos_angle, u_Ns);

  vec3 specular_color = vec3(0.0, 0.0, 0.0);
  if (cos_angle > 0.0) {
    vec3 specular_color = lightColor * cos_angle * u_Ks;
    diffuse_color = diffuse_color * (1.0 - cos_angle);
  }
  vec3 colorPhong = (ambient_color + diffuse_color + specular_color);
  color = vec4(colorPhong, 1.0f);

/*
  vec3 n = normalize( normal );
  vec3 l = normalize( lightdir );
  float cost = clamp( dot( n,l ), 0,1 );
  float ambient = 0.15f;

  color = vec4(vec3(ambient) + blue * cost,1.0f);
*/
}
