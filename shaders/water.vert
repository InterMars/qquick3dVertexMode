VARYING vec3 pos;
VARYING vec3 normal;
VARYING vec2 texcoord;

#define PI 3.14159265359

void MAIN()
{
    pos = VERTEX;
    texcoord = UV0;
    normal = NORMAL;

    pos = normalize(pos);

    float a = PI * pos.x + uTime;
    float b = PI * pos.z + uTime;
    if(uMode == 1) {
        if(VERTEX.y > uHeight){
            VERTEX.y += sin(a)+cos(b);
            NORMAL.x += cos(a)-sin(b);
        }
    }else {
        a += uHeight;
        b += uHeight;
        bool frozen = (uFrozen)?(VERTEX.y > 1.0) : true ;
        if (frozen) {
            if (uEffect == 1){
                VERTEX.y += sin(a)*sin(a) + sin(b)*sin(b);
                NORMAL.x += 2*cos(a) + 2*cos(b);
            }else if(uEffect == 2){
                VERTEX.y += sin(1.5*a);
                NORMAL.y += 1.5*cos(1.5*a);
                VERTEX.x += sin(1.5*b);
                NORMAL.x += 1.5*cos(1.5*b);
            }else {
                VERTEX.y += sin(a);
                NORMAL.x += cos(a);
            }
        }
    }
}


