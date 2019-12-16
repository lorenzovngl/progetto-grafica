uniform int u_genCoords; // 0 = explicit coors, 1 = object linear, 2 = sphere map
uniform vec4 u_sPlane;
uniform vec4 u_tPlane;
uniform mat4 u_biasedShadowMvpMatrix;  // (*) Actually it's already multiplied with vMatrixInverse (in C code, so that the multiplication can be easily done with doubles)
varying vec4 v_shadowCoord;
varying vec4 v_diffuse;
varying vec2 v_texCoord;

varying vec3 N;
varying vec3 L;
varying vec3 E;
varying vec3 H;

varying vec4 vertex;

void main()	{
	vertex = gl_Vertex;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    vec3 normal = gl_NormalMatrix * gl_Normal;
	vec3 lightVector = gl_LightSource[0].position.xyz;// - gl_Vertex.xyz;\n"
	float nxDir = max(0.0, dot(normal, lightVector));
	v_diffuse = gl_LightSource[0].diffuse * nxDir;
	gl_FrontColor = gl_Color;
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;		// Normalizza le coordinate texture
	if (u_genCoords == 0){
		v_texCoord = gl_TexCoord[0].st;
	} else if (u_genCoords == 1){
		// Object linear
		v_texCoord.x = dot(gl_Vertex * u_sPlane, u_sPlane);
		v_texCoord.y = dot(gl_Vertex * u_tPlane, u_tPlane);
	} /*else if (u_genCoords == 2){
		// Sphere map
		vec3 u = normalize(vec3(gl_ModelViewMatrix * position));
		vec3 n = normalize(gl_NormalMatrix);
		vec3 r = reflect(u, n);
		float m = sqrt(r.x*r.x + r.y*r.y + (r.z+0.1)*(r.z+0.1));
		v_texCoord.x = (r.x/m + 0.5)/1;
		v_texCoord.y = (r.y/m + 0.5)/1;
	}*/
	vec4 eyePosition = gl_ModelViewMatrix * gl_Vertex;
	vec4 eyeLightPos = gl_LightSource[1].position;
	N = normalize(gl_NormalMatrix * gl_Normal);
	L = normalize(eyeLightPos.xyz - eyePosition.xyz);
	E = -normalize(eyePosition.xyz);
	H = normalize(L + E);
	v_shadowCoord = u_biasedShadowMvpMatrix*(gl_ModelViewMatrix*gl_Vertex); // (*) We don't pass a 'naked' mMatrix in shaders (not robust to double precision usage). We dress it in a mvMatrix. So here we're passing a mMatrix from camera space to light space (through a mvMatrix).
}                                                                          // (the bias just converts clip space to texture space)