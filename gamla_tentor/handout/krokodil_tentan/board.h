#pragma once

#include <stdbool.h>

/**
 * @typedef tile_t
 * @brief Represents a tile on the game board.
 */
typedef struct tile tile_t;

/**
 * @brief Creates a new tile on the board.
 *
 * @param next Pointer to the next tile in the board, or NULL if this is the last tile.
 * @return A pointer to the newly allocated tile
 */
tile_t *tile_create(tile_t *next);

/**
 * @brief Frees all memory reachable from the given tile.
 *
 * @param tile Pointer to the first tile to free.
 */
void tile_destroy(tile_t *tile);

/**
 * @brief Adds a chute or ladder from one tile to another, unless
 * the starting tile already has one.
 *
 * @param from The tile where the chute or ladder starts.
 * @param to   The tile where it ends.
 * @return `true` if the link was successfully added, otherwise `false`.
 */
bool tile_add_link(tile_t *from, tile_t *to);

/**
 * @brief Checks whether a piece is currently on the given tile.
 *
 * @param tile The tile to check.
 * @return `true` if the piece is on the tile, otherwise `false`.
 */
bool tile_has_piece(tile_t *tile);

/**
 * @brief Moves the piece a given number of steps forward along the board.
 *
 * Moves the piece `steps` tiles forward starting from `start`. If
 * the piece ends up on a tile that has a chute or ladder, it
 * follows that link automatically to the destination tile (but
 * does not follow chutes or ladders from there). If the piece
 * reaches the end of the board, it stops on the last tile.
 *
 * @param start The starting tile (either where the piece currently is, or where it should be placed).
 * @param steps The number of steps to move forward.
 * @return A pointer to the tile where the piece finally ends up.
 */
tile_t *tile_move(tile_t *start, size_t steps);