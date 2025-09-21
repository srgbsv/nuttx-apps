#
# For a description of the syntax of this file, see the NuttX Build
# System manual.
#

include $(APPDIR)/Make.defs

# Rotor control daemon built-in application info

PROGNAME = ${CONFIG_SNOW_ROTOR_PROGNAME}
PRIORITY = ${CONFIG_SNOW_ROTOR_PRIORITY}
STACKSIZE = ${CONFIG_SNOW_ROTOR_STACKSIZE}
MODULE = ${CONFIG_SNOW_ROTOR}

# Rotor control daemon main

MAINSRC = rotor_main.cpp

CXXSRCS += rotor_can.cpp
CXXSRCS += rotor_control.cpp

include $(APPDIR)/Application.mk