#include "Helper.h"
#include "Todo.h"
#include <algorithm>

using std::fill;

// TODO 1: Check the number of liberties of a given stone. Mark the
// connected stones and liberties, and return the number of liberties.

// helper func
void floodfill(const Stone board[][19], int row, int col, bool connected_part[][19], bool liberties[][19], Stone player)
{
	if (row < 0 || row > 18 || col < 0 || col > 18)
		return;
	if (connected_part[row][col])
		return;
	if (board[row][col] == player)
	{
		connected_part[row][col] = true;
		floodfill(board, row - 1, col, connected_part, liberties, player);
		floodfill(board, row, col - 1, connected_part, liberties, player);
		floodfill(board, row + 1, col, connected_part, liberties, player);
		floodfill(board, row, col + 1, connected_part, liberties, player);
	}
	else if (board[row][col] == 0)
	{
		liberties[row][col] = true;
	}
}

int check_liberties(const Stone board[][19], int row, int col, bool connected_part[][19], bool liberties[][19])
{
	if (col < 0 || row < 0 || col > 18 || row > 18)
	{
		return -1;
	}
	if (board[row][col] == 0)
	{
		return -2;
	}
	floodfill(board, row, col, connected_part, liberties, board[row][col]);
	int count = 0;
	for (int i = 0; i < 19; i++)
		for (int j = 0; j < 19; j++)
			if (liberties[i][j])
				count++;
	return count;
}

// TODO 2: Check whether a player has captured stones, and mark them.

bool find_captured(const Stone board[][19], Stone player, bool captured[][19])
{
	bool visited[19][19], connected_part[19][19], liberties[19][19];
	bool has_captured = false;
	fill(&visited[0][0], &visited[18][18] + 1, false);
	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 19; j++)
		{
			if (!visited[i][j] && board[i][j] == player)
			{
				fill(&connected_part[0][0], &connected_part[18][18] + 1, false);
				fill(&liberties[0][0], &liberties[18][18] + 1, false);
				bool curr_captured = check_liberties(board, i, j, connected_part, liberties) == 0;
				has_captured = has_captured || curr_captured;
				for (int ci = 0; ci < 19; ci++)
					for (int cj = 0; cj < 19; cj++)
						if (connected_part[ci][cj])
						{
							captured[ci][cj] = curr_captured;
							visited[ci][cj] = true;
						}
			}
		}
	}
	return has_captured;
}

// TODO 3: Check whether the move is valid and edit the board.

// helper func
int place_stone(Stone board[][19], Stone player, int row, int col)
{
	if (col < 0 || row < 0 || col > 18 || row > 18)
	{
		return -1;
	}
	if (board[row][col] != 0)
	{
		return -2;
	}
	board[row][col] = player;
	bool my_death[19][19], opponent_death[19][19];
	for (int i = 0; i < 19; ++i)
		for (int j = 0; j < 19; ++j)
			my_death[i][j] = opponent_death[i][j] = false;
	bool my_has_death, opponent_has_death;
	my_has_death = find_captured(board, player, my_death);
	opponent_has_death = find_captured(board, switch_player(player), opponent_death);
	if (my_has_death && !opponent_has_death)
	{
		board[row][col] = Empty;
		return -3;
	}
	if (opponent_has_death)
	{
		for (int i = 0; i < 19; ++i)
		{
			for (int j = 0; j < 19; ++j)
			{
				if (opponent_death[i][j])
					board[i][j] = Empty;
			}
		}
	}
	return 0;
}

int edit(Stone board[][19], Stone player, int row, int col, int record[][2], int &counter, int &max_steps)
{
	int rv = place_stone(board, player, row, col);
	if (rv != 0)
		return rv;
	record[counter][0] = row, record[counter][1] = col;
	counter++;
	max_steps = counter;
	return 1;
}

// TODO 4: Jump to the given step of the game.

void jump_to(Stone board[][19], int target, int record[][2], int &counter, int &max_steps)
{
	initialize_board(board);
	for (int i = 0; i < target; i++)
		if (record[i][0] != -1 && record[i][1] != -1)
			place_stone(board, Stone(2 - (i + 1) % 2), record[i][0], record[i][1]);
	counter = target;
}