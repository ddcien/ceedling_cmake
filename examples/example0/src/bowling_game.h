#ifndef BOWLING_GAME_H
#define BOWLING_GAME_H

#ifdef __cplusplus
extern "C" {
#endif

struct bowling_game;

struct bowling_game *bowling_game_creat(void);
void bowling_game_destroy(struct bowling_game *game);
void bowling_game_roll(struct bowling_game *game, int pins);
int bowling_game_score(struct bowling_game const *game);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* BOWLING_GAME_H */
