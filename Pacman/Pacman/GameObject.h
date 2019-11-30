#pragma once
#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include "S2D/S2D.h"
#include "ResourceLoaders.h"

class GameObject {
public:
	GameObject(S2D::Texture2D* texture, S2D::Vector2* position);
	GameObject(S2D::Texture2D* texture, S2D::Vector2* position, S2D::Rect* srcRect);
	~GameObject();

	void Render();
	virtual void Update(int elapsedTime, int frameCount);

	S2D::Vector2* GetPosition() { return mPosition; }
	void SetPosition(S2D::Vector2* newPosition) { mPosition = newPosition; }

	S2D::Texture2D* GetTexture() { return mTexture; }
	S2D::Rect* GetSourceRect() { return mSrcRect; }



protected:
	S2D::Texture2D* mTexture;
	S2D::Vector2* mPosition;
	S2D::Rect* mSrcRect;

	ResourceLoaders rl;
};

#endif
