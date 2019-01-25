add_library(example2
    src/ProductionCode.c
    src/ProductionCode.h
    src/ProductionCode2.c
    src/ProductionCode2.h
    )
target_include_directories(example2 PUBLIC src)
generate_fixture_runner(test Test example2_tests example2)
