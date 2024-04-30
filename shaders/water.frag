VARYING vec3 pos;
VARYING vec2 texcoord;
VARYING vec3 normal;

#define PI 3.14159265359
#define SEA_TIME (1.0 + uTime * 0.3)

const mat2 octave_m = mat2(1.6,0.2,-1.2,1.6);

float hash( vec2 p ) {
    float h = dot(p,vec2(127.1,311.7));
    return fract(sin(h)*43758.5453123);
}
// Perlin
float noise( in vec2 p ) {
    vec2 i = floor( p );
    vec2 f = fract( p );
        vec2 u = f*f*(3.0-2.0*f);
    return -1.0+2.0*mix( mix( hash( i + vec2(0.0,0.0) ),
                     hash( i + vec2(1.0,0.0) ), u.x),
                mix( hash( i + vec2(0.0,1.0) ),
                     hash( i + vec2(1.0,1.0) ), u.x), u.y);
}
float sea_octave(vec2 uv, float choppy) {
    // 添加噪声
    uv += noise(uv);

    // 水波
    vec2 wv = abs(sin(uv));  // 计算波浪的垂直方向上的水波效果
    vec2 swv = abs(cos(uv));       // 计算波浪的水平方向上的水波效果
    wv = mix(wv, swv, wv);         // 将垂直和水平方向的水波效果混合在一起

    // 计算海浪的最终高度，以及混乱度对高度的影响
    return pow(1.0 - pow(wv.x * wv.y, 2.0), choppy);
}

float map(vec3 p) {
    float choppy = 2.0;      //混乱程度或者说颠簸度
    vec2 uv = p.xz; uv += 0.2;

    return p.y*sea_octave(uv, choppy);
}
void MAIN() {
    BASE_COLOR  = vec4(vec3(0.0,0.09,0.18) * map(pos) * SEA_TIME, 1.0);
}
