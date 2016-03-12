enum direction
{
	up, down, left, right
};

struct game_block
{
	r32 x, y;
};

struct nuwa
{
	game_block cur;
	game_block *next;
	game_block *prev;
};

struct game_state
{
	i32 cell_size;
	i32 max_cell_x, max_cell_y;
	direction dir;
	game_block food;
	nuwa player;
};

internal void
nuwa_add(game_memory *memory, nuwa *head, r32 x, r32 y)
{
	head->cur.x = x;
	head->cur.y = y;
	head->next = head->prev = 0;

}
