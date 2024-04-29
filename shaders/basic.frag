VARYING vec3 pos;
VARYING vec2 texcoord;
VARYING vec3 normal;

// sea
const float SEA_HEIGHT = 2.0;
const float SEA_CHOPPY = 4.0;
const float SEA_SPEED = 0.8;
const float SEA_FREQ = 0.16;

#define SEA_TIME (1.0 + iTime * SEA_SPEED)

const mat2 octave_m = mat2(1.6,1.2,-1.2,1.6);

float hash( vec2 p ) {
    float h = dot(p,vec2(127.1,311.7));
    return fract(sin(h)*43758.5453123);
}
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
    // 添加噪声，使得海浪具有随机性
    uv += noise(uv);

    // 计算海浪的水波效果
    vec2 wv = 1.0 - abs(sin(uv));  // 计算波浪的垂直方向上的水波效果
    vec2 swv = abs(cos(uv));       // 计算波浪的水平方向上的水波效果
    wv = mix(wv, swv, wv);         // 将垂直和水平方向的水波效果混合在一起

    // 计算海浪的最终高度，以及混乱度对高度的影响
    return pow(1.0 - pow(wv.x * wv.y, 0.65), choppy);
}

float map(vec3 p) {
    float freq = SEA_FREQ;
    float amp = SEA_HEIGHT;
    float choppy = SEA_CHOPPY;//海浪的混乱程度或者说颠簸度
    vec2 uv = p.xz; uv.x *= 0.75;

    return p.y;
}
void MAIN() {
    BASE_COLOR  = vec4(vec3(0.0,0.09,0.18), 1.0) * map(pos);
}
