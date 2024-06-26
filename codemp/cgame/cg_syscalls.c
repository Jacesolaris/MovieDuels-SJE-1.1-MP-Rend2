/*
===========================================================================
Copyright (C) 1999 - 2005, Id Software, Inc.
Copyright (C) 2000 - 2013, Raven Software, Inc.
Copyright (C) 2001 - 2013, Activision, Inc.
Copyright (C) 2005 - 2015, ioquake3 contributors
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

// cg_syscalls.c -- this file is only included when building a dll
#include "cg_local.h"

static intptr_t(QDECL* Q_syscall)(intptr_t arg, ...) = (intptr_t(QDECL*)(intptr_t, ...)) - 1;

static void TranslateSyscalls(void);

Q_EXPORT void dllEntry(intptr_t(QDECL* syscallptr)(intptr_t arg, ...))
{
	Q_syscall = syscallptr;

	TranslateSyscalls();
}

int PASSFLOAT(const float x)
{
	byteAlias_t fi;
	fi.f = x;
	return fi.i;
}

void trap_Print(const char* fmt)
{
	Q_syscall(CG_PRINT, fmt);
}

void trap_Error(const char* fmt)
{
	Q_syscall(CG_ERROR, fmt);
	exit(1);
}

int trap_Milliseconds(void)
{
	return Q_syscall(CG_MILLISECONDS);
}

void trap_PrecisionTimer_Start(void** theNewTimer)
{
	Q_syscall(CG_PRECISIONTIMER_START, theNewTimer);
}

int trap_PrecisionTimer_End(void* theTimer)
{
	return Q_syscall(CG_PRECISIONTIMER_END, theTimer);
}

void trap_Cvar_Register(vmCvar_t* vmCvar, const char* varName, const char* defaultValue, const uint32_t flags)
{
	Q_syscall(CG_CVAR_REGISTER, vmCvar, varName, defaultValue, flags);
}

void trap_Cvar_Update(vmCvar_t* vmCvar)
{
	Q_syscall(CG_CVAR_UPDATE, vmCvar);
}

void trap_Cvar_Set(const char* var_name, const char* value)
{
	Q_syscall(CG_CVAR_SET, var_name, value);
}

void trap_Cvar_VariableStringBuffer(const char* var_name, char* buffer, const int bufsize)
{
	Q_syscall(CG_CVAR_VARIABLESTRINGBUFFER, var_name, buffer, bufsize);
}

int trap_Cvar_GetHiddenVarValue(const char* name)
{
	return Q_syscall(CG_CVAR_GETHIDDENVALUE, name);
}

int trap_Argc(void)
{
	return Q_syscall(CG_ARGC);
}

void trap_Argv(const int n, char* buffer, const int bufferLength)
{
	Q_syscall(CG_ARGV, n, buffer, bufferLength);
}

void trap_Args(char* buffer, const int bufferLength)
{
	Q_syscall(CG_ARGS, buffer, bufferLength);
}

int trap_FS_FOpenFile(const char* qpath, fileHandle_t* f, const fsMode_t mode)
{
	return Q_syscall(CG_FS_FOPENFILE, qpath, f, mode);
}

void trap_FS_Read(void* buffer, const int len, const fileHandle_t f)
{
	Q_syscall(CG_FS_READ, buffer, len, f);
}

void trap_FS_Write(const void* buffer, const int len, const fileHandle_t f)
{
	Q_syscall(CG_FS_WRITE, buffer, len, f);
}

void trap_FS_FCloseFile(const fileHandle_t f)
{
	Q_syscall(CG_FS_FCLOSEFILE, f);
}

int trap_FS_GetFileList(const char* path, const char* extension, char* listbuf, const int bufsize)
{
	return Q_syscall(CG_FS_GETFILELIST, path, extension, listbuf, bufsize);
}

void trap_SendConsoleCommand(const char* text)
{
	Q_syscall(CG_SENDCONSOLECOMMAND, text);
}

void trap_AddCommand(const char* cmdName)
{
	Q_syscall(CG_ADDCOMMAND, cmdName);
}

void trap_RemoveCommand(const char* cmdName)
{
	Q_syscall(CG_REMOVECOMMAND, cmdName);
}

void trap_SendClientCommand(const char* s)
{
	Q_syscall(CG_SENDCLIENTCOMMAND, s);
}

void trap_UpdateScreen(void)
{
	Q_syscall(CG_UPDATESCREEN);
}

void trap_CM_LoadMap(const char* mapname, const qboolean SubBSP)
{
	Q_syscall(CG_CM_LOADMAP, mapname, SubBSP);
}

int trap_CM_NumInlineModels(void)
{
	return Q_syscall(CG_CM_NUMINLINEMODELS);
}

clip_handle_t trap_CM_InlineModel(const int index)
{
	return Q_syscall(CG_CM_INLINEMODEL, index);
}

clip_handle_t trap_CM_TempBoxModel(const vec3_t mins, const vec3_t maxs)
{
	return Q_syscall(CG_CM_TEMPBOXMODEL, mins, maxs);
}

clip_handle_t trap_CM_TempCapsuleModel(const vec3_t mins, const vec3_t maxs)
{
	return Q_syscall(CG_CM_TEMPCAPSULEMODEL, mins, maxs);
}

int trap_CM_PointContents(const vec3_t p, const clip_handle_t model)
{
	return Q_syscall(CG_CM_POINTCONTENTS, p, model);
}

int trap_CM_TransformedPointContents(const vec3_t p, const clip_handle_t model, const vec3_t origin, const vec3_t angles)
{
	return Q_syscall(CG_CM_TRANSFORMEDPOINTCONTENTS, p, model, origin, angles);
}

void trap_CM_BoxTrace(trace_t* results, const vec3_t start, const vec3_t end, const vec3_t mins, const vec3_t maxs,
	const clip_handle_t model, const int brushmask)
{
	Q_syscall(CG_CM_BOXTRACE, results, start, end, mins, maxs, model, brushmask);
}

void trap_CM_CapsuleTrace(trace_t* results, const vec3_t start, const vec3_t end, const vec3_t mins, const vec3_t maxs,
	const clip_handle_t model, const int brushmask)
{
	Q_syscall(CG_CM_CAPSULETRACE, results, start, end, mins, maxs, model, brushmask);
}

void trap_CM_TransformedBoxTrace(trace_t* results, const vec3_t start, const vec3_t end, const vec3_t mins,
	const vec3_t maxs, const clip_handle_t model, const int brushmask, const vec3_t origin,
	const vec3_t angles)
{
	Q_syscall(CG_CM_TRANSFORMEDBOXTRACE, results, start, end, mins, maxs, model, brushmask, origin, angles);
}

void trap_CM_TransformedCapsuleTrace(trace_t* results, const vec3_t start, const vec3_t end, const vec3_t mins,
	const vec3_t maxs, const clip_handle_t model, const int brushmask,
	const vec3_t origin, const vec3_t angles)
{
	Q_syscall(CG_CM_TRANSFORMEDCAPSULETRACE, results, start, end, mins, maxs, model, brushmask, origin, angles);
}

int trap_CM_MarkFragments(const int num_points, const vec3_t* points, const vec3_t projection, const int maxPoints,
	vec3_t pointBuffer, const int maxFragments, markFragment_t* fragmentBuffer)
{
	return Q_syscall(CG_CM_MARKFRAGMENTS, num_points, points, projection, maxPoints, pointBuffer, maxFragments,
		fragmentBuffer);
}

int trap_S_GetVoiceVolume(const int entityNum)
{
	return Q_syscall(CG_S_GETVOICEVOLUME, entityNum);
}

void trap_S_MuteSound(const int entityNum, const int entchannel)
{
	Q_syscall(CG_S_MUTESOUND, entityNum, entchannel);
}

void trap_S_StartSound(const vec3_t origin, const int entityNum, const int entchannel, const sfxHandle_t sfx)
{
	Q_syscall(CG_S_STARTSOUND, origin, entityNum, entchannel, sfx);
}

void trap_S_StartLocalSound(const sfxHandle_t sfx, const int channelNum)
{
	Q_syscall(CG_S_STARTLOCALSOUND, sfx, channelNum);
}

void trap_S_ClearLoopingSounds(void)
{
	Q_syscall(CG_S_CLEARLOOPINGSOUNDS);
}

void trap_S_AddLoopingSound(const int entityNum, const vec3_t origin, const vec3_t velocity, const sfxHandle_t sfx)
{
	Q_syscall(CG_S_ADDLOOPINGSOUND, entityNum, origin, velocity, sfx);
}

void trap_S_UpdateEntityPosition(const int entityNum, const vec3_t origin)
{
	Q_syscall(CG_S_UPDATEENTITYPOSITION, entityNum, origin);
}

void trap_S_AddRealLoopingSound(const int entityNum, const vec3_t origin, const vec3_t velocity, const sfxHandle_t sfx)
{
	Q_syscall(CG_S_ADDREALLOOPINGSOUND, entityNum, origin, velocity, sfx);
}

void trap_S_StopLoopingSound(const int entityNum)
{
	Q_syscall(CG_S_STOPLOOPINGSOUND, entityNum);
}

void trap_S_Respatialize(const int entityNum, const vec3_t origin, matrix3_t axis, const int inwater)
{
	Q_syscall(CG_S_RESPATIALIZE, entityNum, origin, axis, inwater);
}

void trap_S_ShutUp(const qboolean shutUpFactor)
{
	Q_syscall(CG_S_SHUTUP, shutUpFactor);
}

sfxHandle_t trap_S_RegisterSound(const char* sample)
{
	return Q_syscall(CG_S_REGISTERSOUND, sample);
}

void trap_S_StartBackgroundTrack(const char* intro, const char* loop, const qboolean bReturnWithoutStarting)
{
	Q_syscall(CG_S_STARTBACKGROUNDTRACK, intro, loop, bReturnWithoutStarting);
}

void trap_S_UpdateAmbientSet(const char* name, vec3_t origin)
{
	Q_syscall(CG_S_UPDATEAMBIENTSET, name, origin);
}

void trap_AS_ParseSets(void)
{
	Q_syscall(CG_AS_PARSESETS);
}

void trap_AS_AddPrecacheEntry(const char* name)
{
	Q_syscall(CG_AS_ADDPRECACHEENTRY, name);
}

int trap_S_AddLocalSet(const char* name, vec3_t listener_origin, vec3_t origin, const int ent_id, const int time)
{
	return Q_syscall(CG_S_ADDLOCALSET, name, listener_origin, origin, ent_id, time);
}

sfxHandle_t trap_AS_GetBModelSound(const char* name, const int stage)
{
	return Q_syscall(CG_AS_GETBMODELSOUND, name, stage);
}

void trap_R_LoadWorldMap(const char* mapname)
{
	Q_syscall(CG_R_LOADWORLDMAP, mapname);
}

qhandle_t trap_R_RegisterModel(const char* name)
{
	return Q_syscall(CG_R_REGISTERMODEL, name);
}

qhandle_t trap_R_RegisterSkin(const char* name)
{
	return Q_syscall(CG_R_REGISTERSKIN, name);
}

qhandle_t trap_R_RegisterShader(const char* name)
{
	return Q_syscall(CG_R_REGISTERSHADER, name);
}

qhandle_t trap_R_RegisterShaderNoMip(const char* name)
{
	return Q_syscall(CG_R_REGISTERSHADERNOMIP, name);
}

qhandle_t trap_R_RegisterFont(const char* fontName)
{
	return Q_syscall(CG_R_REGISTERFONT, fontName);
}

int trap_R_Font_StrLenPixels(const char* text, const int iFontIndex, const float scale)
{
	//HACK! RE_Font_StrLenPixels works better with 1.0f scale
	const float width = (float)Q_syscall(CG_R_FONT_STRLENPIXELS, text, iFontIndex, PASSFLOAT(1.0f));
	return width * scale;
}

float trap_R_Font_StrLenPixelsFloat(const char* text, const int iFontIndex, const float scale)
{
	//HACK! RE_Font_StrLenPixels works better with 1.0f scale
	const float width = (float)Q_syscall(CG_R_FONT_STRLENPIXELS, text, iFontIndex, PASSFLOAT(1.0f));
	return width * scale;
}

int trap_R_Font_StrLenChars(const char* text)
{
	return Q_syscall(CG_R_FONT_STRLENCHARS, text);
}

int trap_R_Font_HeightPixels(const int iFontIndex, const float scale)
{
	const float height = (float)Q_syscall(CG_R_FONT_STRHEIGHTPIXELS, iFontIndex, PASSFLOAT(1.0f));
	return height * scale;
}

void trap_R_Font_DrawString(const int ox, const int oy, const char* text, const float* rgba, const int setIndex,
	const int iCharLimit, const float scale)
{
	Q_syscall(CG_R_FONT_DRAWSTRING, ox, oy, text, rgba, setIndex, iCharLimit, PASSFLOAT(scale));
}

qboolean trap_Language_IsAsian(void)
{
	return Q_syscall(CG_LANGUAGE_ISASIAN);
}

qboolean trap_Language_UsesSpaces(void)
{
	return Q_syscall(CG_LANGUAGE_USESSPACES);
}

unsigned int trap_AnyLanguage_ReadCharFromString(const char* psText, int* piAdvanceCount,
	qboolean* pbIsTrailingPunctuation/* = NULL*/)
{
	return Q_syscall(CG_ANYLANGUAGE_READCHARFROMSTRING, psText, piAdvanceCount, pbIsTrailingPunctuation);
}

void trap_R_ClearScene(void)
{
	Q_syscall(CG_R_CLEARSCENE);
}

void trap_R_ClearDecals(void)
{
	Q_syscall(CG_R_CLEARDECALS);
}

void trap_R_AddRefEntityToScene(const refEntity_t* re)
{
	Q_syscall(CG_R_ADDREFENTITYTOSCENE, re);
}

void trap_R_AddPolyToScene(const qhandle_t h_shader, const int numVerts, const polyVert_t* verts)
{
	Q_syscall(CG_R_ADDPOLYTOSCENE, h_shader, numVerts, verts);
}

void trap_R_AddPolysToScene(const qhandle_t h_shader, const int numVerts, const polyVert_t* verts, const int num)
{
	Q_syscall(CG_R_ADDPOLYSTOSCENE, h_shader, numVerts, verts, num);
}

void trap_R_AddDecalToScene(const qhandle_t shader, const vec3_t origin, const vec3_t dir, const float orientation,
	const float r, const float g, const float b, const float a, const qboolean alphaFade,
	const float radius, const qboolean temporary)
{
	Q_syscall(CG_R_ADDDECALTOSCENE, shader, origin, dir, PASSFLOAT(orientation), PASSFLOAT(r), PASSFLOAT(g),
		PASSFLOAT(b), PASSFLOAT(a), alphaFade, PASSFLOAT(radius), temporary);
}

int trap_R_LightForPoint(vec3_t point, vec3_t ambientLight, vec3_t directedLight, vec3_t lightDir)
{
	return Q_syscall(CG_R_LIGHTFORPOINT, point, ambientLight, directedLight, lightDir);
}

void trap_R_AddLightToScene(const vec3_t org, const float intensity, const float r, const float g, const float b)
{
	Q_syscall(CG_R_ADDLIGHTTOSCENE, org, PASSFLOAT(intensity), PASSFLOAT(r), PASSFLOAT(g), PASSFLOAT(b));
}

void trap_R_AddAdditiveLightToScene(const vec3_t org, const float intensity, const float r, const float g,
	const float b)
{
	Q_syscall(CG_R_ADDADDITIVELIGHTTOSCENE, org, PASSFLOAT(intensity), PASSFLOAT(r), PASSFLOAT(g), PASSFLOAT(b));
}

void trap_R_RenderScene(const refdef_t* fd)
{
	Q_syscall(CG_R_RENDERSCENE, fd);
}

void trap_R_SetColor(const float* rgba)
{
	Q_syscall(CG_R_SETCOLOR, rgba);
}

void trap_R_DrawStretchPic(const float x, const float y, const float w, const float h, const float s1, const float t1,
	const float s2, const float t2, const qhandle_t h_shader)
{
	Q_syscall(CG_R_DRAWSTRETCHPIC, PASSFLOAT(x), PASSFLOAT(y), PASSFLOAT(w), PASSFLOAT(h), PASSFLOAT(s1), PASSFLOAT(t1),
		PASSFLOAT(s2), PASSFLOAT(t2), h_shader);
}

void trap_R_ModelBounds(const clip_handle_t model, vec3_t mins, vec3_t maxs)
{
	Q_syscall(CG_R_MODELBOUNDS, model, mins, maxs);
}

int trap_R_LerpTag(orientation_t* tag, const clip_handle_t mod, const int startFrame, const int endFrame,
	const float frac, const char* tagName)
{
	return Q_syscall(CG_R_LERPTAG, tag, mod, startFrame, endFrame, PASSFLOAT(frac), tagName);
}

void trap_R_DrawRotatePic(const float x, const float y, const float w, const float h, const float s1, const float t1,
	const float s2, const float t2, const float a, const qhandle_t h_shader)
{
	Q_syscall(CG_R_DRAWROTATEPIC, PASSFLOAT(x), PASSFLOAT(y), PASSFLOAT(w), PASSFLOAT(h), PASSFLOAT(s1), PASSFLOAT(t1),
		PASSFLOAT(s2), PASSFLOAT(t2), PASSFLOAT(a), h_shader);
}

void trap_R_DrawRotatePic2(const float x, const float y, const float w, const float h, const float s1, const float t1,
	const float s2, const float t2, const float a, const qhandle_t h_shader)
{
	Q_syscall(CG_R_DRAWROTATEPIC2, PASSFLOAT(x), PASSFLOAT(y), PASSFLOAT(w), PASSFLOAT(h), PASSFLOAT(s1), PASSFLOAT(t1),
		PASSFLOAT(s2), PASSFLOAT(t2), PASSFLOAT(a), h_shader);
}

void trap_R_SetRangeFog(const float range)
{
	Q_syscall(CG_R_SETRANGEFOG, PASSFLOAT(range));
}

void trap_R_SetRefractProp(const float alpha, const float stretch, const qboolean prepost, const qboolean negate)
{
	Q_syscall(CG_R_SETREFRACTIONPROP, PASSFLOAT(alpha), PASSFLOAT(stretch), prepost, negate);
}

void trap_R_RemapShader(const char* oldShader, const char* newShader, const char* timeOffset)
{
	Q_syscall(CG_R_REMAP_SHADER, oldShader, newShader, timeOffset);
}

void trap_R_GetLightStyle(const int style, color4ub_t color)
{
	Q_syscall(CG_R_GET_LIGHT_STYLE, style, color);
}

void trap_R_SetLightStyle(const int style, const int color)
{
	Q_syscall(CG_R_SET_LIGHT_STYLE, style, color);
}

void trap_R_GetBModelVerts(const int bmodelIndex, vec3_t* verts, vec3_t normal)
{
	Q_syscall(CG_R_GET_BMODEL_VERTS, bmodelIndex, verts, normal);
}

void trap_R_GetDistanceCull(float* f)
{
	Q_syscall(CG_R_GETDISTANCECULL, f);
}

void trap_R_GetRealRes(int* w, int* h)
{
	Q_syscall(CG_R_GETREALRES, w, h);
}

void trap_R_AutomapElevAdj(const float newHeight)
{
	Q_syscall(CG_R_AUTOMAPELEVADJ, PASSFLOAT(newHeight));
}

qboolean trap_R_InitWireframeAutomap(void)
{
	return Q_syscall(CG_R_INITWIREFRAMEAUTO);
}

void trap_FX_AddLine(vec3_t start, vec3_t end, const float size1, const float size2, const float sizeParm,
	const float alpha1, const float alpha2, const float alphaParm, vec3_t sRGB, vec3_t eRGB,
	const float rgbParm, const int killTime, const qhandle_t shader, const int flags)
{
	Q_syscall(CG_FX_ADDLINE, start, end, PASSFLOAT(size1), PASSFLOAT(size2), PASSFLOAT(sizeParm), PASSFLOAT(alpha1),
		PASSFLOAT(alpha2), PASSFLOAT(alphaParm), sRGB, eRGB, PASSFLOAT(rgbParm), killTime, shader, flags);
}

void trap_GetGlconfig(glconfig_t* glconfig)
{
	Q_syscall(CG_GETGLCONFIG, glconfig);
}

void trap_GetGameState(gameState_t* gamestate)
{
	Q_syscall(CG_GETGAMESTATE, gamestate);
}

void trap_GetCurrentSnapshotNumber(int* snapshotNumber, int* serverTime)
{
	Q_syscall(CG_GETCURRENTSNAPSHOTNUMBER, snapshotNumber, serverTime);
}

qboolean trap_GetSnapshot(const int snapshotNumber, snapshot_t* snapshot)
{
	return Q_syscall(CG_GETSNAPSHOT, snapshotNumber, snapshot);
}

qboolean trap_GetDefaultState(const int entityIndex, entityState_t* state)
{
	return Q_syscall(CG_GETDEFAULTSTATE, entityIndex, state);
}

qboolean trap_GetServerCommand(const int serverCommandNumber)
{
	return Q_syscall(CG_GETSERVERCOMMAND, serverCommandNumber);
}

int trap_GetCurrentCmdNumber(void)
{
	return Q_syscall(CG_GETCURRENTCMDNUMBER);
}

qboolean trap_GetUserCmd(const int cmdNumber, usercmd_t* ucmd)
{
	return Q_syscall(CG_GETUSERCMD, cmdNumber, ucmd);
}

void trap_SetUserCmdValue(const int stateValue, const float sensitivityScale, const float mPitchOverride,
	const float mYawOverride, const float mSensitivityOverride, const int fpSel,
	const int invenSel, const qboolean fighterControls)
{
	Q_syscall(CG_SETUSERCMDVALUE, stateValue, PASSFLOAT(sensitivityScale), PASSFLOAT(mPitchOverride),
		PASSFLOAT(mYawOverride), PASSFLOAT(mSensitivityOverride), fpSel, invenSel, fighterControls);
}

void trap_SetClientForceAngle(const int time, vec3_t angle)
{
	Q_syscall(CG_SETCLIENTFORCEANGLE, time, angle);
}

void trap_SetClientTurnExtent(const float turnAdd, const float turnSub, const int turnTime)
{
	Q_syscall(CG_SETCLIENTTURNEXTENT, PASSFLOAT(turnAdd), PASSFLOAT(turnSub), turnTime);
}

void trap_OpenUIMenu(const int menuID)
{
	Q_syscall(CG_OPENUIMENU, menuID);
}

void testPrintInt(char* string, const int i)
{
	Q_syscall(CG_TESTPRINTINT, string, i);
}

void testPrintFloat(char* string, const float f)
{
	Q_syscall(CG_TESTPRINTFLOAT, string, PASSFLOAT(f));
}

int trap_MemoryRemaining(void)
{
	return Q_syscall(CG_MEMORY_REMAINING);
}

qboolean trap_Key_IsDown(const int keynum)
{
	return Q_syscall(CG_KEY_ISDOWN, keynum);
}

int trap_Key_GetCatcher(void)
{
	return Q_syscall(CG_KEY_GETCATCHER);
}

void trap_Key_SetCatcher(const int catcher)
{
	Q_syscall(CG_KEY_SETCATCHER, catcher);
}

int trap_Key_GetKey(const char* binding)
{
	return Q_syscall(CG_KEY_GETKEY, binding);
}

int trap_PC_AddGlobalDefine(char* define)
{
	return Q_syscall(CG_PC_ADD_GLOBAL_DEFINE, define);
}

int trap_PC_LoadSource(const char* filename)
{
	return Q_syscall(CG_PC_LOAD_SOURCE, filename);
}

int trap_PC_FreeSource(const int handle)
{
	return Q_syscall(CG_PC_FREE_SOURCE, handle);
}

int trap_PC_ReadToken(const int handle, pc_token_t* pc_token)
{
	return Q_syscall(CG_PC_READ_TOKEN, handle, pc_token);
}

int trap_PC_SourceFileAndLine(const int handle, char* filename, int* line)
{
	return Q_syscall(CG_PC_SOURCE_FILE_AND_LINE, handle, filename, line);
}

int trap_PC_LoadGlobalDefines(const char* filename)
{
	return Q_syscall(CG_PC_LOAD_GLOBAL_DEFINES, filename);
}

void trap_PC_RemoveAllGlobalDefines(void)
{
	Q_syscall(CG_PC_REMOVE_ALL_GLOBAL_DEFINES);
}

void trap_S_StopBackgroundTrack(void)
{
	Q_syscall(CG_S_STOPBACKGROUNDTRACK);
}

int trap_RealTime(qtime_t* qtime)
{
	return Q_syscall(CG_REAL_TIME, qtime);
}

void trap_SnapVector(float* v)
{
	Q_syscall(CG_SNAPVECTOR, v);
}

int trap_CIN_PlayCinematic(const char* arg0, const int xpos, const int ypos, const int width, const int height,
	const int bits)
{
	return Q_syscall(CG_CIN_PLAYCINEMATIC, arg0, xpos, ypos, width, height, bits);
}

e_status trap_CIN_StopCinematic(const int handle)
{
	return Q_syscall(CG_CIN_STOPCINEMATIC, handle);
}

e_status trap_CIN_RunCinematic(const int handle)
{
	return Q_syscall(CG_CIN_RUNCINEMATIC, handle);
}

void trap_CIN_DrawCinematic(const int handle)
{
	Q_syscall(CG_CIN_DRAWCINEMATIC, handle);
}

void trap_CIN_SetExtents(const int handle, const int x, const int y, const int w, const int h)
{
	Q_syscall(CG_CIN_SETEXTENTS, handle, x, y, w, h);
}

qboolean trap_GetEntityToken(char* buffer, const int buffer_size)
{
	return Q_syscall(CG_GET_ENTITY_TOKEN, buffer, buffer_size);
}

qboolean trap_R_inPVS(const vec3_t p1, const vec3_t p2, const byte* mask)
{
	return Q_syscall(CG_R_INPVS, p1, p2, mask);
}

int trap_FX_RegisterEffect(const char* file)
{
	return Q_syscall(CG_FX_REGISTER_EFFECT, file);
}

void trap_FX_PlayEffect(const char* file, vec3_t org, vec3_t fwd, const int vol, const int rad)
{
	Q_syscall(CG_FX_PLAY_EFFECT, file, org, fwd, vol, rad);
}

void trap_FX_PlayEntityEffect(const char* file, vec3_t org, matrix3_t axis, const int boltInfo, const int entNum,
	const int vol, const int rad)
{
	Q_syscall(CG_FX_PLAY_ENTITY_EFFECT, file, org, axis, boltInfo, entNum, vol, rad);
}

void trap_FX_PlayEffectID(const int id, vec3_t org, vec3_t fwd, const int vol, const int rad)
{
	Q_syscall(CG_FX_PLAY_EFFECT_ID, id, org, fwd, vol, rad);
}

void trap_FX_PlayPortalEffectID(const int id, vec3_t org, vec3_t fwd, int vol, int rad)
{
	Q_syscall(CG_FX_PLAY_PORTAL_EFFECT_ID, id, org, fwd);
}

void trap_FX_PlayEntityEffectID(const int id, vec3_t org, matrix3_t axis, const int boltInfo, const int entNum,
	const int vol, const int rad)
{
	Q_syscall(CG_FX_PLAY_ENTITY_EFFECT_ID, id, org, axis, boltInfo, entNum, vol, rad);
}

qboolean trap_FX_PlayBoltedEffectID(const int id, vec3_t org, void* ghoul2, const int boltNum, const int entNum,
	const int modelNum, const int iLooptime, const qboolean isRelative)
{
	return Q_syscall(CG_FX_PLAY_BOLTED_EFFECT_ID, id, org, ghoul2, boltNum, entNum, modelNum, iLooptime, isRelative);
}

void trap_FX_AddScheduledEffects(const qboolean skyPortal)
{
	Q_syscall(CG_FX_ADD_SCHEDULED_EFFECTS, skyPortal);
}

void trap_FX_Draw2DEffects(const float screenXScale, const float screenYScale)
{
	Q_syscall(CG_FX_DRAW_2D_EFFECTS, PASSFLOAT(screenXScale), PASSFLOAT(screenYScale));
}

int trap_FX_InitSystem(refdef_t* refdef)
{
	return Q_syscall(CG_FX_INIT_SYSTEM, refdef);
}

void trap_FX_SetRefDef(refdef_t* refdef)
{
	Q_syscall(CG_FX_SET_REFDEF, refdef);
}

qboolean trap_FX_FreeSystem(void)
{
	return Q_syscall(CG_FX_FREE_SYSTEM);
}

void trap_FX_Reset(void)
{
	Q_syscall(CG_FX_RESET);
}

void trap_FX_AdjustTime(const int time)
{
	Q_syscall(CG_FX_ADJUST_TIME, time);
}

void trap_FX_AddPoly(addpolyArgStruct_t* p)
{
	Q_syscall(CG_FX_ADDPOLY, p);
}

void trap_FX_AddBezier(addbezierArgStruct_t* p)
{
	Q_syscall(CG_FX_ADDBEZIER, p);
}

void trap_FX_AddPrimitive(const effectTrailArgStruct_t* p)
{
	Q_syscall(CG_FX_ADDPRIMITIVE, p);
}

void trap_FX_AddSprite(addspriteArgStruct_t* p)
{
	Q_syscall(CG_FX_ADDSPRITE, p);
}

void trap_FX_AddElectricity(addElectricityArgStruct_t* p)
{
	Q_syscall(CG_FX_ADDELECTRICITY, p);
}

qboolean trap_SMP_GetStringTextString(const char* text, char* buffer, const int bufferLength)
{
	return Q_syscall(CG_SP_GETSTRINGTEXTSTRING, text, buffer, bufferLength);
}

qboolean trap_SE_GetStringTextString(const char* text, char* buffer, const int bufferLength)
{
	return Q_syscall(CG_SP_GETSTRINGTEXTSTRING, text, buffer, bufferLength);
}

qboolean trap_ROFF_Clean(void)
{
	return Q_syscall(CG_ROFF_CLEAN);
}

void trap_ROFF_UpdateEntities(void)
{
	Q_syscall(CG_ROFF_UPDATE_ENTITIES);
}

int trap_ROFF_Cache(const char* file)
{
	return Q_syscall(CG_ROFF_CACHE, file);
}

qboolean trap_ROFF_Play(const int ent_id, const int roff_id, const qboolean do_translation)
{
	return Q_syscall(CG_ROFF_PLAY, ent_id, roff_id, do_translation);
}

qboolean trap_ROFF_Purge_Ent(const int ent_id)
{
	return Q_syscall(CG_ROFF_PURGE_ENT, ent_id);
}

void trap_TrueMalloc(void** ptr, const int size)
{
	Q_syscall(CG_TRUEMALLOC, ptr, size);
}

void trap_TrueFree(void** ptr)
{
	Q_syscall(CG_TRUEFREE, ptr);
}

void trap_G2_ListModelSurfaces(void* ghlInfo)
{
	Q_syscall(CG_G2_LISTSURFACES, ghlInfo);
}

void trap_G2_ListModelBones(void* ghlInfo, const int frame)
{
	Q_syscall(CG_G2_LISTBONES, ghlInfo, frame);
}

void trap_G2_SetGhoul2ModelIndexes(void* ghoul2, qhandle_t* model_list, qhandle_t* skin_list)
{
	Q_syscall(CG_G2_SETMODELS, ghoul2, model_list, skin_list);
}

qboolean trap_G2_HaveWeGhoul2Models(void* ghoul2)
{
	return Q_syscall(CG_G2_HAVEWEGHOULMODELS, ghoul2);
}

qboolean trap_G2API_GetBoltMatrix(void* ghoul2, const int modelIndex, const int bolt_index, mdxaBone_t* matrix,
	const vec3_t angles, const vec3_t position, const int frameNum, qhandle_t* model_list,
	vec3_t scale)
{
	return Q_syscall(CG_G2_GETBOLT, ghoul2, modelIndex, bolt_index, matrix, angles, position, frameNum, model_list,
		scale);
}

qboolean trap_G2API_GetBoltMatrix_NoReconstruct(void* ghoul2, const int modelIndex, const int bolt_index,
	mdxaBone_t* matrix, const vec3_t angles, const vec3_t position,
	const int frameNum, qhandle_t* model_list, vec3_t scale)
{
	return Q_syscall(CG_G2_GETBOLT_NOREC, ghoul2, modelIndex, bolt_index, matrix, angles, position, frameNum,
		model_list, scale);
}

qboolean trap_G2API_GetBoltMatrix_NoRecNoRot(void* ghoul2, const int modelIndex, const int bolt_index,
	mdxaBone_t* matrix, const vec3_t angles, const vec3_t position,
	const int frameNum, qhandle_t* model_list, vec3_t scale)
{
	return Q_syscall(CG_G2_GETBOLT_NOREC_NOROT, ghoul2, modelIndex, bolt_index, matrix, angles, position, frameNum,
		model_list, scale);
}

int trap_G2API_InitGhoul2Model(void** ghoul2Ptr, const char* fileName, const int modelIndex,
	const qhandle_t customSkin, const qhandle_t customShader, const int modelFlags,
	const int lodBias)
{
	return Q_syscall(CG_G2_INITGHOUL2MODEL, ghoul2Ptr, fileName, modelIndex, customSkin, customShader, modelFlags,
		lodBias);
}

qboolean trap_G2API_SetSkin(void* ghoul2, const int modelIndex, const qhandle_t customSkin, const qhandle_t render_skin)
{
	return Q_syscall(CG_G2_SETSKIN, ghoul2, modelIndex, customSkin, render_skin);
}

void trap_G2API_CollisionDetect(CollisionRecord_t* collRecMap, void* ghoul2, const vec3_t angles, const vec3_t position,
	const int frameNumber, const int entNum, const vec3_t rayStart, const vec3_t rayEnd,
	const vec3_t scale, const int traceFlags, const int useLod, const float fRadius)
{
	Q_syscall(CG_G2_COLLISIONDETECT, collRecMap, ghoul2, angles, position, frameNumber, entNum, rayStart, rayEnd, scale,
		traceFlags, useLod, PASSFLOAT(fRadius));
}

void trap_G2API_CollisionDetectCache(CollisionRecord_t* collRecMap, void* ghoul2, const vec3_t angles,
	const vec3_t position, const int frameNumber, const int entNum,
	const vec3_t rayStart, const vec3_t rayEnd, const vec3_t scale,
	const int traceFlags, const int useLod, const float fRadius)
{
	Q_syscall(CG_G2_COLLISIONDETECTCACHE, collRecMap, ghoul2, angles, position, frameNumber, entNum, rayStart, rayEnd,
		scale, traceFlags, useLod, PASSFLOAT(fRadius));
}

void trap_G2API_CleanGhoul2Models(void** ghoul2Ptr)
{
	Q_syscall(CG_G2_CLEANMODELS, ghoul2Ptr);
}

qboolean trap_G2API_SetBoneAngles(void* ghoul2, const int modelIndex, const char* boneName, const vec3_t angles,
	const int flags, const int up, const int right, const int forward,
	qhandle_t* model_list, const int blendTime, const int current_time)
{
	return Q_syscall(CG_G2_ANGLEOVERRIDE, ghoul2, modelIndex, boneName, angles, flags, up, right, forward, model_list,
		blendTime, current_time);
}

qboolean trap_G2API_SetBoneAnim(void* ghoul2, const int modelIndex, const char* boneName, const int startFrame,
	const int endFrame, const int flags, const float animSpeed, const int current_time,
	const float setFrame, const int blendTime)
{
	return Q_syscall(CG_G2_PLAYANIM, ghoul2, modelIndex, boneName, startFrame, endFrame, flags, PASSFLOAT(animSpeed),
		current_time, PASSFLOAT(setFrame), blendTime);
}

qboolean trap_G2API_GetBoneAnim(void* ghoul2, const char* boneName, const int current_time, float* current_frame,
	int* startFrame, int* endFrame, int* flags, float* animSpeed, int* model_list,
	const int modelIndex)
{
	return Q_syscall(CG_G2_GETBONEANIM, ghoul2, boneName, current_time, current_frame, startFrame, endFrame, flags,
		animSpeed, model_list, modelIndex);
}

qboolean trap_G2API_GetBoneFrame(void* ghoul2, const char* boneName, const int current_time, float* current_frame,
	int* model_list, const int modelIndex)
{
	return Q_syscall(CG_G2_GETBONEFRAME, ghoul2, boneName, current_time, current_frame, model_list, modelIndex);
}

void trap_G2API_GetGLAName(void* ghoul2, const int modelIndex, char* fillBuf)
{
	Q_syscall(CG_G2_GETGLANAME, ghoul2, modelIndex, fillBuf);
}

int trap_G2API_CopyGhoul2Instance(void* g2_from, void* g2To, const int modelIndex)
{
	return Q_syscall(CG_G2_COPYGHOUL2INSTANCE, g2_from, g2To, modelIndex);
}

void trap_G2API_CopySpecificGhoul2Model(void* g2_from, const int modelFrom, void* g2To, const int modelTo)
{
	Q_syscall(CG_G2_COPYSPECIFICGHOUL2MODEL, g2_from, modelFrom, g2To, modelTo);
}

void trap_G2API_DuplicateGhoul2Instance(void* g2_from, void** g2To)
{
	Q_syscall(CG_G2_DUPLICATEGHOUL2INSTANCE, g2_from, g2To);
}

qboolean trap_G2API_HasGhoul2ModelOnIndex(void* ghlInfo, const int modelIndex)
{
	return Q_syscall(CG_G2_HASGHOUL2MODELONINDEX, ghlInfo, modelIndex);
}

qboolean trap_G2API_RemoveGhoul2Model(void* ghlInfo, const int modelIndex)
{
	return Q_syscall(CG_G2_REMOVEGHOUL2MODEL, ghlInfo, modelIndex);
}

qboolean trap_G2API_SkinlessModel(void* ghlInfo, const int modelIndex)
{
	return Q_syscall(CG_G2_SKINLESSMODEL, ghlInfo, modelIndex);
}

int trap_G2API_GetNumGoreMarks(void* ghlInfo, const int modelIndex)
{
	return Q_syscall(CG_G2_GETNUMGOREMARKS, ghlInfo, modelIndex);
}

void trap_G2API_AddSkinGore(void* ghlInfo, SSkinGoreData* gore)
{
	Q_syscall(CG_G2_ADDSKINGORE, ghlInfo, gore);
}

void trap_G2API_ClearSkinGore(void* ghlInfo)
{
	Q_syscall(CG_G2_CLEARSKINGORE, ghlInfo);
}

int trap_G2API_Ghoul2Size(void* ghlInfo)
{
	return Q_syscall(CG_G2_SIZE, ghlInfo);
}

int trap_G2API_AddBolt(void* ghoul2, const int modelIndex, const char* boneName)
{
	return Q_syscall(CG_G2_ADDBOLT, ghoul2, modelIndex, boneName);
}

qboolean trap_G2API_AttachEnt(int* boltInfo, void* ghlInfoTo, const int toBoltIndex, const int entNum,
	const int toModelNum)
{
	return Q_syscall(CG_G2_ATTACHENT, boltInfo, ghlInfoTo, toBoltIndex, entNum, toModelNum);
}

void trap_G2API_SetBoltInfo(void* ghoul2, const int modelIndex, const int boltInfo)
{
	Q_syscall(CG_G2_SETBOLTON, ghoul2, modelIndex, boltInfo);
}

qboolean trap_G2API_SetRootSurface(void* ghoul2, const int modelIndex, const char* surface_name)
{
	return Q_syscall(CG_G2_SETROOTSURFACE, ghoul2, modelIndex, surface_name);
}

qboolean trap_G2API_SetSurfaceOnOff(void* ghoul2, const char* surface_name, const int flags)
{
	return Q_syscall(CG_G2_SETSURFACEONOFF, ghoul2, surface_name, flags);
}

qboolean trap_G2API_SetNewOrigin(void* ghoul2, const int bolt_index)
{
	return Q_syscall(CG_G2_SETNEWORIGIN, ghoul2, bolt_index);
}

qboolean trap_G2API_DoesBoneExist(void* ghoul2, const int modelIndex, const char* boneName)
{
	return Q_syscall(CG_G2_DOESBONEEXIST, ghoul2, modelIndex, boneName);
}

int trap_G2API_GetSurfaceRenderStatus(void* ghoul2, const int modelIndex, const char* surface_name)
{
	return Q_syscall(CG_G2_GETSURFACERENDERSTATUS, ghoul2, modelIndex, surface_name);
}

int trap_G2API_GetTime(void)
{
	return Q_syscall(CG_G2_GETTIME);
}

void trap_G2API_SetTime(const int time, const int clock)
{
	Q_syscall(CG_G2_SETTIME, time, clock);
}

void trap_G2API_AbsurdSmoothing(void* ghoul2, const qboolean status)
{
	Q_syscall(CG_G2_ABSURDSMOOTHING, ghoul2, status);
}

void trap_G2API_SetRagDoll(void* ghoul2, sharedRagDollParams_t* params)
{
	Q_syscall(CG_G2_SETRAGDOLL, ghoul2, params);
}

void trap_G2API_AnimateG2Models(void* ghoul2, const int time, sharedRagDollUpdateParams_t* params)
{
	Q_syscall(CG_G2_ANIMATEG2MODELS, ghoul2, time, params);
}

qboolean trap_G2API_RagPCJConstraint(void* ghoul2, const char* boneName, vec3_t min, vec3_t max)
{
	return Q_syscall(CG_G2_RAGPCJCONSTRAINT, ghoul2, boneName, min, max);
}

qboolean trap_G2API_RagPCJGradientSpeed(void* ghoul2, const char* boneName, const float speed)
{
	return Q_syscall(CG_G2_RAGPCJGRADIENTSPEED, ghoul2, boneName, PASSFLOAT(speed));
}

qboolean trap_G2API_RagEffectorGoal(void* ghoul2, const char* boneName, vec3_t pos)
{
	return Q_syscall(CG_G2_RAGEFFECTORGOAL, ghoul2, boneName, pos);
}

qboolean trap_G2API_GetRagBonePos(void* ghoul2, const char* boneName, vec3_t pos, vec3_t entAngles, vec3_t entPos,
	vec3_t entScale)
{
	return Q_syscall(CG_G2_GETRAGBONEPOS, ghoul2, boneName, pos, entAngles, entPos, entScale);
}

qboolean trap_G2API_RagEffectorKick(void* ghoul2, const char* boneName, vec3_t velocity)
{
	return Q_syscall(CG_G2_RAGEFFECTORKICK, ghoul2, boneName, velocity);
}

qboolean trap_G2API_RagForceSolve(void* ghoul2, const qboolean force)
{
	return Q_syscall(CG_G2_RAGFORCESOLVE, ghoul2, force);
}

qboolean trap_G2API_SetBoneIKState(void* ghoul2, const int time, const char* boneName, const int ikState,
	sharedSetBoneIKStateParams_t* params)
{
	return Q_syscall(CG_G2_SETBONEIKSTATE, ghoul2, time, boneName, ikState, params);
}

qboolean trap_G2API_IKMove(void* ghoul2, const int time, sharedIKMoveParams_t* params)
{
	return Q_syscall(CG_G2_IKMOVE, ghoul2, time, params);
}

qboolean trap_G2API_RemoveBone(void* ghoul2, const char* boneName, const int modelIndex)
{
	return Q_syscall(CG_G2_REMOVEBONE, ghoul2, boneName, modelIndex);
}

void trap_G2API_AttachInstanceToEntNum(void* ghoul2, const int entityNum, const qboolean server)
{
	Q_syscall(CG_G2_ATTACHINSTANCETOENTNUM, ghoul2, entityNum, server);
}

void trap_G2API_ClearAttachedInstance(const int entityNum)
{
	Q_syscall(CG_G2_CLEARATTACHEDINSTANCE, entityNum);
}

void trap_G2API_CleanEntAttachments(void)
{
	Q_syscall(CG_G2_CLEANENTATTACHMENTS);
}

qboolean trap_G2API_OverrideServer(void* serverInstance)
{
	return Q_syscall(CG_G2_OVERRIDESERVER, serverInstance);
}

void trap_G2API_GetSurfaceName(void* ghoul2, const int surfNumber, const int modelIndex, char* fillBuf)
{
	Q_syscall(CG_G2_GETSURFACENAME, ghoul2, surfNumber, modelIndex, fillBuf);
}

void trap_CG_RegisterSharedMemory(char* memory)
{
	Q_syscall(CG_SET_SHARED_BUFFER, memory);
}

void trap_R_WeatherContentsOverride(const int contents)
{
	Q_syscall(CG_R_WEATHER_CONTENTS_OVERRIDE, contents);
}

void trap_R_WorldEffectCommand(const char* cmd)
{
	Q_syscall(CG_R_WORLDEFFECTCOMMAND, cmd);
}

void trap_WE_AddWeatherZone(vec3_t mins, vec3_t maxs)
{
	Q_syscall(CG_WE_ADDWEATHERZONE, mins, maxs);
}

// Translate import table funcptrs to syscalls

int CGSyscall_FS_Read(void* buffer, const int len, const fileHandle_t f)
{
	trap_FS_Read(buffer, len, f);
	return 0;
}

int CGSyscall_FS_Write(const void* buffer, const int len, const fileHandle_t f)
{
	trap_FS_Write(buffer, len, f);
	return 0;
}

clip_handle_t CGSyscall_CM_TempModel(const vec3_t mins, const vec3_t maxs, const int capsule)
{
	if (capsule) return trap_CM_TempCapsuleModel(mins, maxs);
	return trap_CM_TempBoxModel(mins, maxs);
}

void CGSyscall_CM_Trace(trace_t* results, const vec3_t start, const vec3_t end, const vec3_t mins, const vec3_t maxs,
	const clip_handle_t model, const int brushmask, const int capsule)
{
	if (capsule) trap_CM_CapsuleTrace(results, start, end, mins, maxs, model, brushmask);
	else trap_CM_BoxTrace(results, start, end, mins, maxs, model, brushmask);
}

void CGSyscall_CM_TransformedTrace(trace_t* results, const vec3_t start, const vec3_t end, const vec3_t mins,
	const vec3_t maxs, const clip_handle_t model, const int brushmask,
	const vec3_t origin, const vec3_t angles, const int capsule)
{
	if (capsule) trap_CM_TransformedCapsuleTrace(results, start, end, mins, maxs, model, brushmask, origin, angles);
	else trap_CM_TransformedBoxTrace(results, start, end, mins, maxs, model, brushmask, origin, angles);
}

void CGSyscall_R_AddPolysToScene(const qhandle_t h_shader, const int numVerts, const polyVert_t* verts, int num)
{
	trap_R_AddPolyToScene(h_shader, numVerts, verts);
}

static float CGSyscall_R_GetDistanceCull(void)
{
	float tmp;
	trap_R_GetDistanceCull(&tmp);
	return tmp;
}

static void CGSyscall_FX_PlayEffectID(const int id, vec3_t org, vec3_t fwd, const int vol, const int rad,
	const qboolean isPortal)
{
	if (isPortal) trap_FX_PlayPortalEffectID(id, org, fwd, vol, rad);
	else trap_FX_PlayEffectID(id, org, fwd, vol, rad);
}

static void CGSyscall_G2API_CollisionDetect(CollisionRecord_t* collRecMap, void* ghoul2, const vec3_t angles,
	const vec3_t position, const int frameNumber, const int entNum, vec3_t rayStart,
	vec3_t rayEnd, vec3_t scale, const int traceFlags, const int useLod,
	const float fRadius)
{
	trap_G2API_CollisionDetect(collRecMap, ghoul2, angles, position, frameNumber, entNum, rayStart, rayEnd, scale,
		traceFlags, useLod, fRadius);
}

static void QDECL CG_Error(int level, const char* error, ...)
{
	va_list argptr;
	char text[1024] = { 0 };

	va_start(argptr, error);
	Q_vsnprintf(text, sizeof text, error, argptr);
	va_end(argptr);

	trap_Error(text);
}

static void QDECL CG_Printf(const char* msg, ...)
{
	va_list argptr;
	char text[4096] = { 0 };

	va_start(argptr, msg);
	const int ret = Q_vsnprintf(text, sizeof text, msg, argptr);
	va_end(argptr);

	if (ret == -1)
		trap_Print("CG_Printf: overflow of 4096 bytes buffer\n");
	else
		trap_Print(text);
}

static void TranslateSyscalls(void)
{
	static cgameImport_t import = { 0 };

	trap = &import;

	Com_Error = CG_Error;
	Com_Printf = CG_Printf;

	trap->Print = CG_Printf;
	trap->Error = CG_Error;
	trap->SnapVector = trap_SnapVector;
	trap->MemoryRemaining = trap_MemoryRemaining;
	trap->RegisterSharedMemory = trap_CG_RegisterSharedMemory;
	trap->TrueMalloc = trap_TrueMalloc;
	trap->TrueFree = trap_TrueFree;
	trap->Milliseconds = trap_Milliseconds;
	trap->RealTime = trap_RealTime;
	trap->PrecisionTimerStart = trap_PrecisionTimer_Start;
	trap->PrecisionTimerEnd = trap_PrecisionTimer_End;
	trap->Cvar_Register = trap_Cvar_Register;
	trap->Cvar_Set = trap_Cvar_Set;
	trap->Cvar_Update = trap_Cvar_Update;
	trap->Cvar_VariableStringBuffer = trap_Cvar_VariableStringBuffer;
	trap->AddCommand = trap_AddCommand;
	trap->Cmd_Argc = trap_Argc;
	trap->Cmd_Args = trap_Args;
	trap->Cmd_Argv = trap_Argv;
	trap->RemoveCommand = trap_RemoveCommand;
	trap->SendClientCommand = trap_SendClientCommand;
	trap->SendConsoleCommand = trap_SendConsoleCommand;
	trap->FS_Close = trap_FS_FCloseFile;
	trap->FS_GetFileList = trap_FS_GetFileList;
	trap->FS_Open = trap_FS_FOpenFile;
	trap->FS_Read = CGSyscall_FS_Read;
	trap->FS_Write = CGSyscall_FS_Write;
	trap->UpdateScreen = trap_UpdateScreen;
	trap->CM_InlineModel = trap_CM_InlineModel;
	trap->CM_LoadMap = trap_CM_LoadMap;
	trap->CM_NumInlineModels = trap_CM_NumInlineModels;
	trap->CM_PointContents = trap_CM_PointContents;
	trap->CM_TempModel = CGSyscall_CM_TempModel;
	trap->CM_Trace = CGSyscall_CM_Trace;
	trap->CM_TransformedPointContents = trap_CM_TransformedPointContents;
	trap->CM_TransformedTrace = CGSyscall_CM_TransformedTrace;
	trap->S_AddLocalSet = trap_S_AddLocalSet;
	trap->S_AddLoopingSound = trap_S_AddLoopingSound;
	trap->S_ClearLoopingSounds = trap_S_ClearLoopingSounds;
	trap->S_GetVoiceVolume = trap_S_GetVoiceVolume;
	trap->S_MuteSound = trap_S_MuteSound;
	trap->S_RegisterSound = trap_S_RegisterSound;
	trap->S_Respatialize = trap_S_Respatialize;
	trap->S_Shutup = trap_S_ShutUp;
	trap->S_StartBackgroundTrack = trap_S_StartBackgroundTrack;
	trap->S_StartLocalSound = trap_S_StartLocalSound;
	trap->S_StartSound = trap_S_StartSound;
	trap->S_StopBackgroundTrack = trap_S_StopBackgroundTrack;
	trap->S_StopLoopingSound = trap_S_StopLoopingSound;
	trap->S_UpdateEntityPosition = trap_S_UpdateEntityPosition;
	trap->S_UpdateAmbientSet = trap_S_UpdateAmbientSet;
	trap->AS_AddPrecacheEntry = trap_AS_AddPrecacheEntry;
	trap->AS_GetBModelSound = trap_AS_GetBModelSound;
	trap->AS_ParseSets = trap_AS_ParseSets;
	trap->R_AddAdditiveLightToScene = trap_R_AddAdditiveLightToScene;
	trap->R_AddDecalToScene = trap_R_AddDecalToScene;
	trap->R_AddLightToScene = trap_R_AddLightToScene;
	trap->R_AddPolysToScene = CGSyscall_R_AddPolysToScene;
	trap->R_AddRefEntityToScene = trap_R_AddRefEntityToScene;
	trap->R_AnyLanguage_ReadCharFromString = trap_AnyLanguage_ReadCharFromString;
	trap->R_AutomapElevationAdjustment = trap_R_AutomapElevAdj;
	trap->R_ClearDecals = trap_R_ClearDecals;
	trap->R_ClearScene = trap_R_ClearScene;
	trap->R_DrawStretchPic = trap_R_DrawStretchPic;
	trap->R_DrawRotatePic = trap_R_DrawRotatePic;
	trap->R_DrawRotatePic2 = trap_R_DrawRotatePic2;
	trap->R_Font_DrawString = trap_R_Font_DrawString;
	trap->R_Font_HeightPixels = trap_R_Font_HeightPixels;
	trap->R_Font_StrLenChars = trap_R_Font_StrLenChars;
	trap->R_Font_StrLenPixels = trap_R_Font_StrLenPixels;
	trap->R_GetBModelVerts = trap_R_GetBModelVerts;
	trap->R_GetDistanceCull = CGSyscall_R_GetDistanceCull;
	trap->R_GetEntityToken = trap_GetEntityToken;
	trap->R_GetLightStyle = trap_R_GetLightStyle;
	trap->R_GetRealRes = trap_R_GetRealRes;
	trap->R_InitializeWireframeAutomap = trap_R_InitWireframeAutomap;
	trap->R_InPVS = trap_R_inPVS;
	trap->R_Language_IsAsian = trap_Language_IsAsian;
	trap->R_Language_UsesSpaces = trap_Language_UsesSpaces;
	trap->R_LerpTag = trap_R_LerpTag;
	trap->R_LightForPoint = trap_R_LightForPoint;
	trap->R_LoadWorld = trap_R_LoadWorldMap;
	trap->R_MarkFragments = trap_CM_MarkFragments;
	trap->R_ModelBounds = trap_R_ModelBounds;
	trap->R_RegisterFont = trap_R_RegisterFont;
	trap->R_RegisterModel = trap_R_RegisterModel;
	trap->R_RegisterShader = trap_R_RegisterShader;
	trap->R_RegisterShaderNoMip = trap_R_RegisterShaderNoMip;
	trap->R_RegisterSkin = trap_R_RegisterSkin;
	trap->R_RemapShader = trap_R_RemapShader;
	trap->R_RenderScene = trap_R_RenderScene;
	trap->R_SetColor = trap_R_SetColor;
	trap->R_SetLightStyle = trap_R_SetLightStyle;
	trap->R_SetRangedFog = trap_R_SetRangeFog;
	trap->R_SetRefractionProperties = trap_R_SetRefractProp;
	trap->R_WorldEffectCommand = trap_R_WorldEffectCommand;
	trap->WE_AddWeatherZone = trap_WE_AddWeatherZone;
	trap->GetCurrentSnapshotNumber = trap_GetCurrentSnapshotNumber;
	trap->GetCurrentCmdNumber = trap_GetCurrentCmdNumber;
	trap->GetDefaultState = trap_GetDefaultState;
	trap->GetGameState = trap_GetGameState;
	trap->GetGlconfig = trap_GetGlconfig;
	trap->GetServerCommand = trap_GetServerCommand;
	trap->GetSnapshot = trap_GetSnapshot;
	trap->GetUserCmd = trap_GetUserCmd;
	trap->OpenUIMenu = trap_OpenUIMenu;
	trap->SetClientForceAngle = trap_SetClientForceAngle;
	trap->SetUserCmdValue = trap_SetUserCmdValue;
	trap->Key_GetCatcher = trap_Key_GetCatcher;
	trap->Key_GetKey = trap_Key_GetKey;
	trap->Key_IsDown = trap_Key_IsDown;
	trap->Key_SetCatcher = trap_Key_SetCatcher;
	trap->PC_AddGlobalDefine = trap_PC_AddGlobalDefine;
	trap->PC_FreeSource = trap_PC_FreeSource;
	trap->PC_LoadGlobalDefines = trap_PC_LoadGlobalDefines;
	trap->PC_LoadSource = trap_PC_LoadSource;
	trap->PC_ReadToken = trap_PC_ReadToken;
	trap->PC_RemoveAllGlobalDefines = trap_PC_RemoveAllGlobalDefines;
	trap->PC_SourceFileAndLine = trap_PC_SourceFileAndLine;
	trap->CIN_DrawCinematic = trap_CIN_DrawCinematic;
	trap->CIN_PlayCinematic = trap_CIN_PlayCinematic;
	trap->CIN_RunCinematic = trap_CIN_RunCinematic;
	trap->CIN_SetExtents = trap_CIN_SetExtents;
	trap->CIN_StopCinematic = trap_CIN_StopCinematic;
	trap->FX_AddLine = trap_FX_AddLine;
	trap->FX_RegisterEffect = trap_FX_RegisterEffect;
	trap->FX_PlayEffect = trap_FX_PlayEffect;
	trap->FX_PlayEffectID = CGSyscall_FX_PlayEffectID;
	trap->FX_PlayEntityEffectID = trap_FX_PlayEntityEffectID;
	trap->FX_PlayBoltedEffectID = trap_FX_PlayBoltedEffectID;
	trap->FX_AddScheduledEffects = trap_FX_AddScheduledEffects;
	trap->FX_InitSystem = trap_FX_InitSystem;
	trap->FX_SetRefDef = trap_FX_SetRefDef;
	trap->FX_FreeSystem = trap_FX_FreeSystem;
	trap->FX_AdjustTime = trap_FX_AdjustTime;
	trap->FX_Draw2DEffects = trap_FX_Draw2DEffects;
	trap->FX_AddPoly = trap_FX_AddPoly;
	trap->FX_AddBezier = trap_FX_AddBezier;
	trap->FX_AddPrimitive = trap_FX_AddPrimitive;
	trap->FX_AddSprite = trap_FX_AddSprite;
	trap->FX_AddElectricity = trap_FX_AddElectricity;
	trap->SMP_GetStringTextString = trap_SMP_GetStringTextString;
	trap->SE_GetStringTextString = trap_SE_GetStringTextString;
	trap->ROFF_Clean = trap_ROFF_Clean;
	trap->ROFF_UpdateEntities = trap_ROFF_UpdateEntities;
	trap->ROFF_Cache = trap_ROFF_Cache;
	trap->ROFF_Play = trap_ROFF_Play;
	trap->ROFF_Purge_Ent = trap_ROFF_Purge_Ent;
	trap->G2_ListModelSurfaces = trap_G2_ListModelSurfaces;
	trap->G2_ListModelBones = trap_G2_ListModelBones;
	trap->G2_SetGhoul2ModelIndexes = trap_G2_SetGhoul2ModelIndexes;
	trap->G2_HaveWeGhoul2Models = trap_G2_HaveWeGhoul2Models;
	trap->G2API_GetBoltMatrix = trap_G2API_GetBoltMatrix;
	trap->G2API_GetBoltMatrix_NoReconstruct = trap_G2API_GetBoltMatrix_NoReconstruct;
	trap->G2API_GetBoltMatrix_NoRecNoRot = trap_G2API_GetBoltMatrix_NoRecNoRot;
	trap->G2API_InitGhoul2Model = trap_G2API_InitGhoul2Model;
	trap->G2API_SetSkin = trap_G2API_SetSkin;
	trap->G2API_CollisionDetect = CGSyscall_G2API_CollisionDetect;
	trap->G2API_CollisionDetectCache = CGSyscall_G2API_CollisionDetect;
	trap->G2API_CleanGhoul2Models = trap_G2API_CleanGhoul2Models;
	trap->G2API_SetBoneAngles = trap_G2API_SetBoneAngles;
	trap->G2API_SetBoneAnim = trap_G2API_SetBoneAnim;
	trap->G2API_GetBoneAnim = trap_G2API_GetBoneAnim;
	trap->G2API_GetBoneFrame = trap_G2API_GetBoneFrame;
	trap->G2API_GetGLAName = trap_G2API_GetGLAName;
	trap->G2API_CopyGhoul2Instance = trap_G2API_CopyGhoul2Instance;
	trap->G2API_CopySpecificGhoul2Model = trap_G2API_CopySpecificGhoul2Model;
	trap->G2API_DuplicateGhoul2Instance = trap_G2API_DuplicateGhoul2Instance;
	trap->G2API_HasGhoul2ModelOnIndex = trap_G2API_HasGhoul2ModelOnIndex;
	trap->G2API_RemoveGhoul2Model = trap_G2API_RemoveGhoul2Model;
	trap->G2API_SkinlessModel = trap_G2API_SkinlessModel;
	trap->G2API_GetNumGoreMarks = trap_G2API_GetNumGoreMarks;
	trap->G2API_AddSkinGore = trap_G2API_AddSkinGore;
	trap->G2API_ClearSkinGore = trap_G2API_ClearSkinGore;
	trap->G2API_Ghoul2Size = trap_G2API_Ghoul2Size;
	trap->G2API_AddBolt = trap_G2API_AddBolt;
	trap->G2API_AttachEnt = trap_G2API_AttachEnt;
	trap->G2API_SetBoltInfo = trap_G2API_SetBoltInfo;
	trap->G2API_SetRootSurface = trap_G2API_SetRootSurface;
	trap->G2API_SetSurfaceOnOff = trap_G2API_SetSurfaceOnOff;
	trap->G2API_SetNewOrigin = trap_G2API_SetNewOrigin;
	trap->G2API_DoesBoneExist = trap_G2API_DoesBoneExist;
	trap->G2API_GetSurfaceRenderStatus = trap_G2API_GetSurfaceRenderStatus;
	trap->G2API_GetTime = trap_G2API_GetTime;
	trap->G2API_SetTime = trap_G2API_SetTime;
	trap->G2API_AbsurdSmoothing = trap_G2API_AbsurdSmoothing;
	trap->G2API_SetRagDoll = trap_G2API_SetRagDoll;
	trap->G2API_AnimateG2Models = trap_G2API_AnimateG2Models;
	trap->G2API_RagPCJConstraint = trap_G2API_RagPCJConstraint;
	trap->G2API_RagPCJGradientSpeed = trap_G2API_RagPCJGradientSpeed;
	trap->G2API_RagEffectorGoal = trap_G2API_RagEffectorGoal;
	trap->G2API_GetRagBonePos = trap_G2API_GetRagBonePos;
	trap->G2API_RagEffectorKick = trap_G2API_RagEffectorKick;
	trap->G2API_RagForceSolve = trap_G2API_RagForceSolve;
	trap->G2API_SetBoneIKState = trap_G2API_SetBoneIKState;
	trap->G2API_IKMove = trap_G2API_IKMove;
	trap->G2API_RemoveBone = trap_G2API_RemoveBone;
	trap->G2API_AttachInstanceToEntNum = trap_G2API_AttachInstanceToEntNum;
	trap->G2API_ClearAttachedInstance = trap_G2API_ClearAttachedInstance;
	trap->G2API_CleanEntAttachments = trap_G2API_CleanEntAttachments;
	trap->G2API_OverrideServer = trap_G2API_OverrideServer;
	trap->G2API_GetSurfaceName = trap_G2API_GetSurfaceName;

	trap->ext.R_Font_StrLenPixels = trap_R_Font_StrLenPixelsFloat;
}