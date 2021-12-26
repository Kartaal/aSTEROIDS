#pragma once

#include "Component.hpp"
#include "sre/Sprite.hpp"

/*
* Component used to represent a game object's sprite.
*/

class SpriteComponent : public Component {
public:
	explicit SpriteComponent(GameObject* gameObject);

	void renderSprite(sre::SpriteBatch::SpriteBatchBuilder& spriteBatchBuilder) override;

	void setSprite(const sre::Sprite& sprite);
	sre::Sprite getSprite();
private:
	sre::Sprite sprite;
};

