#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <windows.h>
#include <SFML/Audio.hpp>
#include <fstream>
#include "Values.h"
#include "Classes.h"
#include "Voids.h"

using namespace std; using namespace sf;

void levels_cycle() {

	logg << "Level cycle begin \n";
	onclick = false;

	do {

		if (tick % 10 == 1) {
			if (!music.getStatus()) {
				music.openFromFile("Sounds\\menu.ogg");
				music.play();
			}
		}

		block.setTexture(el); block.setScale(2, 2);
		for (i = 0; i < 10; i++) {
			for (j = 0; j < 6; j++) {
				block.setPosition(i * 256 + level_scroll % 256 - 128, j * 256);
				window.draw(block);
			}
		}

		for (i = 0; i < 35; i++) {
			text.setCharacterSize(UI_scale * 100);
			lvl_but.setScale(UI_scale * 2, UI_scale * 2);
			lvl_but.setPosition(level_scroll + (300 * (i + 1)) * UI_scale, screenh / 2);
			if (lvl_but.getGlobalBounds().intersects(cursor.getGlobalBounds())) {
				lvl_but.scale(1.08, 1.08); text.setCharacterSize(UI_scale * 108);
			}
			if (i + 1 <= levels_opened) {
				lvl_but.setTextureRect(IntRect(0, 0, 128, 128));
				if (lvl_but.getGlobalBounds().intersects(cursor.getGlobalBounds())) {
					if (Mouse::isButtonPressed(Mouse::Left)) {
						if (!onclick) {
							lvlnum = i + 1;
							block.setOrigin(64, 64);
							build(lvlnum);
							mode = "game";
							block.setScale(scale, scale);
						}
					}
				}
			}
			else { lvl_but.setTextureRect(IntRect(128, 0, 128, 128)); }
			text.setFillColor(Color::Color(50, 50, 20, 255));
			text.setPosition(lvl_but.getPosition().x - UI_scale * 72, lvl_but.getPosition().y - UI_scale * 70);
			if (i < 9) { text.move(UI_scale * 42, 0); }
			helper_string = to_string(i + 1);
			text.setString(helper_string);
			window.draw(lvl_but);
			window.draw(text);
		}

		if (cursor.getPosition().x > screenw * 0.9) {
			level_scroll-=20;
		}
		if (cursor.getPosition().x < screenw * 0.1) {
			level_scroll+=20;
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			mode = "menu";
		}

		cursor.setPosition(Mouse::getPosition().x - window.getPosition().x, (Mouse::getPosition().y) - window.getPosition().y);
		cursor.setScale(1.5, 1.5);
		window.draw(cursor);

		sf::Event event;
		while (window.pollEvent(event))
		{
			// "запрос закрытия" событие: мы закрываем окно
			if (event.type == sf::Event::Closed) {
				save_options();
				close = true; window.close();
			}

		}

		tick++;
		window.display();
		Sleep(1);
		window.clear();

	} while (mode == "levels");

}