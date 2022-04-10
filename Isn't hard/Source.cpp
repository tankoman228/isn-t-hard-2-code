#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <windows.h>
#include <SFML/Audio.hpp>
#include <fstream>

#include "Values.h"
#include "Classes.h"
#include "Voids.h"
#include "Game.h"
#include "Editor.h"
#include "Levels.h"

using namespace std;
using namespace sf;

int main() {

	HWND Stealth;
	AllocConsole();
	Stealth = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(Stealth, 0);

	logg.open("log.txt");

	do {

	ifstream screensize("Screen.txt");
	screensize >> helper;
	screenw = helper;
	screensize >> helper;
	screensize >> i;
	screensize >> UI_scale;  UI_scale /= 10;
	if (i == 0) { helper -= 80; }
	screenh = helper;
	screensize.close();

	window.close();
	window.create(sf::VideoMode(screenw, screenh), "Isn't hard 2");
	
	/*RenderWindow window(VideoMode(screenw, screenh), "Isn't hard 2");*/

	if (i == 1) {
		window.close();
		window.create(VideoMode(screenw, screenh), "Isn't hard 2", Style::Fullscreen);
		screenmode.is_checked = true;
	}
	window.setVerticalSyncEnabled(true);
	window.setMouseCursorVisible(false);

	loading(); logg << "Loading was succesfully finished \n";

	while (window.isOpen())
	{
		cursor.setScale(0.2, 0.2);

		if (mode == "game") {
			game_cycle();
		}

		if (mode == "editor") {
			editor_cycle();
		}

		if (mode == "menu") { 

			if (tick % 10 == 1) {
				if (!music.getStatus()) {
					music.openFromFile("Sounds\\menu.ogg");
					music.play();
				}
			}

			block.setTexture(bg); block.setOrigin(128, 128); block.setTextureRect(IntRect(0, 0, 256, 256)); block.setScale(1, 1);
			for (i = 0; i < 8; i++) {
				for (j = 0; j < 6; j++) {
					switch((i + j) % 4) {
					case 0: block.setRotation(0); break;
					case 1: block.setRotation(90); break;
					case 2: block.setRotation(180); break;
					case 3: block.setRotation(270); break;
					}
					block.setPosition(i * 256 + 128, j * 256 + 128);
					window.draw(block);
				}
			}

			text.setPosition(screenw / 2 - (352 * UI_scale), screenh / 20);
			text.setCharacterSize(150 * UI_scale);
			text.setFillColor(Color::Color(0, 0, 0, 255));
			text.setString("Isn't hard 2");

			window.draw(text);

			text.setPosition(screenw/2 - (350 * UI_scale), screenh / 20 + 2);
			text.setCharacterSize(150 * UI_scale);
			text.setFillColor(Color::Color(200,200,255,255));
			text.setString("Isn't hard 2");
			
			window.draw(text); context_menu = false;

			b_levels.cycle();
			b_ach.cycle();
			b_exit.cycle();
			b_options.cycle();
			b_play.cycle();
			b_editor.cycle();

			if (click_button_menu) {
				logg << "click_button_menu \n";
				switch (int(helper_s2)) {
				case 0: mode = "levels"; level_scroll = (levels_opened * 300 - 800) * -1; break;
				case 1: mode = "game"; block.setScale(scale, scale); block.setOrigin(64, 64); music.stop(); if (levels_opened < 35) { build(levels_opened); } else { build(rand()%36); } break;
				case 2: mode = "editor"; music.stop(); break;
				case 3: /*mode = "editor"; music.stop();*/ break;
				case 4: mode = "options"; break;
				case 5: save_options(); close = true; window.close();  return 0;
				}
				click_button_menu = false;
			}

			////cursor.setTextureRect(IntRect(0, 0, 1, 1));
			////cursor.setScale(1, 1);

			//for (i = 0; i < 6; i++) {
			//	bigbut.setPosition((screenw / 2), (screenh / 6 + (screenh / 7.5 * i )) + 75);
			//	bigbut.setScale(1.2 * UI_scale, 0.9 * UI_scale); 
			//	block.setOrigin(64, 64); block.setScale(scale, scale);
			//	if (Mouse::isButtonPressed(Mouse::Left)) {
			//			if (bigbut.getGlobalBounds().intersects(cursor.getGlobalBounds())) {
			//				if (!onclick) {
			//					onclick = true;
			//					switch (i) {
			//					case 0: mode = "game"; music.stop(); lvlnum = -1; build(lvlnum); break;
			//					case 1: mode = "game"; music.stop(); build(lvlnum); break;
			//					case 2: mode = "levels"; level_scroll = (levels_opened * 300 - 800) * -1; break;
			//					case 3: mode = "editor"; music.stop(); break;
			//					case 4: mode = "options"; break;
			//					case 5: save_options(); window.close(); return 0;
			//					}
			//				}
			//			}
			//	}
			//	else { onclick = false; }

			//	if (bigbut.getGlobalBounds().intersects(cursor.getGlobalBounds())) { bigbut.setTextureRect(IntRect(0, 128, 512, 128)); }
			//	else { bigbut.setTextureRect(IntRect(0, 0, 512, 128)); }

			//	window.draw(bigbut);

			//	text.setPosition(bigbut.getPosition().x - (UI_scale * 265), bigbut.getPosition().y - (UI_scale * 30));
			//	/*text.setPosition((screenw / 2 - 275) + (1/UI_scale) * 120 + (70 * UI_scale), (screenh/6 + (screenh / 7.5 * i)) * UI_scale + (40 * UI_scale));*/
			//	text.setCharacterSize(50 * UI_scale);
			//	text.setFillColor(Color::Black);

			//	switch (i) {
			//	case 0: text.setString("Tutorial"); break;
			//	case 1: text.setString("Play"); break;
			//	case 2: text.setString("Levels"); break;
			//	case 3: text.setString("Editor"); break;
			//	case 4: text.setString("Options"); break;
			//	case 5: text.setString("Exit"); break;
			//	}

			//	window.draw(text);

			//}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			if (!onclick) {
				/*mode = "menu"; */
				onclick = true;
				if ((lvlnum == -1)&&(!context_menu)) { lvlnum = 1; }
			}
			if ((mode == "levels") || (mode == "options") || (mode == "editor")) { mode = "menu"; }
			else { context_menu = true; onclick = false; }

		}

		if (mode == "levels") {
			levels_cycle();
		}

		if (mode == "options") {

			if (tick % 10 == 1) {
				if (!music.getStatus()) {
					music.openFromFile("Sounds\\menu.ogg");
					music.play();
				}
			}

			block.setColor(Color::Color(100, 100, 100, 255));
			block.setTexture(bg); block.setOrigin(128, 128); block.setTextureRect(IntRect(0, 0, 256, 256)); block.setScale(1, 1);
			for (i = 0; i < 8; i++) {
				for (j = 0; j < 6; j++) {
					switch ((i + j) % 4) {
					case 0: block.setRotation(0); break;
					case 1: block.setRotation(90); break;
					case 2: block.setRotation(180); break;
					case 3: block.setRotation(270); break;
					}
					block.setPosition(i * 256 + 128, j * 256 + 128);
					window.draw(block);
				}
			}
			block.setColor(Color::Color(255, 255, 255, 255));

			particles_option.cycle();

			text.setFillColor(Color::Color(0, 0, 200, 255));
			text.setCharacterSize(50);
			text.setPosition(450, 420);
			text.setString("Particles density");
			window.draw(text);
			text.move(2, 2);
			text.setFillColor(Color::Color(0, 0, 0, 255));
			window.draw(text);

			reloader.cycle();
			auto_reloader.cycle();

			if (reloader.is_clicked) {
				for (i = 0; i < 2; i++) {
					if (res_but[i].is_picked) {
						helper_s[0] = res_but[i].W; helper_s[1] = res_but[i].H;
						switch (i) {
						case 0: UI_scale = 1; break;
						case 1: UI_scale = 0.5; break;
						}
					}
					ofstream scr;
					scr.open("Screen.txt");
					scr << helper_s[0] << " " << helper_s[1] << " ";
					if (screenmode.is_checked) {
						scr << 1 << " ";
					}
					else { scr << 0 << " "; }
					scr << UI_scale * 10 << " ";
					scr.close();
				}
				break;
			}

			if (auto_reloader.is_clicked) {
				ofstream scr;
				scr.open("Screen.txt");
				UI_scale = helper_s[0] / 1920;
				scr << helper_s[0] << " " << helper_s[1] << " ";
				scr << 1 << " ";
				scr << UI_scale * 10 << " ";
				scr.close();
				break;
			}

			screenmode.cycle();

			for (i = 0; i < 2; i++) {
				res_but[i].cycle();
				if (res_but[i].click) {
					for (j = 0; j < 2; j++) {
						if (i == j) {
							res_but[j].is_picked = true;
						}
						else {
							res_but[j].is_picked = false;
						}
					}
				}
			}

			if (Mouse::isButtonPressed(Mouse::Left)) {
				onclick = true;
			}
			else { onclick = false; }

		}
		
		if (context_menu) {
			if (mode == "game") {
				window.draw(box);
				text.setFillColor(Color::Black); text.setCharacterSize(UI_scale * 50);
				for (i = 0; i < 6; i++) {
					box_but.setPosition(screenw / 2, 150 * UI_scale + 130 * i * UI_scale);
					if (cursor.getGlobalBounds().intersects(box_but.getGlobalBounds())) {
						box_but.setTextureRect(IntRect(0, 128, 512, 128));
						if (Mouse::isButtonPressed(Mouse::Left)) {
							onclick = true;
	/*						music.stop();*/
							switch (i) {
							case 0: context_menu = false; break;
							case 1: checkpoint(); context_menu = false; break;
							case 2: context_menu = false; load_checkpoint(); break;
							case 3: build(lvlnum); context_menu = false; break;
							case 4: checkpoint(); context_menu = false; mode = "menu"; break;
							case 5: context_menu = false; mode = "menu"; break;
							}
						}
					}
					else { box_but.setTextureRect(IntRect(0, 0, 512, 128)); }
					window.draw(box_but);
					text.setPosition(box_but.getPosition());
					text.move(UI_scale * -200, UI_scale * -35);
					switch (i) {
					case 0: text.setString("Resume"); break;
					case 1: text.setString("Save (F5)"); break;
					case 2: text.setString("Back to save"); break;
					case 3: text.setString("Try again (TAB)"); break;
					case 4: text.setString("Save and exit"); break;
					case 5: text.setString("Exit (no saving)"); break;
					}
					window.draw(text);
				}
			}
			if (mode == "editor") {
				ifstream saves;
				saves.open("saves.txt");
				saves >> j;
				window.draw(box);
				text.setFillColor(Color::Black); text.setCharacterSize(UI_scale * 50);

				for (i = 0; i < 4; i++) {
					box_but.setPosition(screenw / 2, 150 * UI_scale + 130 * i * UI_scale);
					text.setPosition(box_but.getPosition());
					text.move(UI_scale * -200, UI_scale * -35);
					saves >> helper_string; text.setString(helper_string);
					
					if (cursor.getGlobalBounds().intersects(box_but.getGlobalBounds())) {
						box_but.setTextureRect(IntRect(0, 128, 512, 128));
						if (Mouse::isButtonPressed(Mouse::Left)) {
							if (!saveornot) {
								lvl_name = helper_string;
								onclick = true;
								build(-2);
								context_menu = false;
							}
							else {
								lvl_name = helper_string;
								save(helper_string);
								build(-2);
								lvlnum = -2;
								context_menu = false;
								onclick = true;
							}
						}
					}
					else { box_but.setTextureRect(IntRect(0, 0, 512, 128)); }

					window.draw(box_but);
					window.draw(text);
					helper_string = lvl_name;
				}

				i++;

				box_but.setPosition(screenw / 2, 150 * UI_scale + 130 * i * UI_scale);
				text.setPosition(box_but.getPosition());
				text.move(UI_scale * -200, UI_scale * -35);
				text.setString("Cancel");

				if (cursor.getGlobalBounds().intersects(box_but.getGlobalBounds())) {
					box_but.setTextureRect(IntRect(0, 128, 512, 128));
					if (Mouse::isButtonPressed(Mouse::Left)) {
						context_menu = false; helper_string = lvl_name;
					}
				}
				else { box_but.setTextureRect(IntRect(0, 0, 512, 128)); }

				window.draw(box_but);
				window.draw(text);

				saves.close();
			}
		}

		//CuRsOr!!!
		if (true) {
			cursor.setPosition(Mouse::getPosition().x - window.getPosition().x, (Mouse::getPosition().y) - window.getPosition().y);
			cursor.setScale(1.5, 1.5);
			window.draw(cursor);
		}

		if (true) {
			if (Keyboard::isKeyPressed(Keyboard::Num1)) {
				UI_scale = 0.5;
				/*robot.next_path();
				system("pause");*/
			}
			if (Keyboard::isKeyPressed(Keyboard::Num2)) {
				UI_scale = 1;
			}
		}

		sf::Event event;
		while (window.pollEvent(event))
		{
			// "запрос закрытия" событие: мы закрываем окно
			if (event.type == sf::Event::Closed) {
				save_options();
				close = true; window.close(); return 1;
			}

		}

		tick++;
		window.display();
		Sleep(1);
		window.clear();
	}
	if (close) {
		window.close();
		return 1;
	}
	window.close();
	} while (true);

	return 3;

}
