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

// world.c -- world query functions

#include "../server/exe_headers.h"
#include "../qcommon/cm_local.h"

/*
Ghoul2 Insert Start
*/

#if !defined(GHOUL2_SHARED_H_INC)
#include "../game/ghoul2_shared.h"	//for CGhoul2Info_v
#endif
#if !defined(G2_H_INC)
#include "../ghoul2/G2.h"
#endif
#if !defined (MINIHEAP_H_INC)
#include "../qcommon/MiniHeap.h"
#endif

#ifdef _DEBUG
#include <float.h>
#endif //_DEBUG
/*
Ghoul2 Insert End
*/

#if 0 //G2_SUPERSIZEDBBOX is not being used
static const float superSizedAdd = 64.0f;
#endif

/*
================
SV_clip_handleForEntity

Returns a headnode that can be used for testing or clipping to a
given entity.  If the entity is a bsp model, the headnode will
be returned, otherwise a custom box tree will be constructed.
================
*/
clip_handle_t SV_clip_handleForEntity(const gentity_t* ent)
{
	if (ent->bmodel)
	{
		// explicit hulls in the BSP model
		return CM_InlineModel(ent->s.modelindex);
	}

	// create a temp tree from bounding box sizes
	return CM_TempBoxModel(ent->mins, ent->maxs); //, ent->contents );
}

/*
===============================================================================

ENTITY CHECKING

To avoid linearly searching through lists of entities during environment testing,
the world is carved up with an evenly spaced, axially aligned bsp tree.  Entities
are kept in chains either at the final leafs, or at the first node that splits
them, which prevents having to deal with multiple fragments of a single entity.

===============================================================================
*/

using worldSector_t = struct worldSector_s
{
	int axis; // -1 = leaf node
	float dist;
	worldSector_s* children[2];
	svEntity_t* entities;
};

constexpr auto AREA_DEPTH = 8;
constexpr auto AREA_NODES = 1024;

worldSector_t sv_worldSectors[AREA_NODES];
int sv_numworldSectors;

/*
===============
SV_CreateworldSector

Builds a uniformly subdivided tree for the given world size
===============
*/
worldSector_t* SV_CreateworldSector(const int depth, vec3_t mins, vec3_t maxs)
{
	vec3_t size;
	vec3_t mins1, maxs1, mins2, maxs2;

	worldSector_t* anode = &sv_worldSectors[sv_numworldSectors];
	sv_numworldSectors++;

	if (depth == AREA_DEPTH)
	{
		anode->axis = -1;
		anode->children[0] = anode->children[1] = nullptr;
		return anode;
	}

	VectorSubtract(maxs, mins, size);
	if (size[0] > size[1])
	{
		anode->axis = 0;
	}
	else
	{
		anode->axis = 1;
	}

	anode->dist = 0.5 * (maxs[anode->axis] + mins[anode->axis]);
	VectorCopy(mins, mins1);
	VectorCopy(mins, mins2);
	VectorCopy(maxs, maxs1);
	VectorCopy(maxs, maxs2);

	maxs1[anode->axis] = mins2[anode->axis] = anode->dist;

	anode->children[0] = SV_CreateworldSector(depth + 1, mins2, maxs2);
	anode->children[1] = SV_CreateworldSector(depth + 1, mins1, maxs1);

	return anode;
}

/*
===============
SV_ClearWorld

===============
*/
void SV_ClearWorld()
{
	vec3_t mins, maxs;

	memset(sv_worldSectors, 0, sizeof sv_worldSectors);
	sv_numworldSectors = 0;

	// get world map bounds
	const clip_handle_t h = CM_InlineModel(0);
	CM_ModelBounds(cmg, h, mins, maxs);
	SV_CreateworldSector(0, mins, maxs);
}

/*
===============
SV_UnlinkEntity

===============
*/
void SV_UnlinkEntity(gentity_t* g_ent)
{
	// this should never be called with a freed entity
	if (!g_ent->inuse)
	{
		return;
	}

	svEntity_t* ent = SV_SvEntityForGentity(g_ent);

	g_ent->linked = qfalse;

	worldSector_t* ws = ent->worldSector;
	if (!ws)
	{
		return; // not linked in anywhere
	}
	ent->worldSector = nullptr;

	if (ws->entities == ent)
	{
		ws->entities = ent->nextEntityInWorldSector;
		return;
	}

	for (svEntity_t* scan = ws->entities; scan; scan = scan->nextEntityInWorldSector)
	{
		if (scan->nextEntityInWorldSector == ent)
		{
			scan->nextEntityInWorldSector = ent->nextEntityInWorldSector;
			return;
		}
	}

	Com_Printf("WARNING: SV_UnlinkEntity: not found in worldSector\n");
}

/*
===============
SV_LinkEntity

===============
*/
constexpr auto MAX_TOTAL_ENT_LEAFS = 128;

void SV_LinkEntity(gentity_t* g_ent)
{
	int leafs[MAX_TOTAL_ENT_LEAFS];
	int i;
	int last_leaf;

	// this should never be called with a freed entity
	if (!g_ent->inuse)
	{
		return;
	}

	svEntity_t* ent = SV_SvEntityForGentity(g_ent);

	if (ent->worldSector)
	{
		SV_UnlinkEntity(g_ent); // unlink from old position
	}

	// encode the size into the entityState_t for client prediction
	if (g_ent->bmodel)
	{
		g_ent->s.solid = SOLID_BMODEL; // a solid_box will never create this value
	}
	else if (g_ent->contents & (CONTENTS_SOLID | CONTENTS_BODY))
	{
		// assume that x/y are equal and symetric
		i = g_ent->maxs[0];
		if (i < 1)
			i = 1;
		if (i > 255)
			i = 255;

		// z is not symetric
		int j = -g_ent->mins[2];
		if (j < 1)
			j = 1;
		if (j > 255)
			j = 255;

		// and z maxs can be negative...
		int k = g_ent->maxs[2] + 32;
		if (k < 1)
			k = 1;
		if (k > 255)
			k = 255;

		g_ent->s.solid = k << 16 | j << 8 | i;
	}
	else
	{
		g_ent->s.solid = 0;
	}

	// get the position
	const float* origin = g_ent->currentOrigin;
	const float* angles = g_ent->currentAngles;

	// set the abs box
	if (g_ent->bmodel && (angles[0] || angles[1] || angles[2]))
	{
		// expand for rotation
		const float max = RadiusFromBounds(g_ent->mins, g_ent->maxs);
		for (int i1 = 0; i1 < 3; i1++)
		{
			g_ent->absmin[i1] = origin[i1] - max;
			g_ent->absmax[i1] = origin[i1] + max;
		}
	}
	else
	{
		// normal
		VectorAdd(origin, g_ent->mins, g_ent->absmin);
		VectorAdd(origin, g_ent->maxs, g_ent->absmax);
	}

	// because movement is clipped an epsilon away from an actual edge,
	// we must fully check even when bounding boxes don't quite touch
	g_ent->absmin[0] -= 1;
	g_ent->absmin[1] -= 1;
	g_ent->absmin[2] -= 1;
	g_ent->absmax[0] += 1;
	g_ent->absmax[1] += 1;
	g_ent->absmax[2] += 1;

	// link to PVS leafs
	ent->numClusters = 0;
	ent->lastCluster = 0;
	ent->areanum = -1;
	ent->areanum2 = -1;

	//get all leafs, including solids
	const int num_leafs = CM_BoxLeafnums(g_ent->absmin, g_ent->absmax,
		leafs, MAX_TOTAL_ENT_LEAFS, &last_leaf);

	// if none of the leafs were inside the map, the
	// entity is outside the world and can be considered unlinked
	if (!num_leafs)
	{
		return;
	}

	// set areas, even from clusters that don't fit in the entity array
	for (i = 0; i < num_leafs; i++)
	{
		const int area = CM_LeafArea(leafs[i]);
		if (area != -1)
		{
			// doors may legally straggle two areas,
			// but nothing should evern need more than that
			if (ent->areanum != -1 && ent->areanum != area)
			{
				if (ent->areanum2 != -1 && ent->areanum2 != area && sv.state == SS_LOADING)
				{
					Com_DPrintf("Object %i touching 3 areas at %f %f %f\n",
						g_ent->s.number,
						g_ent->absmin[0], g_ent->absmin[1], g_ent->absmin[2]);
				}
				ent->areanum2 = area;
			}
			else
			{
				ent->areanum = area;
			}
		}
	}

	// store as many explicit clusters as we can
	ent->numClusters = 0;
	for (i = 0; i < num_leafs; i++)
	{
		const int cluster = CM_LeafCluster(leafs[i]);
		if (cluster != -1)
		{
			ent->clusternums[ent->numClusters++] = cluster;
			if (ent->numClusters == MAX_ENT_CLUSTERS)
			{
				break;
			}
		}
	}

	// store off a last cluster if we need to
	if (i != num_leafs)
	{
		ent->lastCluster = CM_LeafCluster(last_leaf);
	}

	// find the first world sector node that the ent's box crosses
	worldSector_t* node = sv_worldSectors;
	while (true)
	{
		if (node->axis == -1)
			break;
		if (g_ent->absmin[node->axis] > node->dist)
			node = node->children[0];
		else if (g_ent->absmax[node->axis] < node->dist)
			node = node->children[1];
		else
			break; // crosses the node
	}

	// link it in
	ent->worldSector = node;
	ent->nextEntityInWorldSector = node->entities;
	node->entities = ent;

	g_ent->linked = qtrue;
}

/*
============================================================================

AREA QUERY

Fills in a list of all entities who's absmin / absmax intersects the given
bounds.  This does NOT mean that they actually touch in the case of bmodels.
============================================================================
*/

using areaParms_t = struct
{
	const float* mins;
	const float* maxs;
	gentity_t** list;
	int count, maxcount;
};

/*
====================
SV_AreaEntities_r

====================
*/
void SV_AreaEntities_r(worldSector_t* node, areaParms_t* ap)
{
	svEntity_t* next;

	for (svEntity_t* check = node->entities; check; check = next)
	{
		next = check->nextEntityInWorldSector;

		gentity_t* gcheck = SV_GEntityForSvEntity(check);

		if (gcheck->absmin[0] > ap->maxs[0]
			|| gcheck->absmin[1] > ap->maxs[1]
			|| gcheck->absmin[2] > ap->maxs[2]
			|| gcheck->absmax[0] < ap->mins[0]
			|| gcheck->absmax[1] < ap->mins[1]
			|| gcheck->absmax[2] < ap->mins[2])
		{
			continue;
		}

		if (ap->count == ap->maxcount)
		{
			Com_DPrintf("SV_AreaEntities: reached maxcount (%d)\n", ap->maxcount);
			return;
		}

		ap->list[ap->count] = gcheck;
		ap->count++;
	}

	if (node->axis == -1)
	{
		return; // terminal node
	}

	// recurse down both sides
	if (ap->maxs[node->axis] > node->dist)
	{
		SV_AreaEntities_r(node->children[0], ap);
	}
	if (ap->mins[node->axis] < node->dist)
	{
		SV_AreaEntities_r(node->children[1], ap);
	}
}

/*
================
SV_AreaEntities
================
*/
int SV_AreaEntities(const vec3_t mins, const vec3_t maxs, gentity_t** elist, const int maxcount)
{
	areaParms_t ap;

	ap.mins = mins;
	ap.maxs = maxs;
	ap.list = elist;
	ap.count = 0;
	ap.maxcount = maxcount;

	SV_AreaEntities_r(sv_worldSectors, &ap);

	return ap.count;
}

/*
===============
SV_SectorList_f
===============
*/
#if 1

void SV_SectorList_f()
{
	for (int i = 0; i < AREA_NODES; i++)
	{
		const worldSector_t* sec = &sv_worldSectors[i];

		int c = 0;
		for (const svEntity_t* ent = sec->entities; ent; ent = ent->nextEntityInWorldSector)
		{
			c++;
		}
		Com_Printf("sector %i: %i entities\n", i, c);
	}
}

#else

#pragma warning (push, 3)	//go back down to 3 for the stl include
#include <list>
#include <map>
#pragma warning (pop)

class CBBox
{
public:
	float mMins[3];
	float mMaxs[3];

	CBBox(vec3_t mins, vec3_t maxs)
	{
		VectorCopy(mins, mMins);
		VectorCopy(maxs, mMaxs);
	}
};

static multimap<int, pair<int, list<CBBox> > > entStats;

void SV_AreaEntitiesTree(worldSector_t* node, areaParms_t* ap, int level)
{
	svEntity_t* check, * next;
	gentity_t* gcheck;
	int				count;
	list<CBBox>	bblist;

	count = 0;

	for (check = node->entities; check; check = next)
	{
		next = check->nextEntityInWorldSector;

		gcheck = SV_GEntityForSvEntity(check);

		CBBox bBox(gcheck->absmin, gcheck->absmax);
		bblist.push_back(bBox);
		count++;
	}

	entStats.insert(pair<int, pair<int, list<CBBox> > >(level, pair<int, list<CBBox> >(count, bblist)));
	if (node->axis == -1)
	{
		return;		// terminal node
	}

	// recurse down both sides
	SV_AreaEntitiesTree(node->children[0], ap, level + 1);
	SV_AreaEntitiesTree(node->children[1], ap, level + 1);
}

void SV_SectorList_f(void)
{
	areaParms_t		ap;

	//	ap.mins = mins;
	//	ap.maxs = maxs;
	//	ap.list = list;
	//	ap.count = 0;
	//	ap.maxcount = maxcount;

	entStats.clear();
	SV_AreaEntitiesTree(sv_worldSectors, &ap, 0);
	char mess[1000];
	multimap<int, pair<int, list<CBBox> > >::iterator j;
	for (j = entStats.begin(); j != entStats.end(); j++)
	{
		sprintf(mess, "**************************************************\n");
		Sleep(5);
		OutputDebugString(mess);
		sprintf(mess, "level=%i, count=%i\n", (*j).first, (*j).second.first);
		Sleep(5);
		OutputDebugString(mess);
		list<CBBox>::iterator k;
		for (k = (*j).second.second.begin(); k != (*j).second.second.end(); k++)
		{
			sprintf(mess, "mins=%f %f %f, maxs=%f %f %f\n",
				(*k).mMins[0], (*k).mMins[1], (*k).mMins[2], (*k).mMaxs[0], (*k).mMaxs[1], (*k).mMaxs[2]);
			OutputDebugString(mess);
		}
	}
}
#endif

//===========================================================================

using moveclip_t = struct
{
	vec3_t boxmins, boxmaxs; // enclose the test object along entire move
	const float* mins;
	const float* maxs; // size of the moving object
	/*
	Ghoul2 Insert Start
	*/
	vec3_t start;
	/*
	Ghoul2 Insert End
	*/
	vec3_t end;
	int pass_entity_num;
	int contentmask;
	/*
	Ghoul2 Insert Start
	*/
	EG2_Collision e_g2_trace_type;
	int use_lod;
	trace_t trace; // make sure nothing goes under here for Ghoul2 collision purposes
	/*
	Ghoul2 Insert End
	*/
};

/*
====================
SV_ClipMoveToEntities

====================
*/
void SV_ClipMoveToEntities(moveclip_t* clip)
{
	gentity_t* touchlist[MAX_GENTITIES], * owner;
	trace_t trace, old_trace;

	const int num = SV_AreaEntities(clip->boxmins, clip->boxmaxs, touchlist, MAX_GENTITIES);

	if (clip->pass_entity_num != ENTITYNUM_NONE)
	{
		owner = SV_GentityNum(clip->pass_entity_num)->owner;
	}
	else
	{
		owner = nullptr;
	}

	for (int i = 0; i < num; i++)
	{
		if (clip->trace.allsolid)
		{
			return;
		}
		gentity_t* touch = touchlist[i];

		// see if we should ignore this entity
		if (clip->pass_entity_num != ENTITYNUM_NONE)
		{
			if (touch->s.number == clip->pass_entity_num)
			{
				continue; // don't clip against the pass entity
			}
			if (touch->owner && touch->owner->s.number == clip->pass_entity_num)
			{
				continue; // don't clip against own missiles
			}
			if (owner == touch)
			{
				continue; // don't clip against owner
			}
			if (owner && touch->owner == owner)
			{
				continue; // don't clip against other missiles from our owner
			}
		}

		// if it doesn't have any brushes of a type we
		// are looking for, ignore it
		if (!(clip->contentmask & touch->contents))
		{
			continue;
		}

		// might intersect, so do an exact clip
		clip_handle_t clip_handle = SV_clip_handleForEntity(touch);

		const float* origin = touch->currentOrigin;
		const float* angles = touch->currentAngles;

		if (!touch->bmodel)
		{
			angles = vec3_origin; // boxes don't rotate
		}

#if 0 //G2_SUPERSIZEDBBOX is not being used
		bool shrinkBox = true;

		if (clip->e_g2_trace_type != G2_SUPERSIZEDBBOX)
		{
			shrinkBox = false;
		}
		else if (trace.entity_num == touch->s.number && touch->ghoul2.size() && !(touch->contents & CONTENTS_LIGHTSABER))
		{
			shrinkBox = false;
		}
		if (shrinkBox)
		{
			vec3_t sh_mins;
			vec3_t sh_maxs;
			int j;
			for (j = 0; j < 3; j++)
			{
				sh_mins[j] = clip->mins[j] + superSizedAdd;
				sh_maxs[j] = clip->maxs[j] - superSizedAdd;
			}
			CM_TransformedBoxTrace(&trace, clip->start, clip->end,
				sh_mins, sh_maxs, clip_handle, clip->contentmask,
				origin, angles);
		}
		else
#endif
		{
#ifdef __MACOS__
			// compiler bug with const
			CM_TransformedBoxTrace(&trace, (float*)clip->start, (float*)clip->end,
				(float*)clip->mins, (float*)clip->maxs, clip_handle, clip->contentmask,
				origin, angles);
#else
			CM_TransformedBoxTrace(&trace, clip->start, clip->end,
				clip->mins, clip->maxs, clip_handle, clip->contentmask,
				origin, angles);
#endif
			//FIXME: when startsolid in another ent, doesn't return correct entity_num
			//ALSO: 2 players can be standing next to each other and this function will
			//think they're in each other!!!
		}
		old_trace = clip->trace;

		if (trace.allsolid)
		{
			if (!clip->trace.allsolid)
			{
				//We didn't come in here all solid, so set the clip->trace's entity_num
				clip->trace.entity_num = touch->s.number;
			}
			clip->trace.allsolid = qtrue;
			trace.entity_num = touch->s.number;
		}
		else if (trace.startsolid)
		{
			if (!clip->trace.startsolid)
			{
				//We didn't come in here starting solid, so set the clip->trace's entity_num
				clip->trace.entity_num = touch->s.number;
			}
			clip->trace.startsolid = qtrue;
			trace.entity_num = touch->s.number;
		}

		if (trace.fraction < clip->trace.fraction)
		{
			// make sure we keep a startsolid from a previous trace
			const qboolean oldStart = clip->trace.startsolid;

			trace.entity_num = touch->s.number;
			clip->trace = trace;
			if (oldStart)
			{
				clip->trace.startsolid = qtrue;
			}
		}
		/*
		Ghoul2 Insert Start
		*/

		// decide if we should do the ghoul2 collision detection right here
		if (trace.entity_num == touch->s.number && clip->e_g2_trace_type != G2_NOCOLLIDE)
		{
			// do we actually have a ghoul2 model here?
			if (touch->ghoul2.size() && !(touch->contents & CONTENTS_LIGHTSABER))
			{
				int old_trace_rec_size = 0;
				int new_trace_rec_size = 0;
				int z;

				// we have to do this because sometimes you may hit a model's bounding box, but not actually penetrate the Ghoul2 Models polygons
				// this is, needless to say, not good. So we must check to see if we did actually hit the model, and if not, reset the trace stuff
				// to what it was to begin with

				// set our trace record size
				for (z = 0; z < MAX_G2_COLLISIONS; z++)
				{
					if (clip->trace.G2CollisionMap[z].mEntityNum != -1)
					{
						old_trace_rec_size++;
					}
				}

				// if we are looking at an entity then use the player state to get it's angles and origin from
				float radius;
#if 0 //G2_SUPERSIZEDBBOX is not being used
				if (clip->e_g2_trace_type == G2_SUPERSIZEDBBOX)
				{
					radius = (clip->maxs[0] - clip->mins[0] - 2.0f * superSizedAdd) / 2.0f;
				}
				else
#endif
				{
					radius = (clip->maxs[0] - clip->mins[0]) / 2.0f;
				}
				if (touch->client)
				{
					vec3_t world_angles;

					world_angles[PITCH] = 0;
					//legs do not *always* point toward the viewangles!
					//world_angles[YAW] =  touch->client->viewangles[YAW];
					world_angles[YAW] = touch->client->legsYaw;
					world_angles[ROLL] = 0;

					re.G2API_CollisionDetect(clip->trace.G2CollisionMap, touch->ghoul2,
						world_angles, touch->client->origin, sv.time, touch->s.number, clip->start,
						clip->end, touch->s.modelScale, G2VertSpaceServer, clip->e_g2_trace_type,
						clip->use_lod, radius);
				}
				// no, so use the normal entity state
				else
				{
					//use the correct origin and angles!  is this right now?
					re.G2API_CollisionDetect(clip->trace.G2CollisionMap, touch->ghoul2,
						touch->currentAngles, touch->currentOrigin, sv.time, touch->s.number,
						clip->start, clip->end, touch->s.modelScale, G2VertSpaceServer,
						clip->e_g2_trace_type, clip->use_lod, radius);
				}

				// set our new trace record size

				for (z = 0; z < MAX_G2_COLLISIONS; z++)
				{
					if (clip->trace.G2CollisionMap[z].mEntityNum != -1)
					{
						new_trace_rec_size++;
					}
				}

				// did we actually touch this model? If not, lets reset this ent as being hit..
				if (new_trace_rec_size == old_trace_rec_size)
				{
					clip->trace = old_trace;
				}
				else //this trace was valid, so copy the best collision into quake trace place info
				{
					for (z = 0; z < MAX_G2_COLLISIONS; z++)
					{
						if (clip->trace.G2CollisionMap[z].mEntityNum == touch->s.number)
						{
							clip->trace.plane.normal[0] = clip->trace.G2CollisionMap[z].mCollisionNormal[0];
							clip->trace.plane.normal[1] = clip->trace.G2CollisionMap[z].mCollisionNormal[1];
							clip->trace.plane.normal[2] = clip->trace.G2CollisionMap[z].mCollisionNormal[2];
							break;
						}
					}
					assert(z < MAX_G2_COLLISIONS); // hmm well ah, weird
					assert(VectorLength(clip->trace.plane.normal) > 0.1f);
				}
			}
		}
		/*
		Ghoul2 Insert End
		*/
	}
}

/*
==================
SV_Trace

Moves the given mins/maxs volume through the world from start to end.
pass_entity_num and entities owned by pass_entity_num are explicitly not checked.
==================
*/
/*
Ghoul2 Insert Start
*/
void SV_Trace(trace_t* results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end,
	const int pass_entity_num, const int contentmask, const EG2_Collision e_g2_trace_type, const int use_lod)
{
	/*
	Ghoul2 Insert End
	*/
#ifdef _DEBUG
	assert(
		!Q_isnan(start[0]) && !Q_isnan(start[1]) && !Q_isnan(start[2]) && !Q_isnan(end[0]) && !Q_isnan(end[1]) && !
		Q_isnan(end[2]));
#endif// _DEBUG

	moveclip_t clip;

	/*
	startMS = Sys_Milliseconds ();
	numTraces++;
	*/
	if (!mins)
	{
		mins = vec3_origin;
	}
	if (!maxs)
	{
		maxs = vec3_origin;
	}

	memset(&clip, 0, sizeof(moveclip_t) - sizeof clip.trace.G2CollisionMap);

	// clip to world
	//NOTE: this will stop not only on static architecture but also entity brushes such as
	//doors, etc.  This prevents us from being able to shorten the trace so that we can
	//ignore all ents past this endpoint... perhaps need to check the entity_num in this
	//BoxTrace or have it not clip against entity brushes here.
	CM_BoxTrace(&clip.trace, start, end, mins, maxs, 0, contentmask);
	clip.trace.entity_num = clip.trace.fraction != 1.0 ? ENTITYNUM_WORLD : ENTITYNUM_NONE;
	if (clip.trace.fraction == 0)
	{
		// blocked immediately by the world
		*results = clip.trace;
		//		goto addtime;
		return;
	}

	clip.contentmask = contentmask;
	/*
	Ghoul2 Insert Start
	*/
	VectorCopy(start, clip.start);
	clip.e_g2_trace_type = e_g2_trace_type;
	clip.use_lod = use_lod;
	/*
	Ghoul2 Insert End
	*/
	//Shorten the trace to the size of the trace until it hit the world
	VectorCopy(clip.trace.endpos, clip.end);
	//remember the current completion fraction
	const float world_frac = clip.trace.fraction;
	//set the fraction back to 1.0 for the trace vs. entities
	clip.trace.fraction = 1.0f;

	//VectorCopy( end, clip.end );
	// create the bounding box of the entire move
	// we can limit it to the part of the move not
	// already clipped off by the world, which can be
	// a significant savings for line of sight and shot traces
	clip.pass_entity_num = pass_entity_num;

#if 0 //G2_SUPERSIZEDBBOX is not being used
	vec3_t superMin;
	vec3_t superMax;  // prison, in boscobel

	if (e_g2_trace_type == G2_SUPERSIZEDBBOX)
	{
		for (i = 0; i < 3; i++)
		{
			superMin[i] = mins[i] - superSizedAdd;
			superMax[i] = maxs[i] + superSizedAdd;
		}
		clip.mins = superMin;
		clip.maxs = superMax;
	}
	else
#endif
	{
		clip.mins = mins;
		clip.maxs = maxs;
	}

	for (int i = 0; i < 3; i++)
	{
		if (end[i] > start[i])
		{
			clip.boxmins[i] = clip.start[i] + clip.mins[i] - 1;
			clip.boxmaxs[i] = clip.end[i] + clip.maxs[i] + 1;
		}
		else
		{
			clip.boxmins[i] = clip.end[i] + clip.mins[i] - 1;
			clip.boxmaxs[i] = clip.start[i] + clip.maxs[i] + 1;
		}
	}

	// clip to other solid entities
	SV_ClipMoveToEntities(&clip);

	//scale the trace back down by the previous fraction
	clip.trace.fraction *= world_frac;
	*results = clip.trace;

	/*
	addtime:
		endMS = Sys_Milliseconds ();

		timeInTrace += endMS - startMS;
	*/
}

/*
=============
SV_PointContents
=============
*/
int SV_PointContents(const vec3_t p, const int pass_entity_num)
{
	gentity_t* touch[MAX_GENTITIES];

	// get base contents from world
	int contents = CM_PointContents(p, 0);

	// or in contents from all the other entities
	const int num = SV_AreaEntities(p, p, touch, MAX_GENTITIES);

	for (int i = 0; i < num; i++)
	{
		const gentity_t* hit = touch[i];
		if (hit->s.number == pass_entity_num)
		{
			continue;
		}
		// might intersect, so do an exact clip
		const clip_handle_t clip_handle = SV_clip_handleForEntity(hit);

		const int c2 = CM_TransformedPointContents(p, clip_handle, hit->s.origin, hit->s.angles);

		contents |= c2;
	}

	return contents;
}