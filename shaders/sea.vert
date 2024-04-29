VARYING vec3 pos;
VARYING vec3 normal;
VARYING vec2 texcoord;

#define PI 3.14159265359

void MAIN()
{
    pos = VERTEX;
    texcoord = UV0;
    normal = NORMAL;
    if(pos.y > 2.0){
        pos = normalize(pos);
        float a = PI * pos.x;
        float b = PI * pos.z;
        VERTEX.y += sin(a)*cos(b);
        NORMAL.x += cos(a)*sin(b)-sin(a)*cos(b)/*-2*sin(b)*cos(b)*/;
    }
}


