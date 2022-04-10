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

void editor_cycle() {

	helper = 1; text.setCharacterSize(50 * scale);
	
	logg << "the editor has been opened \n";

	do {

		if (Keyboard::isKeyPressed(Keyboard::Delete)) {
				clear_lvl();
			}

		if (!music.getStatus()) {
				reset_music();
			}

		if ((Keyboard::isKeyPressed(Keyboard::Tab))) {
				for (i = 0; i < wid; i++) {
					for (j = 0; j < hei; j++) {
						for (helper = 0; helper < 2; helper++) {
							for (int oo = 0; oo < 3; oo++) {
								level[i][j][helper][oo] = 0;
							}
						}
					}
				}
				helper = 0;
			}

		//zoom
		if (Mouse::isButtonPressed(Mouse::Middle)) {
				if (Mouse::isButtonPressed(Mouse::Right)) {
					scale *= 0.99;
				}
				if (Mouse::isButtonPressed(Mouse::Left)) {
					scale *= 1.01;
				}
				block.setScale(scale, scale);
				text.setCharacterSize(50 * scale);
			}
		if (Keyboard::isKeyPressed(Keyboard::Add) || Keyboard::isKeyPressed(Keyboard::Equal)) {
			scale *= 1.01;

			eyes.setScale(scale, scale);
			block.setScale(scale, scale);
			playerS.setScale(scale, scale);
			playerCol.setScale(scale, scale);
			text.setCharacterSize(50 * scale);
		}
		if (Keyboard::isKeyPressed(Keyboard::Subtract) || Keyboard::isKeyPressed(Keyboard::Hyphen)) {
			scale *= 0.99;

			eyes.setScale(scale, scale);
			block.setScale(scale, scale);
			playerS.setScale(scale, scale);
			playerCol.setScale(scale, scale);
			text.setCharacterSize(50 * scale);
		}

		if (cursor.getPosition().x > screenw * 0.95) { scrollx += 5; }
		if (cursor.getPosition().x < screenw * 0.05) { scrollx -= 5; }
		if (cursor.getPosition().y > screenh * 0.95) { scrolly += 5; }
		if (cursor.getPosition().y < screenh * 0.05) { scrolly -= 5; }

		/*if (Keyboard::isKeyPressed(Keyboard::Tab)) { mode = "game"; }*/

		//layer 1
		for (i = 0; i < wid; i++) {
				for (j = 0; j < hei; j++) {
					render_block_el1();
					if (editor_layer == 1) {
						if (onclick == false) {
							if (true) {
								if (block.getGlobalBounds().intersects(cursor.getGlobalBounds())) {
									cursor.setPosition(block.getPosition()); cursor.setScale(scale, scale);
									if (helper == 0) {
										if (!button_overlapped) {
											if (!Mouse::isButtonPressed(Mouse::Middle)) {
												if (Mouse::isButtonPressed(Mouse::Right)) {
													if (editor_mode == 1) {
														level[i][j][1][0] = 0;
													}
													if (editor_mode == 4) {
														level[i][j][1][2] = 0;
													}
												}
												if (Mouse::isButtonPressed(Mouse::Left)) {
													onclick = true;
													switch (editor_mode) {
													case 1: level[i][j][1][0] = editor_block; helper = 1; place(i, j, 1); break;
													case 2: switch (level[i][j][1][1]) {
													case 0: level[i][j][1][1] = 90; break;
													case 90: level[i][j][1][1] = 180; break;
													case 180: level[i][j][1][1] = 270; break;
													case 270: level[i][j][1][1] = 0; break;
													} break;
													case 3: level[i][j][0][0] = 0; level[i][j][1][0] = 0; break;
													case 4: level[i][j][1][2] += 1; break;
													}

													if (Keyboard::isKeyPressed(Keyboard::LControl)) {
														onclick = false; helper = 0;
													}

												}
											}
										}
									}
								}
							}
						}
					}
				}
			}

		for (i = 0; i < wid; i++) {
				for (j = 0; j < hei; j++) {
					render_block_el0();
					if (editor_layer == 0) {
						if (onclick == false) {
							if (true) {
								if (block.getGlobalBounds().intersects(cursor.getGlobalBounds())) {
									cursor.setPosition(block.getPosition()); cursor.setScale(scale, scale);
									if (helper == 0) {
										if (!button_overlapped) {
											if (!Mouse::isButtonPressed(Mouse::Middle)) {
												if (Mouse::isButtonPressed(Mouse::Right)) {
													if (editor_mode == 1) {
														level[i][j][0][0] = 0;
													}
													if (editor_mode == 4) {
														level[i][j][0][2] = 0;
													}
												}

												if (Mouse::isButtonPressed(Mouse::Left)) {

													onclick = true;

													switch (editor_mode) {
													case 1: level[i][j][0][0] = editor_block; helper = 1; place(i, j, 0); break;
													case 2: switch (level[i][j][0][1]) {
													case 0: level[i][j][0][1] = 90; break;
													case 90: level[i][j][0][1] = 180; break;
													case 180: level[i][j][0][1] = 270; break;
													case 270: level[i][j][0][1] = 0; break;
													} break;
													case 3: level[i][j][0][0] = 0; level[i][j][1][0] = 0; break;
													case 4: level[i][j][0][2] += 1; break;
													}

													if (Keyboard::isKeyPressed(Keyboard::LControl)) {
														onclick = false; helper = 0;
													}

												}
											}
										}
									}
								}
							}
						}
					}
				}
			}

		if (helper == 1) {
				if ((!Mouse::isButtonPressed(Mouse::Left)) && (!Mouse::isButtonPressed(Mouse::Right))) { helper = 0; onclick = false; }
		}

		window.draw(cursor);
		button_overlapped = false;

		//UI
		block.setOrigin(0, 0);
		block.setTexture(grb);
		for (i = 1; i < 110; i++) {
				render_block_UI(screenh, get_ID(i));
				if (block.getGlobalBounds().intersects(cursor.getGlobalBounds())) {
					button_overlapped = true;
					if (Mouse::isButtonPressed(Mouse::Left)) {
						editor_block = get_ID(i);
					}
				}
			}
		block.setScale(scale, scale);
		block.setOrigin(64, 64);

		if (onclick == true) {
				if (!Mouse::isButtonPressed(Mouse::Left)) {
					onclick = false;
				}
			}

		//cursor
		cursor.setPosition(Mouse::getPosition().x - window.getPosition().x, (Mouse::getPosition().y) - window.getPosition().y);
		cursor.setScale(1.5, 1.5);
		window.draw(cursor);

		if (E_plus.if_click()) {
			editor_mode = 1;
			E_plus.is_checked =true;
			E_Rt.is_checked = false;
			E_M.is_checked = false;
			E_player.is_checked = false;
		}

		if (E_Rt.if_click()) {
			editor_mode = 2;
			E_plus.is_checked = false;
			E_Rt.is_checked = true;
			E_M.is_checked = false;
			E_player.is_checked = false;
		}

		if (E_M.if_click()) {
			editor_mode = 4;
			E_plus.is_checked = false;
			E_Rt.is_checked = false;
			E_M.is_checked = true;
			E_player.is_checked = false;
		}

		if (E_player.if_click()) {
			editor_mode = 5;
			E_plus.is_checked = false;
			E_Rt.is_checked = false;
			E_M.is_checked = false;
			E_player.is_checked = true;
		}

		if (E_play.if_click()) {
			mode = "game"; save(lvl_name); lvlnum = -2; build(-2); break;
		}

		if (E_L.if_click()) {
			editor_layer = 0;
			E_L.is_checked = true;
			E_L2.is_checked = false;
		}

		if (E_L2.if_click()) {
			editor_layer = 1;
			E_L.is_checked = false;
			E_L2.is_checked = true;
		}

		if (E_save.if_click()) {
			context_menu = true;
			saveornot = true;
		}

		if (E_load.if_click()) {
			saveornot = false;
			context_menu = true;
		}

		if (E_prev.if_click()) {
			editor_scroll += 128;
		}

		if (E_next.if_click()) {
			editor_scroll -= 128;
		}

		if (E_B.if_click()) {
			editor_choose = 1;
			E_B.is_checked = true;
			E_R.is_checked = false;
			E_Gr.is_checked = false;
			E_Gy.is_checked = false;
			E_Mech.is_checked = false;
			E_Q.is_checked = false;
			E_E.is_checked = false;
		}

		if (E_R.if_click()) {
			editor_choose = 2;
			E_B.is_checked = false;
			E_R.is_checked = true;
			E_Gr.is_checked = false;
			E_Gy.is_checked = false;
			E_Mech.is_checked = false;
			E_Q.is_checked = false;
			E_E.is_checked = false;
		}

		if (E_Gr.if_click()) {
			editor_choose = 3;
			E_B.is_checked = false;
			E_R.is_checked = false;
			E_Gr.is_checked = true;
			E_Gy.is_checked = false;
			E_Mech.is_checked = false;
			E_Q.is_checked = false;
			E_E.is_checked = false;
		}

		if (E_Gy.if_click()) {
			editor_choose = 4;
			E_B.is_checked = false;
			E_R.is_checked = false;
			E_Gr.is_checked = false;
			E_Gy.is_checked = true;
			E_Mech.is_checked = false;
			E_Q.is_checked = false;
			E_E.is_checked = false;
		}

		if (E_Mech.if_click()) {
			editor_choose = 5;
			E_B.is_checked = false;
			E_R.is_checked = false;
			E_Gr.is_checked = false;
			E_Gy.is_checked = false;
			E_Mech.is_checked = true;
			E_Q.is_checked = false;
			E_E.is_checked = false;
		}

		if (E_Q.if_click()) {
			editor_choose = 6;
			E_B.is_checked = false;
			E_R.is_checked = false;
			E_Gr.is_checked = false;
			E_Gy.is_checked = false;
			E_Mech.is_checked = false;
			E_Q.is_checked = true;
			E_E.is_checked = false;
		}

		if (E_E.if_click()) {
			editor_choose = 7;
			E_B.is_checked = false;
			E_R.is_checked = false;
			E_Gr.is_checked = false;
			E_Gy.is_checked = false;
			E_Mech.is_checked = false;
			E_Q.is_checked = false;
			E_E.is_checked = true;
		}

		//for (i = 0; i < 11; i++) {
		//		window.draw(button[i]);
		//		if (cursor.getGlobalBounds().intersects(button[i].getGlobalBounds())) {
		//			button_overlapped = true;
		//			if (onclick == false) {

		//				if (Mouse::isButtonPressed(Mouse::Left)) {
		//					onclick = true;
		//					switch (i) {
		//					case 0: editor_mode = 1; break;
		//					case 1: editor_mode = 2; break;
		//					case 2: mode = "game"; save(lvl_name); lvlnum = -2; build(-2); break;
		//					case 3: editor_mode = 4; break;
		//					case 4: editor_layer = 0; break;
		//					case 5: editor_layer = 1; break;
		//					case 6: editor_mode = 5; break;
		//					case 7:
		//						context_menu = true;
		//						saveornot = true;
		//						break;
		//					case 8:
		//						/*build(-2);*/
		//						saveornot = false;
		//						context_menu = true;
		//						break;
		//					case 9: editor_scroll += 128; break;
		//					case 10: editor_scroll -= 128; break;
		//					}
		//				}
		//			}
		//		}
		//	}

		if (editor_mode == 5) {
				if (Mouse::isButtonPressed(Mouse::Right)) {
					px = (Mouse::getPosition().x) / scale + scrollx;
					py = (Mouse::getPosition().y) / scale + scrolly;
				}
				playerS.setPosition((px - scrollx) * scale, (py - scrolly) * scale);
				playerS.setScale(scale, scale);
				eyes.setScale(scale, scale);
				eyes.setPosition(playerS.getPosition());
				eyes.setRotation(playerS.getRotation());
				window.draw(playerS); window.draw(eyes);
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			if (!onclick) {
				onclick = true;
				mode = "menu";
			}
		}

		if (context_menu) { check_saves(); page = 0;}
		while (context_menu) {

			ifstream saves;
			saves.open("saves.txt");
			saves >> helper_string;

			window.draw(box);
			text.setFillColor(Color::Black); text.setCharacterSize(UI_scale * 50);

			box_but.setPosition(screenw / 2, 150 * UI_scale);
			box_but.setScale(UI_scale, UI_scale * 0.75);
			text.setPosition(box_but.getPosition());
			text.move(UI_scale * -200, UI_scale * -35);
			text.setString("Save as a new file");

			if (page < 0) { page = 0; }
			if (E_prev_page.if_click()) {
				onclick = true; page--; Sleep(100); continue;
			}
			if (E_next_page.if_click()) {
				onclick = true; page++; Sleep(100); continue;
			}

			if (!Mouse::isButtonPressed(Mouse::Left)) { onclick = false; }

			if (saveornot) {
				if (cursor.getGlobalBounds().intersects(box_but.getGlobalBounds())) {
					box_but.setTextureRect(IntRect(0, 128, 512, 128));
					if (Mouse::isButtonPressed(Mouse::Left)) {
						helper_string = "";

						while (!Keyboard::isKeyPressed(Keyboard::Enter)) {
							window.draw(box_but);

							text.setString(helper_string);
							window.draw(text);

							window.display();
							window.clear();

							sf::Event event;
							while (window.pollEvent(event))
							{
								// "запрос закрытия" событие: мы закрываем окно
								if (event.type == sf::Event::Closed) {
									save_options();
									close = true; window.close();
								}

								if (event.type == sf::Event::TextEntered) {
									if (event.text.unicode < 128) {
										if (Keyboard::isKeyPressed(Keyboard::Space)) {
											helper_string += "_"; continue;
										}
										if (Keyboard::isKeyPressed(Keyboard::BackSpace)) {
											helper_string = helper_string.substr(0, helper_string.size() - 1); continue;
										}
										if (Keyboard::isKeyPressed(Keyboard::Enter)) {
											helper_string += "_"; break;
										}
										helper_string += static_cast<char>(event.text.unicode);
									}
								}

							}

						}
						save(helper_string + ".txt");
						lvl_name = helper_string;
						build(-2);
						lvlnum = -2;
						context_menu = false;
						onclick = true;
						window.setKeyRepeatEnabled(true);
					}

				}
				else { box_but.setTextureRect(IntRect(0, 0, 512, 128)); }
				window.draw(box_but);
				window.draw(text);

				for (i = 0; i < 4 * page; i++) {
					saves >> helper_string;
				}
			}
			else {
				for (i = 0; i < 5 * page; i++) {
					saves >> helper_string;
				}
			}

			for (i = int(saveornot); i < 5; i++) {

				saves >> helper_string;
				if (helper_string == "Does_not_exist") { break; }

					box_but.setPosition(screenw / 2 - UI_scale * 50, 150 * UI_scale + 130 * i * UI_scale);
					box_but.setScale(UI_scale * 0.9, UI_scale * 0.8);
					text.setPosition(box_but.getPosition());
					text.move(UI_scale * -200, UI_scale * -35);
					
					text.setString(helper_string);
					E_trash.init(trash, box_but.getPosition().x + UI_scale * 200, box_but.getPosition().y, helper_string, 18);
					E_trash.box.move(UI_scale * 38, -UI_scale*51);
					E_trash.box.scale(0.8, 0.8);

					onclick = false;
					if (E_trash.if_click()) {
						while (Mouse::isButtonPressed(Mouse::Left)) {}
						onclick = false;
						for (int qiq = 0; qiq < 320; qiq++) {
							if (E_trash.if_click()) {
								helper_string = "Saves\\" + helper_string;
								const char* chr = helper_string.c_str();
								logg << "Delete: " << chr << endl;
								std::remove(chr);
								check_saves();
								break;
							}
							Sleep(1);
						}
					}

					if (cursor.getGlobalBounds().intersects(box_but.getGlobalBounds())) {
						box_but.setTextureRect(IntRect(0, 128, 512, 128));
						if (Mouse::isButtonPressed(Mouse::Left)) {
							if (true) {
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
					}
					else { box_but.setTextureRect(IntRect(0, 0, 512, 128)); }

					window.draw(box_but);
					window.draw(text);
					helper_string = lvl_name;

					//switch (i) {
					////case 4: text.setString("Save and exit"); break;
					////case 5: text.setString("Exit (no saving)"); break;
					//}
				}

			i++;

			box_but.setPosition(screenw / 2, 150 * UI_scale + 130 * 5 * UI_scale);
			box_but.setScale(UI_scale * 0.7, UI_scale * 0.7);
			text.setPosition(screenw / 2, 150 * UI_scale + 130 * 5 * UI_scale);
			text.move(UI_scale * -150, UI_scale * -35);
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

			cursor.setPosition(Mouse::getPosition().x - window.getPosition().x, (Mouse::getPosition().y)-window.getPosition().y);
			cursor.setScale(1.5, 1.5);
			window.draw(cursor);

			window.display();
			window.clear();

			sf::Event event;
			while (window.pollEvent(event))
			{
				// "запрос закрытия" событие: мы закрываем окно
				if (event.type == sf::Event::Closed) {
					save_options();
					close = true; window.close();
				}
			}
			helper = 0;

			if (Keyboard::isKeyPressed(Keyboard::Escape)) {
				onclick = true;
				context_menu = false;
			}
		}

		window.draw(cursor);

		tick++;
		window.display();
		Sleep(1);
		window.clear();

		sf::Event event;
		while (window.pollEvent(event))
		{
			// "запрос закрытия" событие: мы закрываем окно
			if (event.type == sf::Event::Closed) {
				save_options();
				close = true; window.close();
			}

		}

	} while (mode == "editor");
}