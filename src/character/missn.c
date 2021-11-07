/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "missn.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//MissN character structure
enum
{
	MissN_ArcMain_Idle0,
	MissN_ArcMain_Idle1,
	MissN_ArcMain_Idle2,
	MissN_ArcMain_Idle3,
	MissN_ArcMain_Idle4,
	MissN_ArcMain_Idle5,
	MissN_ArcMain_Idle6,
	MissN_ArcMain_Left0,
	MissN_ArcMain_Left1,
	MissN_ArcMain_Left2,
	MissN_ArcMain_Down0,
	MissN_ArcMain_Down1,
	MissN_ArcMain_Down2,
	MissN_ArcMain_Up0,
	MissN_ArcMain_Up1,
	MissN_ArcMain_Up2,
	MissN_ArcMain_Right0,
	MissN_ArcMain_Right1,
	MissN_ArcMain_Right2,
	
	MissN_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[MissN_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_MissN;

//MissN character definitions
static const CharFrame char_missn_frame[] = {
	{MissN_ArcMain_Idle0, {  0,   0, 256, 256}, { 125, 250}}, //0 idle 1
	{MissN_ArcMain_Idle1, {  0,   0, 256, 256}, { 125, 250}}, //1 idle 2
	{MissN_ArcMain_Idle2, {  0,   0, 256, 256}, { 125, 250}}, //2 idle 3
	{MissN_ArcMain_Idle3, {  0,   0, 256, 256}, { 125, 250}}, //3 idle 4
	{MissN_ArcMain_Idle4, {  0,   0, 256, 256}, { 125, 250}}, //4 idle 5
	{MissN_ArcMain_Idle5, {  0,   0, 256, 256}, { 125, 250}}, //5 idle 6
	{MissN_ArcMain_Idle6, {  0,   0, 256, 256}, { 125, 250}}, //6 idle 7
	
	{MissN_ArcMain_Left0, {  0,   0, 256, 256}, { 125, 250}}, //7 left 8
	{MissN_ArcMain_Left1, {  0,   0, 256, 256}, { 125, 250}}, //8 left 9
	{MissN_ArcMain_Left2, {  0,   0, 256, 256}, { 125, 250}}, //9 left 10
	
	{MissN_ArcMain_Down0, {  0,   0, 256, 256}, { 125, 250}}, //10 down 11
	{MissN_ArcMain_Down1, {  0,   0, 256, 256}, { 125, 250}}, //11 down 12
	{MissN_ArcMain_Down2, {  0,   0, 256, 256}, { 125, 250}}, //12 down 13
	
	{MissN_ArcMain_Up0, {   0,   0,  256, 256}, { 125, 250}}, //13 up 14
	{MissN_ArcMain_Up1, {   0,   0,  256, 256}, { 125, 250}}, //14 up 15
	{MissN_ArcMain_Up1, {   0,   0,  256, 256}, { 125, 250}}, //15 up 16

	
	{MissN_ArcMain_Right0, {  0,   0, 256, 256}, { 125, 250}}, //16 right 17
	{MissN_ArcMain_Right1, {  0,   0, 256, 256}, { 125, 250}}, //17 right 18
	{MissN_ArcMain_Right2, {  0,   0, 256, 256}, { 125, 250}}, //18 right 19
};

static const Animation char_missn_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0,  1,  2,  3,  4,  5,  6, ASCR_REPEAT, 1}},      	   //CharAnim_Idle
	{2, (const u8[]){ 7,  8,  9,  8, ASCR_BACK, 1}},       		   	   //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},                     	   //CharAnim_LeftAlt
	{2, (const u8[]){ 10, 11, 12,  11, ASCR_BACK, 1}},  		   	   //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   	          	   //CharAnim_DownAlt
	{2, (const u8[]){ 13,  14,  15, 14, ASCR_BACK, 1}},                	   //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},                             //CharAnim_UpAlt
	{2, (const u8[]){ 16,  17,  18, 17, ASCR_BACK, 1}}, 			   //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},                             //CharAnim_RightAlt
};

//MissN character functions
void Char_MissN_SetFrame(void *user, u8 frame)
{
	Char_MissN *this = (Char_MissN*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_missn_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_MissN_Tick(Character *character)
{
	Char_MissN *this = (Char_MissN*)character;
	
	//Perform idle dance
	Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_MissN_SetFrame);
	Character_Draw(character, &this->tex, &char_missn_frame[this->frame]);
}

void Char_MissN_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_MissN_Free(Character *character)
{
	Char_MissN *this = (Char_MissN*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_MissN_New(fixed_t x, fixed_t y)
{
	//Allocate missn object
	Char_MissN *this = Mem_Alloc(sizeof(Char_MissN));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_MissN_New] Failed to allocate missn object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_MissN_Tick;
	this->character.set_anim = Char_MissN_SetAnim;
	this->character.free = Char_MissN_Free;
	
	Animatable_Init(&this->character.animatable, char_missn_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character stage information
	this->character.health_i = 4;
	
	this->character.focus_x = FIXED_DEC(25,1);
	this->character.focus_y = FIXED_DEC(-90,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\MISSN.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim", //MissN_ArcMain_Idle0
		"idle1.tim", //MissN_ArcMain_Idle1
		"idle2.tim", //MissN_ArcMain_Idle2
		"idle3.tim", //MissN_ArcMain_Idle3
		"idle4.tim", //MissN_ArcMain_Idle4
		"idle5.tim", //MissN_ArcMain_Idle5
		"idle6.tim", //MissN_ArcMain_Idle6
		"left0.tim",  //MissN_ArcMain_Left0
		"left1.tim",  //MissN_ArcMain_Left1
		"left2.tim",  //MissN_ArcMain_Left2
		"down0.tim",  //MissN_ArcMain_Down0
		"down1.tim",  //MissN_ArcMain_Down1
		"down2.tim",  //MissN_ArcMain_Down2
		"up0.tim",    //MissN_ArcMain_Up0
		"up1.tim",    //MissN_ArcMain_Up1
		"up2.tim",    //MissN_ArcMain_Up2
		"right0.tim", //MissN_ArcMain_Right0
		"right1.tim", //MissN_ArcMain_Right1
		"right2.tim", //MissN_ArcMain_Right2
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
