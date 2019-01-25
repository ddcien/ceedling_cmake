add_library(bowling_game
    src/bowling_game.c
    src/bowling_game.h
    )
target_include_directories(bowling_game PUBLIC src)
generate_test_runner(test/test_bowling_game.c bowling_game)
