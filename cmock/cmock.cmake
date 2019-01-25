set(CMOCK_PATH ${CMAKE_CURRENT_LIST_DIR})
set(UNITY_PATH ${CMOCK_PATH}/vendor/unity)
set(CEXCEPTION_PATH ${CMOCK_PATH}/vendor/c_exception)

add_library(c_exception
    ${CEXCEPTION_PATH}/lib/CException.c
    ${CEXCEPTION_PATH}/lib/CException.h
    )
target_include_directories(c_exception PUBLIC ${CEXCEPTION_PATH}/lib)


add_library(unity
    ${UNITY_PATH}/src/unity.c
    ${UNITY_PATH}/src/unity.h
    ${UNITY_PATH}/src/unity_internals.h
    ${UNITY_PATH}/extras/fixture/src/unity_fixture.c
    ${UNITY_PATH}/extras/fixture/src/unity_fixture.h
    ${UNITY_PATH}/extras/fixture/src/unity_fixture_internals.h
    ${UNITY_PATH}/extras/fixture/src/unity_fixture_malloc_overrides.h
    )
target_include_directories(unity PUBLIC
    ${UNITY_PATH}/src
    ${UNITY_PATH}/extras/fixture/src
    )
target_link_libraries(unity c_exception)


add_library(cmock
    ${CMOCK_PATH}/src/cmock.c
    ${CMOCK_PATH}/src/cmock.h
    ${CMOCK_PATH}/src/cmock_internals.h
    )
target_include_directories(cmock PUBLIC ${CMOCK_PATH}/src)
target_link_libraries(cmock unity)


function(generate_mock src)
    set(mock_path "${CMAKE_BINARY_DIR}/mocks")
    file(MAKE_DIRECTORY ${mock_path})

    get_filename_component(ext ${src} EXT)
    if(NOT (${ext} STREQUAL ".h"))
        message(FATAL_ERROR "The file for generating mocks must be a C header file.")
    endif()

    get_filename_component(src ${src} ABSOLUTE)
    get_filename_component(name ${src} NAME_WE)
    get_filename_component(path ${src} DIRECTORY)
    add_custom_command(
        OUTPUT ${mock_path}/mock_${name}.c ${mock_path}/mock_${name}.h
        COMMAND ruby ${CMOCK_PATH}/lib/cmock.rb -o${CMOCK_PATH}/config.yml ${src}
        DEPENDS ${src}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )

    add_library(mock_${name}
        ${src}
        ${mock_path}/mock_${name}.c
        ${mock_path}/mock_${name}.h
        )
    target_include_directories(mock_${name} PUBLIC ${mock_path} ${path})
    target_link_libraries(mock_${name} cmock)
endfunction()

function(generate_test_runner src)
    get_filename_component(ext ${src} EXT)
    if(NOT (${ext} STREQUAL ".c"))
        message(FATAL_ERROR "The file for generating a runner must be a C source file.")
    endif()

    list(SUBLIST ARGV 1 -1 libs)

    set(runner_path "${CMAKE_BINARY_DIR}/runners")
    file(MAKE_DIRECTORY ${runner_path})
    get_filename_component(src ${src} ABSOLUTE)
    get_filename_component(name ${src} NAME_WE)

    add_custom_command(
        OUTPUT ${runner_path}/${name}_runner.c
        COMMAND ruby ${UNITY_PATH}/auto/generate_test_runner.rb ${src} ${CMOCK_PATH}/config.yml ${runner_path}/${name}_runner.c
        DEPENDS ${src}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )
    add_executable(${name}
        ${src}
        ${runner_path}/${name}_runner.c
        )
    target_link_libraries(${name} unity ${libs})
    add_test(${name} ${name})
endfunction()

function(generate_fixture_runner path prefix name)
    list(SUBLIST ARGV 3 -1 libs)

    get_filename_component(path ${path} ABSOLUTE)
    file(GLOB srcs "${path}/${prefix}*.c")

    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/${name}.c
        COMMAND python3 ${CMOCK_PATH}/scripts/gen_fixture_runner.py -t ${path} -p ${prefix} -o ${CMAKE_BINARY_DIR}/${name}.c
        DEPENDS ${srcs}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )
    add_executable(${name}
        ${srcs}
        ${CMAKE_BINARY_DIR}/all_tests.c
        )
    target_link_libraries(${name} unity ${libs})
    add_test(${name} ${name} -v)
endfunction()

