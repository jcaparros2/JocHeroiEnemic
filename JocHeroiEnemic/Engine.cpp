#include "Engine.h"
Engine::Engine() {
	// Get the screen resolution and create an SFML window and View
	//mides de la finestra
	viewSize.x = 1024;
	viewSize.y = 768;
	//construïm la finestra de joc.
	window.create(sf::VideoMode(viewSize.x, viewSize.y),
		"Joc heroi - enemic",
		sf::Style::Default);
}

Engine::~Engine() {
}
int Engine::run() {
	sf::Clock clock;
	init();
	while (window.isOpen()) {
		updateInput();
		sf::Time dt = clock.restart();
		if (!gameover) {
			update(dt.asSeconds());
		}
		draw();
	}
	return 0;
}
void Engine::init() {
	//carregam les imatges del fons
	skyTexture.loadFromFile("Assets/graphics/sky.png");
	skySprite.setTexture(skyTexture);
	bgTexture.loadFromFile("Assets/graphics/bg.png");
	bgSprite.setTexture(bgTexture);
	bgSprite.setScale(4, 5);
	
	
	// Load font
	headingFont.loadFromFile("Assets/fonts/SnackerComic.ttf");
	scoreFont.loadFromFile("Assets/fonts/arial.ttf");
	// Set Heading Text
	headingText.setFont(headingFont);
	headingText.setString("Tiny Bazooka");
	headingText.setCharacterSize(84);
	headingText.setFillColor(sf::Color::Red);
	sf::FloatRect headingbounds = headingText.getLocalBounds();
	headingText.setOrigin(headingbounds.width / 2, headingbounds.height / 2);
	headingText.setPosition(sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.10f));
	// Set Score Text
	 // Tutorial Text
	tutorialText.setFont(scoreFont);
	tutorialText.setString("Press Down Arrow to Fire and Start Game, Up Arrow to Jump");
	tutorialText.setCharacterSize(35);
	tutorialText.setFillColor(sf::Color::Red);
	sf::FloatRect tutorialbounds = tutorialText.getLocalBounds();
	tutorialText.setOrigin(tutorialbounds.width / 2, tutorialbounds.height / 2);
	tutorialText.setPosition(sf::Vector2f(viewSize.x * 0.5f, viewSize.y * 0.20f));
	// Audio
	bgMusic.openFromFile("Assets/audio/bgMusic.ogg");
	bgMusic.play();
	//inicialitzem el jugador
	
	hero.init("Assets/graphics/hero.png", sf::Vector2f(viewSize.x * 0.25f, viewSize.y * 0.5f), 200);
	//inicialitzem els nombre aleatoris.

	srand((int)time(0));

}
void Engine::updateInput() {
	sf::Event event;
	// while there are pending events...
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::KeyPressed) { //revisem les tecles premudes
			if (event.key.code == sf::Keyboard::Up) {//personatge sala
				hero.jump(750.0f);

			}
			if (event.key.code == sf::Keyboard::Down) {
				if (gameover) {//si no joguem llavors engeguem el joc.
					gameover = false;
					reset();

				}

			}
			if (event.key.code == sf::Keyboard::Right) {
				spawnBullet();
			}
		}
		//condicions per tancar el programa
		if (event.key.code == sf::Keyboard::Escape || event.type == sf::Event::Closed)
			window.close();
	
		
	}


}
void Engine::update(float dt) {
	//actualitzem la posició del personatge
	hero.update(dt);
	//modifiquem el temps per sabre el que ha passat
	currentTime += dt;
	// Spawn Enemies
	if (currentTime >= prevTime + 1.125f) {
		spawnEnemy();
		prevTime = currentTime;
	}
	
	// Update Enemies
	for (auto i : enemies) {
 		i->update(dt);
	}
	
	// Update rockets
	for (auto i : bullets) {
		i->update(dt);
	}
	// Check collision between Rocket and Enemies
	for (int i = 0; i < enemies.size(); i++) {
		for (int j = 0; j < bullets.size(); j++) {
			if (checkCollision(enemies[i]->getSprite(), bullets[j]->getSprite())) {
				delete(enemies[i]);
				enemies.erase(enemies.begin() + i);
				delete(bullets[j]);
				bullets.erase(bullets.begin() + j);
			}
		}
	}
}

void Engine::draw() {
	// netejem la pantalla
	window.clear(sf::Color::Red);
	//dibuixem elements
	window.draw(skySprite);
	window.draw(bgSprite);
	
	window.draw(hero.getSprite());
	for (Enemy* e : enemies)
	{
		window.draw(e->getSprite());
	}
	for (Bullet* e : bullets)
	{
		
		window.draw(e->getSprite());
	
	}
	//mostrem text depenent si estem jugant o no
	if (gameover) {
		window.draw(headingText);
		window.draw(tutorialText);
	}
	else {
		window.draw(scoreText);
	}
	//enviem a la pantalla.
	window.display();
}

void Engine::reset() {
	 score = 0;
	 currentTime = 0.0f;
	 prevTime = 0.0;
	 scoreText.setString("Score: 0");
	 for(Enemy* enemy:enemies) {
		 delete(enemy);
	 }
	 for (Bullet* bullet:bullets) {
		 delete(bullet);
	 }
	 enemies.clear();
	 bullets.clear();
}

void Engine::spawnEnemy() {
	int randLoc = rand() % 3;
	sf::Vector2f enemyPos;
	float speed;
	switch (randLoc) {
	case 0: enemyPos = sf::Vector2f(viewSize.x, viewSize.y * 0.75f); speed = 400;
		break;
	case 1: enemyPos = sf::Vector2f(viewSize.x, viewSize.y * 0.60f); speed = 550;
		break;
	case 2: enemyPos = sf::Vector2f(viewSize.x, viewSize.y * 0.40f); speed = 650;
		break;
	default: printf("incorrect y value \n"); break;
	}
	Enemy* enemy = new Enemy();
	enemy->init("Assets/graphics/enemy.png", enemyPos, speed);
	enemies.push_back(enemy);
}
void Engine::spawnBullet() {
	sf::Vector2f bulletPos;
	bulletPos = hero.getSprite().getPosition();
	float speed = -700.0;
	Bullet* bullet = new Bullet();
	bullet->init("Assets/graphics/bullets.png", bulletPos, speed);
	bullets.push_back(bullet);
	
}
bool Engine::checkCollision(sf::Sprite element1, sf::Sprite element2) {
	sf::FloatRect shape1 = element1.getGlobalBounds();
	sf::FloatRect shape2 = element2.getGlobalBounds();
	if (shape1.intersects(shape2)) {
		return true;

	}
	else
	{
		return false;
	}
}