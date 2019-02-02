add_library(bowling_game_0
    src/bowling_game.c
    src/bowling_game.h
    )
target_include_directories(bowling_game_0 PUBLIC src)
add_executable(bowling_game_test test/test_bowling_game.c)
target_include_directories(bowling_game_test PRIVATE ${CMAKE_SOURCE_DIR}/src )
target_link_libraries(bowling_game_test bowling_game_0 unity)

