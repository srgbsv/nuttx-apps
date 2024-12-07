
message("INCLUDED")

set(CXXSRCS ${CXXSRCS}
        ${CMAKE_CURRENT_SOURCE_DIR}/MainController.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/EjectionController.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/PwmInput.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/PwmOutput.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/GpioInput.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/GpioOutput.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/InputController.cpp
        PARENT_SCOPE
        )