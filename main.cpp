#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include <string>
#include <fstream>
using namespace sf;

int main() {

	srand(time(0));

	std::ifstream fin;
	fin.open("highscore.txt");
	int highScore = 0;
	fin >> highScore;
	fin.close();

	std::ofstream fout;
	fout.open("highscore.txt", std::ios::trunc);

	RenderWindow app(VideoMode(400, 533), "Ninja Kara");
	app.setFramerateLimit(60);

	Texture t1, t2, t3;
	t1.loadFromFile("assets/background.png");
	t2.loadFromFile("assets/platform.png");
	t3.loadFromFile("assets/kara.png");

	Sprite sBackground(t1), sPlat(t2), sPers(t3);

	Text text, textScore, liveScore, maxScore;
	Font font;

	if (!font.loadFromFile("assets/font.otf")) exit(0);
	text.setCharacterSize(60);
	text.setFillColor(sf::Color::Red);
	text.setFont(font);
	text.move(35, 100);
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(3);

	textScore.setCharacterSize(60);
	textScore.setFillColor(sf::Color::Red);
	textScore.setFont(font);
	textScore.move(180, 230);
	textScore.setOutlineColor(sf::Color::Black);
	textScore.setOutlineThickness(3);

	liveScore.setCharacterSize(20);
	liveScore.setFillColor(sf::Color::Red);
	liveScore.setFont(font);
	liveScore.move(10, 10);
	liveScore.setOutlineColor(sf::Color::Black);
	liveScore.setOutlineThickness(3);

	maxScore.setCharacterSize(20);
	maxScore.setFillColor(sf::Color::Red);
	maxScore.setFont(font);
	maxScore.move(360, 10);
	maxScore.setOutlineColor(sf::Color::Black);
	maxScore.setOutlineThickness(3);
	maxScore.setString(std::to_string(highScore));

	Vector2i plat[20];

	// schDir == 0 => nu trebuie schimbat nimic, 1 == spre dreapta, -1 == spre stanga
	// lr == LeftRight == directia spre care se uita Kara
	int schDir = 1;
	int lr = -1;

	// la inceput, se genereaza 5 platforme
	for (int i = 0; i < 5; i++) {
		plat[i].x = rand() % 330 + 10;
		plat[i].y = 100 * i + rand() % 53;
	}

	plat[4].x = 160;

	int x = 160, y = 100, h = 200;
	float dx = 0, dy = 0;

	int score = 0;
	int gameOver = 0;

	while (app.isOpen()) {

		Event e;
		while (app.pollEvent(e)) {

			if (e.type == Event::Closed) {
				app.close();
			}
		}

		if (gameOver == 0) {
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				x += 3;
				if (lr == -1) {
					lr = 1;
					schDir = 1;
				}
				else {
					schDir = 0;
				}
			}
			else {
				if (Keyboard::isKeyPressed(Keyboard::Left)) {
					if (lr == 1) {
						lr = -1;
						schDir = -1;
					}
					else {
						schDir = 0;
					}
					x -= 3;
				}
			}
		}
		dy += 0.2; // viteza cu care urca
		y += dy;

		// GameOver
		if (y > 500) {

			dy = -10;

			if (score >= highScore) {
				text.setString(" GAME OVER\nNew highscore:");
				highScore = score;
				if (fout.is_open()) {
					fout << highScore;
					fout.close();
				}
			}
			else {
				text.setString(" GAME OVER\n   Final score:");
				if (fout.is_open()) {
					fout << highScore;
					fout.close();
				}
			}
			textScore.setString(std::to_string(score));
			gameOver = 1;
			// play a song
		}

		for (int i = 0; i < 5; i++) {
			plat[i].y += 1;
		}

		if (y < h) {

			for (int i = 0; i < 5; i++) {

				y = h;
				plat[i].y = plat[i].y - dy;

				if (plat[i].y > 534) {
					plat[i].y = 0; plat[i].x = rand() % 330 + 10;
					if (gameOver == 0) {
						score++;
					}
				}
			}
		}

		if (gameOver == 0) {
			for (int i = 0; i < 5; i++) {
				if ((x + 50 > plat[i].x) && (x + 20 < plat[i].x + 68)
					&& (y + 70 > plat[i].y) && (y + 70 < plat[i].y + 14) && (dy > 0))
				{
					dy = -10;
					// play meow jump sound
				}
			}
		}

		liveScore.setString(std::to_string(score));
		// sPers.getTexture()->getSize().x * sPers.getScale().x == 80 pt dreapta, -80 pt stanga
		float newX = x - ((lr - 1) / 2) * 80;
		sPers.setPosition(newX, y);

		if (schDir == 1) {
			sPers.scale(-1.f, 1.f);
			schDir = 0;
		}
		else if (schDir == -1) {
			sPers.scale(-1.f, 1.f);
			schDir = 0;
		}

		app.draw(sBackground);

		for (int i = 0; i < 5; i++) {
			sPlat.setPosition(plat[i].x, plat[i].y);
			app.draw(sPlat);
		}

		if (gameOver == 0) {
			app.draw(sPers);
			app.draw(liveScore);
			app.draw(maxScore);
		}
		else {
			app.draw(text);
			app.draw(textScore);
		}

		app.display();
	}

	return 0;
}
