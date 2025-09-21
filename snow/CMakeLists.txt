# ##############################################################################
# apps/sdr/CMakeLists.txt
#
# Licensed to the Apache Software Foundation (ASF) under one or more contributor
# license agreements.  See the NOTICE file distributed with this work for
# additional information regarding copyright ownership.  The ASF licenses this
# file to you under the Apache License, Version 2.0 (the "License"); you may not
# use this file except in compliance with the License.  You may obtain a copy of
# the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
# License for the specific language governing permissions and limitations under
# the License.
#
# ##############################################################################

nuttx_add_subdirectory()

nuttx_generate_kconfig(MENUDESC "Snowblower Software")

# Include FetchContent for managing external dependencies
include(FetchContent)

# Fetch DroneCAN DSDL definitions
FetchContent_Declare(
    DSDL
    GIT_REPOSITORY https://github.com/DroneCAN/DSDL.git
    GIT_TAG master # Using master as in the Makefile, consider a specific tag for stability
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(DSDL)

# Fetch DroneCAN DSDL compiler
FetchContent_Declare(
    dronecan_dsdlc
    GIT_REPOSITORY https://github.com/DroneCAN/dronecan_dsdlc.git
    GIT_TAG master # Using master as in the Makefile
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(dronecan_dsdlc)


