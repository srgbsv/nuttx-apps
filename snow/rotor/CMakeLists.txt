set(STANDALONE_MAKE "$ENV{STANDALONE_MAKE}")
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(LIB_DIRECTORY "lib")

if (STANDALONE_MAKE)
    cmake_minimum_required(VERSION 3.20...3.25)
    include(CMakePrintHelpers)

    project(snowblower
        VERSION 1.0
        DESCRIPTION "Snowblower controller"
        )

    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    # set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_C_STANDARD 99)

    set(NUTTX_PATH "${CMAKE_SOURCE_DIR}/../../../nuttx/nuttx-export-12.10.0")

    message(STATUS "Nuttx directory: ${NUTTX_PATH}")

    #include(cmake/nucleo-f4x1re.cmake)

    set(AC_COMMON_FLAGS "${AC_COMMON_FLAGS} -Wall -Wshadow -Wundef -fno-strict-aliasing -Os")
    set(AC_COMMON_FLAGS "${AC_COMMON_FLAGS} -D_DEBUG -D_LIBCPP_BUILD_STATIC -D_LIBCPP_NO_EXCEPTIONS ")
    set(AC_COMMON_FLAGS "${AC_COMMON_FLAGS} -fno-exceptions -fcheck-new -fno-rtti -pedantic ")
    #set(AC_COMMON_FLAGS "${AC_COMMON_FLAGS} -nostdinc++")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c11 -std=gnu++17")

    set(AC_DEFINES "${AC_DEFINES} -DCONFIG_WCHAR_BUILTIN -DDRONECAN_CXX_WRAPPERS")

    include_directories(
        include
        ${NUTTX_PATH}/include
        ${NUTTX_PATH}/include/cxx
        ${NUTTX_PATH}/arch/chip
        #lib/uClibc++/include
        lib/libcanard
        lib/dsdl_generated/include
    )

    set(EXE_NAME rotor_main)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${AC_HW_FLAGS} ${AC_DEFINES}")
    set(CMAKE_CXX_FLAGS     "${AC_HW_FLAGS} ${AC_DEFINES} ${AC_COMMON_FLAGS} ${AC_CXX_EXTRA_FLAGS}")
    if (PARAM_DEBUG)
        set(CMAKE_CXX_FLAGS     "${CMAKE_CXX_FLAGS} -g")
    endif()

    set(CMAKE_SKIP_RPATH ON)
    set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_LINKER} ${AC_LINKER_FLAGS} -o ${EXE_NAME}.elf <OBJECTS> <LINK_LIBRARIES>")
    set(BUILD_SHARED_LIBS OFF)

    add_subdirectory(src)
    # Определяем путь к базе Canard
    set(CANARD_BASE ${CMAKE_CURRENT_SOURCE_DIR}/${LIB_DIRECTORY}/libcanard)

    # Создаем список исходных файлов
    #message("${LIBS_C}")


    add_executable(${EXE_NAME} ${SOURCE_FILES} rotor_main.cpp ${HEADER_FILES} ${LIB_C})

    add_custom_command(
            TARGET ${EXE_NAME}
            POST_BUILD
            COMMAND ${CMAKE_OBJCOPY} ARGS -S -O binary ${CMAKE_BINARY_DIR}/${EXE_NAME}.elf ${CMAKE_BINARY_DIR}/${EXE_NAME}.bin
    )

    link_directories(${EXE_NAME} ${NUTTX_PATH}/libs)

    cmake_print_variables(SOURCE_FILES)

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

    


    function(clone_libcanard)
    # Проверяем, существует ли уже директория lib
    if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${LIB_DIRECTORY})
        # Создаем директорию, если её нет
        file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
    endif()

    # Проверяем, существует ли уже репозиторий в директории lib
    if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${LIB_DIRECTORY}/libcanard)
        # Клонируем репозиторий
        message(STATUS "Clonning libcanard...")
        execute_process(
            COMMAND git clone https://github.com/dronecan/libcanard.git ${CMAKE_CURRENT_SOURCE_DIR}/${LIB_DIRECTORY}/libcanard
            RESULT_VARIABLE git_clone_result
        )

        # Проверяем результат выполнения команды
        if(NOT git_clone_result EQUAL 0)
            message(FATAL_ERROR "Error when cloning libcanard repo")
        endif()
    else()
        message(STATUS "Directory libcanard already exists")
    endif()
endfunction()

add_custom_target(clone_libcanard_target
    ALL
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/clone_libcanard.cmake
    COMMENT "Cloning libcanard in directory lib"
)
endif()

if(CONFIG_SNOW_APP_SNOWBLOWER)
    message("SNOWBLOWER PRINT")

    set(CXXSRCS)
    set(HEADER_FILES)

    include_directories(include)
    add_subdirectory(include)
    add_subdirectory(src)

    #target_sources(snowblower PRIVATE ${CSRCS})
    #target_include_directories(snowblower PRIVATE ${INCDIR})

    message("CXXSRCS:::")
    message("${CXXSRCS}")
    message("${HEADER_FILES}")

    # Определяем функцию для клонирования репозитория

    nuttx_add_application(
            NAME
            snowblower
            STACKSIZE
            ${CONFIG_DEFAULT_TASK_STACKSIZE}
            MODULE
            ${CONFIG_SNOW_APP_SNOWBLOWER}
            SRCS
            snowblower_main.cpp
            ${CXXSRCS}
            INCLUDE_DIRECTORIES
            ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    target_sources(apps PRIVATE ${CXXSRCS})
    target_include_directories(apps PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
endif()

