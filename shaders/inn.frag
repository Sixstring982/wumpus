#version 330
precision highp float;
precision mediump int;

out vec4 fragColor;

vec2 fragCoord = gl_FragCoord.xy;

uniform vec2 iResolution;
uniform float iGlobalTime;

#define INFTY 1e20
#define EPS 0.01

float botPlane(in vec3 p, in float start) {
  return p.y - start;
}

float box(in vec3 p, in vec3 b) {
  vec3 d = abs(p) - b;
  return min(max(d.x, max(d.y, d.z)), 0.0) +
  length(max(d, 0.0));
}

float boxRep(in vec3 p, in vec3 b, in vec3 c) {
  vec3 q = mod(p, c) - 0.5 * c;
  return box(q, b);
}

float boxDisplace(in vec3 p, in vec3 b) {
  return box(p, b) + box(p, b * 1.2);
}

float opS(in float a, in float b) {
  return max(a, -b);
}

float opU(in float a, in float b) {
  return min(a, b);
}

float map(in vec3 p) {
  float d = 1e20;
  d = opU(d,botPlane(p, -1.0));
  d = opU(d, boxRep(p - vec3(0.0, 3.5, 11.0), vec3(0.5, 4.0, 0.5), vec3(8.0, 10.0, 20.0)));
  d = opU(d, box(p - vec3(0.0, 2.5, 1.1), vec3(5.0, 0.25, 0.5)));
  d = opU(d, box(p, vec3(5.0, 5.0, 0.5)));
  return d;
}

vec3 normalAt(in vec3 ro) {
  vec2 e = vec2(EPS, 0.0);
  return normalize(vec3(map(ro + e.xyy) - map(ro - e.xyy),
                        map(ro + e.yxy) - map(ro - e.yxy),
                        map(ro + e.yyx) - map(ro - e.yyx)));
}

float marchRay(in vec3 ro, in vec3 rd) {
  float d = EPS;
  float td = 0.0;

  for (int i = 0; i < 100; i++) {
    if (d < EPS ||
        td > 8.0) {
      continue; // break;
    }

    d = map(ro);
    td += d;
    ro += rd * d;
  }

  if (d < EPS) {
    return td;
  } else {
    return INFTY;
  }
}

float occlusion(in vec3 p, in vec3 n, in float k) {
  float s = 0.0;
  float delta = 0.5;
  for (float i = 1.0; i <= 5.0; i++) {
    float c = (1.0 / pow(2.0, i)) *
    (i * delta -
     map(p + n * i * delta));
    s += c;
  }

  return 1.0 - k * s;
}

bool isRug(in vec3 ro) {
  return abs(ro.x) < 2.5 &&
  ro.y < -0.99 &&
  ro.z > 2.5;
}

bool isLight(in vec3 ro) {
  return abs(ro.x) < 3.5 &&
  ro.y < -0.99 &&
  abs(ro.z) < 1.5;
}

vec3 colorAt(in vec3 p) {
  if (isLight(p)) {
    return vec3(1.0);
  } else if (isRug(p)) {
    return vec3(1.0, 0.0, 0.0);
  }
  return vec3(0.1725, 0.149, 0.1137) * 1.2;
}

vec3 fireColor(in float strength) {
  return vec3(0.5 + 0.5 * strength, 0.3 * strength, 0.0);
}

vec3 lighting(in vec3 ro, in vec3 rd) {
  vec3 N = normalAt(ro);
  vec3 V = -rd;

  float diff = 0.0;
  float spec = 0.0;
  float strength;

  if (isLight(ro)) {
    diff = spec = 1.0;
  } else {
    for (float i = -3.0; i <= 3.0; i += 0.5) {
      vec3 light = vec3(i, -0.5, 1.0);
      vec3 L = normalize(light - ro);
      vec3 R = reflect(-L, N);

      strength = 0.5 + 0.5 * sin(iGlobalTime + i * i);
      diff = max(0.0, diff + dot(N, L) * strength);
      spec = max(0.0, spec + pow(dot(R, V) * strength, 3.0));
    }
  }

  return mix(colorAt(ro) * (diff + spec), fireColor(diff + spec), 0.2) * occlusion(ro, N, 0.7);
}

// #define MOVING_CAMERA
#define CAM_DIST 5.0
void setupCamera(in vec2 uv, out vec3 ro, out vec3 rd) {
#ifdef MOVING_CAMERA
  ro = vec3(CAM_DIST * cos(iGlobalTime), 1.0, CAM_DIST * sin(iGlobalTime));
#else
  ro = vec3(0.0, 1.0, CAM_DIST);
#endif
  vec3 t = vec3(0.0);
  vec3 up = vec3(0.0, 1.0, 0.0);

  vec3 cdir = normalize(t - ro);
  vec3 cright = normalize(cross(cdir, up));
  vec3 cup = cross(cright, cdir);

  rd = normalize(cdir + cup * uv.y + cright * uv.x);
}

  void main(void) {
  vec2 uv = ((fragCoord.xy / iResolution.xy) - vec2(0.5)) * 2.0;
  uv.x *= iResolution.x / iResolution.y;

  vec3 ro, rd;
  setupCamera(uv, ro, rd);
  float td = marchRay(ro, rd);

  if (td < INFTY) {
  fragColor = vec4(lighting(ro + td * rd, rd), 1.0);
} else {
  fragColor = vec4(0.0, 0.0, 0.0, 1.0);
}
}
