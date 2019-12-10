#include "GameObject.h"

GameObject::GameObject(S2D::Texture2D* texture, S2D::Vector2* position, S2D::Rect* srcRect)
{
	mTexture = texture; //sets the texture
	mPosition = position; //sets the position
	mSrcRect = srcRect; //sets the src rect

}


GameObject::~GameObject()
{
	//deletes all class data
	delete mTexture;
	delete mPosition;
	delete mSrcRect;
	delete this;
}

void GameObject::Render()
{
	if(this != nullptr) //if the object exists
		S2D::SpriteBatch::Draw(mTexture, mPosition, mSrcRect); //render
}

void GameObject::Update(int elapsedTime, int frameCount)
{
	//empty class as most object will override it 
}

