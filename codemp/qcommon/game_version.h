/*
===========================================================================
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

/// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ///
///																																///
///																																///
///													SERENITY JEDI ENGINE														///
///										          LIGHTSABER COMBAT SYSTEM													    ///
///																																///
///						      System designed by Serenity and modded by JaceSolaris. (c) 2024 SJE   		                    ///
///								    https://www.moddb.com/mods/serenityjediengine-20											///
///																																///
/// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ///

#ifndef GAME_VERSION_H
#define GAME_VERSION_H
#define _STR(x) #x
#define STR(x) _STR(x)

// Current version of the multi player game

#define VERSION_MAJOR_RELEASE		24  // Build year
#define VERSION_MINOR_RELEASE		03  // Build month
#define VERSION_INTERNAL_BUILD		07  // Build number

#define VERSION_STRING				"Day-18,Month-03,Year-24,BuildNum-07" // build date
#define VERSION_STRING_DOTTED		"Day-18,Month-03,Year-24,BuildNum-07" // build date

#if defined(_DEBUG)
#define	JK_VERSION		"(debug)MovieDuels-mp: " VERSION_STRING_DOTTED
#define JK_VERSION_OLD	"(debug)MovieDuels-mp: " VERSION_STRING_DOTTED
#else
#define	JK_VERSION		"MovieDuels-mp: " VERSION_STRING_DOTTED
#define JK_VERSION_OLD	"MovieDuels-mp: " VERSION_STRING_DOTTED
#endif
#endif // GAME_VERSION_H
