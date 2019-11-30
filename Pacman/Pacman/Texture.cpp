#include "Texture.h"

Texture::Texture(const char* filePath, bool buildMinMaps /* = false */) {
	LoadTexture(filePath, buildMinMaps);
}

S2D::Texture2D* Texture::LoadTexture(const char* filePath, bool buildMinMaps /*= false*/)
{
	S2D::Texture2D* texture = new S2D::Texture2D();
	texture->Load(filePath, buildMinMaps);

	return texture;
}
