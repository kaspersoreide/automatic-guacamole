#version 430

in vec2 uv;

out vec4 FragColor;

uniform vec3 viewDir;
uniform vec3 up;
uniform vec3 right;
uniform vec3 sunDir;
uniform vec2 screenScale;

vec3 convSpherical(vec2 a) {
    return vec3(
		cos(a.y) * sin(a.x), 
		sin(a.y),
		cos(a.y) * cos(a.x)
	);
}

void main() {
    vec3 c0 = vec3(0.0, 0.0, 0.1);
    vec3 c1 = vec3(0.2, 0.0, 0.9);
    vec2 suv = uv * screenScale;
    vec3 sight = normalize(suv.x * right + suv.y * up + viewDir);
    //vec3 sun = vec3(0.0);
    float a = dot(sunDir, sight);
    vec3 sun = vec3(clamp(0.3 * a, 0.0, 1.0));

    //if (distance(sun, sight) < 0.5) sun = vec3(1.0);
    //FragColor = vec4(sun + mix(c1, c0, 1.5 * sight.y), 1.0);
    vec3 fogColor = vec3(0.2, 0.0, 0.0);
    FragColor = vec4(fogColor, 1.0);
}