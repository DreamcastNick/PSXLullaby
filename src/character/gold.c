/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "gold.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//Gold character structure
enum
{
	Gold_ArcMain_Idle0,
	Gold_ArcMain_Idle1,
	Gold_ArcMain_Idle2,
	Gold_ArcMain_Idle3,
	Gold_ArcMain_Idle4,
	Gold_ArcMain_Idle5,
	Gold_ArcMain_Idle6,
	Gold_ArcMain_Left0,
	Gold_ArcMain_Left1,
	Gold_ArcMain_Left2,
	Gold_ArcMain_Left3,
	Gold_ArcMain_Down0,
	Gold_ArcMain_Down1,
	Gold_ArcMain_Down2,
	Gold_ArcMain_Down3,
	Gold_ArcMain_Up0,
	Gold_ArcMain_Up1,
	Gold_ArcMain_Right0,
	Gold_ArcMain_Right1,
	Gold_ArcMain_Right2,
	Gold_ArcMain_Right3,
	Gold_ArcMain_Right4,
	Gold_ArcMain_Appear0,
	Gold_ArcMain_Appear1,
	Gold_ArcMain_Appear2,
	Gold_ArcMain_Appear3,
	Gold_ArcMain_Appear4,
	Gold_ArcMain_Appear5,
	Gold_ArcMain_Appear6,
	Gold_ArcMain_Jump0,
	
	Gold_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[Gold_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_Gold;

//Gold character definitions
static const CharFrame char_gold_frame[] = {
	{Gold_ArcMain_Idle0, {  0,   0, 256, 256}, { 125, 250}}, //0 idle 1
	{Gold_ArcMain_Idle1, {  0,   0, 256, 256}, { 125, 250}}, //1 idle 2
	{Gold_ArcMain_Idle2, {  0,   0, 256, 256}, { 125, 250}}, //2 idle 3
	{Gold_ArcMain_Idle3, {  0,   0, 256, 256}, { 125, 250}}, //3 idle 4
	{Gold_ArcMain_Idle4, {  0,   0, 256, 256}, { 125, 250}}, //4 idle 5
	{Gold_ArcMain_Idle5, {  0,   0, 256, 256}, { 125, 250}}, //5 idle 6
	{Gold_ArcMain_Idle6, {  0,   0, 256, 256}, { 125, 250}}, //6 idle 7
	
	{Gold_ArcMain_Left0, {  0,   0, 256, 256}, { 175, 250}}, //7 left 8
	{Gold_ArcMain_Left1, {  0,   0, 256, 256}, { 175, 250}}, //8 left 9
	{Gold_ArcMain_Left2, {  0,   0, 256, 256}, { 175, 250}}, //9 left 10
	{Gold_ArcMain_Left3, {  0,   0, 256, 256}, { 175, 250}}, //10 left 11
	
	{Gold_ArcMain_Down0, {  0,   0, 256, 256}, { 125, 250}}, //11 down 12
	{Gold_ArcMain_Down1, {  0,   0, 256, 256}, { 125, 250}}, //12 down 13
	{Gold_ArcMain_Down2, {  0,   0, 256, 256}, { 125, 250}}, //13 down 14
	{Gold_ArcMain_Down3, {  0,   0, 256, 256}, { 125, 250}}, //14 down 15
	
	{Gold_ArcMain_Up0, {   0,   0,  256, 256}, { 125, 250}}, //15 up 16
	{Gold_ArcMain_Up1, {   0,   0,  256, 256}, { 125, 250}}, //16 up 17
	
	{Gold_ArcMain_Right0, {  0,   0, 256, 256}, { 110, 250}}, //17 right 18
	{Gold_ArcMain_Right1, {  0,   0, 256, 256}, { 110, 250}}, //18 right 19
	{Gold_ArcMain_Right2, {  0,   0, 256, 256}, { 110, 250}}, //19 right 20
	{Gold_ArcMain_Right3, {  0,   0, 256, 256}, { 110, 250}}, //20 right 21
	{Gold_ArcMain_Right4, {  0,   0, 256, 256}, { 110, 250}}, //21 right 22
	
	{Gold_ArcMain_Appear0, {  0,   0, 256, 256}, { 125, 250}}, //22 appear 23
	{Gold_ArcMain_Appear1, {  0,   0, 256, 256}, { 125, 250}}, //23 appear 24
	{Gold_ArcMain_Appear2, {  0,   0, 256, 256}, { 125, 250}}, //24 appear 25
	{Gold_ArcMain_Appear3, {  0,   0, 256, 256}, { 125, 250}}, //25 appear 26
	{Gold_ArcMain_Appear4, {  0,   0, 256, 256}, { 125, 250}}, //26 appear 27
	{Gold_ArcMain_Appear5, {  0,   0, 256, 256}, { 125, 250}}, //27 appear 28
	{Gold_ArcMain_Appear6, {  0,   0, 256, 256}, { 125, 250}}, //28 appear 29
	
	{Gold_ArcMain_Jump0, {  0,   0, 256, 256}, { 125, 250}}, //29 jumpscare 30
};

static const Animation char_gold_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0, ASCR_REPEAT, 1}},      	   							      		 //CharAnim_Idle
	{2, (const u8[]){ 7,  8,  9, 10,  9,  8, ASCR_BACK, 1}},       		              		 //CharAnim_Left
	{2, (const u8[]){ 29, ASCR_BACK, 1}},                    	         					 //CharAnim_LeftAlt
	{2, (const u8[]){ 11, 12,  13,  14,  13,  12, ASCR_BACK, 1}},  		                     //CharAnim_Down
	{2, (const u8[]){ 29, ASCR_BACK, 1}},                    	         					 //CharAnim_DownAlt
	{2, (const u8[]){ 15,  16,  15, ASCR_BACK, 1}},                		   	        	     //CharAnim_Up
	{2, (const u8[]){ 29, ASCR_BACK, 1}},                    	         					 //CharAnim_UpAlt
	{2, (const u8[]){ 17,  18,  19,  20,  21, 20, 19, 18, 17, ASCR_BACK, 1}}, 		  		 //CharAnim_Right
	{2, (const u8[]){ 29, ASCR_BACK, 1}},                    	         					 //CharAnim_RightAlt
	{2, (const u8[]){22, 22, 22, 23, 23, 23, 23, 24, 25, 26, 27, 28, ASCR_BACK, 1}},  		 //CharAnim_Appear
	{2, (const u8[]){28, 27, 26, 25, 24, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, ASCR_BACK, 1}}, 						 //CharAnim_Disappear
};

//Gold character functions
void Char_Gold_SetFrame(void *user, u8 frame)
{
	Char_Gold *this = (Char_Gold*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_gold_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_Gold_Tick(Character *character)
{
	Char_Gold *this = (Char_Gold*)character;
	
	//Perform idle dance
	Character_PerformIdle(character);

	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_Gold_SetFrame);
	
	if (stage.stage_id == StageId_5_2 && stage.song_step <= 1)
	Character_Draw(character, &this->tex, &char_gold_frame[this->frame]);

	if (stage.stage_id == StageId_1_3 && stage.song_step >= 1)
	Character_Draw(character, &this->tex, &char_gold_frame[this->frame]);

	if (stage.stage_id == StageId_5_2 && stage.song_step <= 1)
	Character_Draw(character, &this->tex, &char_gold_frame[this->frame]);

	if (stage.stage_id == StageId_1_3 && stage.song_step >= 1558)
	Character_Draw(character, &this->tex, &char_gold_frame[this->frame]);

	if (stage.flag & STAGE_FLAG_JUST_STEP)
    //Stage specific animations
	if (stage.note_scroll >= 0)
	{
		switch (stage.stage_id)
		{
			case StageId_1_3: //Monochrome
				if ((stage.song_step) == 0)
					character->set_anim(character, CharAnim_Appear);
				if ((stage.song_step) == 1552)
					character->set_anim(character, CharAnim_Disappear);
			break;
		default:
			break;
		}
	}
}

void Char_Gold_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_Gold_Free(Character *character)
{
	Char_Gold *this = (Char_Gold*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_Gold_New(fixed_t x, fixed_t y)
{
	//Allocate gold object
	Char_Gold *this = Mem_Alloc(sizeof(Char_Gold));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_Gold_New] Failed to allocate gold object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_Gold_Tick;
	this->character.set_anim = Char_Gold_SetAnim;
	this->character.free = Char_Gold_Free;
	
	Animatable_Init(&this->character.animatable, char_gold_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character stage information
	this->character.health_i = 3;
	
	this->character.focus_x = FIXED_DEC(0,1);
	this->character.focus_y = FIXED_DEC(-120,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\GOLD.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim", //Gold_ArcMain_Idle0
		"idle1.tim", //Gold_ArcMain_Idle1
		"idle2.tim", //Gold_ArcMain_Idle2
		"idle3.tim", //Gold_ArcMain_Idle3
		"idle4.tim", //Gold_ArcMain_Idle4
		"idle5.tim", //Gold_ArcMain_Idle5
		"idle6.tim", //Gold_ArcMain_Idle6
		"left0.tim",  //Gold_ArcMain_Left0
		"left1.tim",  //Gold_ArcMain_Left1
		"left2.tim",  //Gold_ArcMain_Left2
		"left3.tim",  //Gold_ArcMain_Left3
		"down0.tim",  //Gold_ArcMain_Down0
		"down1.tim",  //Gold_ArcMain_Down1
		"down2.tim",  //Gold_ArcMain_Down2
		"down3.tim",  //Gold_ArcMain_Down3
		"up0.tim",    //Gold_ArcMain_Up0
		"up1.tim",    //Gold_ArcMain_Up1
		"right0.tim", //Gold_ArcMain_Right0
		"right1.tim", //Gold_ArcMain_Right1
		"right2.tim", //Gold_ArcMain_Right2
		"right3.tim", //Gold_ArcMain_Right3
		"right4.tim", //Gold_ArcMain_Right4
		"appear0.tim", //Gold_ArcMain_Appear0
		"appear1.tim", //Gold_ArcMain_Appear1
		"appear2.tim", //Gold_ArcMain_Appear2
		"appear3.tim", //Gold_ArcMain_Appear3
		"appear4.tim", //Gold_ArcMain_Appear4
		"appear5.tim", //Gold_ArcMain_Appear5
		"appear6.tim", //Gold_ArcMain_Appear6
		"jump0.tim",   //Gold_ArcMain_Jump0
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
