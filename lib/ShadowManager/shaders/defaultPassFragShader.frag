uniform sampler2D u_shadowMap;
uniform vec2 u_shadowDarkening; // .x = fDarkeningFactor [10.0-80.0], .y = min value clamp [0.0-1.0]
varying vec4 v_shadowCoord;
varying vec4 v_diffuse;
void main() {
    float shadowFactor = 1.0;
    vec4 shadowCoordinateWdivide = v_shadowCoord/v_shadowCoord.w;
    shadowFactor = clamp(exp(u_shadowDarkening.x*(texture2D(u_shadowMap,(shadowCoordinateWdivide.st)).r - shadowCoordinateWdivide.z)),u_shadowDarkening.y,1.0);
    //shadowFactor = clamp(exp(u_shadowDarkening.x*texture2D(u_shadowMap,(shadowCoordinateWdivide.st)).r) *   exp(-u_shadowDarkening.x*shadowCoordinateWdivide.z),u_shadowDarkening.y,1.0);
    gl_FragColor = gl_LightSource[0].ambient + (v_diffuse * vec4(gl_Color.rgb*shadowFactor,1.0));
}