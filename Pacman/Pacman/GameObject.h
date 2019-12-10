#pragma once
#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include "S2D/S2D.h"
#include "ResourceLoaders.h"
#include "CommonOperations.h"
#include "Constants.h"

class GameObject { //base class for all game objects within the game
public:
	GameObject(S2D::Texture2D* texture, S2D::Vector2* position, S2D::Rect* srcRect); //takes a texture, position and source rect
	~GameObject(); //destructor

	void Render(); //renders the game object
	virtual void Update(int elapsedTime, int frameCount); //updates the games object

	S2D::Vector2* GetPosition() { return mPosition; } //returns the position
	void SetPosition(S2D::Vector2* newPosition) { mPosition = newPosition; } //sets the position

	S2D::Texture2D* GetTexture() { return mTexture; } //gets the texture
	S2D::Rect* GetSourceRect() { return mSrcRect; } //gets the source rect



protected:
	//only derived classes can access these variables
	S2D::Texture2D* mTexture; 
	S2D::Vector2* mPosition;
	S2D::Rect* mSrcRect;

	//used to load resources and generate positions
	ResourceLoaders rl;
	CommonOperations co;
};

#endif
