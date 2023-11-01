#pragma once

#define MAIN_TIMER			1
#define MENU_TIMER			2

#define MAIN_TIMER_FRAME	20 // 60FPS
#define MENU_TIMER_FRAME	150

#define FRAME_WIDTH			1200
#define FRAME_HEIGHT		720

#define MEM_WIDTH			1200
#define MEM_HEIGHT			1550

#define STAGE2_HEIGHT		4650

#define PLAYER_IMAGE_SIZE	512
#define CLOUD_IMAGE_SIZE	512
#define RAINCLOUD_IMAGE		329
#define RAIN_IMAGE			562

#define CLOUD_WIDTH						172
#define CLOUD_HEIGHT					130
#define CLOUD_COLLIDE_WIDTH				150
#define CLOUD_COLLIDE_HEIGHT			90

#define ITEM_SIZE						30
#define ITEM_START						700

#define PLAYER_WIDTH					CLOUD_WIDTH / 2
#define PLAYER_HEIGHT					CLOUD_HEIGHT - 50
#define PLAYER_COLLIDE_WIDTH			CLOUD_WIDTH / 2 - 35

#define PLAYERMOVE_STOP					MEM_HEIGHT - (FRAME_HEIGHT/2)		//처음 시작할 때 플레이어가 화면의 높이 반 이상 올라가면 화면 움직임
#define PLAYERMOVE_START				FRAME_HEIGHT/2						//나중에 클리어 지점에 가까이 도달했을 때는 화면 멈추고 플레이어만 움직임
#define PLAYER_FIRSTY					MEM_HEIGHT- (CLOUD_HEIGHT + 50)		//플레이어 처음 시작 y지점

#define HPBAR_WIDTH						500

#define EFFECT_NUMBER					5		//효과음 개수

// KeyBoard
enum VK_KEY
{
	VK_0 = 48,
	VK_1 = 49,
	VK_2,
	VK_3,
	VK_4,
	VK_5,
	VK_6,
	VK_7,
	VK_8,
	VK_9,
	VK_A = 0x41,
	VK_B,
	VK_C,
	VK_D,
	VK_E,
	VK_F,
	VK_G,
	VK_H,
	VK_I,
	VK_J,
	VK_K,
	VK_L,
	VK_M,
	VK_N,
	VK_O,
	VK_P,
	VK_Q,
	VK_R,
	VK_S,
	VK_T,
	VK_U,
	VK_V,
	VK_W,
	VK_X,
	VK_Y,
	VK_Z
};
// KeyBoard
