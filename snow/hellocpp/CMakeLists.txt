if(CONFIG_SNOW_APP_HELLOCPP)
    set(CXXSRCS)

    set(
            CXXSRCS
            src/HelloWorld.cxx
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