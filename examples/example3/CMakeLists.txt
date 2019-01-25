add_library(config
    src/config.c
    src/config.h
    )
target_include_directories(config PUBLIC src)
generate_mock(src/flash.h)
generate_test_runner(test/test_config.c mock_flash config)
