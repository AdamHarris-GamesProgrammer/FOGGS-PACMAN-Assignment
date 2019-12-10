#include "GameObject.h"
#include "ResourceLoaders.h"

GameObject::GameObject(S2D::Texture2D* texture, S2D::Vector2* position)
{
	mTexture = texture;
	mPosition = position;
	mSrcRect = new S2D::Rect(0.0f, 0.0f, mTexture->GetWidth(), mTexture->GetHeight());

}

GameObject::GameObject(S2D::Texture2D* texture, S2D::Vector2* position, S2D::Rect* srcRect)
{
	mTexture = texture;
	mPosition = position;
	mSrcRect = srcRect;

}


GameObject::~GameObject()
{
	delete mTexture;
	delete mPosition;
	delete mSrcRect;
	delete this;
}

void GameObject::Render()
{
	if(this != nullptr)
		S2D::SpriteBatch::Draw(mTexture, mPosition, mSrcRect);
}

void GameObject::Update(int elapsedTime, int frameCount)
{

}

