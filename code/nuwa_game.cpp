#include<stdint.h>

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float r32;
typedef double r64;

typedef i32 b32;

#define KB(N) ((N)*1000LL)
#define MB(N) (KB(N)*1000)
#define GB(N) (MB(N)*1000)
#define TB(N) (GB(N)*1000)

#define __STRINGIZE__(X) #X
#define _STRINGIZE_(x) __STRINGIZE__(x)

#define ASSERT(E) if(!(E)) { MessageBox(0, _STRINGIZE_(__LINE__) ## _STRINGIZE_(__FILE__) ,"assert error", MB_OK); *(int*)0=0;}

struct game_memory
{
	u64 permanent_storage_size;
	u64 permanent_storage_used_size;
	void *permanent_storage;

	b32 is_initialized;
};

struct game_bitmap_buffer
{
	int width;
	int height;
	void *memory;
};

struct game_controller
{
	b32 moveup;
	b32 movedown;
	b32 moveleft;
	b32 moveright;
};

struct game_input
{
	game_controller controllers[5];
};

internal void *
alloc_mem(game_memory *memory, size_t size)
{

}

#include "nuwa.cpp"
#include <stdlib.h>

internal i32
custom_rand()
{
	return rand();
}

internal void
gameupdateandrender(game_bitmap_buffer *buffer, game_memory *memory, game_input *input)
{
	game_state *gamestate = (game_state *)memory->permanent_storage;
	if(!memory->is_initialized)
	{
		gamestate->dir = right;
		gamestate->cell_size = 20;

		gamestate->max_cell_x = buffer->width / gamestate->cell_size;
		gamestate->max_cell_y = buffer->height/ gamestate->cell_size;

		gamestate->food.x = custom_rand()%(gamestate->max_cell_x);
		gamestate->food.y = custom_rand()%(gamestate->max_cell_y);

		nuwa_add(memory, &gamestate->player, 0, 0);
	}

	game_controller *keyboard = &input->controllers[0];

	if(keyboard->moveup)
	{
		gamestate->dir = up;
	}

	if(keyboard->movedown)
	{
		gamestate->dir = down;
	}

	if(keyboard->moveleft)
	{
		gamestate->dir = left;
	}

	if(keyboard->moveright)
	{
		gamestate->dir = right;
	}

	switch(gamestate->dir)
	{
		case up:
			break;
		case down:
			break;
		case left:
			break;
		case right:
			break;
	}
}
