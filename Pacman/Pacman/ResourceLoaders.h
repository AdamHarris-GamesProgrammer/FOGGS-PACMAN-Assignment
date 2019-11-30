#pragma once
#ifndef _RESOURCELOADERS_H
#define _RESOURCELOADERS_H

#include "S2D/S2D.h"
#include <iostream>

class ResourceLoaders {
public:
	S2D::Texture2D* LoadTexture(const char* filePath, bool buildMinMaps = false)
	{
		S2D::Texture2D* texture = new S2D::Texture2D();
		texture->Load(filePath, buildMinMaps);

		if (texture == nullptr) {
			std::cout << "Error loading texture at file path: " << filePath << ". " << std::endl;
			return nullptr;
		}
		else {
			return texture;
		}
		
	}

	S2D::SoundEffect* LoadSound(const char* filePath, bool looping = false)
	{
		S2D::SoundEffect* sound = new S2D::SoundEffect();
		sound->Load(filePath);
		sound->SetLooping(looping);

		return sound;
	}
};
#endif
