#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <CUnit/Basic.h>

#include "board.h"

int init_suite(void)
{
  return 0;
}

int clean_suite(void)
{
  return 0;
}

void test_tile_create(void)
{
  tile_t *t = tile_create(NULL);
  CU_ASSERT_PTR_NOT_NULL_FATAL(t);
  CU_ASSERT_FALSE(tile_has_piece(t));
  tile_destroy(t);
}

void test_link_single(void)
{
  tile_t *a = tile_create(NULL);
  tile_t *b = tile_create(NULL);

  CU_ASSERT_PTR_NOT_NULL_FATAL(a);
  CU_ASSERT_PTR_NOT_NULL_FATAL(b);

  CU_ASSERT_TRUE(tile_add_link(a, b));
  CU_ASSERT_FALSE(tile_add_link(a, b)); // cannot add second link

  tile_destroy(a);
  tile_destroy(b);
}

void test_move_simple_chain(void)
{
  // Build chain: a -> b -> c -> d
  tile_t *d = tile_create(NULL);
  tile_t *c = tile_create(d);
  tile_t *b = tile_create(c);
  tile_t *a = tile_create(b);

  CU_ASSERT_PTR_NOT_NULL_FATAL(a);

  // Move 2 steps forward: should land on c
  CU_ASSERT_EQUAL(tile_has_piece(a), false);
  tile_t *pos = tile_move(a, 2);
  CU_ASSERT_PTR_EQUAL(pos, c);
  CU_ASSERT_TRUE(tile_has_piece(c));
  CU_ASSERT_FALSE(tile_has_piece(a));
  CU_ASSERT_FALSE(tile_has_piece(b));

  tile_destroy(a);
}

void test_move_with_ladder(void)
{
  // Build chain: a -> b -> c -> d -> e
  tile_t *e = tile_create(NULL);
  tile_t *d = tile_create(e);
  tile_t *c = tile_create(d);
  tile_t *b = tile_create(c);
  tile_t *a = tile_create(b);

  // Add a link (ladder) from c to e
  CU_ASSERT_TRUE(tile_add_link(c, e));

  // Move 2 steps from a (lands on c, follows ladder to e)
  tile_t *pos = tile_move(a, 2);
  CU_ASSERT_PTR_EQUAL(pos, e);
  CU_ASSERT_FALSE(tile_has_piece(a));
  CU_ASSERT_TRUE(tile_has_piece(e));
  CU_ASSERT_FALSE(tile_has_piece(c));

  tile_destroy(a);
}

void test_move_collide(void)
{
  // Build chain: a -> b -> c
  tile_t *c = tile_create(NULL);
  tile_t *b = tile_create(c);
  tile_t *a = tile_create(b);

  // Move 2 steps from a (lands on c)
  tile_t *pos = tile_move(a, 2);
  CU_ASSERT_PTR_EQUAL(pos, c);
  CU_ASSERT_FALSE(tile_has_piece(a));
  CU_ASSERT_TRUE(tile_has_piece(c));

  // Try to move another piece 2 steps from a. Don't move anywhere
  pos = tile_move(a, 2);
  CU_ASSERT_PTR_EQUAL(pos, a);
  CU_ASSERT_TRUE(tile_has_piece(a));
  CU_ASSERT_TRUE(tile_has_piece(c));

  tile_destroy(a);
}

void test_move_with_chute(void)
{
  // Build chain: a -> b -> c -> d -> e
  tile_t *e = tile_create(NULL);
  tile_t *d = tile_create(e);
  tile_t *c = tile_create(d);
  tile_t *b = tile_create(c);
  tile_t *a = tile_create(b);

  // Add a link (chute) from d to b
  CU_ASSERT_TRUE(tile_add_link(d, b));

  // Move 3 steps from a (lands on d, follows chute back to b)
  tile_t *pos = tile_move(a, 3);
  CU_ASSERT_PTR_EQUAL(pos, b);
  CU_ASSERT_FALSE(tile_has_piece(a));
  CU_ASSERT_TRUE(tile_has_piece(b));
  CU_ASSERT_FALSE(tile_has_piece(d));

  tile_destroy(a);
}

void test_collide_with_chute(void)
{
  // Build chain: a -> b -> c -> d -> e
  tile_t *e = tile_create(NULL);
  tile_t *d = tile_create(e);
  tile_t *c = tile_create(d);
  tile_t *b = tile_create(c);
  tile_t *a = tile_create(b);

  // Add a link (chute) from d to b
  CU_ASSERT_TRUE(tile_add_link(d, b));

  // Move 3 steps from a (lands on d, follows chute back to b)
  tile_t *pos = tile_move(a, 3);
  CU_ASSERT_PTR_EQUAL(pos, b);
  CU_ASSERT_FALSE(tile_has_piece(a));
  CU_ASSERT_TRUE(tile_has_piece(b));
  CU_ASSERT_FALSE(tile_has_piece(d));

  // Try to move 3 steps from a (lands on d, follows chute back to b which is taken, stay on a)
  pos = tile_move(a, 3);
  CU_ASSERT_PTR_EQUAL(pos, a);
  CU_ASSERT_TRUE(tile_has_piece(a));
  CU_ASSERT_TRUE(tile_has_piece(b));
  CU_ASSERT_FALSE(tile_has_piece(d));

  tile_destroy(a);
}

void test_move_to_end(void)
{
  // Build chain: a -> b -> c
  tile_t *c = tile_create(NULL);
  tile_t *b = tile_create(c);
  tile_t *a = tile_create(b);

  // Move 5 steps (beyond end): should stop at c
  tile_t *pos = tile_move(a, 5);
  CU_ASSERT_PTR_EQUAL(pos, c);
  CU_ASSERT_TRUE(tile_has_piece(c));

  tile_destroy(a);
}

void test_move_existing(void)
{
  // Build chain: a -> b -> c
  tile_t *c = tile_create(NULL);
  tile_t *b = tile_create(c);
  tile_t *a = tile_create(b);

  // Add a piece to a
  tile_t *pos = tile_move(a, 0);
  CU_ASSERT_PTR_EQUAL(pos, a);
  CU_ASSERT_TRUE(tile_has_piece(a));

  // Move that piece
  pos = tile_move(a, 2);
  CU_ASSERT_PTR_EQUAL(pos, c);
  CU_ASSERT_FALSE(tile_has_piece(a));
  CU_ASSERT_FALSE(tile_has_piece(b));
  CU_ASSERT_TRUE(tile_has_piece(c));

  tile_destroy(a);
}

void test_move_long(void)
{
  // Build a loooong chain
  size_t alot = 1000;
  tile_t *tiles[alot];
  tile_t *next = NULL;
  for (size_t i = 0; i < alot; i++)
  {
    next = tile_create(next);
    tiles[alot - 1 - i] = next;
  }
  CU_ASSERT_PTR_NOT_NULL_FATAL(tiles[0]);

  // No one has a piece
  bool has_piece = false;
  for (size_t i = 0; i < alot; i++)
  {
    has_piece = has_piece || tile_has_piece(tiles[i]);
  }
  CU_ASSERT_FALSE(has_piece);

  // Fill up the board with pieces
  for (size_t i = 0; i < alot; i++)
  {
    tile_move(tiles[0], alot - 1 - i);
  }

  has_piece = true;
  // Everyone has a piece
  for (size_t i = 0; i < alot; i++)
  {
    has_piece = has_piece && tile_has_piece(tiles[i]);
  }
  CU_ASSERT_TRUE(has_piece);

  tile_destroy(tiles[0]);
}

void test_example()
{
  // Bygg banan a -> b -> c -> d -> e -> f -> g -> h
  tile_t *h = tile_create(NULL);
  tile_t *g = tile_create(h);
  tile_t *f = tile_create(g);
  tile_t *e = tile_create(f);
  tile_t *d = tile_create(e);
  tile_t *c = tile_create(d);
  tile_t *b = tile_create(c);
  tile_t *a = tile_create(b);

  // Lägg till en stege från c till f och en rutschkana från g till b
  tile_add_link(c, f);
  tile_add_link(g, b);

  // Flytta en pjäs från a tre steg framåt (hamnar på d)
  CU_ASSERT_PTR_EQUAL(tile_move(a, 3), d);

  // Nu har d en pjäs, men inte a (till exempel)
  CU_ASSERT_TRUE(tile_has_piece(d));
  CU_ASSERT_FALSE(tile_has_piece(a));

  // Flytta en pjäs från a två steg framåt (hamnar på c och tar stegen till f)
  CU_ASSERT_PTR_EQUAL(tile_move(a, 2), f);

  // Flytta pjäsen från d tre steg framåt (hamnar på g och åker rutschkana ner till b)
  CU_ASSERT_PTR_EQUAL(tile_move(d, 3), b);

  // Försök flytta pjäsen från b fyra steg framåt (där står redan en pjäs, stå still)
  CU_ASSERT_PTR_EQUAL(tile_move(b, 4), b);

  // Frigör minnet
  tile_destroy(a);
}

int main()
{
  CU_pSuite suite = NULL;
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  suite = CU_add_suite("Board Test Suite", init_suite, clean_suite);
  if (NULL == suite)
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (
    (NULL == CU_add_test(suite, "Test creating a tile", test_tile_create)) ||
    (NULL == CU_add_test(suite, "Test adding a chute or ladder", test_link_single)) ||
    (NULL == CU_add_test(suite, "Test moving along a simple chain", test_move_simple_chain)) ||
    (NULL == CU_add_test(suite, "Test moving and colliding", test_move_collide)) ||
    (NULL == CU_add_test(suite, "Test moving and following a ladder", test_move_with_ladder)) ||
    (NULL == CU_add_test(suite, "Test moving and following a chute", test_move_with_chute)) ||
    (NULL == CU_add_test(suite, "Test moving and colliding at a chute", test_collide_with_chute)) ||
    (NULL == CU_add_test(suite, "Test moving beyond the end of the board", test_move_to_end)) ||
    (NULL == CU_add_test(suite, "Test moving an existing piece", test_move_existing)) ||
    (NULL == CU_add_test(suite, "Test moving on a large board", test_move_long)) ||
    (NULL == CU_add_test(suite, "Test example", test_example)) ||
    false
  )
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
