#pragma once

#define WIN_WIDTH 896.0
#define WIN_HEIGHT 504.0


#define SHADOW_WIDTH  1024.0f //IF YOU CHANGE THIS SIZE; CHANGE IT IN THE FINAL PASS AS WELL.
#define SHADOW_HEIGHT 1024.0f // Thats right, to lazy for cbuffers

//////////////////////////////////////////
//DIS IS BECAUSE JOHAN ASKED ME TO CHANGE AMOUNT OF TEXTURES TO MUCH
#define TEXTUREAMOUNT		6
#define SHADOWMAPAMOUNT     1
#define LIGHT_SHADOW_AMOUNT 1
////////////////////////////////////

#define MAX_NUM_POINTLIGHTS 5
#define MAX_NUM_DIRLIGHTS	1
#define MAX_NUM_SPOTLIGHTS	2

#define UNIQUE_INSTANCED_BUFFER_AMOUNT 3 //atm, three unique buffers are used, for world matrix, animation and billboard
#define INSTANCED_WORLD_BUFFER_AMOUNT  5


#define MAX_INSTANCED_GEOMETRY   200
#define MAX_BILLBOARDED_GEOMETRY 500

#define WINDOWED    true
#define WIREFRAME   false
#define VSYNC		1 //1 == ON, 0 = OFF
#define FPS_CAP		60

#define DEBUG 2/* 2 = debug,  or : D3D11_CREATE_DEVICE_DEBUG */

#define D3D_DEBUG_INFO
#define TEXTURE_PATH "textures/"
#define MODEL_PATH   "models/"

//IMPORTANT! This is only to be used with directX COM objects
#define SAFE_RELEASE(p) { if ( (p) && (p) != nullptr ) { (p)->Release(); (p) = 0; } }


#define _CRTDBG_MAP_ALLOC
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)


#define CBUFFERPERFRAME_INDEX			 0	// BUFFER INDEX
#define WORLDBUFFER_INDEX				 1	// BUFFER INDEX
#define SAMPLEBOOLSBUFFER_INDEX			 2	// BUFFER INDEX
#define UV_BUFFER_INDEX					 3  // BUFFER INDEX



#define SPOTLIGHTS_BUFFER_INDEX			 7  // TEXTURE INDEX
#define POINTLIGHTS_BUFFER_INDEX		 8  // TEXTURE INDEX
#define DIRLIGHTS_BUFFER_INDEX			 9  // TEXTURE INDEX

#define ANIMHEADER_BUFFER_INDEX			 10 // TEXTURE INDEX
#define MORPHANIM_BUFFER_START_INDEX     11	//Texture INDEX + amount of Animations
