set(STANDALONE_MAKE "$ENV{STANDALONE_MAKE}")
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_CXX_STANDARD 17)

set(LIB_DIRECTORY "lib")

if(CONFIG_SNOW_ROTOR_APP)
    message("ROTOR BUILDING...")

    set(CXXSRCS)
    # Collect source files from src/CMakeLists.txt
    add_subdirectory(src)
    set(CXXSRCS ${SOURCE_FILES})
    set(HEADER_FILES)

    set(INCLUDE_DIRS
        ${NUTTX_APPS_DIR}/snow/rotor/include
        ${NUTTX_APPS_DIR}/snow/rotor/lib/libcanard
        ${NUTTX_APPS_DIR}/snow/rotor/lib/libcanard/drivers/socketcan
        ${NUTTX_APPS_DIR}/snow/rotor/lib/dsdl_generated/include
    )

    set(CXX_FLAGS -DDRONECAN_CXX_WRAPPERS -D_GLIBCXX_USE_CXX17 -D_STDLIB_H_)

    include_directories(include)
    add_subdirectory(include)

    set(CXXSRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libcanard/canard.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libcanard/drivers/socketcan/socketcan.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/dsdl_generated/src/uavcan.protocol.NodeStatus.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/dsdl_generated/src/uavcan.protocol.GetNodeInfo_res.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/dsdl_generated/src/uavcan.protocol.GetNodeInfo_req.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/dsdl_generated/src/uavcan.equipment.esc.RawCommand.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/dsdl_generated/src/uavcan.equipment.esc.Status.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/dsdl_generated/src/uavcan.protocol.dynamic_node_id.Allocation.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/dsdl_generated/src/uavcan.protocol.param.GetSet_req.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/dsdl_generated/src/uavcan.protocol.param.GetSet_res.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/dsdl_generated/src/uavcan.protocol.param.ExecuteOpcode_req.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/dsdl_generated/src/uavcan.protocol.param.ExecuteOpcode_res.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/dsdl_generated/src/uavcan.equipment.actuator.Command.c
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/dsdl_generated/src/uavcan.equipment.actuator.ArrayCommand.c
        ${CXXSRCS}
    )

    message("CXXSRCS:::")
    message("${CXXSRCS}")
    message("${HEADER_FILES}")
    message("${INCLUDE_DIRS}")

    nuttx_add_application(
            NAME
            rotor
            STACKSIZE
            ${CONFIG_DEFAULT_TASK_STACKSIZE}
            MODULE
            ${CONFIG_SNOW_ROTOR_APP}
            SRCS
            rotor_main.cpp
            ${CXXSRCS}
            INCLUDE_DIRECTORIES
            ${INCLUDE_DIRS}
            COMPILE_FLAGS
            ${CXX_FLAGS}
    )

    target_sources(apps PRIVATE ${CXXSRCS})
    target_include_directories(apps PRIVATE ${INCLUDE_DIRS})
    target_compile_options(apps PRIVATE ${CXX_FLAGS})
    target_link_options(apps PRIVATE ${CXX_FLAGS})

    link_directories(${CMAKE_CURRENT_BINARY_DIR}/${LIB_DIRECTORY})

endif()

