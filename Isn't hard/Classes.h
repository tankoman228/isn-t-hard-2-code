#pragma once
#pragma comment(lib,"User32.lib")

#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <windows.h>
#include <SFML/Audio.hpp>
#include <fstream>
#include "Values.h"
using namespace sf; using namespace std;

class ButtonRes {
public:
	Sprite _box; Text _text; Texture _t;
	bool is_picked = false, click = false;
	int W, H;
	void init(int w, int h) {
		W = w, H = h;
		helper_string = to_string(w) + " : " + to_string(h);
		_text.setString(helper_string);
		_box.setScale(UI_scale / 1.5, UI_scale);
		_t.loadFromFile("Textures\\ResBut.png");
		_box.setTexture(_t);
		_box.setPosition(50 + i * 200, 750 * UI_scale);
		_text.setPosition(50 + i * 200, 770 * UI_scale);
		_text.setFont(font); _text.setFillColor(Color::Black);
		_text.setCharacterSize(UI_scale * 30);
	}
	void cycle() {
		click = false;
		if (_box.getGlobalBounds().intersects(cursor.getGlobalBounds())) {
			_box.setTextureRect(IntRect(0, 64, 256, 64));
			if (!onclick && (Mouse::isButtonPressed(Mouse::Left))) {
				click = true;
			}
		}
		else {
			_box.setTextureRect(IntRect(0, 0, 256, 64));
		}
		if (is_picked) {
			_box.setTextureRect(IntRect(0, 64, 256, 64));
		}
		window.draw(_box);
		window.draw(_text);
	}

};
ButtonRes res_but[2];

class ThreeBut {
public:
	int minval = 10, maxval = 40, currentval, x = 50, y = 400, option = 0;
	Sprite lower, middle, more;
	Text value;
	Texture texture_;
	void init() {
		lower.setPosition(x, y);
		lower.setTexture(texture_);
		lower.setTextureRect(IntRect(0, 0, 128, 128));
		lower.setScale(UI_scale, UI_scale);
		middle.setPosition(x + 128 * UI_scale, y);
		middle.setTexture(texture_);
		middle.setTextureRect(IntRect(128, 0, 128, 128));
		middle.setScale(UI_scale, UI_scale);
		more.setPosition(x + 256 * UI_scale, y);
		more.setTexture(texture_);
		more.setTextureRect(IntRect(256, 0, 128, 128));
		more.setScale(UI_scale, UI_scale);
		texture_.loadFromFile("Textures\\Opt.png");
		value.setPosition(x + 168 * UI_scale, y + UI_scale * 38);
		value.setFont(font);
		value.setCharacterSize(40 * UI_scale);
		value.setFillColor(Color::Black);

	}
	void cycle() {
		if (lower.getGlobalBounds().intersects(cursor.getGlobalBounds())) {
			lower.setColor(Color::Color(200, 200, 200, 255));
			if (!onclick) {
				if (Mouse::isButtonPressed(Mouse::Left)) {
					onclick = true;
					if (currentval > minval) {
						currentval -= 1;
					}
					switch (option) {
					case 0: partnum = currentval;
						break;
					}
				}
			}
		}
		else
		{
			lower.setColor(Color::Color(255, 255, 255, 255));
		}

		if (more.getGlobalBounds().intersects(cursor.getGlobalBounds())) {
			more.setColor(Color::Color(200, 200, 200, 255));
			if (!onclick) {
				if (Mouse::isButtonPressed(Mouse::Left)) {
					onclick = true;
					if (currentval < maxval) {
						currentval += 1;
					}
					switch (option) {
					case 0: partnum = currentval;
						ofstream options;
						options.open("Options.txt");
						options << partnum << " ";
						options.close();
						break;
					}
				}
			}
		}
		else
		{
			more.setColor(Color::Color(255, 255, 255, 255));
		}

		if (!Mouse::isButtonPressed(Mouse::Left)) { onclick = false; }

		value.setString(to_string(currentval));

		window.draw(lower);
		window.draw(middle);
		window.draw(more);
		window.draw(value);
	}
};
ThreeBut particles_option;

Texture robot_t;
class NPC_object {
public:
	bool enabled = false;
	void spawn() {
		enabled = true;
		switch (current_NPC) {
		case 0: health = 100; x = 770; y = 750; MaxSpeed = 4;
			path[0][0] = 790; path[0][1] = 750; direction = 7;
			path[1][0] = 820; path[1][1] = 750; direction = 7;
			break;
		}
	}
	void stop() { enabled = false; }
	void move() {
		if (abs(x - path[current_path][0]) > 5) {
			if (x > path[current_path][0]) { sx_ -= accel; }
			if (x < path[current_path][0]) { sx_ += accel; }
		}
		else { sx_ /= 2; }
		if (abs(y - path[current_path][1]) > 5) {
			if (y > path[current_path][1]) { sy_ -= accel; }
			if (y < path[current_path][1]) { sy_ += accel; }
		}
		else { sy_ /= 2; }
		if (abs(sx_) < 0.3) { sx_ = 0; } if (abs(sy_) < 0.3) { sy_ = 0; }
		if (sx_ > 0) { sx -= 0.1; } if (sx_ < 0) { sx += 0.1; }
		if (sy_ > 0) { sy -= 0.1; } if (sy_ < 0) { sy += 0.1; }
		if (sx_ > MaxSpeed) { sx_ = MaxSpeed; } if (sx_ < -MaxSpeed) { sx_ = -MaxSpeed; }
		if (sy_ > MaxSpeed) { sy_ = MaxSpeed; } if (sy_ < -MaxSpeed) { sy_ = -MaxSpeed; }
		if (NPC.getGlobalBounds().intersects(playerCol.getGlobalBounds())) {
			sx = sx_;
			sy = sy_;
		}
		x += sx_; y += sy_;
	}
	void render() {
		NPC.setPosition((x - scrollx) * scale, (y - scrolly) * scale);
		NPC.setScale(scale * 1.5, scale * 1.5);
		NPC.setRotation(rotation);
		if (tick % 20 == 5) {
			frame = rand() % 5;
		}
		if (frame == 4) {
			if (rand() % 2 == 1) {
				frame = rand() % 5;
			}
		}
		switch (current_NPC) {
		case 0: NPC.setTexture(robot_t);
			NPC.setTextureRect(IntRect(frame * 75, 0, 75, 75));
			break;
		}
		switch (direction) {
		case 1:
			if (rotation != 270) {
				if ((rotation < 90) || (rotation > 270)) {
					rotation -= 5;
				}
				else {
					rotation += 5;
				}
			}
			break;
		case 2:
			if (rotation != 315) {
				if ((rotation < 315) && (rotation > 135)) {
					rotation += 5;
				}
				else {
					rotation -= 5;
				}
			}
			break;
		case 3:
			if (rotation != 0) {
				if (rotation < 180) {
					rotation -= 5;
				}
				else {
					rotation += 5;
				}
			}
			break;
		case 4:
			if (rotation != 45) {
				if ((rotation < 225) && (rotation > 45)) {
					rotation -= 5;
				}
				else {
					rotation += 5;
				}
			}
			break;
		case 5:
			if (rotation != 90) {
				if ((rotation < 270) && (rotation > 90)) {
					rotation -= 5;
				}
				else {
					rotation += 5;
				}
			}
			break;
		case 6:
			if (rotation != 135) {
				if ((rotation < 315) && (rotation > 135)) {
					rotation -= 5;
				}
				else {
					rotation += 5;
				}
			}
			break;
		case 7:
			if (rotation != 180) {
				if (rotation > 180) {
					rotation -= 5;
				}
				else {
					rotation += 5;
				}
			}
			break;
		case 8:
			if (rotation != 225) {
				if ((rotation < 225) && (rotation > 45)) {
					rotation += 5;
				}
				else {
					rotation -= 5;
				}
			}
			break;
		}
		if ((rotation < 0) || (rotation > 360)) {
			rotation = 0;
		}
		//if (current_path == 9) {
		//	current_path = 0;
		//}
	}
	void next_path() {
		current_path++;
		switch (current_NPC) {
		case 0:
			switch (current_path) {
			case 1: direction = 7; break;
			case 2: direction = 1; break;
			case 3: direction = 3; break;
			case 4: direction = 4; break;
			case 5: direction = 5; break;
			case 6: direction = 6; break;
			case 7: direction = 7; break;
			case 8: direction = 8; break;
			}
		}
	}
	int health = 100, current_path = 0, path[10][2], x = 0, y = 0, direction = 3;
	float sx_ = 0, sy_ = 0, MaxSpeed = 12, accel = 0.3, rotation = 0, frame = 0;
};
NPC_object robot;

Text dialog_NPC, dialog1, dialog2, dialog3; Sound voice; SoundBuffer voice_f;

class Story {
public:
	void stop() { enabled = false; dialog = false; camera_pin = false; stage = 0; variant = 1; help_bool1 = false; dialog_i = 1; }
	void next_stage() {
		stage++; wait = 0;
		if (lvlnum == -1) {
			switch (stage) {
			case 1: camera_pin = true, dialog = true, target_x = 0, target_y = 550, target_scale = 1.6 * UI_scale;
				scrollx = target_x; scrolly = target_y, scale = 2;

				if (!help_bool1) {
					dialog_NPC.setString("Hellow");
					switch (rand() % 4) {
					case 0: voice_f.loadFromFile("Dialogs\\Robot_medium.ogg"); break;
					case 1: voice_f.loadFromFile("Dialogs\\Robot_short.ogg"); break;
					case 2: voice_f.loadFromFile("Dialogs\\Robot_short2.ogg"); break;
					case 3: voice_f.loadFromFile("Dialogs\\Robot_short3.ogg"); break;
					}
					voice.play(); help_bool1 = true;
				}
				dialog1.setString("Hellow");
				dialog2.setString("Okay");
				dialog3.setString(":-)");
				break;
			case 2: camera_pin = false, dialog = false; robot.next_path(); break;
			}
		}
	}
	void render(int screenw, int screenh) {
		dialog_NPC.setPosition(UI_scale * 36, screenh - 430 * UI_scale);
		dialog1.setPosition(UI_scale * 36, screenh - 325 * UI_scale);
		dialog2.setPosition(UI_scale * 36, screenh - 215 * UI_scale);
		dialog3.setPosition(UI_scale * 36, screenh - 105 * UI_scale);
	}
	void scroll() {
		if (target_x < scrollx) {
			scrollx += 1;
		}
		if (target_x > scrollx) {
			scrollx -= 1;
		}
		if (target_y > scrolly) {
			scrolly += 1;
		}
		if (target_y < scrolly) {
			scrolly -= 1;
		}
	}
	void zoom() {
		if (target_scale - scale < 0.05) {
			scale *= 0.999;
		}
		if (target_scale - scale > -0.05) {
			scale *= 1.001;
		}
		eyes.setScale(scale, scale);
		block.setScale(scale, scale);
		playerS.setScale(scale, scale);
		playerCol.setScale(scale, scale);
	}
	bool enabled = false, camera_pin = true, dialog = false;
	string txt, txt1, txt2, txt3;
	int stage = 0, target_x = 300, target_y = 200, dialog_i = 0, wait = 0, variant;
	float target_scale = 1.5;
	bool help_bool1 = false;
};
Story story;

class Particles {
public:
	bool enabled = false;
	float size, x = 0, y = 0, alpha, alpha_speed, speed_x, speed_y, scale_, scale_speed;
	int timer, rotation_speed;
	Sprite particle;
	void render() {
		particle.setPosition(scale * (x - scrollx), scale * (y - scrolly));
		x += speed_x; y += speed_y;
		particle.setColor(Color::Color(255, 255, 255, alpha));
		alpha += alpha_speed;
		particle.setScale(scale_ * scale, scale_ * scale);
		scale_ *= scale_speed;
		if (timer < 0) { enabled = false; }
		timer -= 1;
		particle.rotate(rotation_speed);
		window.draw(particle);
	}
};
Particles particle[3][40];

class Checkbox {
public:
	Sprite box_;
	Text text_;
	Texture texture_;
	string text__ = "Fullscreen";
	int type = 0;
	bool is_checked = false;

	void init(int x, int y) {
		text_.setFont(font);
		text_.setCharacterSize(30);
		text_.setFillColor(Color::Blue);
		text_.setPosition(x, y);
		texture_.loadFromFile("Textures\\Check_Box.png");
		box_.setScale(UI_scale / 2, UI_scale / 2);
		box_.setTexture(texture_);
		box_.setPosition(x, y);
		text_.setPosition(x + 72 * UI_scale, y + 10 * UI_scale);
		text_.setString(text__);
	}

	void cycle() {
		if (is_checked) {
			box_.setTextureRect(IntRect(0, 128, 128, 128));
		}
		else {
			box_.setTextureRect(IntRect(0, 0, 128, 128));
		}
		if (cursor.getGlobalBounds().intersects(box_.getGlobalBounds())) {
			if (!onclick) {
				if (Mouse::isButtonPressed(Mouse::Left)) {
					if (is_checked) {
						is_checked = false;
					}
					else {
						is_checked = true;
					}
					switch (type) {
					case 0: break;
					}
				}
			}
		}
		window.draw(box_); window.draw(text_);
	}
};
Checkbox screenmode;

class Option_Button {
public:
	Sprite button_;
	string text_ = "The first button";
	Texture texture_;
	Text txt_;
	int type = 0;
	bool is_clicked = false;
	void init() {
		txt_.setFont(font);
		txt_.setString(text_);
		button_.setScale(1 * UI_scale, 0.7 * UI_scale);
		texture_.loadFromFile("Textures\\ContextMenuB.png");
		button_.setTexture(texture_);
		button_.setPosition((type) * 600 * UI_scale, 200 * UI_scale);
		txt_.setPosition((type) * 600 * UI_scale + 10, 210 * UI_scale);
		txt_.setCharacterSize(40 * UI_scale);
		txt_.setFillColor(Color::Black);
	}
	void cycle() {
		is_clicked = false;
		if (cursor.getGlobalBounds().intersects(button_.getGlobalBounds())) {
			button_.setTextureRect(IntRect(0, 128, 512, 128));
			if (!onclick) {
				if (Mouse::isButtonPressed(Mouse::Left)) {
					switch (type) {
					case 0: 
						if (true) {
							helper_s[0] = 1920;
							helper_s[1] = 1080;
							ofstream file;
							file.open("Screen.txt");
							file << helper_s[0] << " ";
							file << helper_s[1] << " ";
							if (screenmode.is_checked) {
								file << 1 << " ";
							}
							else {
								file << 0 << " ";
							}
							file << UI_scale * 10 << " ";
							file.close();
						}
						break;
					case 1:

						if (true) {

							helper_s[0] = GetSystemMetrics(0);
							helper_s[1] = GetSystemMetrics(1);

							ofstream file;
							file.open("Screen.txt");
							file << helper_s[0] << " ";
							file << helper_s[1] << " ";
							file << 1 << " ";
							file << UI_scale * 10 << " ";
							file.close();

						}
						break;


					}
					is_clicked = true;
				}
			}
		}
		else {
			button_.setTextureRect(IntRect(0, 0, 512, 128));
		}
		window.draw(button_);
		window.draw(txt_);
	}
};
Option_Button reloader, auto_reloader;

class UI {
public:
	Sprite box; Text txt;
	int x, y, id;
	void init(Texture& _t, int _x, int _y, string _text, int _id) {
		box.setTexture(_t);
		x = _x; y = _y;
		txt.setFont(font);
		txt.setString(_text);
		id = _id;
		box.setPosition(x, y);
		txt.setFillColor(Color::White);
		txt.setPosition(x, y);
		box.setOrigin(128, 128);
		box.setScale(UI_scale, UI_scale);
	}
};

class Button : public UI {
public:
	void cycle() {
		if (Mouse::isButtonPressed(Mouse::Left)) {
			if (box.getGlobalBounds().intersects(cursor.getGlobalBounds())) {
				if (!onclick) {
					onclick = true;
					click_button_menu = true;
					helper_s2 = id;
				}
			}
		}
		else { onclick = false; }

		if (box.getGlobalBounds().intersects(cursor.getGlobalBounds())) { box.setTextureRect(IntRect(256, 0, 256, 256)); }
		else { box.setTextureRect(IntRect(0, 0, 256, 256)); }

		window.draw(box);
	}
};
Button b_play, b_levels, b_editor, b_ach, b_options, b_exit;

class Button2 : public UI {
public:
	bool is_checked = false;
	
	bool if_click() {
		box.setOrigin(0, 0);
		if (box.getGlobalBounds().intersects(cursor.getGlobalBounds())) {

			button_overlapped = true;

			if  (Mouse::isButtonPressed(Mouse::Left)) {
				if (!onclick) {
					onclick = true;
					return true;
				}
			}

			if (is_checked) {
				box.setTextureRect(IntRect(0, 0, 128, 128));
			}
			else { box.setTextureRect(IntRect(128, 0, 128, 128)); }

		}
		else { 
			if (is_checked) {
				box.setTextureRect(IntRect(128, 0, 128, 128));
			}
			else { box.setTextureRect(IntRect(0, 0, 128, 128)); }
			
		} 
		window.draw(box);
		return false;
	}
	
};
Button2 E_B, E_E, E_Gr, E_Gy, E_L, E_L2, E_M, E_Mech, E_next, E_prev, E_play, E_player, E_plus, E_Q, E_R, E_Rt, E_save, E_load, E_trash, E_next_page, E_prev_page;