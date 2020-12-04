#version 130 core

#define MAX_STEPS 100
#define MAX_DIST 100.
#define SURF_DIST 0.01

uniform vec2 screenSize;
uniform float time;
uniform bool shadow;

// getting spheres from main program //
uniform int numSpheres;
uniform vec4 sphere[100];
uniform vec4 sphereCol[100];
// ----------------------------------//

// getting capsules from main program //
uniform int numCaps;
uniform vec3 capsuleA[100];
uniform vec3 capsuleB[100];
uniform float capsuleR[100];
uniform vec4 capsuleCol[100];
// --------------------------------- //

// getting planes from main program //
uniform int numPlanes;
uniform vec3 planeP[100];
uniform vec3 planeN[100];
uniform vec4 planeCol[100];
// -------------------------------- //

struct distCol
{
    float dist;
    vec3 col;
};


float capsuleSDF(vec3 p, vec3 a, vec3 b, float r)
{
    vec3 ab = b - a;
    vec3 ap = p - a;

    float t = dot(ab, ap) / dot(ab, ab);
    t = clamp(t, 0., 1.);

    vec3 c = a + t * ab;
    return length(p - c) - r;
}

float planeSDF(vec3 p, vec3 pp, vec3 n)
{
    return dot(n, p - pp);
}

float sphereSDF(vec3 p, vec4 sphere)
{
        return length(p - sphere.xyz) - sphere.w;
}

distCol sceneSDF(vec3 p)
{
    distCol min = distCol(MAX_DIST, vec3(0));
    // spheres
    for (int i = 0; i < numSpheres; i++)
    {
        float dist = sphereSDF(p, sphere[i]);
        if (min.dist > dist)
        {
            min = distCol(dist, sphereCol[i].xyz);
        }
    }
    // capsules
    for (int i = 0; i < numCaps; i++)
    {
        float dist = capsuleSDF(p, capsuleA[i], capsuleB[i], capsuleR[i]);
        if (min.dist > dist)
        {
            min = distCol(dist, capsuleCol[i].xyz);
        }
    }
    // planes
    for (int i = 0; i < numPlanes; i++)
    {
        float dist = planeSDF(p, planeP[i], planeN[i]);
        if (min.dist > dist)
        {
            min = distCol(dist, planeCol[i].xyz);
        }
    }

    return min;
}

float GetDist(vec3 p)
{
    //vec4 sphere = vec4(0, 1, 6, 1);
    float sceneDist = sceneSDF(p).dist;
    float planeDist = p.y;
    float d = min(sceneDist, planeDist);
    return d;
}

distCol RayMarch(vec3 ro, vec3 rd)
{
    float dO = 0.;
    vec3 col;

    for (int i = 0; i < MAX_STEPS; i++)
    {
        vec3 p = ro + dO * rd;
        distCol dS = sceneSDF(p);
        dO += dS.dist;
        if (dS.dist < SURF_DIST || dO > MAX_DIST)
        {
            col = dS.col;
            break;
        }
    }
    return distCol(dO, col);
}

vec3 GetNormal(vec3 p)
{
    float d = GetDist(p);
    vec2 e = vec2(.01, 0);

    vec3 n = d - vec3(
        GetDist(p - e.xyy),
        GetDist(p - e.yxy),
        GetDist(p - e.yyx));
    
    return normalize(n);

}

float GetLight(vec3 p)
{
    vec3 lightPos = vec3(0, 10, 6);
    lightPos.xz += vec2(sin(time), cos(time)) * 2;

    vec3 l = normalize(lightPos - p);
    vec3 n = GetNormal(p);

    float dif = clamp(dot(n, l), 0., 1.);

    if (shadow)
    {
        float d = RayMarch(p + n * SURF_DIST * 2, l).dist;
        if (d < length(lightPos - p)) dif *= .1;
    }

    return dif;
}

void main()
{
    vec2 uv = (gl_FragCoord.xy - .5 * screenSize) / min(screenSize.x, screenSize.y);
    vec3 col = vec3(0);

    vec3 ro = vec3(0, 4, 0);
    vec3 rd = normalize(vec3(uv.x, uv.y, 1)); 

    distCol dc = RayMarch(ro, rd);

    float d = dc.dist;

    vec3 p =  ro + rd * d;

    float dif = GetLight(p);
    col = dc.col;
    col *= dif;

    gl_FragColor = vec4(col, 1.0);
};