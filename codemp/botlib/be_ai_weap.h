/*
===========================================================================
Copyright (C) 1999 - 2005, Id Software, Inc.
Copyright (C) 2000 - 2013, Raven Software, Inc.
Copyright (C) 2001 - 2013, Activision, Inc.
Copyright (C) 2013 - 2015, OpenJK contributors

This file is part of the OpenJK source code.

OpenJK is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License version 2 as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see <http://www.gnu.org/licenses/>.
===========================================================================
*/

/*****************************************************************************
 * name:		be_ai_weap.h
 *
 * desc:		weapon AI
 *
 * $Archive: /source/code/botlib/be_ai_weap.h $
 * $Author: osman $
 * $Revision: 1.4 $
 * $Modtime: 10/05/99 3:32p $
 * $Date: 2003/03/15 23:44:00 $
 *
 *****************************************************************************/

#pragma once

 //projectile flags
#define PFL_WINDOWDAMAGE			1		//projectile damages through window
#define PFL_RETURN					2		//set when projectile returns to owner
//weapon flags
#define WFL_FIRERELEASED			1		//set when projectile is fired with key-up event
//damage types
#define DAMAGETYPE_IMPACT			1		//damage on impact
#define DAMAGETYPE_RADIAL			2		//radial damage
#define DAMAGETYPE_VISIBLE			4		//damage to all entities visible to the projectile

using projectileinfo_t = struct projectileinfo_s
{
	char name[MAX_STRINGFIELD];
	char model[MAX_STRINGFIELD];
	int flags;
	float gravity;
	int damage;
	float radius;
	int visdamage;
	int damagetype;
	int healthinc;
	float push;
	float detonation;
	float bounce;
	float bouncefric;
	float bouncestop;
};

using weapon_info_t = struct weapon_info_s
{
	int valid; //true if the weapon info is valid
	int number; //number of the weapon
	char name[MAX_STRINGFIELD];
	char model[MAX_STRINGFIELD];
	int level;
	int weapon_index;
	int flags;
	char projectile[MAX_STRINGFIELD];
	int numprojectiles;
	float hspread;
	float vspread;
	float speed;
	float acceleration;
	vec3_t recoil;
	vec3_t offset;
	vec3_t angleoffset;
	float extrazvelocity;
	int ammoamount;
	int ammoindex;
	float activate;
	float reload;
	float spinup;
	float spindown;
	projectileinfo_t proj; //pointer to the used projectile
};

//setup the weapon AI
int BotSetupWeaponAI(void);
//shut down the weapon AI
void BotShutdownWeaponAI(void);
//returns the best weapon to fight with
int BotChooseBestFightWeapon(int weaponstate, int* inventory);
//returns the information of the current weapon
void BotGetweapon_info(int weaponstate, int weapon, weapon_info_t* weaponInfo);
//loads the weapon weights
int BotLoadWeaponWeights(int weaponstate, char* filename);
//returns a handle to a newly allocated weapon state
int BotAllocWeaponState(void);
//frees the weapon state
void BotFreeWeaponState(int handle);
//resets the whole weapon state
void BotResetWeaponState(int weaponstate);
