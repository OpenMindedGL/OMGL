#shader vertex
#version 330 core
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 uv_coords;
layout(location = 2) in vec3 normals;
out vec3 v_normal;
out vec3 v_vertex;
out vec2 uv;
uniform mat4 u_MVP;
uniform mat4 u_M;
uniform mat4 u_V;
void main(){
	uv = uv_coords;
	v_vertex = ( u_V * u_M * vec4(vPos,1)).xyz;
	v_normal = ( u_V * u_M * vec4(normals, 0)).xyz;
	gl_Position =  u_MVP * vec4(vPos,1);	

}
#shader fragment
#version 330 core
layout (location = 0) out vec4 color;
in vec3 v_normal;
in vec3 v_vertex;
in vec2 uv;
uniform float u_Ni;
uniform float u_Ke;
uniform mat4 u_V;
uniform vec3 u_Kd;
uniform vec3 u_Ks;
uniform vec3 u_Ka;
uniform float u_D;
uniform float u_Ns;
void main(){
  	vec3 n = normalize( v_normal );
  	vec3 lightPos = (u_V * vec4(0.0f,10000.0f,0.0f,1)).xyz;
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
	color = vec4(colorPhong, vec4(u_Kd, 1.0).a);

}
