set(HEADER_FILES
        HelloWorld.h
        )

set(SOURCE_FILES
        HelloWorld.cxx
        )

set(CXXSRCS)

list(
        APPEND
        CXXSRCS
        HelloWorld.cxx)

#target_sources(hellocpp PRIVATE ${CXXSRCS})

#add_executable(${EXE_NAME} ${SOURCE_FILES} ../hellocpp_main.cxx ${HEADER_FILES})

