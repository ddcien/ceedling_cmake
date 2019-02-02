#include "bowling_game.h"

#include "ddctest.h"

static struct bowling_game *game = NULL;

TEST_SUIT(bowling_game);
TEST_SUIT_SETUP(bowling_game) {}
TEST_SUIT_TEARDOWN(bowling_game) {}
TEST_SUIT_TEST_SETUP(bowling_game) { game = bowling_game_creat(); }
TEST_SUIT_TEST_TEARDOWN(bowling_game) {
  bowling_game_destroy(game);
  game = NULL;
}

static void rool_many(struct bowling_game *game, int n, int pins) {
  for (int i = 0; i < n; ++i) {
    bowling_game_roll(game, pins);
  }
}

TEST_CASE_DEC(bowling_game, gutter_game) {
  rool_many(game, 20, 0);
  TEST_ASSERT_EQUAL(0, bowling_game_score(game));
}
TEST_CASE_RUN_ONCE(bowling_game, gutter_game);

TEST_CASE_DEC(bowling_game, all_ones) {
  rool_many(game, 20, 1);
  TEST_ASSERT_EQUAL(20, bowling_game_score(game));
}
TEST_CASE_RUN_ONCE(bowling_game, all_ones);

TEST_CASE_DEC(bowling_game, one_spare) {
  rool_many(game, 2, 5);
  bowling_game_roll(game, 3);
  rool_many(game, 17, 0);
  TEST_ASSERT_EQUAL(16, bowling_game_score(game));
}
TEST_CASE_RUN_ONCE(bowling_game, one_spare);

TEST_CASE_DEC(bowling_game, one_strike) {
  bowling_game_roll(game, 10);
  bowling_game_roll(game, 3);
  bowling_game_roll(game, 4);
  rool_many(game, 16, 0);
  TEST_ASSERT_EQUAL(24, bowling_game_score(game));
}
TEST_CASE_RUN_ONCE(bowling_game, one_strike);

TEST_CASE_DEC(bowling_game, perfect_game) {
  rool_many(game, 12, 10);
  TEST_ASSERT_EQUAL(300, bowling_game_score(game));
}
TEST_CASE_RUN_ONCE(bowling_game, perfect_game);

int main(void) {
  TEST_RUN_ALL();
  return 0;
}
