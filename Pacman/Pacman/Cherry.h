#pragma once
#ifndef _CHERRY_H
#define _CHERRY_H

#include "GameObject.h"

class Cherry : public GameObject {
public:
	Cherry(S2D::Texture2D* texture, S2D::Vector2* position, S2D::Rect* srcRect) : GameObject(texture, position, srcRect) {}

};

#endif
