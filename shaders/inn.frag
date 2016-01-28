#version 300 es
precision highp float;
precision mediump int;

out vec4 fragColor;

vec2 fragCoord = gl_FragCoord.xy;

uniform vec2 iResolution;
uniform float iGlobalTime;

#define INFTY 1e20
#define EPS 0.01
#define PI 3.1415926535

float botPlane(in vec3 p, in float start) {
  return p.y - start;
}

float cylinder(vec3 p, vec2 h) {
  vec2 d = abs(vec2(length(p.xz),p.y)) - h;
  return min(max(d.x,d.y),0.0) + length(max(d,0.0));
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

float opS(in float a, in float b) {
  return max(a, -b);
}

float opU(in float a, in float b) {
  return min(a, b);
}

float tankard(in vec3 p) {
  float d = 1e20;
  d = opU(d, cylinder(p, vec2(0.25)));
  d = opS(d, cylinder(p, vec2(0.22, 0.5)));
  d = opU(d, box(p - vec3(0.0, 0.1, 0.35), vec3(0.025, 0.025, 0.1)));
  d = opU(d, box(p - vec3(0.0, -0.2, 0.35), vec3(0.025, 0.025, 0.1)));
  d = opU(d, box(p - vec3(0.0, -0.05, 0.425), vec3(0.025, 0.15, 0.025)));
  return d;
}

float map(in vec3 p) {
  float d = 1e20;
  d = opU(d, box(p, vec3(1.6, 0.1, 15.0)));
  d = opU(d, box(p + vec3(3.0, 0.0, 0.0), vec3(0.1, 3.0, 15.0))); // Wall behind bar
  d = opU(d, box(p + vec3(0.0, 7.0, 0.0), vec3(0.1, 6.0, 15.0))); // Bottom of bar
  d = opU(d, tankard(p - vec3(1.2, 0.44, 0.0)));
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
        td > 20.0) {
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

float lightIntensity(in float t) {
  float x = iGlobalTime + t;
  float cs = (cos(x) + cos(x * 1.5) + cos(x * 3.0) + cos(x * 4.5) + 2.5) / 6.5;
  cs += (sin(x + 1.0) + sin(x * 2.5) + sin(x * 4.0) + sin(x * 5.5) + 2.5) / 6.5;
  return 0.5 + 0.5 * clamp(cs, 0.0, 1.0);
}


vec3 fireColor(in float strength) {
  return vec3(0.5 + 0.5 * strength, 0.3 * strength, 0.0);
}

vec3 woodGrain(in vec3 p, in vec3 light, in vec3 dark) {
  p = p.yzx;
  float radius = length(p.xz) - p.y / 5.0;
  float angle;

  if (p.x == 0.0) {
    angle = PI / 2.0;
  } else {
    angle = atan(p.z / p.x) * 0.4;
  }

  radius += 2.0 * sin(20.0 * angle + p.y / 150.0);
  float grain = mod(radius, 60.0);

  if (grain < 40.0) {
    return light;
  } else {
    return dark;
  }
}

bool isTankardHandle(in vec3 p) {
  return p.x > 1.0 &&
  p.y < 0.7 &&
  p.y > 0.2 &&
  p.z > 0.26;
}

bool isBar(in vec3 p) {
  return p.x > -1.6;
}

vec3 colorAt(in vec3 p) {
  if (isTankardHandle(p)) {
    return vec3(0.0);
  } else if (isBar(p)) {
    return woodGrain(p, vec3(0.59, 0.4, 0.2) * 0.4, vec3(0.59, 0.4, 0.2) * 0.6) -
    woodGrain(p * 1.5 + vec3(0.2), vec3(0.59, 0.4, 0.2) * 0.1, vec3(0.59, 0.4, 0.2) * 0.2);
  } else {
    return vec3(1.0, 0.8, 0.5);
  }
}

vec3 lighting(in vec3 ro, in vec3 rd) {
  vec3 N = normalAt(ro);
  vec3 V = -rd;

  float diff = 0.0;
  float spec = 0.0;
  float shadow = 0.25;
  float strength;
  vec3 fire = vec3(0.0);

  /* Brick bump map on back wall */
  if (!isBar(ro)) {
    float offset = mod(ro.y, 1.25) < 0.625 ? PI : 0.0;
    N = normalize(vec3(0.4, pow(0.5 + 0.5 * sin(ro.y * 10.0 + 1.7), 200.0) +
                       pow(0.5 + 0.5 * sin(ro.z * 2.0 + offset), 2000.0), 0.0));
  }

  for (float i = -3.0; i <= 3.0; i += 3.0) {
    float j =i == 0.0 ? 4.0 : 0.0;
    vec3 light = vec3(j, 3.0, i);
    vec3 L = normalize(light - ro);
    vec3 R = reflect(-L, N);

    strength = lightIntensity((i + j * j) / 2.0) * 0.5;
    float lDiff = max(diff, diff + dot(N, L) * strength);
    float lSpec = max(spec, spec + pow(dot(R, V) * strength, 3.0));

    spec += lSpec * 0.5;
    diff += lDiff * 0.5;

    fire += fireColor(lDiff + lSpec);

    float td = marchRay(light, -L);
    vec3 lp = td * -L + light;
    if (shadow < 0.5) {
      if (length(lp - ro) < 0.1) {
        shadow = 1.0;
      }
    }
  }

  return mix(colorAt(ro) * (diff + spec), fire, 0.15) *
  occlusion(ro, N, 0.7) *
  shadow;
}

#define MOVING_CAMERA
#define MOVE_SPEED 0.2
#define CAM_DIST 2.0
void setupCamera(in vec2 uv, out vec3 ro, out vec3 rd) {
#ifdef MOVING_CAMERA
  float theta = sin(iGlobalTime * MOVE_SPEED) * 0.125;
  ro = vec3(CAM_DIST * cos(theta), 1.0, CAM_DIST * sin(theta));
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
