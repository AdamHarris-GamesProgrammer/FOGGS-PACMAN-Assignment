#pragma once
#include "S2D/S2D.h"
#include "S2D/Texture2D.h"


class Texture {
public:
	Texture(const char* filePath, bool buildMinMaps = false);


public:
	S2D::Texture2D* LoadTexture(const char* filePath, bool buildMinMaps = false);
};