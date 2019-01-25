add_library(example4
    src/foo.c
    src/foo.h
    )
target_include_directories(example4 PUBLIC src)
generate_mock(src/bar.h)
generate_fixture_runner(test test_ example4_test mock_bar example4)
