#pragma once

struct Static_translate {
	float x; float y;
};

struct Translation {
	float x = 0.f;
	float y = 0.f;
	float dx = 0.f;
	float dy = 0.f;
	float dmax = 0.f;
	float inc = 0.f;
	float dragx = 0.f;
	float dragy = 0.f;
	float xold = 0.f;
	float yold = 0.f;
	bool changed = false;
};