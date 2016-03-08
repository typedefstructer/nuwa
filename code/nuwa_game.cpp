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

#define ASSERT(E) if(!(E)) { MessageBox(0, _STRINGIZE_(__LINE__) ## _STRINGIZE_(__FILE__) ,"assert error", MB_OK);

struct game_memory
{
	u64 permanent_storage_size;
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

internal void
gameupdateandrender(game_bitmap_buffer *buffer, game_memory *memory, game_input *input)
{
}
