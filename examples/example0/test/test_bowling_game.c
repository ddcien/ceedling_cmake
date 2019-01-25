#include "bowling_game.h"

#include <unity.h>

static struct bowling_game *game = NULL;

void setUp(void) {
    game = bowling_game_creat();
}

void tearDown(void) {
    bowling_game_destroy(game);
    game = NULL;
}

static void rool_many(struct bowling_game *game, int n, int pins) {
    for (int i = 0; i < n; ++i) {
        bowling_game_roll(game, pins);
    }
}

void test_gutter_game(void) {
    rool_many(game, 20, 0);
    TEST_ASSERT_EQUAL(0, bowling_game_score(game));
}

void test_all_ones(void) {
    rool_many(game, 20, 1);
    TEST_ASSERT_EQUAL(20, bowling_game_score(game));
}

void test_one_spare(void) {
    rool_many(game, 2, 5);
    bowling_game_roll(game, 3);
    rool_many(game, 17, 0);
    TEST_ASSERT_EQUAL(16, bowling_game_score(game));
}

void test_one_strike(void) {
    bowling_game_roll(game, 10);
    bowling_game_roll(game, 3);
    bowling_game_roll(game, 4);
    rool_many(game, 16, 0);
    TEST_ASSERT_EQUAL(24, bowling_game_score(game));
}

void test_perfect_game(void) {
    rool_many(game, 12, 10);
    TEST_ASSERT_EQUAL(300, bowling_game_score(game));
}

