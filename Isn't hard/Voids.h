#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <windows.h>
#include <SFML/Audio.hpp>
#include <fstream>
#include "Values.h"
#include "Classes.h"
using namespace sf; using namespace std;

void reset_music() {
	switch (tick % 10) {
	case 0: music.openFromFile("Sounds\\music1.ogg"); break;
	case 1: music.openFromFile("Sounds\\music2.ogg"); break;
	case 2: music.openFromFile("Sounds\\Ambient2.ogg"); break;
	case 3: music.openFromFile("Sounds\\Ambient4.ogg"); break;
	case 4: music.openFromFile("Sounds\\Music of fun.ogg"); break;
	case 5: music.openFromFile("Sounds\\Something sad.ogg"); break;
	case 6: music.openFromFile("Sounds\\Not an ambient.ogg"); break;
	case 7: music.openFromFile("Sounds\\Usual M.ogg"); break;
	case 8: music.openFromFile("Sounds\\Ambient.ogg"); break;
	case 9: music.openFromFile("Sounds\\Ambient3.ogg"); break;
	}
	if (lvlnum == 1 || lvlnum == 15 || lvlnum == 23 || lvlnum > 30) {
		music.openFromFile("Sounds\\Music of music.ogg");
	}
	music.play();
	logg << "Music reset \n";
}

void save_options() {
	logg << "Options are being saved \n";
	ofstream options;
	options.open("Options.txt");
	options << partnum << " " << levels_opened << " ";
	for (i = 0; i < 2; i++) {
		if (res_but[i].is_picked) {
			options << i << " ";
			break;
		}
	}
	options.close();
	logg << "Options are saved\n";
}

void rendpp() {
	playerS.setPosition(scale * (px - scrollx), scale * (py - scrolly));
	playerCol.setPosition((px - scrollx) * scale, (py - scrolly) * scale);
	playerCol.setScale(scale, scale);
	playerCol.scale(0.9, 0.9);
	eyes.setPosition((px - scrollx + ex) * scale, (py - scrolly + ey) * scale);
	playerS.setScale(scale, scale);
	eyes.setScale(playerS.getScale());
	switch (dir) {
	case 1: playerCol.move(0, -1); break;
	case 2: playerCol.move(1, -1); break;
	case 3: playerCol.move(1, 0); break;
	case 4: playerCol.move(1, 1); break;
	case 5: playerCol.move(0, 1); break;
	case 6: playerCol.move(-1, 1); break;
	case 7: playerCol.move(-1, 0); break;
	case 8: playerCol.move(-1, -1); break;
	}
}

void build(int l) {

	logg << "Level build " + l << "\n";

	helper = 0;
	coins_gathered = 0;
	coins_required = 0;
	helper_s[0] = 0;
	helper_s[1] = 0;
	save_options();
	for (i = 0; i < 5; i++) {
		dmode[i] = 0; crystal[i] = false;
		barmode[i] = 0;
	}
	for (i = 0; i < 3; i++) {
		tx[i] = -200; ty[i] = -200;
		spiral[i].setPosition(-300, -300);
	}
	alpha = 255; sy = 0; sx = 0;
	robot.stop(); story.stop();
	for (i = 0; i < 11; i++) {
		lmode[i] = false;
	}

	ifstream save;

	helper_string = lvl_name;

	if (l > 0) { helper_string = "Levels\\" + to_string(l) + ".txt"; }
	if (l == -2) {
		helper_string = "Saves\\" + lvl_name;
	}
	if (l == -1) {
		helper_string = "tutorial.txt";
		current_NPC = 0;
		robot.spawn();
		story.enabled = true; story.dialog = true; story.next_stage();
	}
	if (l == 2) { music.stop(); }
	save.open(helper_string);
	//cout << "Level's filename: " + helper_string << endl;

	for (i = 0; i < hei; i++) {
		for (j = 0; j < wid; j++) {
			for (k = 0; k < 2; k++) {
				save >> level[j][i][k][0];
				save >> level[j][i][k][1];
				save >> level[j][i][k][2];
				if (k == 0) {
					if (level[j][i][k][0] == 3) {
						coins_required++;
					}
				}
				if (k == 1) {
					if (level[j][i][1][0] == 8) {
						tx[level[j][i][1][2]] = j * 128 + 64;
						ty[level[j][i][1][2]] = i * 128 + 64;
						//cout << tx[0] << " " << ty[0] << endl;
						//cout << tx[1] << " " << ty[1] << endl;
						//cout << tx[2] << " " << ty[2] << endl;
					}
					if (33 > level[j][i][k][0] > 24) {
						level[j][i][k][2] = 0;
					}
				}
			}
		}
	}

	save >> px;
	save >> py;

	save.close();

	helper = 0;

	helper_string = lvl_name;

	if ((!onclick)&&(lvlnum == 1 || lvlnum == 2 || lvlnum == 15 || lvlnum == 16 || lvlnum == 23 || lvlnum == 24 || lvlnum == 31 )) {
		music.stop();
	}

	for (i = 0; i < wid; i++) {
		for (j = 0; j < hei; j++) {
			if (level[i][j][1][0] != 0) {
				helper_s[0] += i;
				helper += 1;
			}
		}
	}

	scrollx = px;

	scrolly = py;

	lvlnum = l;

	logg << "The level was built \n";

}

void clear_lvl() {
	for (i = 0; i < wid; i++) {
		for (j = 0; j < hei; j++) {
			for (int qqyyyq = 0; qqyyyq < 3; qqyyyq++) {
				level[i][j][0][qqyyyq] = 0;
				level[i][j][1][qqyyyq] = 0;
			}
		}
	}
}

Texture broken, smoke, Coin, dark, tp_p;
void smoke_spawn(string type) {

	logg << "Smoke spawn:" << type << "/n";;

	srand((unsigned)time(NULL));
	for (k = 0; k < 3; k++) {
		if (!particle[k][0].enabled) {
			for (int ws = 0; ws < partnum; ws++) {
				particle[k][ws].x = i * 128 + 64;
				particle[k][ws].y = j * 128 + 64;
				particle[k][ws].alpha = 255;
				particle[k][ws].enabled = true;
				particle[k][ws].rotation_speed = 0;
				particle[k][ws].particle.setRotation(rand() % 360);
			}
			if (type == "smoke") {
				for (int ws = 0; ws < partnum; ws++) {
					particle[k][ws].speed_x = 5 - rand() % 10;
					particle[k][ws].speed_y = 5 - rand() % 10;
					particle[k][ws].scale_speed = 1.04;
					particle[k][ws].alpha = 200;
					particle[k][ws].alpha_speed = -3;
					particle[k][ws].particle.setTexture(smoke);
					particle[k][ws].timer = 65;
					particle[k][ws].rotation_speed = 3 - rand() % 6;
					particle[k][ws].particle.setOrigin(128, 128);
					particle[k][ws].scale_ = 0.5;
					particle[k][ws].particle.setTextureRect(IntRect(0, 0, 256, 256));
				}
				}
			if (type == "cryst") {
				for (int ws = 0; ws < partnum; ws++) {
					particle[k][ws].speed_x = 2 - rand() % 4;
					particle[k][ws].speed_y = 2 - rand() % 4;
					particle[k][ws].scale_speed = 0.99;
					particle[k][ws].alpha_speed = -3;
					particle[k][ws].alpha = 155;
					particle[k][ws].particle.setTexture(broken);
					particle[k][ws].timer = 38;
					particle[k][ws].scale_ = 1;
					particle[k][ws].rotation_speed = 4 - rand() % 8;
					particle[k][ws].particle.setOrigin(rand() % 32, rand() % 32);
					particle[k][ws].y -= 10;
					particle[k][ws].particle.setTextureRect(IntRect(0, 0, 32, 32));
				}
			}
			if (type == "coin") {
				for (int ws = 0; ws < partnum; ws++) {
					particle[k][ws].speed_x = 0.6;
					particle[k][ws].speed_y = -0.6;
					if (ws != 1) { particle[k][ws].x = -100; particle[k][ws].y = -100; }
					particle[k][ws].scale_speed = 1.002;
					particle[k][ws].alpha_speed = -4;
					particle[k][ws].alpha = 255;
					particle[k][ws].particle.setTexture(Coin);
					particle[k][ws].timer = 60;
					particle[k][ws].scale_ = 1;
					particle[k][ws].rotation_speed = 0;
					particle[k][ws].particle.setOrigin(32, 32);
					particle[k][ws].particle.setRotation(0);
					particle[k][ws].y -= 10;
					particle[k][ws].particle.setTextureRect(IntRect(0, 0, 64, 64));
				}
				}
			if (type == "dark") {
				for (int ws = 0; ws < partnum; ws++) {
					particle[k][ws].particle.setTexture(dark);
					particle[k][ws].speed_x = 1 - rand() % 3;
					particle[k][ws].speed_y = 1 - rand() % 3;
					particle[k][ws].scale_speed = 1.04;
					particle[k][ws].alpha = 61;
					particle[k][ws].alpha_speed = -1;
					particle[k][ws].scale_ = 2.6;
					particle[k][ws].timer = 60;
					particle[k][ws].rotation_speed = rand() % 3;
					particle[k][ws].particle.setOrigin(40, 40);
					particle[k][ws].particle.setTextureRect(IntRect(0, 0, 100, 100));
				}
				}
			if (type == "tp") {
				for (int ws = 0; ws < partnum; ws++) {
					particle[k][ws].particle.setTexture(tp_p);
					particle[k][ws].speed_x = 2 - rand() % 5;
					particle[k][ws].speed_y = 2 - rand() % 5;
					particle[k][ws].scale_speed = 0.99;
					particle[k][ws].alpha = 128;
					particle[k][ws].alpha_speed = -2;
					particle[k][ws].scale_ = float(rand() % 20) / 10;
					particle[k][ws].timer = 60;
					particle[k][ws].rotation_speed = 3 - rand() % 7;
					particle[k][ws].particle.setOrigin(64, 64);
					particle[k][ws].particle.setTextureRect(IntRect(0, 0, 128, 128));
				}
			}
			if (type == "mech") {
				for (int ws = 0; ws < partnum; ws++) {
					particle[k][ws].particle.setTexture(dark);
					particle[k][ws].speed_x = 3 - rand() % 7;
					particle[k][ws].speed_y = 3 - rand() % 7;
					particle[k][ws].scale_speed = 1.015;
					particle[k][ws].alpha = 128;
					particle[k][ws].alpha_speed = -2;
					particle[k][ws].scale_ = 0.8;
					particle[k][ws].timer = 60;
					particle[k][ws].rotation_speed = 3 - rand() % 7;
					particle[k][ws].particle.setOrigin(64, 64);
					particle[k][ws].particle.setTextureRect(IntRect(128 * level[i][j][0][2], 0, 128, 128));
				}
			}
			break;
		}
	}
}

void place(int i, int j, int k) {
	if (level[i][j][k][0] < 0) {
		level[i][j][k][0] = 0;
	}
	if (level[i][j][k][0] != 1) {
		level[i][j][k][1] = 0;
		level[i][j][k][2] = 0;
	}
	if (((level[i][j][k][0] == 1) && (k == 0)) || (((level[i][j][k][0] == 9) || ((level[i][j][k][0] == 12))) && (k == 1))) {
		level[i][j][k][2] = rand() % 6;
		switch (rand() % 4)
		{
		case 0: level[i][j][k][1] = 0; break;
		case 1: level[i][j][k][1] = 90; break;
		case 2: level[i][j][k][1] = 180; break;
		case 3: level[i][j][k][1] = 270; break;
		}
	}
	if ((level[i][j][k][0] == 5) && (k == 0)) {
		switch (editor_choose) {
		case 1: level[i][j][k][1] = 180; break;
		case 2: level[i][j][k][1] = 0; break;
		case 3: level[i][j][k][1] = 90; break;
		case 4: level[i][j][k][1] = 270; break;
			deafult: level[i][j][k][1] = 0; break;
		}
	}
	if ((level[i][j][k][0] == 1) && (k == 1)) {
		switch (rand() % 4)
		{
		case 0: level[i][j][1][1] = 0; break;
		case 1: level[i][j][1][1] = 90; break;
		case 2: level[i][j][1][1] = 180; break;
		case 3: level[i][j][1][1] = 270; break;
		}
	}
	if ((level[i][j][k][0] == 10) && (k == 1)) {
		if (editor_choose == 2) {
			level[i][j][k][2] = 1;
		}
		else {
			level[i][j][k][2] = 0;
		}
	}

	if (((level[i][j][k][0] == 3) || (level[i][j][k][0] == 4) || (level[i][j][k][0] == 6) || (level[i][j][k][0] == 18) || (level[i][j][k][0] == 19) || (level[i][j][k][0] == 20)) && (k == 0)) {
		switch (editor_choose)
		{
		case 1: level[i][j][k][2] = 1; break;
		case 2: level[i][j][k][2] = 3; break;
		case 3: level[i][j][k][2] = 2; break;
		case 4: level[i][j][k][2] = 0; break;
		}
	}
}

void rech2(int x, int y, int xx, int yy) {

	if (level[xx][yy][1][2] == level[x][y][1][2]) { return; }

	if (level[xx][yy][1][2] > level[x][y][1][2]) {
		level[x][y][1][2]++;
		level[xx][yy][1][2] -= 1;

		if (level[xx][yy][1][2] > level[x][y][1][2] + 1) {
			level[x][y][1][2] += 2;
			level[xx][yy][1][2] -= 2;
		}
		return;
	}

	level[x][y][1][2] -= 1;
	level[xx][yy][1][2] ++;
	if (level[xx][yy][1][2] + 1 > level[x][y][1][2]) { return; }
	level[x][y][1][2] -= 2;
	level[xx][yy][1][2] += 2;

}

void rech(int x, int y) {

	if (level[x][y][1][0] > 32) {
		return;
	}

	if (level[i][j][1][2] == level[x][y][1][2]) { return; }

	if (level[i][j][1][2] > level[x][y][1][2]) {
		level[x][y][1][2]++;
		level[i][j][1][2] -= 1;

	if (level[i][j][1][2] > level[x][y][1][2] + 1) {
		level[x][y][1][2] += 2;
		level[i][j][1][2] -= 2;
	}
		return;
	}

	level[x][y][1][2] -= 1;
	level[i][j][1][2] ++;
	if (level[i][j][1][2] + 1 > level[x][y][1][2]) { return; }
	level[x][y][1][2] -= 2;
	level[i][j][1][2] += 2;
}

void save(string filename_) {

	logg << " saving...\n";

	ofstream saver;

	saver.open("Saves\\" + filename_);

	for (i = 0; i < hei; i++) {
		for (j = 0; j < wid; j++) {
			for (k = 0; k < 2; k++) {
				for (helper = 0; helper < 3; helper++) {
					saver << level[j][i][k][helper] << " ";
				}
			}
		}
	}

	saver << px << " " << py << " ";

	saver.close();

	helper = 0;
	helper_string = lvl_name;

	logg << "Saved \n";

}

#include <filesystem>

void check_saves() {

	logg << "Searching for levels... \n";

	ofstream saves;
	saves.open("saves.txt");

	j = 0;

	WIN32_FIND_DATA FindFileData;
	HANDLE hf;
	hf = FindFirstFile(TEXT("Saves\\*"), &FindFileData);
	if (hf != INVALID_HANDLE_VALUE)
	{
		while (FindNextFile(hf, &FindFileData) != 0)
		{
			wstring ws = (FindFileData.cFileName);
			saves << string(ws.begin(), ws.end()) << endl;
			logg << string(ws.begin(), ws.end()) << endl;
			j++;

		} 
		FindClose(hf);
	}

	saves.close();
	logg << "Searching for levels complete \n";
}

void checkpoint() {

	logg << "Checkpoint \n";

	ofstream saver;

	saver.open("checkpoint.txt");

	for (i = 0; i < hei; i++) {
		for (j = 0; j < wid; j++) {
			for (k = 0; k < 2; k++) {
				for (helper = 0; helper < 3; helper++) {
					saver << level[j][i][k][helper] << " ";
				}
			}
		}
	}

	saver << int(px) << " " << int(py) << " " << coins_gathered << " " << coins_required << " ";

	saver << story.stage << " " << int(robot.x) << " " << int(robot.y) << " ";

	for (i = 0; i < 3; i++) {
		if (dmode[i]) { saver << 1 << " "; }
		else { saver << 0 << " "; }
		if (crystal[i]) { saver << 1 << " "; }
		else { saver << 0 << " "; }
	}

	saver << lvlnum;

	saver.close();

	helper = 0;

	logg << "Checkpoint was set \n";
}

void load_checkpoint() {

	logg << "loading checkpoint \n";

	ifstream save;
	save.open("checkpoint.txt");

	for (i = 0; i < hei; i++) {
		for (j = 0; j < wid; j++) {
			for (k = 0; k < 2; k++) {
				save >> level[j][i][k][0];
				save >> level[j][i][k][1];
				save >> level[j][i][k][2];
				if (k == 1) {
					if (level[j][i][1][0] == 8) {
						tx[level[j][i][1][2]] = j * 128 + 64;
						ty[level[j][i][1][2]] = i * 128 + 64;
						//cout << tx[0] << " " << ty[0] << endl;
						//cout << tx[1] << " " << ty[1] << endl;
						//cout << tx[2] << " " << ty[2] << endl;
					}
				}
			}
		}
	}

	save >> px;
	save >> py;
	save >> coins_gathered;
	save >> coins_required;

	save >> story.stage;
	save >> robot.x;
	save >> robot.y;

	for (i = 0; i < 3; i++) {
		save >> helper;
		if (helper == 1) { dmode[i] = true; }
		else { dmode[i] = false; }
		save >> helper;
		if (helper == 1) { crystal[i] = true; }
		else { crystal[i] = false; }
	}

	alpha = 100;

	save >> lvlnum;

	save.close();

	helper = 0;

	logg << "Success! \n";

}

void loading() {
	if (true) {

		logg << "loading \n";

		ifstream options;
		options.open("Options.txt");
		options >> partnum;
		options >> levels_opened;
		options >> helper_s2;
		options.close();
		particles_option.currentval = partnum;

		//somethings
		eyes_t.loadFromFile("Textures\\Eyes.png");
		body_t.loadFromFile("Textures\\Player.png");
		font.loadFromFile("Textures\\Text.ttf");
		text.setFont(font);
		Coin.loadFromFile("Textures\\Coin.png");
		smoke.loadFromFile("Textures\\Smoke.png");
		broken.loadFromFile("Textures\\Broken.png");
		cur.loadFromFile("Textures\\Choose.png");
		dark.loadFromFile("Textures\\Dark.png");

		cursor.setTexture(cur);
		cursor.setOrigin(8, 8);

		spir.loadFromFile("Textures\\Spiral.png");
		for (i = 0; i < 3; i++) {
			spiral[i].setTexture(spir);
			spiral[i].setOrigin(128, 128);
			spiral[i].setTextureRect(IntRect(0, 0, 256, 256));
			spiral[i].setRotation(rand() % 360);
		}
		spiral[0].setColor(Color::Blue);
		spiral[1].setColor(Color::Red);
		spiral[2].setColor(Color::Magenta);
		inter.loadFromFile("Textures\\Int.png");
		blend.setPosition(0, 0);
		blend.setTexture(cur);
		blend.setTextureRect(IntRect(0, 0, screenw, screenh));
		lev.loadFromFile("Textures\\Levels.png");
		lvl_but.setTexture(lev); lvl_but.setOrigin(64, 64);

		dialog_NPC.setFont(font); dialog1.setFont(font); dialog2.setFont(font); dialog3.setFont(font);
		dialog_NPC.setCharacterSize(UI_scale * 70); dialog1.setCharacterSize(UI_scale * 65); dialog2.setCharacterSize(UI_scale * 65); dialog3.setCharacterSize(UI_scale * 65);
		dialog_NPC.setFillColor(Color::Color(12, 12, 150, 200)); dialog1.setFillColor(dialog_NPC.getFillColor()); dialog2.setFillColor(dialog1.getFillColor()); dialog3.setFillColor(dialog1.getFillColor());
		dialog_but.setTexture(diabut); dialog_but.setTextureRect(IntRect(0, 0, 724, 128));
		diabut.loadFromFile("Textures\\Dialog.png");

		t_ach.loadFromFile("Textures\\Ach.png");
		t_editor.loadFromFile("Textures\\Editor.png");
		t_exit.loadFromFile("Textures\\Exit.png");
		t_levels.loadFromFile("Textures\\Lvls.png");
		t_options.loadFromFile("Textures\\Options.png");
		t_play.loadFromFile("Textures\\Play.png");

		b_levels.init(t_levels, screenw/4, screenh / 2, " ", 0);
		b_play.init(t_play, screenw / 2, screenh / 2, " ", 1);
		b_editor.init(t_editor, screenw / 4 * 3, screenh / 2, " ", 2);
		b_ach.init(t_ach, screenw / 4, screenh / 3.7 * 3, " ", 3);
		b_options.init(t_options, screenw / 2, screenh / 3.7 * 3, " ", 4);
		b_exit.init(t_exit, screenw / 4 * 3, screenh / 3.7 * 3, " ", 5);


		//blocks
		block.setOrigin(64, 64);
		blue.loadFromFile("Textures\\Blue.png");
		el.loadFromFile("Textures\\Floor.png");
		switch_t.loadFromFile("Textures\\switch.png");
		door.loadFromFile("Textures\\Electric trap.png");
		barr.loadFromFile("Textures\\Barrier.png");
		smth.loadFromFile("Textures\\Something.png");
		smth2.loadFromFile("Textures\\Something2.png");
		smth3.loadFromFile("Textures\\Something3.png");
		str.loadFromFile("Textures\\Stripes.png");
		corn.loadFromFile("Textures\\Corner.png");
		movable.loadFromFile("Textures\\Box.png");
		finon.loadFromFile("Textures\\Finish(ON).png");
		finoff.loadFromFile("Textures\\Finish(OFF).png");
		wall.loadFromFile("Textures\\Wall.png");
		bg.loadFromFile("Textures\\BG.png");
		port_in.loadFromFile("Textures\\Portal.png");
		port_o.loadFromFile("Textures\\Portal_out.png");
		gray.loadFromFile("Textures\\Gray.png");
		gr.loadFromFile("Textures\\Gr.png");
		grb.loadFromFile("Textures\\Grb.png");
		strp.loadFromFile("Textures\\StripesP.png");
		differ.loadFromFile("Textures\\Different.png");
		rb.loadFromFile("Textures\\RB.png");
		rose.loadFromFile("Textures\\Rose.png");
		green.loadFromFile("Textures\\Green.png");
		greenf.loadFromFile("Textures\\GreenFloor.png");
		greenw.loadFromFile("Textures\\GreenWall.png");
		roseb.loadFromFile("Textures\\RoseBricks.png");
		greenf2.loadFromFile("Textures\\GreenFloor2.png");
		fl.loadFromFile("Textures\\Flowers.png");
		crys.loadFromFile("Textures\\Crystal.png");
		act.loadFromFile("Textures\\GreenSwitch.png");
		bar2.loadFromFile("Textures\\Barrier2.png");
		gravs.loadFromFile("Textures\\gravS.png");
		grav.loadFromFile("Textures\\grav.png");
		buttonblock.loadFromFile("Textures\\Button.png");
		noth.loadFromFile("Textures\\Nothing.png");
		glass.loadFromFile("Textures\\Glass.png");
		_arrow.loadFromFile("Textures\\Arrow.png");
		white.loadFromFile("Textures\\White.png");
		rosebut.loadFromFile("Textures\\RoseBut.png");
		whitewall.loadFromFile("Textures\\White_wall.png");
		rose_door.loadFromFile("Textures\\Rose_door.png");
		barrier_r.loadFromFile("Textures\\RoseBarrier.png");
		gray2.loadFromFile("Textures\\Gray2.png");
		rosefloor2.loadFromFile("Textures\\Blue_floor2.png");
		blue_floor2.loadFromFile("Textures\\Rosefloor2.png");
		rosebricks.loadFromFile("Textures\\RoseBricks2.png");
		lamp_blue.loadFromFile("Textures\\Lamp_blue.png");
		lamp_green.loadFromFile("Textures\\Lamp_barrier.png");
		speed_det.loadFromFile("Textures\\Speed_detector.png");
		portal2.loadFromFile("Textures\\Portal2.png");
		view_det.loadFromFile("Textures\\View_detector.png");
		player_det.loadFromFile("Textures\\Player_detector.png");
		player_det_inv.loadFromFile("Textures\\Player_detector_inv.png");
		pusher.loadFromFile("Textures\\Pusher.png");
		some_help.loadFromFile("Textures\\Some_help.png");
		tp_p.loadFromFile("Textures\\tp.png");

		c_minus.loadFromFile("Textures\\Electricity\\-.png");
		c_plus.loadFromFile("Textures\\Electricity\\+.png");
		c_ac.loadFromFile("Textures\\Electricity\\Acc.png");
		BGel.loadFromFile("Textures\\Electricity\\BG.png");
		c_corner.loadFromFile("Textures\\Electricity\\C corner.png");
		c_cross3.loadFromFile("Textures\\Electricity\\C cross 3.png");
		c_cross4.loadFromFile("Textures\\Electricity\\C cross 4.png");
		c_one.loadFromFile("Textures\\Electricity\\C 1.png");
		c_line.loadFromFile("Textures\\Electricity\\C linear.png");
		c_resist.loadFromFile("Textures\\Electricity\\Resistor.png");
		c_sensor.loadFromFile("Textures\\Electricity\\Sensor_off.png");
		c_sensor_on.loadFromFile("Textures\\Electricity\\Sensor_on.png");
		c_switch.loadFromFile("Textures\\Electricity\\Switch_on.png");
		c_switch_off.loadFromFile("Textures\\Electricity\\Switch_off.png");
		c_switcher3.loadFromFile("Textures\\Electricity\\Switcher_main.png");
		void_el.loadFromFile("Textures\\Electricity\\void.png");

		//sounds
		scl.loadFromFile("Sounds\\S1.ogg");
		push.loadFromFile("Sounds\\Push.ogg");
		coin_s.loadFromFile("Sounds\\Coin.ogg");
		boom.loadFromFile("Sounds\\Boom.ogg");
		cryst.loadFromFile("Sounds\\crys.ogg");
		s1.setBuffer(scl); sound_p.setBuffer(push); sound_c.setBuffer(coin_s); sound_b.setBuffer(boom); _break.setBuffer(cryst);
		telep.loadFromFile("Sounds\\Tp.ogg"); tp_s.setBuffer(telep);
		voice.setBuffer(voice_f);
		arpush.loadFromFile("Sounds\\Arrow_move.ogg");
		sound_ab.setBuffer(arpush);
		elbuf.loadFromFile("Sounds\\Electro.ogg");
		electro.setBuffer(elbuf);

		tp1_buf.loadFromFile("Sounds\\tp1.ogg"); tp1.setBuffer(tp1_buf);
		tp2_buf.loadFromFile("Sounds\\tp2.ogg"); tp2.setBuffer(tp2_buf);
		push2_buf.loadFromFile("Sounds\\push2.ogg"); push2.setBuffer(push2_buf);
		open_buf.loadFromFile("Sounds\\Open.ogg"); open.setBuffer(open_buf);

		//NPCs
		robot_t.loadFromFile("Textures\\robot.png");
		NPC.setOrigin(37.5, 37.5);

		playerS.setTexture(body_t);
		eyes.setTexture(eyes_t);
		playerS.setOrigin(37.5, 37.5); eyes.setOrigin(37.5, 37.5);
		playerCol.setTextureRect(playerS.getTextureRect());
		playerCol.setOrigin(playerS.getOrigin());
		blink.loadFromFile("Textures\\Blink.png");

		playerS.setPosition(50, 50);


		build(-1);

		dialog_but.setOrigin(362, 64); dialog_but.setScale(UI_scale, UI_scale * 0.8);
		box_t.loadFromFile("Textures\\Context_menu.png"); box_but_t.loadFromFile("Textures\\ContextMenuB.png");
		box.setTexture(box_t); box_but.setTexture(box_but_t);
		box.setScale(UI_scale, UI_scale); box_but.setScale(UI_scale, UI_scale * 0.8);
		box.setOrigin(320, 0); box.setPosition(screenw / 2, 50 * UI_scale);
		box_but.setOrigin(256, 64);

		particles_option.init();

		reloader.type = 0;
		reloader.text_ = "Change screen resolution";
		reloader.init();

		auto_reloader.type = 1;
		auto_reloader.text_ = "Smart screen setting";
		auto_reloader.init();

		screenmode.init(600 * UI_scale,100 * UI_scale);

		i = 0;
		res_but[0].init(1920, 1080);
		i = 1;
		res_but[1].init(1280, 720);

		res_but[int(helper_s2)].is_picked = true;

		ach_text.setFont(font);
		ach_bg.loadFromFile("Textures\\AchievementBG.png");
		ach_BG.setTexture(ach_bg);
		ach_text.setCharacterSize(30);

		e_B.loadFromFile("Textures\\Editor_UI\\B.png");
		e_E.loadFromFile("Textures\\Editor_UI\\E.png");
		e_Gr.loadFromFile("Textures\\Editor_UI\\G.png");
		e_Gy.loadFromFile("Textures\\Editor_UI\\Gy.png");
		e_L.loadFromFile("Textures\\Editor_UI\\Layers.png");
		e_L2.loadFromFile("Textures\\Editor_UI\\Layers2.png");
		e_load.loadFromFile("Textures\\Editor_UI\\Load.png");
		e_M.loadFromFile("Textures\\Editor_UI\\M.png");
		e_Mech.loadFromFile("Textures\\Editor_UI\\Mech.png");
		e_next.loadFromFile("Textures\\Editor_UI\\Next.png");
		e_play.loadFromFile("Textures\\Editor_UI\\Play.png");
		e_player.loadFromFile("Textures\\Editor_UI\\Player.png");
		e_plus.loadFromFile("Textures\\Editor_UI\\Plus.png");
		e_prev.loadFromFile("Textures\\Editor_UI\\Previous.png");
		e_Q.loadFromFile("Textures\\Editor_UI\\Q.png");
		e_R.loadFromFile("Textures\\Editor_UI\\R.png");
		e_Rt.loadFromFile("Textures\\Editor_UI\\Rotate.png");
		e_save.loadFromFile("Textures\\Editor_UI\\Save.png");
		trash.loadFromFile("Textures\\Editor_UI\\Trash.png");

		E_plus.init(e_plus, 0, 0, " ", 0);
		E_Rt.init(e_Rt, 128 * UI_scale, 0, " ", 1);
		E_M.init(e_M, 256 * UI_scale, 0, " ", 2);
		E_player.init(e_player, 384 * UI_scale, 0, " ", 3);
		E_play.init(e_play, screenw - 384 * UI_scale, 0, " ", 4);
		E_save.init(e_save, screenw - 256 * UI_scale, 0, " ", 5);
		E_load.init(e_load, screenw - 128 * UI_scale, 0, " ", 6);
		E_prev.init(e_prev, 0, screenh - 128 * UI_scale, " ", 7);
		E_next.init(e_next, 128 * UI_scale, screenh - 128 * UI_scale, " ", 8);
		E_L.init(e_L, 0, screenh - 256 * UI_scale, " ", 9);
		E_L2.init(e_L2, 128 * UI_scale, screenh - 256 * UI_scale, " ", 10);
		E_B.init(e_B, 256 * UI_scale, screenh - 256 * UI_scale, " ", 11);
		E_R.init(e_R, 384 * UI_scale, screenh - 256 * UI_scale, " ", 12);
		E_Gr.init(e_Gr, 512 * UI_scale, screenh - 256 * UI_scale, " ", 13);
		E_Gy.init(e_Gy, 640 * UI_scale, screenh - 256 * UI_scale, " ", 14);
		E_Mech.init(e_Mech, 768 * UI_scale, screenh - 256 * UI_scale, " ", 15);
		E_Q.init(e_Q, 896 * UI_scale, screenh - 256 * UI_scale, " ", 16);
		E_E.init(e_E, 1024 * UI_scale, screenh - 256 * UI_scale, " ", 17);
		E_trash.init(trash, 0, 0, "", 18);
		E_next_page.init(e_next, screenw / 2 + UI_scale * 190, screenh * 0.75, " ", 19); E_next_page.box.scale(0.75, 0.75);
		E_prev_page.init(e_prev, screenw / 2 - UI_scale * 285, screenh * 0.75, " ", 20); E_prev_page.box.scale(0.75, 0.75);

		E_plus.is_checked = true;
		E_L.is_checked = true;
		E_B.is_checked = true;

	}
}

void achievement(int _lvl, int _n) {

	ifstream ach;
	ach.open("Achievements\\" + to_string(_lvl) + " " + to_string(_n) + ".txt");

	ach >> helper;
	if (helper > 0) { return; }
	
	ach >> helper_string;
	ach.close();

	ach_t.loadFromFile("Achievements\\Textures\\" + to_string(_lvl) + " " + to_string(_n) + ".png");
	ach_box.setTexture(ach_t);
	ach_text.setString(helper_string);
	ach_text.setFillColor(Color::White);
	
	ach_b.loadFromFile("Achievements\\Sounds\\" + to_string(_lvl) + " " + to_string(_n) + ".ogg");
	ach_s.setBuffer(ach_b);

	ofstream achr;
	achr.open("Achievements\\" + to_string(_lvl) + " " + to_string(_n) + ".txt");

	achr << 1;
	achr << " ";
	achr << helper_string;
	achr.close();

	achieve_fade = 1400;
	ach_s.play();

}

void render_block_el0() {
	block.setTextureRect(IntRect(0, 0, 128, 128));
	block.setPosition((i * 128 + 64 - scrollx) * scale, (j * 128 + 64 - scrolly) * scale);
	block.setRotation(level[i][j][0][1]);
	switch (level[i][j][0][0]) {
	case 1:
		block.setTexture(blue);
		block.setTextureRect(IntRect(level[i][j][0][2] * 128, 0, 128, 128));
		break;
	case 2:
		block.setTexture(switch_t);
		block.setTextureRect(IntRect(0, 0, 128, 128));
		text.setFillColor(Color::White);
		text.setPosition(block.getPosition());
		if (level[i][j][0][2] < 5) {
			text.setString(to_string(level[i][j][0][2] + 1));
		}
		else {
			text.setString("Error");
		}
		text.move(scale, scale);
		break;
	case 3:
		//the secret coin
		block.setTexture(smth);
		block.setTextureRect(IntRect(level[i][j][0][2] * 128, 0, 128, 128));
		text.setPosition(block.getPosition());
		text.setFillColor(Color::White);
		text.setString("coin");
		text.move(scale * -60, scale * -40);
		break;
	case 4:
		//the secret (gathered)
		block.setTexture(smth2);
		block.setTextureRect(IntRect(level[i][j][0][2] * 128, 0, 128, 128));
		break;
	case 5:
		//movable
		block.setTexture(movable);
		block.setTextureRect(IntRect(level[i][j][0][1] * 1.422, 0, 128, 128));
		break;
	case 6:
		//the secret (smoke)
		block.setTexture(smth);
		block.setTextureRect(IntRect(level[i][j][0][2] * 128, 0, 128, 128));
		text.setPosition(block.getPosition());
		text.setFillColor(Color::Red);
		text.setString("boom");
		text.move(scale * -60, scale * -40);
		break;
	case 7:
		//the wall
		block.setTexture(wall);
		break;
	case 8:
		//gray
		block.setTexture(gr);
		break;
	case 9:
		//gray_wall
		block.setTexture(grb);
		break;
	case 10:
		//greenn
		block.setTexture(green);
		break;
	case 11:
		//green_wall
		block.setTexture(greenw);
		break;
	case 12:
		block.setTexture(roseb);
		block.setTextureRect(IntRect(level[i][j][0][2] * 128, 0, 128, 128));
		break;
	case 13:
		block.setTexture(rb);
		break;
	case 14:
		block.setTexture(fl);
		block.setTextureRect(IntRect(level[i][j][0][2] * 128, 0, 128, 128));
		break;
	case 15:
		block.setTexture(crys);
		text.setFillColor(Color::Color(0,0,0,255));
		text.setPosition(block.getPosition());
		if (level[i][j][0][2] < 5) {
			text.setString(to_string(level[i][j][0][2] + 1));
		}
		else {
			text.setString("Error");
		}
		text.move(scale, scale);
		break;
	case 16:
		block.setTextureRect(IntRect(level[i][j][0][2] * 128, 0, 128, 128));
		block.setTexture(gravs);
		/*block.rotate(tick % 360);*/
		window.draw(block);
		/*block.rotate(-tick % 360);*/
		block.setTexture(grav);
		break;
	case 17:
		block.setTexture(glass);
		block.setTextureRect(IntRect(level[i][j][0][2] * 128, 0, 128, 128));
		break;
	case 18:
		//hidden block
		block.setTexture(smth);
		block.setTextureRect(IntRect(level[i][j][0][2] * 128, 0, 128, 128));
		text.setPosition(block.getPosition());
		text.setFillColor(Color::White);
		text.setString("Rose");
		text.move(scale * -60, scale * -40);
		break;
	case 19:
		//Closer
		block.setTexture(smth);
		block.setTextureRect(IntRect(level[i][j][0][2] * 128, 0, 128, 128));
		text.setPosition(block.getPosition());
		text.setFillColor(Color::White);
		text.setString("Closer");
		text.move(scale * -60, scale * -40);
		break;
	case 20:
		//Opener
		block.setTexture(smth);
		block.setTextureRect(IntRect(level[i][j][0][2] * 128, 0, 128, 128));
		text.setPosition(block.getPosition());
		text.setFillColor(Color::White);
		text.setString("Opener");
		text.move(scale * -60, scale * -40);
		break;
	case 21:
		block.setTexture(smth3); break;
	case 22:
		block.setTexture(white); break;
	case 23:
		block.setTexture(whitewall); break;
	case 24:
		block.setTexture(rosebut);
		text.setFillColor(Color::Color(255, 0, 0, 255));
		text.setPosition(block.getPosition());
		if (level[i][j][0][2] < 11) {
			text.setString(to_string(level[i][j][0][2] + 1));
		}
		else {
			text.setString("Error");
		}
		text.move(scale, scale);
		break;
	case 25:
		block.setTexture(rosebricks); break;
	case 26:
		block.setTexture(player_det);
		text.setFillColor(Color::White);
		text.setPosition(block.getPosition());
		if (level[i][j][0][2] < 5) {
			text.setString(to_string(level[i][j][0][2] + 1));
		}
		else {
			text.setString("Error");
		}
		text.move(scale, scale);
		break;
	case 27:
		block.setTexture(player_det_inv);
		text.setFillColor(Color::White);
		text.setPosition(block.getPosition());
		if (level[i][j][0][2] < 5) {
			text.setString(to_string(level[i][j][0][2] + 1));
		}
		else {
			text.setString("Error");
		}
		text.move(scale, scale);
		break;
	case 28:
		block.setTexture(view_det);
		text.setFillColor(Color::Blue);
		text.setPosition(block.getPosition());
		if (level[i][j][0][2] < 5) {
			text.setString(to_string(level[i][j][0][2] + 1));
		}
		else {
			text.setString("Error");
		}
		text.move(scale, scale);
		break;
	case 29:
		block.setTexture(pusher);
		if (level[i][j][0][2] > 0) {
			level[i][j][0][2] -= 1;
			block.setTextureRect(IntRect(128, 0, 128, 128));
			break;
		}
		block.setTextureRect(IntRect(0, 0, 128, 128));
		break;
	case 30:
		block.setTexture(speed_det);
		text.setFillColor(Color::Blue);
		text.setPosition(block.getPosition());
		if (level[i][j][0][2] < 5) {
			text.setString(to_string(level[i][j][0][2] + 1));
		}
		else {
			text.setString("Error");
		}
		text.move(scale, scale);
		break;
	case 31:
		block.setTexture(glass);
		text.setFillColor(Color::White);
		text.setPosition(block.getPosition());
		if (level[i][j][0][2] < 5) {
			text.setString("A " + to_string(level[i][j][0][2] + 1));
		}
		else {
			text.setString("Error");
		}
		text.move(scale, scale);
		break;
	case 99:
		block.setTexture(bar2); break;
	case 100:
		//this is a barrier
		block.setTexture(barr);
		if (level[i][j][1][0] != 2) { level[i][j][0][0] == 0; }
		break;
	case 101:
		//this is a barrier 2
		block.setTexture(barr);
		if (level[i][j][1][0] != 3) { level[i][j][0][0] == 0; }
		break;
	case 102:
		//rose barrier
		block.setTexture(barrier_r); break;
	case 103:
		block.setTexture(barrier_r); break;
	}
	if (level[i][j][0][0] != 0) { window.draw(block); window.draw(text); }
}

void render_block_el1() { 
	block.setPosition((i * 128 + 64 - scrollx) * scale, (j * 128 + 64 - scrolly) * scale);
	if (level[i][j][1][0] != 0) {
		block.setRotation(level[i][j][1][1]);
		block.setTextureRect(IntRect(level[i][j][1][2] * 128, 0, 128, 128));
	}
	switch (level[i][j][1][0]) {
	case 0:
		block.setTexture(glass);
		block.setColor(Color::Color(255, 255, 255, 50));
		break;
	case 1:
		block.setTexture(el);
		break;
	case 2:
		block.setTexture(door);
		block.setTextureRect(IntRect(0, 0, 128, 128));
		text.setFillColor(Color::White);
		text.setPosition(block.getPosition());
		if (level[i][j][0][2] < 5) {
			text.setString(to_string(level[i][j][1][2] + 1));
		}
		else {
			text.setString("Error");
		}
		text.move(scale, scale);
		break;
	case 3:
		block.setTexture(door);
		block.setTextureRect(IntRect(128, 0, 128, 128));
		text.setFillColor(Color::White);
		text.setPosition(block.getPosition());
		if (level[i][j][0][2] < 5) {
			text.setString(to_string(level[i][j][1][2] + 1));
		}
		else {
			text.setString("Error");
		}
		text.move(scale, scale);
		break;
	case 4:
		block.setTexture(str);
		break;
	case 5:
		block.setTexture(corn);
		break;
	case 6:
		block.setTexture(finon);
		break;
	case 7:
		block.setTexture(port_in);
		text.setPosition(block.getPosition().x, block.getPosition().y);
		block.setTextureRect(IntRect(0, 0, 128, 128));
		text.setString("Error"); text.setFillColor(Color::Red);
		switch (level[i][j][1][2]) {
		case 0: text.setString("1"); break;
		case 1: text.setString("2"); break;
		case 2: text.setString("3"); break;
		}
		break;
	case 8:
		block.setTexture(port_o);
		text.setPosition(block.getPosition().x, block.getPosition().y);
		text.setString("Error"); text.setFillColor(Color::Red);
		switch (level[i][j][1][2]) {
		case 0: text.setString("1"); break;
		case 1: text.setString("2"); break;
		case 2: text.setString("3"); break;
		}
		break;
	case 9:
		block.setTexture(gray); break;
	case 10:
		block.setTexture(strp); break;
	case 11:
		block.setTexture(greenf); break;
	case 12:
		block.setTexture(differ); break;
	case 13:
		block.setTexture(rose); break;
	case 14:
		block.setTexture(greenf2); break;
	case 15:
		block.setTexture(act);
		block.setTextureRect(IntRect(0, 0, 128, 128));
		text.setPosition(block.getPosition());
		text.setFillColor(Color::Color(0, 0, 0, 255));
		text.setPosition(block.getPosition());
		if (level[i][j][0][2] < 5) {
			text.setString(to_string(level[i][j][1][2] + 1));
		}
		else {
			text.setString("Error");
		}
		text.move(scale, scale);
		break;
	case 16:
		block.setTexture(act);
		block.setTextureRect(IntRect(128, 0, 128, 128));
		text.setPosition(block.getPosition());
		text.setFillColor(Color::Color(0, 0, 0, 255));
		text.setPosition(block.getPosition());
		if (level[i][j][0][2] < 5) {
			text.setString(to_string(level[i][j][1][2] + 1));
		}
		else {
			text.setString("Error");
		}
		text.move(scale, scale);
		break;
	case 17:
		block.setTexture(buttonblock);
		block.setTextureRect(IntRect(0, 0, 128, 128));
		text.setFillColor(Color::White);
		text.setPosition(block.getPosition());
		if (level[i][j][0][2] < 11) {
			text.setString(to_string(level[i][j][1][2] + 1));
		}
		else {
			text.setString("Error");
		}
		text.move(scale, scale);
		break;
	case 18:
		block.setRotation(level[i][j][1][1]);
		switch (level[i][j][1][2]) {
		case 1:
			block.setTexture(blue);
			block.setTextureRect(IntRect(0, 0, 128, 128));
			break;
		case 2:
			block.setTexture(switch_t);
			block.setTextureRect(IntRect(0, 0, 128, 128));
			break;
		case 3:
			//the secret coin
			block.setTexture(smth);
			break;
		case 4:
			//the secret (gathered)
			block.setTexture(smth2);
			break;
		case 5:
			//movable
			block.setTexture(movable);
			break;
		case 6:
			//the secret (smoke)
			block.setTexture(smth);
			break;
		case 7:
			//the wall
			block.setTexture(wall);
			break;
		case 8:
			//gray
			block.setTexture(gr);
			break;
		case 9:
			//gray_wall
			block.setTexture(grb);
			break;
		case 10:
			//greenn
			block.setTexture(green);
			break;
		case 11:
			//green_wall
			block.setTexture(greenw);
			break;
		case 12:
			block.setTexture(roseb);
			break;
		case 13:
			block.setTexture(rb);
			break;
		case 14:
			block.setTexture(fl);
			break;
		case 15:
			block.setTexture(crys); break;
		case 16:
			/*block.setTexture(gravs);
			window.draw(block);*/
			block.setTexture(grav);
			break;
		}
		block.setTextureRect(IntRect(0, 0, 128, 128));
		window.draw(block);
		block.setColor(Color::Color(255, 255, 255, 128));
		block.setTexture(noth);
		break;
	case 19:
		block.setTexture(_arrow);
		block.setRotation(level[i][j][1][1]);
		break;
	case 20:
		block.setTexture(blue_floor2);
		break;
	case 21:
		block.setTexture(rosefloor2);
		break;
	case 22:
		block.setTexture(gray2);
		break;
	case 23:
		block.setTexture(rose_door);
		text.setPosition(block.getPosition());
		text.setFillColor(Color::Color(255, 0, 0, 255));
		block.setTextureRect(IntRect(0, 0, 128, 128));

		if (level[i][j][1][2] < 11) {
			text.setString(to_string(level[i][j][1][2] + 1));
		}
		else {
			text.setString("Error");
		}

		break;
	case 24:
		block.setTexture(rose_door);
		block.setTextureRect(IntRect(128, 0, 128, 128)); 

		text.setPosition(block.getPosition());
		text.setFillColor(Color::Color(255, 0, 0, 255));
		if (level[i][j][1][2] < 11) {
			text.setString(to_string(level[i][j][1][2] + 1));
		}
		else {
			text.setString("Error");
		}
		break;
	case 25:
		block.setTexture(c_one);
		block.setTextureRect(IntRect(0, 0, 128, 128));
		break;
	case 26:
		block.setTexture(c_line);
		block.setTextureRect(IntRect(0, 0, 128, 128));
		break;
	case 27:
		block.setTexture(c_corner);
		block.setTextureRect(IntRect(0, 0, 128, 128));
		break;
	case 28:
		block.setTexture(c_cross3);
		block.setTextureRect(IntRect(0, 0, 128, 128));
		break;
	case 29:
		block.setTexture(c_cross4);
		block.setTextureRect(IntRect(0, 0, 128, 128));
		break;
	case 30:
		block.setTexture(c_plus);
		block.setTextureRect(IntRect(0, 0, 128, 128));
		text.setString("+"); text.setPosition(block.getPosition());
		break;
	case 31:
		block.setTexture(c_minus);
		block.setTextureRect(IntRect(0, 0, 128, 128));
		text.setString("-"); text.setPosition(block.getPosition());
		break;
	case 32:
		block.setTexture(c_resist);
		block.setTextureRect(IntRect(0, 0, 128, 128));
		break;
	case 33:
		block.setTexture(c_switcher3);
		text.setPosition(block.getPosition());
		text.setFillColor(Color::Color(255, 0, 0, 255));
		block.setTextureRect(IntRect(0, 0, 128, 128));
		if (level[i][j][1][2] < 11) {
			text.setString(to_string(level[i][j][1][2] + 1));
		}
		else {
			text.setString("Error");
		}
		break;
	case 34:
		block.setTexture(c_switch);
		text.setPosition(block.getPosition());
		text.setFillColor(Color::Color(255, 0, 0, 255));
		block.setTextureRect(IntRect(0, 0, 128, 128));
		if (level[i][j][1][2] < 11) {
			text.setString(to_string(level[i][j][1][2] + 1));
		}
		else {
			text.setString("Error");
		}
		break;
	case 35:
		block.setTexture(c_switch_off);
		text.setPosition(block.getPosition());
		text.setFillColor(Color::Color(255, 0, 0, 255));
		block.setTextureRect(IntRect(0, 0, 128, 128));
		if (level[i][j][1][2] < 11) {
			text.setString(to_string(level[i][j][1][2] + 1));
		}
		else {
			text.setString("Error");
		}
		break;
	case 36:
		block.setTexture(c_ac);
		text.setPosition(block.getPosition());
		text.setFillColor(Color::Color(255, 0, 0, 255));
		block.setTextureRect(IntRect(0, 0, 128, 128));
		if (level[i][j][1][2] < 11) {
			text.setString(to_string(level[i][j][1][2] + 1));
		}
		else {
			text.setString("Error");
		}
		break;
	case 37:
		block.setTexture(c_sensor);
		text.setPosition(block.getPosition());
		text.setFillColor(Color::Color(255, 0, 0, 255));
		block.setTextureRect(IntRect(0, 0, 128, 128));
		if (level[i][j][1][2] < 11) {
			text.setString(to_string(level[i][j][1][2] + 1));
		}
		else {
			text.setString("Error");
		}
		break;
	case 38:
		block.setTexture(lamp_blue); block.setTextureRect(IntRect(0, 0, 128, 128)); 
		text.setPosition(block.getPosition());
		text.setFillColor(Color::Blue);
		if (level[i][j][1][2] < 5) {
			text.setString(to_string(level[i][j][1][2] + 1));
		}
		else {
			text.setString("Error");
		}
		break;
	case 39:
		block.setTexture(lamp_blue); block.setTextureRect(IntRect(128, 0, 128, 128)); 
		text.setPosition(block.getPosition());
		text.setFillColor(Color::Blue);
		if (level[i][j][1][2] < 5) {
			text.setString(to_string(level[i][j][1][2] + 1));
		}
		else {
			text.setString("Error");
		}
		break;
	case 40:
		block.setTexture(lamp_green); block.setTextureRect(IntRect(0, 0, 128, 128));
		text.setPosition(block.getPosition());
		text.setFillColor(Color::Blue);
		if (level[i][j][1][2] < 5) {
			text.setString(to_string(level[i][j][1][2] + 1));
		}
		else {
			text.setString("Error");
		}
		break;
	case 41:
		block.setTexture(lamp_green); block.setTextureRect(IntRect(128, 0, 128, 128));
		text.setPosition(block.getPosition());
		text.setFillColor(Color::Blue);
		if (level[i][j][1][2] < 5) {
			text.setString(to_string(level[i][j][1][2] + 1));
		}
		else {
			text.setString("Error");
		}
		break;
	case 42:
		block.setTexture(void_el);
		break;
	case 43:
		block.setTextureRect(IntRect(0, 0, 128, 128));
		block.setTexture(_arrow);
		window.draw(block);
		block.setTexture(portal2);
		text.setPosition(block.getPosition());
		text.setFillColor(Color::White);
		text.setString(to_string(level[i][j][1][2] + 1));
		break;
	case 44:
		block.setTexture(some_help);
		break;
	}
	window.draw(block); window.draw(text);
	block.setColor(Color::Color(255, 255, 255, 255));
}

void render_block_pl0() {

	block.setTextureRect(IntRect(0, 0, 128, 128));
	switch (level[i][j][0][0]) {
	case 1:
		block.setTexture(blue);
		block.setTextureRect(IntRect(level[i][j][0][2] * 128, 0, 128, 128));
		break;
	case 2:
		block.setTexture(switch_t);
		block.setTextureRect(IntRect(int(dmode[level[i][j][0][2]]) * 128, 0, 128, 128));
		break;
	case 3:
		//the secret coin
		block.setTexture(smth);
		block.setTextureRect(IntRect(level[i][j][0][2] * 128, 0, 128, 128));
		break;
	case 4:
		//the secret (gathered)
		block.setTexture(smth2);
		block.setTextureRect(IntRect(level[i][j][0][2] * 128, 0, 128, 128));
		break;
	case 5:
		//movable
		block.setTexture(movable);
		block.setTextureRect(IntRect(level[i][j][0][1] * 1.422, 0, 128, 128));

		if (level[i][j][1][0] == 17) {
			helper = (cos(float(tick) / 100) + 1) * 32 + 190;
			block.setColor(Color::Color(255, 255, 255, helper));
			dmode[level[i][j][1][2]] = true;
		}
		if (level[i][j][0][2] != 0) {

			helper = floor(level[i][j][0][2] / 128);

			switch (helper) {
			case 0: break;
			case 1: block.move(0, (level[i][j][0][2] % 128) * scale * -1); break;
			case 2: block.move(0, (level[i][j][0][2] % 128) * scale); break;
			case 3: block.move((level[i][j][0][2] % 128) * scale, 0); break;
			case 4: block.move((level[i][j][0][2] % 128) * scale * -1, 0); break;
			}

			if (level[i][j][0][2] % 128 < 64) { level[i][j][0][2] += 2; }
			if (level[i][j][0][2] % 128 < 100) { level[i][j][0][2] += 1; }
			level[i][j][0][2] += 1;

			/*if (level[i][j][1][0] == 19 || level[i][j][1][0] == 43) {
				if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {
					switch (helper) {
					case 1: sy = 7;  break;
					case 2: sy = -7;  break;
					case 3: sx = -7;  break;
					case 4: sx = 7;  break;
					}
					switch (helper) {
					case 1: py -= 17;  break;
					case 2: py += 17;  break;
					case 3: px += 17;  break;
					case 4: px -= 17;  break;
					}
					break;
				}
			}*/

			if (((level[i][j][0][2] % 128 == 0) || (level[i][j][0][2] % 128 > 125)) && (level[i][j][0][2] != 0)) {

				if (level[i][j][1][0] == 17) { dmode[level[i][j][1][2]] = false; }

				level[i][j][0][2] = 0;

				//smoke_spawn();
				//particle = "teleport";

				switch (helper) {
				case 1:
					level[i][j][0][0] = 0; level[i][j][0][2] = 0;
					level[i][j - 1][0][0] = 5; level[i][j - 1][0][1] = level[i][j][0][1];
					level[i][j - 1][0][2] = 0;
					break;
				case 2:
					level[i][j][0][0] = 0; level[i][j][0][2] = 0;
					level[i][j + 1][0][0] = 5; level[i][j + 1][0][1] = level[i][j][0][1];
					level[i][j + 1][0][2] = 0;
					break;
				case 3:
					level[i][j][0][0] = 0; level[i][j][0][2] = 0;
					level[i + 1][j][0][0] = 5; level[i + 1][j][0][1] = level[i][j][0][1];
					level[i + 1][j][0][2] = 0;
					break;
				case 4:
					level[i][j][0][0] = 0; level[i][j][0][2] = 0;
					level[i - 1][j][0][0] = 5; level[i - 1][j][0][1] = level[i][j][0][1];
					level[i - 1][j][0][2] = 0;
					break;
				}

			}

			helper = 0;

		}
		else { 
			if (level[i][j][1][0] == 19 || ((level[i][j][1][0] == 43) && (tp_rech > 0))) {
				switch (level[i][j][1][1]) {
				case 0: level[i][j][0][2] = 387; break;
				case 90: level[i][j][0][2] = 257; break;
				case 180: level[i][j][0][2] = 513; break;
				case 270: level[i][j][0][2] = 129; break;
				}
				sound_ab.play();
			}
		}
		if (tp_rech == 299 && level[i][j][1][0] == 43) {
			smoke_spawn("tp");
		}
		break;
	case 6:
		//the secret (smoke)
		block.setTexture(smth);
		block.setTextureRect(IntRect(level[i][j][0][2] * 128, 0, 128, 128));
		break;
	case 7:
		//the wall
		block.setTexture(wall);
		break;
	case 8:
		//gray
		block.setTexture(gr);
		break;
	case 9:
		//gray_wall
		block.setTexture(grb);
		break;
	case 10:
		//greenn
		block.setTexture(green);
		break;
	case 11:
		//green_wall
		block.setTexture(greenw);
		break;
	case 12:
		block.setTexture(roseb);
		block.setTextureRect(IntRect(level[i][j][0][2] * 128, 0, 128, 128));
		break;
	case 13:
		block.setTexture(rb);
		break;
	case 14:
		block.setTexture(fl);
		block.setTextureRect(IntRect(level[i][j][0][2] * 128, 0, 128, 128));
		break;
	case 15:
		block.setTexture(crys);
		if (crystal[level[i][j][0][2]]) { block.setTextureRect(IntRect(128, 0, 128, 128)); }
		break;
	case 16:
		block.setTextureRect(IntRect(level[i][j][0][2] * 128, 0, 128, 128));
		block.setTexture(gravs);
		if (level[i][j][0][2] == 0) { block.rotate(-2 * tick % 360); }
		else { block.rotate(2 * tick % 360); }
		window.draw(block);
		if (level[i][j][0][2] == 0) { block.rotate(2 * tick % 360); }
		else { block.rotate(2 * tick % 360); }
		block.setTexture(grav);

		helper_s[0] = (i * 128 + 64 - px);
		helper_s2 = helper_s[0] * helper_s[0];
		helper_s[1] = (j * 128 + 64 - py);
		helper_s2 += helper_s[1] * helper_s[1];
		if (helper_s2 > 250000) { break; }
		/*cout << helper_s << endl;*/
		max_speed = 6;
		if (level[i][j][0][2] == 0) {
			sx += helper_s[0] / helper_s2 * 100;
			sy += helper_s[1] / helper_s2 * 100;
			break;
		}
		sx -= helper_s[0] / helper_s2 * 80;
		sy -= helper_s[1] / helper_s2 * 80;
		break;
	case 17:
		block.setTexture(glass);
		block.setTextureRect(IntRect(level[i][j][0][2] * 128, 0, 128, 128));
		helper_s2 = 0;
		helper_s[0] = (block.getPosition().x - playerCol.getPosition().x) / scale;
		helper_s2 += helper_s[0] * helper_s[0];
		helper_s[1] = (block.getPosition().y - playerCol.getPosition().y) / scale;
		helper_s2 += helper_s[1] * helper_s[1];
		helper_s2 = sqrt(helper_s2);
		if (helper_s2 < 384) {
			block.setColor(Color::Color(255, 255, 255, 280 - (helper_s2 / 1.5)));
			if (helper_s2 < 100) {
				block.setColor(Color::Color(255, 255, 255, 280 - (helper_s2 / 3)));
			}
		}
		else { block.setColor(Color::Color(255, 255, 255, 0)); }
		break;
	case 18:
		//hidden block
		block.setTexture(smth);
		block.setTextureRect(IntRect(level[i][j][0][2] * 128, 0, 128, 128));
		break;
	case 19:
		//Closer
		block.setTexture(smth);
		block.setTextureRect(IntRect(level[i][j][0][2] * 128, 0, 128, 128));
		break;
	case 20:
		//Opener
		block.setTexture(smth);
		block.setTextureRect(IntRect(level[i][j][0][2] * 128, 0, 128, 128));
		break;
	case 21:
		block.setTexture(smth3); break;
	case 22:
		block.setTexture(white); break;
	case 23:
		block.setTexture(whitewall); break;
	case 24:
		block.setTexture(rosebut);
		block.setTextureRect(IntRect(0, 0, 128, 128));
		if (lmode[level[i][j][0][2]]) {
			block.setTextureRect(IntRect(128, 0, 128, 128));
		}
		break;
	case 25:
		block.setTexture(rosebricks); break;
	case 26:
		block.setTexture(player_det);
		switch (level[i][j][0][1]) {
		case 0:
			block.move(128 * scale, 0);
			if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {
				dmode[level[i][j][0][2]] = false;
			}
			block.move(128 * scale, 0);
			if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {
				dmode[level[i][j][0][2]] = false;
			}
			block.move(128 * scale, 0);
			if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {
				dmode[level[i][j][0][2]] = false;
			}
			block.move(-384 * scale, 0);
			break;
		case 90:
			block.move(0, 128 * scale);
			if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {
				dmode[level[i][j][0][2]] = false;
			}
			block.move(0, 128 * scale);
			if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {
				dmode[level[i][j][0][2]] = false;
			}
			block.move(0, 128 * scale);
			if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {
				dmode[level[i][j][0][2]] = false;
			}
			block.move(0, -384 * scale);
			break;
		case 180:
			block.move(-128 * scale, 0);
			if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {
				dmode[level[i][j][0][2]] = false;
			}
			block.move(-128 * scale, 0);
			if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {
				dmode[level[i][j][0][2]] = false;
			}
			block.move(-128 * scale, 0);
			if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {
				dmode[level[i][j][0][2]] = false;
			}
			block.move(384 * scale, 0);
			break;
		case 270:
			block.move(0, -128 * scale);
			if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {
				dmode[level[i][j][0][2]] = false;
			}
			block.move(0, -128 * scale);
			if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {
				dmode[level[i][j][0][2]] = false;
			}
			block.move(0, -128 * scale);
			if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {
				dmode[level[i][j][0][2]] = false;
			}
			block.move(0, 384 * scale);
			break;
		}
		break;
	case 27:
		block.setTexture(player_det_inv);
		switch (level[i][j][0][1]) {
		case 0:
			block.move(128 * scale, 0);
			if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {
				dmode[level[i][j][0][2]] = true;
			}
			block.move(128 * scale, 0);
			if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {
				dmode[level[i][j][0][2]] = true;
			}
			block.move(128 * scale, 0);
			if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {
				dmode[level[i][j][0][2]] = true;
			}
			block.move(-384 * scale, 0);
			break;
		case 90:
			block.move(0, 128 * scale);
			if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {
				dmode[level[i][j][0][2]] = true;
			}
			block.move(0, 128 * scale);
			if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {
				dmode[level[i][j][0][2]] = true;
			}
			block.move(0, 128 * scale);
			if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {
				dmode[level[i][j][0][2]] = true;
			}
			block.move(0, -384 * scale);
			break;
		case 180:
			block.move(-128 * scale, 0);
			if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {
				dmode[level[i][j][0][2]] = true;
			}
			block.move(-128 * scale, 0);
			if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {
				dmode[level[i][j][0][2]] = true;
			}
			block.move(-128 * scale, 0);
			if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {
				dmode[level[i][j][0][2]] = true;
			}
			block.move(384 * scale, 0);
			break;
		case 270:
			block.move(0, -128 * scale);
			if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {
				dmode[level[i][j][0][2]] = true;
			}
			block.move(0, -128 * scale);
			if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {
				dmode[level[i][j][0][2]] = true;
			}
			block.move(0, -128 * scale);
			if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {
				dmode[level[i][j][0][2]] = true;
			}
			block.move(0, 384 * scale);
			break;
		}
		break;
	case 28:
		block.setTexture(view_det); block.setTextureRect(IntRect(0, 0, 128, 128));
		
		helper_s[0] = px - i * 128 + 64; helper_s[1] = py - j * 128 + 64;
		helper_s[0] *= helper_s[0]; helper_s[1] *= helper_s[1];
		helper_s[0] += helper_s[1];

		cursor.setPosition(playerS.getPosition());
		cursor.setRotation(playerS.getRotation());
		cursor.setOrigin(0.2, 8);
		cursor.setScale(150 * scale, 27 * scale); 
		/*window.draw(cursor);*/
		if (cursor.getGlobalBounds().intersects(block.getGlobalBounds())) {
			barmode[level[i][j][0][2]] = true;
			
		}
		else { barmode[level[i][j][0][2]] = false; block.setTextureRect(IntRect(128, 0, 128, 128)); }
		cursor.setScale(1, 1); cursor.setPosition(Mouse::getPosition().x, Mouse::getPosition().y);

		break;
	case 29:
		block.setTexture(pusher);
		if (level[i][j][0][2] > 0) {
			level[i][j][0][2] -= 1;
			block.setTextureRect(IntRect(128, 0, 128, 128));

			helper_s[0] = (i * 128 + 64 - px);
			helper_s2 = helper_s[0] * helper_s[0];
			helper_s[1] = (j * 128 + 64 - py);
			helper_s2 += helper_s[1] * helper_s[1];
			/*cout << helper_s << endl;*/
			max_speed = 6;
			sx -= helper_s[0] / helper_s2 * 300;
			sy -= helper_s[1] / helper_s2 * 300;
			break;
		}
		block.setTextureRect(IntRect(0, 0, 128, 128));
		break;
	case 30:
		helper = abs(sx); helper_s2 = abs(sy);
		if (helper < helper_s2) { helper = helper_s2; }
		block.setTexture(speed_det);
		block.setTextureRect(IntRect(128, 0, 128, 128));
		block.setColor(Color::Color(255, 255, 255, 190 + helper * 5));
		if (tick%90 == 1) { barmode[level[i][j][0][2]] = false;  }
		if (helper > 4) { barmode[level[i][j][0][2]] = true;  }
		//if (helper < 2) { barmode[level[i][j][0][2]] = false; }
		block.setRotation(helper * 2 + tick%360 / 2);
		window.draw(block);
		block.setColor(Color::Color(255, 255, 255, 255));
		block.setTextureRect(IntRect(0, 0, 128, 128));
		break;
	case 31:
		block.setColor(Color::Color(255, 255, 255, 0));

		if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {
			for (helper = 0; helper < wid; helper++) {
				for (k = 0; k < hei; k++) {
					if (level[i][j][0][2] == level[helper][k][0][2]) {
						if (level[helper][k][0][0] == 31) {
							level[helper][k][0][0] = 0;
							
						}
					}
				}
			}
			achievement(lvlnum, level[i][j][0][2]);

		}
		break;
	case 99:
		block.setTexture(bar2); break;
	case 100:
		//this is a barrier
		block.setTexture(barr);
		break;
	case 101:
		//this is a barrier 2
		block.setTexture(barr);
		break;
	case 102:
		//this is a barrier 2
		block.setTexture(barrier_r);
		break;
	case 105:
		//this is invisible barrier
		block.setColor(Color::Color(255, 255, 255, 0));
		if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {

			helper_s[0] = (playerCol.getPosition().x - block.getPosition().x) / scale;
			helper_s[1] = (playerCol.getPosition().y - block.getPosition().y) / scale;

			if (abs(helper_s[0]) > abs(helper_s[1])) {

				helper_s[0] *= helper_s[0] * helper_s[0];

				helper_s[0] = 791950 / helper_s[0];

				sx += helper_s[0];

			}
			else {

				helper_s[1] *= helper_s[1] * helper_s[1];

				helper_s[1] = 791950 / helper_s[1];

				sy += helper_s[1];

			}

			sx /= 1.2;
			sy /= 1.2;
		}
		break;
	}
	block.setRotation(level[i][j][0][1]);
	window.draw(block);

	if (level[i][j][0][0] == 14) {
		block.scale(0.5, 0.5);
	}

	block.setColor(Color::Color(255, 255, 255, 255));

}

void render_block_pl1() {

	block.setTextureRect(IntRect(level[i][j][1][2] * 128, 0, 128, 128));
	switch (level[i][j][1][0]) {
	case 1:
		block.setTexture(el);
		break;
	case 2:
		block.setTexture(door);
		block.setTextureRect(IntRect(dmode[level[i][j][1][2]] * 128, 0, 128, 128));
		if (dmode[level[i][j][1][2]] == 1) {
			if ((level[i][j][0][0] != 100) && (level[i][j][0][0] != 0)) { smoke_spawn("smoke"); sound_b.play(); }
			level[i][j][0][0] = 100; level[i][j][0][1] = level[i][j][1][1];
		}
		else {
			if (level[i][j][0][0] == 100) { level[i][j][0][0] = 0; }
		}
		break;
	case 3:
		block.setTexture(door);
		block.setTextureRect(IntRect(128 - (128 * dmode[level[i][j][1][2]]), 0, 128, 128));
		if (dmode[level[i][j][1][2]] == 0) {
			if ((level[i][j][0][0] != 101) && (level[i][j][0][0] != 0)) { smoke_spawn("smoke"); sound_b.play(); }
			level[i][j][0][0] = 101; level[i][j][0][1] = level[i][j][1][1];
		}
		else {
			if (level[i][j][0][0] == 101) { level[i][j][0][0] = 0; }
		}
		break;
	case 4:
		block.setTexture(str);
		break;
	case 5:
		block.setTexture(corn);
		break;
	case 6:
		if (coins_required <= coins_gathered) {
			block.setTexture(finon);
		}
		else { block.setTexture(finoff); }

		break;
	case 7:
		block.setTexture(port_in); block.setTextureRect(IntRect(0, 0, 128, 128));
		if (on_teleport) {
			if (teleport_to == level[i][j][1][2]) {
				if (teleport_frame > 64) {
					px = tx[teleport_to];
					py = ty[teleport_to];
					on_teleport = false;
					playerS.setScale(scale, scale);
					eyes.setScale(scale, scale);
					spiral[level[i][j][1][2]].setTextureRect(IntRect(0, 0, 256, 256));
					rendpp();
				}
			}
		}
		break;
	case 8:
		block.setTexture(port_o);
		break;
	case 9:
		block.setTexture(gray); break;
	case 10:
		block.setTexture(strp); break;
	case 11:
		block.setTexture(greenf); break;
	case 12:
		block.setTexture(differ); break;
	case 13:
		block.setTexture(rose); break;
	case 14:
		block.setTexture(greenf2); break;
	case 15:
		block.setTexture(act);
		if (!crystal[level[i][j][1][2]]) { block.setTextureRect(IntRect(0, 0, 128, 128)); level[i][j][0][0] = 99; }
		if (crystal[level[i][j][1][2]]) { block.setTextureRect(IntRect(128, 0, 128, 128)); if (level[i][j][0][0] == 99) { level[i][j][0][0] = 0; } }
		break;
	case 16:
		block.setTexture(act);
		if (crystal[level[i][j][1][2]]) { block.setTextureRect(IntRect(0, 0, 128, 128)); level[i][j][0][0] = 99; }
		if (!crystal[level[i][j][1][2]]) { block.setTextureRect(IntRect(128, 0, 128, 128)); if (level[i][j][0][0] == 99) { level[i][j][0][0] = 0; } }
		break;
	case 17:
		block.setTexture(buttonblock);
		if (level[i][j][0][0] != 0) {
			block.setTextureRect(IntRect(128, 0, 128, 128));
			if (!dmode[level[i][j][1][2]]) { open.play(); }
			dmode[level[i][j][1][2]] == true; break;
		}
		else { block.setTextureRect(IntRect(0, 0, 128, 128)); }

		if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds()) && player_at_button < 0) {
			block.setTextureRect(IntRect(128, 0, 128, 128));
			dmode[level[i][j][1][2]] = 1;
			player_at_button = level[i][j][1][2];
			open.play();
		}
		else {
			if (player_at_button == level[i][j][1][2]) {
				if (!playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {
					player_at_button = -1; dmode[level[i][j][1][2]] = 0; open.play();
				}
			}
		}

		break;
	case 18:
		block.setTextureRect(IntRect(0, 0, 128, 128));
		helper_s2 = 0;
		helper_s[0] = (block.getPosition().x - playerCol.getPosition().x) / scale;
		helper_s2 += helper_s[0] * helper_s[0];
		helper_s[1] = (block.getPosition().y - playerCol.getPosition().y) / scale;
		helper_s2 += helper_s[1] * helper_s[1];
		helper_s2 = sqrt(helper_s2);
		switch (level[i][j][1][2]) {
		case 1:
			block.setTexture(blue);
			block.setTextureRect(IntRect(0, 0, 128, 128));
			break;
		case 2:
			block.setTexture(switch_t);
			block.setTextureRect(IntRect(0, 0, 128, 128));
			break;
		case 3:
			//the secret coin
			block.setTexture(smth);
			break;
		case 4:
			//the secret (gathered)
			block.setTexture(smth2);
			break;
		case 5:
			//movable
			block.setTexture(movable);
			break;
		case 6:
			//the secret (smoke)
			block.setTexture(smth);
			break;
		case 7:
			//the wall
			block.setTexture(wall);
			break;
		case 8:
			//gray
			block.setTexture(gr);
			break;
		case 9:
			//gray_wall
			block.setTexture(grb);
			break;
		case 10:
			//greenn
			block.setTexture(green);
			break;
		case 11:
			//green_wall
			block.setTexture(greenw);
			break;
		case 12:
			block.setTexture(roseb);
			break;
		case 13:
			block.setTexture(rb);
			break;
		case 14:
			block.setTexture(fl);
			break;
		case 15:
			block.setTexture(crys); break;
		case 16:
			/*block.setTexture(gravs);
			window.draw(block);*/
			block.setTexture(grav);
			break;
		}
		block.setRotation(level[i][j][1][1]);
		window.draw(block);
		if (helper_s2 < 262) {
			block.setColor(Color::Color(255, 255, 255, 256 - (helper_s2 - 8)));
			if (helper_s2 < 50) {
				block.setColor(Color::Color(255, 255, 255, 255));
			}
			block.setTexture(noth);
		}
		break;
	case 19:
		block.setTexture(_arrow);
		block.setRotation(level[i][j][1][1]);
		break;
	case 20:
		block.setTexture(blue_floor2);
		break;
	case 21:
		block.setTexture(rosefloor2);
		break;
	case 22:
		block.setTexture(gray2);
		break;
	case 23:
		block.setTexture(rose_door);
		if (lmode[level[i][j][1][2]]) {
			if (level[i][j][0][0] != 102) {
				level[i][j][0][0] = 102;
				level[i][j][0][1] = level[i][j][1][1];
				electro.play();
			}
			
			block.setTextureRect(IntRect(128, 0, 128, 128));
		}
		else {
			if (level[i][j][0][0] == 102) {
				level[i][j][0][0] = 0;
				electro.play();
			}
			else {
				if (level[i][j][0][0] != 0) {
					level[i][j][0][0] = 0;
					smoke_spawn("smoke");
					sound_b.play();
				}
			}
			block.setTextureRect(IntRect(0, 0, 128, 128));
		}
		break;
	case 24:
		block.setTexture(rose_door);
		if (!lmode[level[i][j][1][2]]) {
			if (level[i][j][0][0] != 102) {
				level[i][j][0][0] = 102;
				level[i][j][0][1] = level[i][j][1][1];
				electro.play();
			}
			block.setTextureRect(IntRect(128, 0, 128, 128));
		}
		else {
			if (level[i][j][0][0] == 102) {
				level[i][j][0][0] = 0;
				electro.play();
			}
			else {
				if (level[i][j][0][0] != 0) {
					level[i][j][0][0] = 0;
					smoke_spawn("smoke");
					sound_b.play();
				}
			}
			block.setTextureRect(IntRect(0, 0, 128, 128));
		}
		break;

	case 25:
		block.setTextureRect(IntRect(0, 0, 128, 128));
		block.setColor(Color::Color(126 + level[i][j][1][2] * 2, 129 - level[i][j][1][2] * 2, 129 - level[i][j][1][2] * 2, 126 + level[i][j][1][2] * 2));
		block.setTexture(BGel);
		window.draw(block);
		block.setTexture(c_one);
		block.setColor(Color::Color(255, 255, 255, 255));

		switch (level[i][j][1][1]) {
		case 0:
			rech(i - 1, j);
			break;
		case 90:
			rech(i, j - 1);
			break;
		case 180:
			rech(i + 1, j);
			break;
		case 270:
			rech(i, j + 1);
			break;
		}

		break;
	case 26:
		block.setTextureRect(IntRect(0, 0, 128, 128));
		block.setColor(Color::Color(126 + level[i][j][1][2] * 2, 129 - level[i][j][1][2] * 2, 129 - level[i][j][1][2] * 2, 126 + level[i][j][1][2] * 2));
		block.setTexture(BGel);
		window.draw(block);
		block.setTexture(c_line);
		block.setColor(Color::Color(255, 255, 255, 255));

		switch (level[i][j][1][1]) {
		case 0:
			rech(i - 1, j); rech(i + 1, j);
			break;
		case 90:
			rech(i, j + 1); rech(i, j - 1);
			break;
		case 180:
			rech(i - 1, j); rech(i + 1, j);
			break;
		case 270:
			rech(i, j + 1); rech(i, j - 1);
			break;
		}

		break;
	case 27:
		block.setTextureRect(IntRect(0, 0, 128, 128));
		block.setColor(Color::Color(126 + level[i][j][1][2] * 2, 129 - level[i][j][1][2] * 2, 129 - level[i][j][1][2] * 2, 126 + level[i][j][1][2] * 2));
		block.setTexture(BGel);
		window.draw(block);
		block.setTexture(c_corner);
		block.setColor(Color::Color(255, 255, 255, 255));

		switch (level[i][j][1][1]) {
		case 0:
			rech(i - 1, j); rech(i, j - 1);
			break;
		case 90:
			rech(i, j - 1); rech(i + 1, j);
			break;
		case 180:
			rech(i + 1, j); rech(i, j + 1);
			break;
		case 270:
			rech(i, j + 1); rech(i - 1, j);
			break;
		}
		break;
	case 28:
		block.setTextureRect(IntRect(0, 0, 128, 128));
		block.setColor(Color::Color(126 + level[i][j][1][2] * 2, 129 - level[i][j][1][2] * 2, 129 - level[i][j][1][2] * 2, 126 + level[i][j][1][2] * 2));
		block.setTexture(BGel);
		window.draw(block);
		block.setTexture(c_cross3);
		block.setColor(Color::Color(255, 255, 255, 255));

		switch (level[i][j][1][1]) {
		case 0:
			rech(i - 1, j); rech(i, j - 1); rech(i + 1, j);
			break;
		case 90:
			rech(i, j - 1); rech(i + 1, j); rech(i, j + 1);
			break;
		case 180:
			rech(i + 1, j); rech(i, j + 1); rech(i - 1, j);
			break;
		case 270:
			rech(i, j + 1); rech(i - 1, j); rech(i, j - 1);
			break;
		}

		break;
	case 29:
		block.setTextureRect(IntRect(0, 0, 128, 128));
		block.setColor(Color::Color(126 + level[i][j][1][2] * 2, 129 - level[i][j][1][2] * 2, 129 - level[i][j][1][2] * 2, 126 + level[i][j][1][2] * 2));
		block.setTexture(BGel);
		window.draw(block);
		block.setTexture(c_cross4);
		block.setColor(Color::Color(255, 255, 255, 255));

		rech(i - 1, j); rech(i + 1, j); rech(i, j - 1); rech(i, j + 1);

		break;
	case 30:
		block.setTextureRect(IntRect(0, 0, 128, 128));
		block.setColor(Color::Color(126 + level[i][j][1][2] * 2, 129 - level[i][j][1][2] * 2, 129 - level[i][j][1][2] * 2, 126 + level[i][j][1][2] * 2));
		block.setTexture(BGel);
		window.draw(block);
		block.setTexture(c_plus);
		block.setColor(Color::Color(255, 255, 255, 255));

		if (level[i][j][1][2] < 56) {
			level[i][j][1][2]+=7;
		}
		if (level[i][j][1][2] < 20) { level[i][j][1][2] += 12; }

		switch (level[i][j][1][1]) {
		case 0:
			rech(i - 1, j); rech(i - 1, j);
			break;
		case 90:
			rech(i, j + 1); rech(i, j + 1);
			break;
		case 180:
			rech(i + 1, j); rech(i + 1, j);
			break;
		case 270:
			rech(i, j - 1); rech(i, j - 1);
			break;
		}

		break;
	case 31:
		block.setTextureRect(IntRect(0, 0, 128, 128));
		block.setColor(Color::Color(126 + level[i][j][1][2] * 2, 129 - level[i][j][1][2] * 2, 129 - level[i][j][1][2] * 2, 126 + level[i][j][1][2] * 2));
		block.setTexture(BGel);
		window.draw(block);
		block.setTexture(c_minus);
		block.setColor(Color::Color(255, 255, 255, 255));

		if (level[i][j][1][2] > 0) {
			level[i][j][1][2]-=1;
			if (level[i][j][1][2] > 10) {
				level[i][j][1][2] -= 6;
			}
		}

		switch (level[i][j][1][1]) {
		case 0:
			rech(i - 1, j);
			break;
		case 90:
			rech(i, j - 1);
			break;
		case 180:
			rech(i + 1, j);
			break;
		case 270:
			rech(i, j + 1);
			break;
		}
		break;
	case 32:
		block.setTextureRect(IntRect(0, 0, 128, 128));
		block.setColor(Color::Color(126 + level[i][j][1][2] * 2, 129 - level[i][j][1][2] * 2, 129 - level[i][j][1][2] * 2, 126 + level[i][j][1][2] * 2));
		block.setTexture(BGel);
		window.draw(block);
		block.setTexture(c_resist);
		block.setColor(Color::Color(255, 255, 255, 255));

		if (tick % 2 == 0) {
			switch (level[i][j][1][1]) {
			case 0:
				rech(i - 1, j); rech(i + 1, j);
				break;
			case 90:
				rech(i, j + 1); rech(i, j - 1);
				break;
			case 180:
				rech(i - 1, j); rech(i + 1, j);
				break;
			case 270:
				rech(i, j + 1); rech(i, j - 1);
				break;
			}
		}

		break;
	case 33:
		block.setTexture(c_switcher3);
		block.setColor(Color::Color(255, 255, 255, 255));

		if (!lmode[level[i][j][1][2]]) {
			block.setTextureRect(IntRect(0, 0, 128, 128));
			switch (level[i][j][1][1]) {
			case 0:
				rech2(i - 1, j, i, j - 1);
				break;
			case 90:
				rech2(i, j - 1, i + 1, j);
				break;
			case 180:
				rech2(i + 1, j, i, j + 1);
				break;
			case 270:
				rech2(i - 1, j, i, j + 1);
				break;
			}
			break;
		}

		block.setTextureRect(IntRect(128, 0, 128, 128));
		
		switch (level[i][j][1][1]) {
		case 0:
			rech2(i + 1, j, i, j - 1);
			break;
		case 90:
			rech2(i + 1, j, i, j + 1);
			break;
		case 180:
			rech2(i - 1, j, i, j + 1);
			break;
		case 270:
			rech2(i - 1, j, i, j - 1);
			break;
		}

		break;
	case 34:
		block.setTextureRect(IntRect(0, 0, 128, 128));
		block.setTexture(c_switch);
		block.setColor(Color::Color(255, 255, 255, 255));
		if (!lmode[level[i][j][1][2]]) {
			switch (level[i][j][1][1]) {
			case 0:
				rech2(i - 1, j,i + 1, j);
				break;
			case 90:
				rech2(i, j + 1, i, j - 1);
				break;
			case 180:
				rech2(i - 1, j, i + 1, j);
				break;
			case 270:
				rech2(i, j + 1, i, j - 1);
				break;
			}
			break;
		}
		block.setTexture(c_switch_off);
		break;
	case 35:
		block.setTextureRect(IntRect(0, 0, 128, 128));
		window.draw(block);
		block.setTexture(c_switch_off);
		block.setColor(Color::Color(255, 255, 255, 255));
		if (lmode[level[i][j][1][2]]) {
			block.setTexture(c_switch);
			switch (level[i][j][1][1]) {
			case 0:
				rech2(i - 1, j, i + 1, j);
				break;
			case 90:
				rech2(i, j + 1, i, j - 1);
				break;
			case 180:
				rech2(i - 1, j, i + 1, j);
				break;
			case 270:
				rech2(i, j + 1, i, j - 1);
				break;
			}
			break;
		}
		break;
	case 36:
		block.setTextureRect(IntRect(0, 0, 128, 128));
		block.setColor(Color::Color(126 + level[i][j][1][2] * 2, 129 - level[i][j][1][2] * 2, 129 - level[i][j][1][2] * 2, 126 + level[i][j][1][2] * 2));
		block.setTexture(BGel);
		window.draw(block);
		block.setTexture(c_ac);
		block.setColor(Color::Color(255, 255, 255, 255));

		if (lmode[level[i][j][1][2]]) {
			switch (level[i][j][1][1]) {
			case 0:
				if (level[i + 1][j][1][2] < 50) {
					level[i + 1][j][1][2] += 4;
				}
				if (level[i - 1][j][1][2] < 50) {
					level[i - 1][j][1][2] += 4;
				}
				break;
			case 90:
				if (level[i][j + 1][1][2] < 50) {
					level[i][j + 1][1][2] += 4;
				}
				if (level[i][j - 1][1][2] < 50) {
					level[i][j - 1][1][2] += 4;
				}
				break;
			case 180:
				if (level[i + 1][j][1][2] < 50) {
					level[i + 1][j][1][2] += 4;
				}
				if (level[i - 1][j][1][2] < 50) {
					level[i - 1][j][1][2] += 4;
				}
				break;
			case 270:
				if (level[i][j + 1][1][2] < 50) {
					level[i][j + 1][1][2] += 4;
				}
				if (level[i][j - 1][1][2] < 50) {
					level[i][j - 1][1][2] += 4;
				}
				break;
			}
		}

		break;
	case 37:
		block.setTextureRect(IntRect(0, 0, 128, 128));
		block.setColor(Color::Color(126 + level[i][j][1][2] * 2, 129 - level[i][j][1][2] * 2, 129 - level[i][j][1][2] * 2, 126 + level[i][j][1][2] * 2));
		block.setTexture(BGel);
		window.draw(block);
		block.setTexture(c_sensor);
		block.setColor(Color::Color(255, 255, 255, 255));

		lmode[level[i][j][1][2]] = false;

		switch (level[i][j][1][1]) {
		case 0:
			if (level[i - 1][j][1][2] > 33) {
				lmode[level[i][j][1][2]] = true;
				block.setTexture(c_sensor_on);
			}
			break;
		case 90:
			if (level[i][j - 1][1][2] > 33) {
				lmode[level[i][j][1][2]] = true;
				block.setTexture(c_sensor_on);
			}
			break;
		case 180:
			if (level[i + 1][j][1][2] > 33) {
				lmode[level[i][j][1][2]] = true;
				block.setTexture(c_sensor_on);
			}
			break;
		case 270:
			if (level[i][j + 1][1][2] > 33) {
				lmode[level[i][j][1][2]] = true;
				block.setTexture(c_sensor_on);
			}
			break;
		}

		break;
	case 38:
		block.setTexture(lamp_blue); 
		if (!barmode[level[i][j][1][2]]) {
			block.setTextureRect(IntRect(0, 0, 128, 128)); level[i][j][0][0] = 0;
		}
		else {
			block.setTextureRect(IntRect(128, 0, 128, 128));
			if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {
				level[i][j][0][0] = 105;
			}
		}
		break;
	case 39:
		block.setTexture(lamp_blue);
		if (barmode[level[i][j][1][2]]) {
			block.setTextureRect(IntRect(0, 0, 128, 128)); level[i][j][0][0] = 0;
		}
		else {
			block.setTextureRect(IntRect(128, 0, 128, 128));
			if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {
				level[i][j][0][0] = 105;
			}
		}
		break;
	case 40:
		block.setTexture(lamp_green);
		if (!barmode[level[i][j][1][2]]) {
			block.setTextureRect(IntRect(0, 0, 128, 128)); level[i][j][0][0] = 0;
		}
		else {
			block.setTextureRect(IntRect(128, 0, 128, 128));
			if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {
				level[i][j][0][0] = 105;
			}
		}
		break;
	case 41:
		block.setTexture(lamp_green);
		if (barmode[level[i][j][1][2]]) {
			block.setTextureRect(IntRect(0, 0, 128, 128)); level[i][j][0][0] = 0;
		}
		else {
			block.setTextureRect(IntRect(128, 0, 128, 128));
			if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {
				level[i][j][0][0] = 105;
			}
		}
		break;
	case 42:
		block.setTexture(void_el);
		break;
	case 43:
		block.setTexture(portal2);
		block.setTextureRect(IntRect(128, 0, 128, 128));
		block.setRotation(tick % 360);
		if (tp_rech > 0) {
			block.setColor(Color::Color(255, 255, 255, 255 - tp_rech / 6));
			window.draw(block);
			block.setColor(Color::Color(255, 255, 255, 255));
		}
		else {
			window.draw(block);
		}
		block.setRotation(0);
		block.setTextureRect(IntRect(0, 0, 128, 128));

		if (tp_rech == 0) {
			if (level[i][j][0][0] == 5) {
				for (helper = 0; helper < wid; helper++) {
					for (k = 0; k < hei; k++) {
						if (level[helper][k][1][0] == 43 && (level[helper][k][1][2] == level[i][j][1][2])) {
							level[helper][k][0][0] = 5; level[i][j][0][0] = 0;
							level[helper][k][0][1] = level[i][j][0][1];
							tp_rech = 300; smoke_spawn("tp"); tp2.play(); break;
						}
					}
				}
			}
			else {
				if (playerCol.getGlobalBounds().intersects(block.getGlobalBounds())) {
					for (helper = 0; helper < wid; helper++) {
						for (k = 0; k < hei; k++) {
							if (level[helper][k][1][0] == 43 && (level[helper][k][1][2] == level[i][j][1][2])) {
								if (i != helper) {
									px = helper * 128 + 64;
									py = k * 128 + 64; tp_rech = 260;
									smoke_spawn("tp"); tp2.play(); teleport_frame = 70;
									break;
								}
							}
						}
					}
				}
			}
		}

		break;
	case 44:
		block.setTexture(some_help);
		break;
	}
	block.setRotation(level[i][j][1][1]);
	window.draw(block);
}

int get_ID(int num) {
	if (editor_layer == 0) {
		if (editor_choose == 1) {
			switch (num) {
			case 1: return 5;
			case 2: return 1;
			case 3: return 2;
			case 4: return 7;
			case 5: return 26;
			case 6: return 27;
			case 7: return 28;
			case 8: return 29;
			case 9: return 3;
			case 10: return 6;
			case 11: return 18;
			case 12: return 19;
			case 13: return 20;
			case 14: return 4;
			}
		}
		if (editor_choose == 2) {
			switch (num) {
			case 1: return 5;
			case 2: return 12;
			case 3: return 13;
			case 4: return 22;
			case 5: return 23;
			case 6: return 24;
			case 7: return 25;
			case 8: return 3;
			case 9: return 6;
			case 10: return 18;
			case 11: return 19;
			case 12: return 20;
			case 13: return 4;
			}
		}
		if (editor_choose == 3) {
			switch (num) {
			case 1: return 5;
			case 2: return 10;
			case 3: return 11;
			case 4: return 15;
			case 5: return 30;
			case 6: return 3;
			case 7: return 6;
			case 8: return 18;
			case 9: return 19;
			case 10: return 20;
			case 11: return 4;
			}
		}
		if (editor_choose == 4) {
			switch (num) {
			case 1: return 5;
			case 2: return 8;
			case 3: return 9;
			case 4: return 14;
			case 5: return 16;
			case 6: return 17;
			case 7: return 3;
			case 8: return 22;
			case 9: return 23;
			case 10: return 6;
			case 11: return 18;
			case 12: return 19;
			case 13: return 20;
			case 14: return 4;
			case 15: return 21;
			}
		}
		if (editor_choose == 5) {
			switch (num) {
			case 1: return 2;
			case 2: return 3;
			case 3: return 5;
			case 4: return 6;
			case 5: return 15;
			case 6: return 16;
			case 7: return 17;
			case 8: return 18;
			case 9: return 19;
			case 10: return 20;
			case 11: return 24;
			case 12: return 26;
			case 13: return 27;
			case 14: return 28;
			case 15: return 29;
			case 16: return 30;
			case 17: return 31;
			}
		}
		if (editor_choose == 6) {
			switch (num) {
			case 1: return 3;
			case 2: return 4;
			case 3: return 6;
			case 4: return 18;
			case 5: return 19;
			case 6: return 20;
			case 7: return 29;
			}
		}
		if (editor_choose == 7) {
			switch (num) {
			case 1: return 24;
			}
		} 
		return 17;
	}
	if (editor_choose == 1) {
		switch (num) {
		case 1: return 1;
		case 2: return 2;
		case 3: return 3;
		case 4: return 10;
		case 5: return 17;
		case 6: return 18;
		case 7: return 21;
		case 8: return 38;
		case 9: return 39;
		}
	}
	if (editor_choose == 2) {
		switch (num) {
		case 1: return 10;
		case 2: return 13;
		case 3: return 18;
		case 4: return 20;
		case 5: return 23;
		case 6: return 24;
		case 7: return 25;
		case 8: return 26;
		case 9: return 27;
		case 10: return 28;
		case 11: return 29;
		case 12: return 30;
		case 13: return 31;
		case 14: return 32;
		case 15: return 33;
		case 16: return 34;
		case 17: return 35;
		case 18: return 36;
		case 19: return 37;
		case 20: return 42;
		}
	}
	if (editor_choose == 3) {
		switch (num) {
		case 1: return 11;
		case 2: return 14;
		case 3: return 15;
		case 4: return 16;
		case 5: return 18;
		case 6: return 21;
		case 7: return 40;
		case 8: return 41;
		case 9: return 43;
		}
	}
	if (editor_choose == 4) {
		switch (num) {
		case 1: return 4;
		case 2: return 5;
		case 3: return 6;
		case 4: return 7;
		case 5: return 8;
		case 6: return 9;
		case 7: return 10;
		case 8: return 12;
		case 9: return 18;
		case 10: return 19;
		case 11: return 22;
		}
	}
	if (editor_choose == 5) {
		switch (num) {
		case 1: return 2;
		case 2: return 3;
		case 3: return 6;
		case 4: return 7;
		case 5: return 8;
		case 6: return 15;
		case 7: return 16;
		case 8: return 17;
		case 9: return 19;
		case 10: return 23;
		case 11: return 24;
		case 12: return 38;
		case 13: return 39;
		case 14: return 40;
		case 15: return 41;
		case 16: return 43;
		}
	}
	if (editor_choose == 6) {
		if (num == 1) { return 44; }
		return 18;
	}
	if (editor_choose == 7) {
		switch (num) {
		case 1: return 23;
		case 2: return 24;
		case 3: return 25;
		case 4: return 26;
		case 5: return 27;
		case 6: return 28;
		case 7: return 29;
		case 8: return 30;
		case 9: return 31;
		case 10: return 32;
		case 11: return 33;
		case 12: return 34;
		case 13: return 35;
		case 14: return 36;
		case 15: return 37;
		case 16: return 42;
		}
	}
	return 22;
}

void render_block_UI(int screenh, int ID) {
	block.setTexture(cur);
	block.setPosition(128 * UI_scale * i + editor_scroll * UI_scale, screenh - 128 * UI_scale);
	if (editor_layer == 0) {
		switch (ID) {
		case 1:
			block.setTexture(blue);
			break;
		case 2:
			block.setTexture(switch_t);
			break;
		case 3:
			//the secret coin
			block.setTexture(smth);
			switch (editor_choose) {
			case 1: block.setTextureRect(IntRect(128, 0, 128, 128)); break;
			case 2: block.setTextureRect(IntRect(384, 0, 128, 128)); break;
			case 3: block.setTextureRect(IntRect(256, 0, 128, 128)); break;
			case 4: block.setTextureRect(IntRect(0, 0, 128, 128)); break;
			}
			break;
		case 4:
			//the secret (gathered)
			block.setTexture(smth2);
			switch (editor_choose) {
			case 1: block.setTextureRect(IntRect(128, 0, 128, 128)); break;
			case 2: block.setTextureRect(IntRect(384, 0, 128, 128)); break;
			case 3: block.setTextureRect(IntRect(256, 0, 128, 128)); break;
			case 4: block.setTextureRect(IntRect(0, 0, 128, 128)); break;
			}
			break;
		case 5:
			//movable
			block.setTexture(movable);
			switch (editor_choose) {
			case 1: block.setTextureRect(IntRect(256, 0, 128, 128)); break;
			case 2: block.setTextureRect(IntRect(0, 0, 128, 128)); break;
			case 3: block.setTextureRect(IntRect(128, 0, 128, 128)); break;
			case 4: block.setTextureRect(IntRect(384, 0, 128, 128)); break;
			}
			break;
		case 6:
			//the secret (smoke)
			block.setTexture(smth);
			switch (editor_choose) {
			case 1: block.setTextureRect(IntRect(128, 0, 128, 128)); break;
			case 2: block.setTextureRect(IntRect(384, 0, 128, 128)); break;
			case 3: block.setTextureRect(IntRect(256, 0, 128, 128)); break;
			case 4: block.setTextureRect(IntRect(0, 0, 128, 128)); break;
			}
			break;
		case 7:
			//the wall
			block.setTexture(wall);
			break;
		case 8:
			//gray
			block.setTexture(gr);
			break;
		case 9:
			//gray_wall
			block.setTexture(grb);
			break;
		case 10:
			//greenn
			block.setTexture(green);
			break;
		case 11:
			//green_wall
			block.setTexture(greenw);
			break;
		case 12:
			block.setTexture(roseb);
			break;
		case 13:
			block.setTexture(rb);
			break;
		case 14:
			block.setTexture(fl);
			break;
		case 15:
			block.setTexture(crys); break;
		case 16:
			block.setTexture(gravs);
			window.draw(block);
			block.setTexture(grav);
			break;
		case 17:
			block.setTexture(glass); break;
		case 18:
			//hidden block
			block.setTexture(smth);
			switch (editor_choose) {
			case 1: block.setTextureRect(IntRect(128, 0, 128, 128)); break;
			case 2: block.setTextureRect(IntRect(384, 0, 128, 128)); break;
			case 3: block.setTextureRect(IntRect(256, 0, 128, 128)); break;
			case 4: block.setTextureRect(IntRect(0, 0, 128, 128)); break;
			}
			break;
		case 19:
			//Closer
			block.setTexture(smth);
			switch (editor_choose) {
			case 1: block.setTextureRect(IntRect(128, 0, 128, 128)); break;
			case 2: block.setTextureRect(IntRect(384, 0, 128, 128)); break;
			case 3: block.setTextureRect(IntRect(256, 0, 128, 128)); break;
			case 4: block.setTextureRect(IntRect(0, 0, 128, 128)); break;
			}
			break;
		case 20:
			//Opener
			block.setTexture(smth);
			switch (editor_choose) {
			case 1: block.setTextureRect(IntRect(128, 0, 128, 128)); break;
			case 2: block.setTextureRect(IntRect(384, 0, 128, 128)); break;
			case 3: block.setTextureRect(IntRect(256, 0, 128, 128)); break;
			case 4: block.setTextureRect(IntRect(0, 0, 128, 128)); break;
			}
			break;
		case 21:
			block.setTexture(smth3); break;
		case 22:
			block.setTexture(white); break;
		case 23:
			block.setTexture(whitewall); break;
		case 24:
			block.setTexture(rosebut); break;
		case 25:
			block.setTexture(rosebricks); break;
		case 26:
			block.setTexture(player_det); break;
		case 27:
			block.setTexture(player_det_inv); break;
		case 28:
			block.setTexture(view_det); break;
		case 29:
			block.setTexture(pusher); break;
		case 30:
			block.setTexture(speed_det); break;
		case 99:
			block.setTexture(bar2); break;
		case 100:
			//this is a barrier
			block.setTexture(barr); break;
			break;
		case 101:
			//this is a barrier 2
			block.setTexture(barr); break;
			break;
		}
	}
	else {
		switch (ID) {
		case 1:
			block.setTexture(el);
			break;
		case 2:
			block.setTexture(door);
			block.setTextureRect(IntRect(0, 0, 128, 128));
			break;
		case 3:
			block.setTexture(door);
			block.setTextureRect(IntRect(128, 0, 128, 128));
			break;
		case 4:
			block.setTexture(str);
			break;
		case 5:
			block.setTexture(corn);
			break;
		case 6:
			block.setTexture(finon);
			break;
		case 7:
			block.setTexture(port_in);
			break;
		case 8:
			block.setTexture(port_o);
			break;
		case 9:
			block.setTexture(gray); break;
		case 10:
			block.setTexture(strp);
			if (editor_choose == 2) {
				block.setTextureRect(IntRect(0, 0, 128, 128));
			}
			break;
		case 11:
			block.setTexture(greenf); break;
		case 12:
			block.setTexture(differ);
			break;
		case 13:
			block.setTexture(rose); break;
		case 14:
			block.setTexture(greenf2); break;
		case 15:
			block.setTexture(act); break;
		case 16:
			block.setTextureRect(IntRect(128, 0, 128, 128));
			block.setTexture(act); break;
		case 17:
			block.setTexture(buttonblock); block.setTextureRect(IntRect(0, 0, 128, 128)); break;
		case 18:
			block.setTexture(noth); break;
		case 19:
			block.setTexture(_arrow); break;
		case 20:
			block.setTexture(blue_floor2);
			break;
		case 21:
			block.setTexture(rosefloor2);
			break;
		case 22:
			block.setTexture(gray2);
			break;
		case 23:
			block.setTexture(rose_door); break;
		case 24:
			block.setTexture(rose_door);
			block.setTextureRect(IntRect(128, 0, 128, 128)); break;

		case 25:
			block.setTexture(c_one);
			break;
		case 26:
			block.setTexture(c_line);
			break;
		case 27:
			block.setTexture(c_corner);
			break;
		case 28:
			block.setTexture(c_cross3);
			break;
		case 29:
			block.setTexture(c_cross4);
			break;
		case 30:
			block.setTexture(c_plus);
			break;
		case 31:
			block.setTexture(c_minus);
			break;
		case 32:
			block.setTexture(c_resist);
			break;
		case 33:
			block.setTexture(c_switcher3);
			break;
		case 34:
			block.setTexture(c_switch);
			break;
		case 35:
			block.setTexture(c_switch_off);
			break;
		case 36:
			block.setTexture(c_ac);
			break;
		case 37:
			block.setTexture(c_sensor);
			break;
		case 38:
			block.setTexture(lamp_blue); block.setTextureRect(IntRect(0, 0, 128, 128)); break;
		case 39:
			block.setTexture(lamp_blue); block.setTextureRect(IntRect(128, 0, 128, 128)); break;
		case 40:
			block.setTexture(lamp_green); block.setTextureRect(IntRect(0, 0, 128, 128)); break;
		case 41:
			block.setTexture(lamp_green); block.setTextureRect(IntRect(128, 0, 128, 128)); break;
		case 42:
			block.setTexture(void_el);
			break;
		case 43:
			block.setTexture(portal2);
			break;
		}
	}
	if (get_ID(i) == editor_block) { block.scale(0.8, 0.8); block.move(7, 7); }
	else { block.setScale(UI_scale / 1.1, UI_scale / 1.1); }
	window.draw(block);
	block.setTextureRect(IntRect(0, 0, 128, 128));
}
