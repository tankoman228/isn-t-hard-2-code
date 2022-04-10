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

void game_cycle(int screenw, int screenh) {

		do {
			//управление
			while (true) {

				if (Keyboard::isKeyPressed(Keyboard::F5)) {
					checkpoint();
					alpha = 40;
				}

				if (Keyboard::isKeyPressed(Keyboard::Tab)) {
					build(lvlnum);
				}

				if (Keyboard::isKeyPressed(Keyboard::Up)) {
					if (Keyboard::isKeyPressed(Keyboard::Right)) {
						sx += 0.6; sy -= 0.6; dir = 2; ex += 0.4; ey -= 0.4;
					}
					else {
						if (Keyboard::isKeyPressed(Keyboard::Left)) {
							sx -= 0.6; sy -= 0.6; dir = 8; ex -= 0.4; ey -= 0.4;
						}
						else {
							sy -= 0.6; dir = 1; ey -= 0.4;
						}
					}
					break;

				}
				if (Keyboard::isKeyPressed(Keyboard::Down)) {
					if (Keyboard::isKeyPressed(Keyboard::Right)) {
						sx += 0.6; sy += 0.6; dir = 4; ex += 0.4; ey += 0.4;
					}
					else {
						if (Keyboard::isKeyPressed(Keyboard::Left)) {
							sx -= 0.6; sy += 0.6; dir = 6; ex -= 0.4; ey += 0.4;
						}
						else {
							sy += 0.6; dir = 5; ey += 0.4;
						}
					}
					break;
				}
				if (Keyboard::isKeyPressed(Keyboard::Right)) {
					sx += 0.6; dir = 3; ex += 0.4; break;
				}
				if (Keyboard::isKeyPressed(Keyboard::Left)) {
					sx -= 0.6; dir = 7; ex -= 0.4; break;
				}

				break;
			}

			if (ex > 0) { ex -= 0.2; }
			if (ex < 0) { ex += 0.2; }
			if (ey > 0) { ey -= 0.2; }
			if (ey < 0) { ey += 0.2; }
			if (abs(sx) < 0.4) { sx = 0; } if (abs(sy) < 0.4) { sy = 0; }

			if (sx > max_speed) { sx = max_speed; } if (sx < -max_speed) { sx = -max_speed; }
			if (sy > max_speed) { sy = max_speed; } if (sy < -max_speed) { sy = -max_speed; }

			if (sx > 0) { sx -= 0.35; } if (sx < 0) { sx += 0.35; }
			if (sy > 0) { sy -= 0.35; } if (sy < 0) { sy += 0.35; }

			if (story.camera_pin) {
				if (playerCol.getPosition().x > screenw) { sx -= 1; }
				if (playerCol.getPosition().y > screenh) { sy -= 1; }
				if (playerCol.getPosition().x < 0) { sx += 1; }
				if (playerCol.getPosition().y < 0) { sy += 1; }
			}

			px += sx;
			py += sy;

			max_speed = 9;

			if (ex > 3) { ex = 3; }
			if (ex < -4) { ex = -4; }
			if (ey > 3) { ey = 3; }
			if (ey < -3) { ey = -3; }

			if (scrollx < 0) { scrollx = 0; }
			if (scrolly < 0) { scrolly = 0; }

			//zoom
			if (!story.camera_pin) {
				if (Mouse::isButtonPressed(Mouse::Middle)) {
					if (Mouse::isButtonPressed(Mouse::Right)) {
						if (scale > 0.65) {
							scale *= 0.99;
						}
					}
					if (Mouse::isButtonPressed(Mouse::Left)) {
						scale *= 1.01;
					}

					if (int(playerS.getPosition().x / 2) > screenw / 4) { scrollx += 2; }
					if (int(playerS.getPosition().x / 2) < screenw / 4) { scrollx -= 2; }
					if (int(playerS.getPosition().y / 2) > screenh / 4) { scrolly += 2; }
					if (int(playerS.getPosition().y / 2) < screenh / 4) { scrolly -= 2; }

					eyes.setScale(scale, scale);
					block.setScale(scale, scale);
					playerS.setScale(scale, scale);
					playerCol.setScale(scale, scale);
				}
			}
			else {
				story.zoom();
			}

			rendpp();

			//повороты
			switch (dir) {
			case 1:
				if (playerS.getRotation() != 270) {
					if ((playerS.getRotation() < 90) || (playerS.getRotation() > 270)) {
						playerS.rotate(-5);
					}
					else {
						playerS.rotate(5);
					}
				}
				break;
			case 2:
				if (playerS.getRotation() != 315) {
					if ((playerS.getRotation() < 315) && (playerS.getRotation() > 135)) {
						playerS.rotate(5);
					}
					else {
						playerS.rotate(-5);
					}
				}
				break;
			case 3:
				if (playerS.getRotation() != 0) {
					if (playerS.getRotation() < 180) {
						playerS.rotate(-5);
					}
					else {
						playerS.rotate(5);
					}
				}
				break;
			case 4:
				if (playerS.getRotation() != 45) {
					if ((playerS.getRotation() < 225) && (playerS.getRotation() > 45)) {
						playerS.rotate(-5);
					}
					else {
						playerS.rotate(5);
					}
				}
				break;
			case 5:
				if (playerS.getRotation() != 90) {
					if ((playerS.getRotation() < 270) && (playerS.getRotation() > 90)) {
						playerS.rotate(-5);
					}
					else {
						playerS.rotate(5);
					}
				}
				break;
			case 6:
				if (playerS.getRotation() != 135) {
					if ((playerS.getRotation() < 315) && (playerS.getRotation() > 135)) {
						playerS.rotate(-5);
					}
					else {
						playerS.rotate(5);
					}
				}
				break;
			case 7:
				if (playerS.getRotation() != 180) {
					if (playerS.getRotation() > 180) {
						playerS.rotate(-5);
					}
					else {
						playerS.rotate(5);
					}
				}
				break;
			case 8:
				if (playerS.getRotation() != 225) {
					if ((playerS.getRotation() < 225) && (playerS.getRotation() > 45)) {
						playerS.rotate(5);
					}
					else {
						playerS.rotate(-5);
					}
				}
				break;
			}

			if (ey > 0) { ey -= 0.05; } if (ey < 0) { ey += 0.05; }
			if (ex > 0) { ex -= 0.05; } if (ex < 0) { ex += 0.05; }

			eyes.setRotation(playerS.getRotation());

			//scrolling
			if (!story.camera_pin) {
				if (playerS.getPosition().x > screenw - screenw / 2.2) {
					scrollx += 2;
				}
				if (playerS.getPosition().x < screenw / 2.2) {
					scrollx -= 2;
				}
				if (playerS.getPosition().y > screenh - screenh / 2.2) {
					scrolly += 2;
				}
				if (playerS.getPosition().y < screenh / 2.2) {
					scrolly -= 2;
				}

				if (playerS.getPosition().x > screenw - 400) {
					scrollx += 5;
				}
				if (playerS.getPosition().x < 400) {
					scrollx -= 5;
				}
				if (playerS.getPosition().y > screenh - 280) {
					scrolly += 5;
				}
				if (playerS.getPosition().y < 280) {
					scrolly -= 5;
				}

				if (playerS.getPosition().x > screenw - 200) {
					scrollx += 3;
					if (playerS.getPosition().x > screenw) {
						scrollx += 200; sx = 0;
					}
				}
				if (playerS.getPosition().x < 200) {
					scrollx -= 3;
					if (playerS.getPosition().x < 0) {
						scrollx -= 200; sx = 0;
					}
				}
				if (playerS.getPosition().y > screenh - 100) {
					scrolly += 3;
					if (playerS.getPosition().y > screenh) {
						scrolly += 200; sy = 0;
					}
				}
				if (playerS.getPosition().y < 100) {
					scrolly -= 3;
					if (playerS.getPosition().y < 0) {
						scrolly -= 200; sy = 0;
					}
				}
			}
			else {
				story.scroll();
			}

			//layer 1
			for (i = 0; i < wid; i++) {
				for (j = 0; j < hei; j++) {
					if (level[i][j][1][0] != 0) {
						block.setPosition((i * 128 + 64 - scrollx) * scale, (j * 128 + 64 - scrolly) * scale);
						if ((block.getPosition().x < screenw + 256) && (block.getPosition().x > -256)) {
							if ((block.getPosition().y < screenh + 256) && (block.getPosition().y > -256)) {

								render_block_pl1();

								if (level[i][j][1][0] == 6) {
									if (playerS.getGlobalBounds().intersects(block.getGlobalBounds())) {
										if (coins_gathered >= coins_required) {
											if (level[i][j][0][0] != 5) {
												alpha -= 1;
											}
										}

									}
								}
								if (level[i][j][1][0] == 7) {
									block.setScale(0.5 * scale, 0.5 * scale);
									spiral[level[i][j][1][2]].setPosition(block.getPosition().x, block.getPosition().y);
									if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {
										if ((!on_teleport) && (teleport_frame == 0)) {
											tp_s.play();
										}
										on_teleport = true;
										px = i * 128 + 64;
										py = j * 128 + 64;
										playerS.scale(0.99, 0.99);
										teleport_to = level[i][j][1][2];

										teleport_frame += 1.75;
									}
									block.setScale(scale, scale);

									if (teleport_frame > 10) {
										if (teleport_frame > 13) {
											if (teleport_frame > 16) {
												spiral[level[i][j][1][2]].setTextureRect(IntRect(768, 0, 256, 256));
											}
											else {
												spiral[level[i][j][1][2]].setTextureRect(IntRect(512, 0, 256, 256));
											}
										}
										else {
											spiral[level[i][j][1][2]].setTextureRect(IntRect(256, 0, 256, 256));
										}
									}


								}
								if (level[i][j][1][0] == 18) {
									block.setColor(Color::Color(255, 255, 255, 255));
								}
							}
						}
					}
				}
			}

			if (on_teleport) {
				playerS.rotate(15);
			}

			rendpp();
			//layer 0
			for (i = 0; i < wid; i++) {
				for (j = 0; j < hei; j++) {
					if (level[i][j][0][0] != 0) {
						/*if (abs(py - j * 128) < 800 / scale + 150) {
							if (abs(px - i * 128) < 1500 / scale + 150) {*/
						block.setPosition((i * 128 + 64 - scrollx) * scale, (j * 128 + 64 - scrolly) * scale);
						if ((block.getPosition().x < screenw + 256) && (block.getPosition().x > -256)) {
							if ((block.getPosition().y < screenh + 256) && (block.getPosition().y > -256)) {
								render_block_pl0();

								if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {

									colint += 2;

									block.setTexture(inter); block.setTextureRect(IntRect(0, 0, 128, 128));

									switch (level[i][j][0][0]) {
									case 2: block.setColor(Color::Color(0, 75, 255, colint * 5)); window.draw(block); break;
									case 3: block.setColor(Color::Color(50, 255, 50, colint * 20)); window.draw(block); break;
									case 5: block.setColor(Color::Color(255, 100, 100, colint * 20)); window.draw(block); break;
									case 6: block.setColor(Color::Color(255, 0, 0, colint * 20)); window.draw(block); break;
									case 15: block.setColor(Color::Color(25, 255, 25, colint * 10)); window.draw(block); break;
									}

									block.setColor(Color::Color(255, 255, 255, 255));

									if (dir % 2 == 1) {

										if (colint > 15) {
											if (level[i][j][0][0] == 3) {
												coins_gathered++;
												level[i][j][0][0] = 4;
												sound_c.play(); colint = 0;
												if (level[i][j][0][2] == 3) {
													switch (tick % 4) {
													case 0: level[i][j][0][1] = 0; break;
													case 1: level[i][j][0][1] = 90; break;
													case 2: level[i][j][0][1] = 180; break;
													case 3: level[i][j][0][1] = 270; break;
													}
												}
												smoke_spawn("coin");
											}
											if (level[i][j][0][0] == 5) {
												if (level[i][j][0][2] == 0) {
													colint = 0;
													switch (dir) {
													case 1: if (level[i][j - 1][0][0] == 0) { level[i][j][0][2] = 128; sound_p.play(); } break;
													case 3: if (level[i + 1][j][0][0] == 0) { level[i][j][0][2] = 384; sound_p.play(); } break;
													case 5: if (level[i][j + 1][0][0] == 0) { level[i][j][0][2] = 256; sound_p.play(); } break;
													case 7: if (level[i - 1][j][0][0] == 0) { level[i][j][0][2] = 512; sound_p.play(); } break;
													}
													if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {
														switch (dir) {
														case 1: py += 5; break;
														case 2: py += 5; px -= 5; break;
														case 3: px -= 5; break;
														case 4: py -= 5; px -= 5; break;
														case 5: py -= 5; break;
														case 6: px += 5; py -= 5; break;
														case 7: px += 5; break;
														case 8: px += 5; py += 5; break;
														}
													}
												}
											}
											if (level[i][j][0][0] == 6) {
												level[i][j][0][0] = 0;
												smoke_spawn("smoke"); sound_b.play(); colint = 0;
											}
											if (level[i][j][0][0] == 18) {
												smoke_spawn("dark");
												level[i][j][0][0] = 5;
												_break.play();
												colint = -10;
											}
											if (level[i][j][0][0] == 19) {
												smoke_spawn("dark");
												level[i][j][0][0] = 21;
												for (k = 0; k < 3; k++) {
													dmode[k] = true;
												}
											}
											if (level[i][j][0][0] == 20) {
												smoke_spawn("dark");
												level[i][j][0][0] = 21;
												for (k = 0; k < 3; k++) {
													dmode[k] = false;
												}
											}

										}
										if (colint > 30) {
											if (!crystal[level[i][j][0][2]]) {
												if (level[i][j][0][0] == 15) {
													_break.play();
													smoke_spawn("cryst"); smoke_spawn("cryst");
													crystal[level[i][j][0][2]] = true; colint = 0;
												}
											}
											if (level[i][j][0][0] == 2) {
												if (dmode[level[i][j][0][2]] == 0) { dmode[level[i][j][0][2]] = 1; }
												else { dmode[level[i][j][0][2]] = 0; }
												s1.play(); colint = -5;
											}
										}

									}

									if (sx > 0) { playerCol.move(1, 0); }
									else { playerCol.move(-1, 0); }
									if (sy > 0) { playerCol.move(0, 1); }
									else { playerCol.move(0, -1); }

									if (level[i][j][0][0] == 14) {
										block.scale(0.5, 0.5);
									}

									if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {
										px -= sx * 1.07; py -= sy * 1.07;
										if (in_zone) {
											colint += 5;
											while (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {
												if (sx < 0) { px += 1; } if (sx > 0) { px -= 1; }
												if (sy < 0) { py += 1; } if (sy > 0) { py -= 1; }
												rendpp();
											}
											in_zone = false;
										}
										sx = 0; sy = 0;
									}

									if (level[i][j][0][0] == 14) {
										block.scale(2, 2);
									}
								}
							}
						}
					}
				}
			}

			colint -= 1; if (colint < 0) { colint = 0; }

			//spiral
			if (true) {
				for (i = 0; i < 3; i++) {
					spiral[i].rotate(3.2);
					if (i == teleport_to) { spiral[i].setScale((teleport_frame * 0.01 + 0.25) * scale, (teleport_frame * 0.01 + 0.25) * scale); }
					else { spiral[i].setScale(0.25 * scale, 0.25 * scale); }
					window.draw(spiral[i]);
					spiral[i].scale(2, 2);
					window.draw(spiral[i]);
					spiral[i].scale(0.5, 0.5);
					window.draw(spiral[i]);
					if (i == teleport_to) { spiral[i].setScale((teleport_frame * 0.01 + 0.25) * scale, (teleport_frame * 0.01 + 0.25) * scale); }
					else { spiral[i].setScale(0.25 * scale, 0.25 * scale); }
					spiral[i].setPosition((tx[i] - scrollx) * scale, (ty[i] - scrolly) * scale);
					window.draw(spiral[i]);
					spiral[i].scale(2, 2);
					window.draw(spiral[i]);
					spiral[i].scale(0.5, 0.5);
					window.draw(spiral[i]);
					spiral[i].setPosition(-200, -200);
				}
			}
			if (teleport_frame != 0) {
				playerS.setColor(Color::Color(255, 255, 255, 260 - teleport_frame * 4));
				eyes.setColor(Color::Color(255, 255, 255, 260 - teleport_frame * 4));
			}
			if (!on_teleport && (teleport_frame > 0)) {
				teleport_frame -= 3; spiral[teleport_to].rotate(-4); if (teleport_frame < 0) {
					teleport_frame = 0;
					playerS.setColor(Color::Color(255, 255, 255, 255));
					eyes.setColor(Color::Color(255, 255, 255, 255));
				}
			}

			window.draw(playerS); window.draw(eyes);

			if (tick % 500 > 460) {
				eyes.setPosition(playerS.getPosition());
				eyes.setTexture(blink);
				if (tick % 500 < 480) {
					eyes.setTextureRect(IntRect(75 * floor((tick % 500 - 480) / 2), 0, 75, 75));
				}
				if (tick % 500 >= 480) {
					eyes.setTextureRect(IntRect(75 * (8 - floor((tick % 500 - 480) / 2)), 0, 75, 75));
				}
				window.draw(eyes);
				eyes.setTexture(eyes_t);
				eyes.setTextureRect(IntRect(0, 0, 75, 75));
			}

			if (robot.enabled) {
				current_NPC = 0;
				robot.move();
				robot.render();
				window.draw(NPC);
				/*	if (tick % 100 == 50) { robot.next_path(); }*/
			}

			for (i = 0; i < 3; i++) {
				if (particle[i][0].enabled) {
					for (j = 0; j < partnum; j++) {
						particle[i][j].render();
					}
				}
			}

			if (story.dialog) {

				dialog_but.setPosition(UI_scale * 370, screenh + 50 * UI_scale);
				for (i = 0; i < 3; i++) {
					dialog_but.move(0, UI_scale * -110);
					if (cursor.getGlobalBounds().intersects(dialog_but.getGlobalBounds())) {
						dialog_but.setTextureRect(IntRect(0, 128, 724, 128));
						if (Mouse::isButtonPressed(Mouse::Left)) {
							story.next_stage();
						}
					}
					else { dialog_but.setTextureRect(IntRect(0, 0, 724, 128)); }
					window.draw(dialog_but);
				}

				dialog_NPC.setFillColor(Color::Color(120, 120, 120, 255));
				dialog1.setFillColor(Color::Color(140, 140, 140, 255));
				dialog2.setFillColor(dialog1.getFillColor()); dialog3.setFillColor(dialog1.getFillColor());
				story.render(screenw, screenh);
				window.draw(dialog_NPC);
				window.draw(dialog1); window.draw(dialog2); window.draw(dialog3);
				dialog_NPC.setFillColor(Color::Color(0, 0, 0, 255));
				dialog1.setFillColor(Color::Color(50, 50, 50, 255));
				dialog2.setFillColor(dialog1.getFillColor()); dialog3.setFillColor(dialog1.getFillColor());
				dialog_NPC.move(2, 2); dialog1.move(2, 2); dialog2.move(2, 2); dialog3.move(2, 2);
				window.draw(dialog1); window.draw(dialog2); window.draw(dialog3); window.draw(dialog_NPC);
			}

			if (abs(alpha) > 0) {
				alpha -= 5;
				if (alpha > 0) {
					blend.setColor(Color::Color(0, 0, 0, abs(alpha)));
				}
				if (alpha < 0) {
					blend.setColor(Color::Color(0, 0, 0, abs(alpha)));
					if (alpha < -253) {
						if (levels_opened == lvlnum) {
							levels_opened++;
						}
						if (lvlnum == -2) {
							mode = "editor"; lvlnum = -2; build(-2);
						}
						else {
							if (lvlnum == -1) {
								mode = "menu";
								lvlnum = 1;
							}
							else { lvlnum++; build(lvlnum); checkpoint(); }
						}

						build(lvlnum);
					}
				}
				window.draw(blend);
			}

			helper = 0;
			if (in_zone) {
				if (tick % 5 == 3) {
					in_zone = false;
				}
			}

			if (!music.getStatus()) {
				switch (rand() % 2) {
				case 0: music.openFromFile("Sounds\\music1.ogg"); break;
				case 1: music.openFromFile("Sounds\\music2.ogg"); break;
				}
				music.play();
			}
		} while (mode == "game");
	}