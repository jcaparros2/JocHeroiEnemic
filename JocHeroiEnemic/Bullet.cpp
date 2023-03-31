#include "Bullet.h"

using namespace sf;
Bullet::Bullet() {
}

void Bullet::init(std::string textureName, sf::Vector2f position, float velocitat) {
	m_position = position;
	m_speed = velocitat;
	// Load a Texture
	m_texture.loadFromFile(textureName.c_str());
	// Create Sprite and Attach a Texture
	m_sprite.setTexture(m_texture);
	m_sprite.setPosition(m_position);
	m_sprite.setOrigin(m_texture.getSize().x / 2,
		m_texture.getSize().y / 2);
	m_sprite.setScale(0.15, 0.15);
}
void Bullet::update(float dt) {

	m_position.x -= m_speed * dt;
	m_sprite.setPosition(m_position);
	

}

sf::Sprite Bullet::getSprite() {
	return m_sprite;
}
Bullet::~Bullet() {
}