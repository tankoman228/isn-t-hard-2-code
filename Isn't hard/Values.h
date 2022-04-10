#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <windows.h>
#include <SFML/Audio.hpp>
#include <fstream>
using namespace sf; using namespace std;

const int wid = 60, hei = 30; int screenw, screenh;

float ex = 0, ey = 0, mx = 3, my = 0, dmode[5]{ 0, 0, 0, 0, 0 }, scale = 1, px, py;
float UI_scale = 1, teleport_frame, sx = 0, sy = 0, helper_s[2], helper_s2;

int level[wid][hei][2][3], i, j, k, s, scrollx = 0, max_speed = 9, scrolly = 0;
int coins_gathered = 0, dir = 0, helper, coins_required, lvlnum = 1, teleport_to = -1;
int colint = 0, alpha = 255, tx[3], ty[3], tick = 0, current_NPC = 0, level_scroll = 0, levels_opened = 6;
int editor_scroll = 128, partnum, editor_mode = 1, editor_layer = 0, editor_block = 1, tp_rech = 0;
int achieve_fade = 0, editor_choose = 1, player_at_button = -1, page = 0;

bool saveornot, onclick = false, on_teleport = false, crystal[5]{ false, false, false, false, false };
bool lmode[11]{ false, false, false, false, false, false, false, false, false, false, false};
bool button_overlapped = false, context_menu = false, close = false;
bool barmode[5] = { false, false, false, false, false };
bool click_button_menu = false;

std::string mode = "menu", helper_string = "", lvl_name = "Does_not_exist";

Sprite playerS, playerCol, eyes, block, cursor, spiral[3], blend, NPC, lvl_but, dialog_but, box, box_but;
Font font; Text text, ach_text; Sprite ach_box, ach_BG;

Texture body_t, eyes_t, box_t, box_but_t, cur, spir, inter, lev, diabut, blink;
Texture blue, el, switch_t, door, barr, smth, smth2, smth3, str, corn, movable, finon, finoff, wall, bg, port_in, port_o, noth, glass;
Texture gray, gr, grb, strp, differ, rb, rose, green, greenf, greenw, roseb, greenf2, fl, crys, act, bar2, gravs, grav, buttonblock;
Texture _arrow, white, rosebut, whitewall, rosefloor2, barrier_r, rose_door, gray2, blue_floor2;
Texture rosebricks, portal2, lamp_blue, lamp_green, speed_det, view_det, player_det, player_det_inv, pusher, void_el;
Texture t_play, t_levels, t_editor, t_ach, t_options, t_exit, ach_t, ach_bg;
Texture e_B, e_E, e_Gr, e_Gy, e_L, e_L2, e_M, e_Mech, e_next, e_prev, e_play, e_player, e_plus, e_Q, e_R, e_Rt, e_save, e_load;
Texture some_help, trash;

Texture c_plus, c_minus, c_ac, BGel, c_one, c_corner, c_cross3, c_cross4, c_line, c_resist, c_sensor, c_sensor_on, c_switch, c_switch_off, c_switcher3;

SoundBuffer scl, push, coin_s, boom, cryst, ach_b;
Sound s1, sound_p, sound_c, sound_b, _break, ach_s; Sound electro; SoundBuffer elbuf;
SoundBuffer telep; Sound tp_s; Sound sound_ab; SoundBuffer arpush;
Sound tp1, tp2, push2, open; SoundBuffer tp1_buf, tp2_buf, push2_buf, open_buf;
Music music;

RenderWindow window; ofstream logg;