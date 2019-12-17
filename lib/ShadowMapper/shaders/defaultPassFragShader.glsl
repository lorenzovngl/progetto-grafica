uniform sampler2D u_shadowMap;
uniform sampler2D u_texture;
uniform int u_colorOrTexture; // 0 = color, 1 = texture
uniform int u_fogEnabled;
uniform vec4 u_color;
varying vec2 v_texCoord;
uniform vec2 u_shadowDarkening; // .x = fDarkeningFactor [10.0-80.0], .y = min value clamp [0.0-1.0]
varying vec4 v_shadowCoord;
varying vec4 v_diffuse;

varying vec3 N;
varying vec3 L;
varying vec3 E;
varying vec3 H;

varying vec4 v_position;
uniform mat4 u_cameraViewMatrix;

float getFogFactor(float d) {
    const float FogMax = 50.0;
    const float FogMin = 5.0;

    if (d>=FogMax) return 1.0;
    if (d<=FogMin) return 0.0;

    return 1.0 - (FogMax - d) / (FogMax - FogMin);
}

void main() {
    float shadowFactor = 1.0;
    //vec4 shadowCoordinateWdivide = v_shadowCoord / v_shadowCoord.w;
    float shadowTexCoord = texture2D(u_shadowMap, v_shadowCoord.st).r;
    if (shadowTexCoord < 0.5){
        // This control prevent the drawing of a big strange shadow between sea and sky
        shadowTexCoord -= v_shadowCoord.z;
    }
    float exp = exp(u_shadowDarkening.x * shadowTexCoord);
    shadowFactor = clamp(exp, u_shadowDarkening.y, 1.0);
    //shadowFactor = clamp(exp(u_shadowDarkening.x*(texture2D(u_shadowMap,(shadowCoordinateWdivide.st)).r - shadowCoordinateWdivide.z)),u_shadowDarkening.y,1.0);

    vec3 Normal = normalize(N);
    vec3 Light  = normalize(L);
    vec3 Eye    = normalize(E);
    vec3 Half   = normalize(H);

    float Kd = max(dot(Normal, Light), 0.5);
    float Ks = pow(max(dot(Half, Normal), 1.0), gl_FrontMaterial.shininess);
    float Ka = 0.5;
    float intensity = 0.2;

    vec4 diffuse  = Kd * gl_FrontLightProduct[1].diffuse*intensity;
    vec4 specular = Ks * gl_FrontLightProduct[1].specular*intensity;
    vec4 ambient  = Ka * gl_FrontLightProduct[1].ambient*intensity;

    if (u_colorOrTexture == 1){
        gl_FragColor = ambient + diffuse + specular + vec4(texture2D(u_texture, v_texCoord).rgb*shadowFactor, 1);
    } else {
        gl_FragColor = ambient + diffuse + specular + vec4(u_color.rgb*shadowFactor, u_color.a);
    }
    if (u_fogEnabled == 1){
        float d = distance(u_cameraViewMatrix*v_position, v_position);
        float alpha = getFogFactor(d);
        vec4 FogColor = vec4(0.5, 0.5, 0.7, 1.0);
        gl_FragColor = mix(gl_FragColor, FogColor, alpha);
    }
    //gl_FragColor = vec4(v_shadowCoord.xyz, shadowTexCoord);
}