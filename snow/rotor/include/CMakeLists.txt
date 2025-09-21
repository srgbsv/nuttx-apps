set(HEADER_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/Logging.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/controllers/CanController.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/controllers/MainController.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/controllers/RotationController.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/state/CanState.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/state/State.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/providers/CanardInterface.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/providers/CanRotorNode.hpp
)

set_source_files_properties(${HEADER_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)