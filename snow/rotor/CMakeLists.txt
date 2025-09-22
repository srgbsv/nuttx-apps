#
# For a description of the syntax of this file, see the NuttX Build
# System manual.
#
set(INCDIR ${NUTTX_APPS_DIR}/snow/rotor/lib/libdronecan/libcanard
             ${NUTTX_APPS_DIR}/snow/rotor/lib/libcanard/drivers/nuttx)
list(APPEND INCDIR ${NUTTX_APPS_DIR}/canutils/libdronecan/libc)

if(CONFIG_CANUTILS_LIBOPENCYPHAL)
  set(CFLAGS -DcanardInit=dronecanardInit)
endif()
include(include)
  
nuttx_add_executable(rotor
  rotor_main.cpp
  src/providers/CanProvider.cpp
  src/controllers/CanController.cpp
  src/state/CanState.cpp
  src/state/State.cpp
  src/controllers/MainController.cpp
  src/EjectionController.cpp
  src/providers/PwmInput.cpp
  src/providers/PwmOutput.cpp
  )

target_include_directories(rotor PUBLIC include ${INCDIR})