#pragma once
#include <SFML/Graphics.hpp>
class Bullet
{
public:
	Bullet();
	~Bullet();
	void init(std::string textureName, sf::Vector2f position, float speed);
	void update(float dt);
	sf::Sprite getSprite();
private:
	sf::Texture m_texture;
	sf::Sprite m_sprite;
	sf::Vector2f m_position;
	float m_speed;
};

