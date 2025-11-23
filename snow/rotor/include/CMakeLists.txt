set(HEADER_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/Logging.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/controllers/MainController.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/controllers/EjectionController.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/state/CanState.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/state/State.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/state/RotateState.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/state/EjectionState.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/state/MotorState.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/providers/EncoderProvider.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/providers/CanardInterface.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/providers/CanRotorNode.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/providers/PwmInput.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/providers/PwmOutput.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/providers/GpioInput.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/providers/GpioOutput.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tools/SystemTools.hpp
)

set_source_files_properties(${HEADER_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)