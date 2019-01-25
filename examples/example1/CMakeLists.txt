add_library(example1
    src/ProductionCode.c
    src/ProductionCode.h
    src/ProductionCode2.c
    src/ProductionCode2.h
    )
target_include_directories(example1 PUBLIC src)
generate_test_runner(test/TestProductionCode.c example1)
generate_test_runner(test/TestProductionCode2.c example1)
