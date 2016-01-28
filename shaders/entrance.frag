#version 300 es
precision highp float;
precision mediump int;

out vec4 fragColor;

vec2 fragCoord = gl_FragCoord.xy;

uniform vec2 iResolution;
uniform float iGlobalTime;

#define EPS 0.01
#define INFTY 1e20
#define PI 3.1415926536
#define STOT 0.70710678118

#define ROTX45 mat3(1.0, 0.0, 0.0, 0.0, STOT, -STOT, 0.0, STOT, STOT)
#define ROTZ45 mat3(STOT, -STOT, 0.0, STOT, STOT, 0.0, 0.0, 0.0, 1.0)

#define GRASS_COLOR vec3(0.0, 0.36, 0.03)
#define DIRT_COLOR vec3(0.47, 0.13, 0.0)

#define MOVING_CAMERA
#define CAMERA_DIST 5.0
#define CAMERA_HEIGHT 1.0
vec3 cameraPos() {
#ifdef MOVING_CAMERA
  float theta = iGlobalTime * 0.5;//  + iMouse.x * 0.02;
  return vec3(CAMERA_DIST * sin(theta),
  CAMERA_HEIGHT,
  CAMERA_DIST * cos(theta));
#else
  return vec3(0.0, CAMERA_HEIGHT, CAMERA_DIST);
#endif
}

  float sphere(in vec3 p, in float r) {
  return length(p) - r;
}

  float opU(in float a, in float b) {
  return min(a, b);
}

  float opS(in float a, in float b) {
  return max(a, -b);
}

  float box( vec3 p, vec3 b ) {
  return length(max(abs(p)-b,0.0));
}

  float boxFloor(vec3 p) {
  return p.y + (sin(p.x) * p.z + cos(p.z) * p.x) * 0.015;
}

  float cylinder(vec3 p, vec3 c) {
  return length(p.xz-c.xy)-c.z;
}

  float cappedCylinder(in vec3 p, in vec2 h) {
  vec2 d = abs(vec2(length(p.xz),p.y)) - h;
  return min(max(d.x,d.y),0.0) + length(max(d,0.0));
}

  float repCapCyl(in vec3 p, in vec3 c, in vec2 h) {
  vec3 q = mod(p,c)-0.5*c;
  return cappedCylinder(q, h);
}

  float map(in vec3 p) {
  float d = 0.0;
  d = opS(d, sphere(p, 10.0));
  d = opS(d, cylinder(p.yzx, vec3(0.0, 0.0, 4.0)));
  d = opS(d, cylinder(p.yxz, vec3(0.0, 0.0, 4.0)));
  d = opS(d, cylinder(p, vec3(0.0, 0.0, 4.0)));
  d = opS(d, cylinder(p, vec3(0.0, 0.0, 4.0)));
  d = opU(d, boxFloor(p + vec3(0.0, 3.0, 0.0)));
  d = opU(d, cylinder(p, vec3(0.0, 4.0, 0.125)));
  d = opU(d, cylinder(p, vec3(2.0, 4.0, 0.125)));

  d = opU(d, repCapCyl(p.yxz - vec3(0.0, 18.5, 21.5),
  vec3(1.0, 35.0, 35.0),
  vec2(0.125, 0.9)));
  return d;
}

  vec3 normalAt(in vec3 p) {
  vec2 e = vec2(EPS, 0.0);
  return normalize(vec3(map(p + e.xyy) - map(p - e.xyy),
  map(p + e.yxy) - map(p - e.yxy),
  map(p + e.yyx) - map(p - e.yyx)));
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

  float marchRay(in vec3 ro, in vec3 rd) {
  float d = EPS;
  float td = 0.0;

  for (int i = 0; i < 100; i++) {
  if (d < EPS ||
  td > 30.0) {
  continue; // break;
}

  d = map(ro);
  ro += d * rd;
  td += d;
}

  if (d < EPS) {
  return td;
} else {
  return INFTY;
}
}

  bool isGrass(in vec3 p) {
  return length(p.xz) < 9.0 &&
  p.y < -2.7;
}

  float shininessAt(in vec3 p) {
  if (isGrass(p)) {
  return INFTY;
} else {
  return 32.0;
}
}

  vec3 colorAt(in vec3 p) {
  if (isGrass(p)) {
  return GRASS_COLOR;
} else {
  return DIRT_COLOR;
}
}


  float lightIntensity(in float t) {
  float x = iGlobalTime + t;
  float cs = (cos(x) + cos(x * 1.5) + cos(x * 3.0) + cos(x * 4.5) + 2.5) / 6.5;
  cs += (sin(x + 1.0) + sin(x * 2.5) + sin(x * 4.0) + sin(x * 5.5) + 2.5) / 6.5;
  return 0.5 + 0.5 * clamp(cs, 0.0, 1.0);
}

#define LIGHT_COUNT 2
  bool lightAt(in int idx, out vec3 light) {
  if (idx == 0) {
  light = vec3(2.0, 25.0, 3.0);
  return true;
} else if (idx == 1) {
  light = cameraPos() + vec3(0.5);
  return true;
}

  return false;
}

  bool lightColor(in int idx, out vec3 color) {
  if (idx == 0) {
  color = vec3(1.0);
  return true;
} else if (idx == 1) {
  color = vec3(1.0, 0.5, 0.0) * lightIntensity(1.0);
  return true;
}
  return false;
}

  vec3 lighting(in vec3 ro, in vec3 rd) {
  vec3 N = normalAt(ro);
  vec3 V = -rd;

  vec3 diff = vec3(0.0);
  vec3 spec = vec3(0.0);
  float shadow = 1.0;

  for (int i = 0; i < LIGHT_COUNT; i++) {
  vec3 light;
  vec3 lColor;
  if (!lightAt(i, light)) {
  continue;
}
  lightColor(i, lColor);
  vec3 L = normalize(light - ro);
  vec3 R = reflect(-L, N);
  diff = max(vec3(0.0), diff + lColor * dot(L, N));
  spec = max(vec3(0.0), spec + lColor * pow(dot(V, R), shininessAt(ro)));

  float td = marchRay(light, -L);
  if (length((td * -L + light) - ro) > 0.05) {
  shadow *= 0.5;
}
}


  return (colorAt(ro) * 0.25 +
  colorAt(ro) * diff +
  colorAt(ro) * spec) * shadow *
  occlusion(ro, N, 0.7);
}

  void setupCamera(in vec2 uv, out vec3 ro, out vec3 rd) {
  ro = cameraPos();
  vec3 t = vec3(0.0);
  vec3 up = vec3(0.0, 1.0, 0.0);

  vec3 cdir = normalize(t - ro);
  vec3 cright = normalize(cross(cdir, up));
  vec3 cup = cross(cright, cdir);

  rd = normalize(cdir + uv.x * cright + uv.y * cup);
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
  fragColor = vec4(0.0);
}
}
