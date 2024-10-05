set(STANDALONE_MAKE "$ENV{STANDALONE_MAKE}")

if (STANDALONE_MAKE)
    cmake_minimum_required(VERSION 3.2...3.15)
    include(CMakePrintHelpers)

    project(snowblower
        VERSION 1.0
        DESCRIPTION "Snowblower controller"
        )

    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    # set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_C_STANDARD 99)

    set(SOURCE_FILES ../hellocpp/hellocpp_main.cxx
            src/PwmOutput.cpp
    )

    set(NUTTX_PATH "${CMAKE_SOURCE_DIR}/nuttx-export-12.6.0-RC0")

    include(cmake/nucleo-f4x1re.cmake)

    set(AC_COMMON_FLAGS "${AC_COMMON_FLAGS} -Wall -Wshadow -Wundef -fno-strict-aliasing -Os")
    set(AC_COMMON_FLAGS "${AC_COMMON_FLAGS} -D_DEBUG -D_LIBCPP_BUILD_STATIC -D_LIBCPP_NO_EXCEPTIONS ")
    set(AC_COMMON_FLAGS "${AC_COMMON_FLAGS} -fno-exceptions -fcheck-new -fno-rtti -pedantic ")
    set(AC_COMMON_FLAGS "${AC_COMMON_FLAGS} -nostdinc++")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -v -stdlib=libc++")

    set(AC_DEFINES "${AC_DEFINES} -DCONFIG_WCHAR_BUILTIN")

    include_directories(
        include
        ${NUTTX_PATH}/include
        ${NUTTX_PATH}/include/uClibc++
        ${NUTTX_PATH}/arch/chip
    )

    set(EXE_NAME snowblower)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${AC_HW_FLAGS} ${AC_DEFINES}")
    set(CMAKE_CXX_FLAGS     "${AC_HW_FLAGS} ${AC_DEFINES} ${AC_COMMON_FLAGS} ${AC_CXX_EXTRA_FLAGS}")
    if (PARAM_DEBUG)
        set(CMAKE_CXX_FLAGS     "${CMAKE_CXX_FLAGS} -g")
    endif()

    set(CMAKE_SKIP_RPATH ON)
    set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_LINKER} ${AC_LINKER_FLAGS} -o ${EXE_NAME}.elf <OBJECTS> <LINK_LIBRARIES>")
    set(BUILD_SHARED_LIBS OFF)
    add_subdirectory(src)

    link_directories(${EXE_NAME} ${NUTTX_PATH}/libs)

    cmake_print_variables(SOURCE_FILES)

    add_executable(${EXE_NAME} ${SOURCE_FILES} snowblower_main.cpp ${HEADER_FILES})

    add_custom_command(
            TARGET ${EXE_NAME}
            POST_BUILD
            COMMAND ${CMAKE_OBJCOPY} ARGS -S -O binary ${CMAKE_BINARY_DIR}/${EXE_NAME}.elf ${CMAKE_BINARY_DIR}/${EXE_NAME}.bin
    )

    target_link_libraries(${EXE_NAME} --start-group)

    target_link_libraries(${EXE_NAME} sched)
    target_link_libraries(${EXE_NAME} drivers)
    target_link_libraries(${EXE_NAME} boards)
    target_link_libraries(${EXE_NAME} c)
    target_link_libraries(${EXE_NAME} mm)
    target_link_libraries(${EXE_NAME} arch)
    target_link_libraries(${EXE_NAME} xx)
    target_link_libraries(${EXE_NAME} apps)
    target_link_libraries(${EXE_NAME} fs)
    target_link_libraries(${EXE_NAME} binfmt)
    target_link_libraries(${EXE_NAME} board)
    target_link_libraries(${EXE_NAME} gcc)
    target_link_libraries(${EXE_NAME} supc++)

    target_link_libraries(${EXE_NAME} --end-group)
endif()

if(CONFIG_SNOW_APP_HELLOCPP)
    set(CXXSRCS)

    set(
            CXXSRCS
            src/HelloWorld.cpp
            hellocpp_main.cxx
    )

    nuttx_add_application(
            NAME
            hellocpp
            STACKSIZE
            ${CONFIG_DEFAULT_TASK_STACKSIZE}
            MODULE
            ${CONFIG_SNOW_APP_HELLOCPP}
            SRCS
            hellocpp_main.cxx
    )

    target_sources(apps PRIVATE ${CXXSRCS})
    #target_include_directories(hellocpp PRIVATE src)
endif()

