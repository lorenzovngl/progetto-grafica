uniform mat4 u_biasedShadowMvpMatrix;  // (*) Actually it's already multiplied with vMatrixInverse (in C code, so that the multiplication can be easily done with doubles)
varying vec4 v_shadowCoord;
varying vec4 v_diffuse;
varying vec2 v_texCoord;

void main()	{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    vec3 normal = gl_NormalMatrix * gl_Normal;
	vec3 lightVector = gl_LightSource[0].position.xyz;// - gl_Vertex.xyz;\n"
	float nxDir = max(0.0, dot(normal, lightVector));
	v_diffuse = gl_LightSource[0].diffuse * nxDir;
	gl_FrontColor = gl_Color;
	vec4 sPlane = vec4(1.0, 1.0, 1.0, 0.0);
	vec4 tPlane = vec4(0.0, 1.0, 1.0, 0.0);
	v_texCoord.x = dot(gl_Vertex * vec4(1, 1, 0, 0), sPlane);
	v_texCoord.y = dot(gl_Vertex * vec4(0, 1, 1, 0), tPlane);
	v_shadowCoord = u_biasedShadowMvpMatrix*(gl_ModelViewMatrix*gl_Vertex); // (*) We don't pass a 'naked' mMatrix in shaders (not robust to double precision usage). We dress it in a mvMatrix. So here we're passing a mMatrix from camera space to light space (through a mvMatrix).
}                                                                          // (the bias just converts clip space to texture space)