#version 330 core
#define numCircles 10
#define numColors 5

out vec4 FragColor;

in vec4 vertexColor;
in vec4 gl_FragCoord;

uniform vec3[numCircles] pointArray;

uniform float size;
uniform vec3[numColors] colors;

void drawCircles() {
	for (int i = 0; i < numCircles; i++) {
		float dist = distance(gl_FragCoord.xy, pointArray[i].xy);
		if (dist < pointArray[i].z) {
			FragColor = vec4(1.0, 1.0, 1.0, 1.0);
			return;
		}
	}
	FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}

float calculateCharge(bool useRadius) {
	float charge = 0;
	for (int i = 0; i < numCircles; i++) {
		float dist = distance(gl_FragCoord.xy, pointArray[i].xy);
		float curr = (int(useRadius) * pointArray[i].z + int(!useRadius))  / dist;
		charge += curr;
	}
	return charge;
}

vec3 splitGroups(float charge) {
	charge = min(1, charge);
	
	float cMinPrev = 0;
	for (int i = 0; i < numColors; i++) {
		float cMin = 1 - size * (numColors - i - 1);
		if (charge > cMin) {
			continue;
		}
		float scale = (cMin - charge) / size;//(cMin - cMinPrev);
		cMinPrev = cMin;
		return mix(colors[i], colors[i + 1], 1.0 - scale);
	}
	return vec3(0.0);
}

vec3 splitGroups2(float charge) {
	if (charge > 1) {
		return colors[numColors - 1];
	}
	charge = mod(charge, size * numColors);
	int color = int(charge / size);
	
	
	return (1 - charge) * colors[color];
}

void main() {
	float charge = calculateCharge(true);
	FragColor = mix(vertexColor, vec4(splitGroups2(charge), 1.0), 0.5);
}