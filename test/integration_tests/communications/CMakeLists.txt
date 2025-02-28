#---------------------------------------------------------------------------
#
#  bitpit
#
#  Copyright (C) 2015-2021 OPTIMAD engineering Srl
#
#  -------------------------------------------------------------------------
#  License
#  This file is part of bitpit.
#
#  bitpit is free software: you can redistribute it and/or modify it
#  under the terms of the GNU Lesser General Public License v3 (LGPL)
#  as published by the Free Software Foundation.
#
#  bitpit is distributed in the hope that it will be useful, but WITHOUT
#  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
#  License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with bitpit. If not, see <http://www.gnu.org/licenses/>.
#
#---------------------------------------------------------------------------*/

# Specify the version being used as well as the language
cmake_minimum_required(VERSION 3.10)

# Name of the current module
get_filename_component(MODULE_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)

# List of tests
set(TESTS "")
if (BITPIT_ENABLE_MPI)
    list(APPEND TESTS "test_communications_parallel_00001")
    list(APPEND TESTS "test_communications_parallel_00002:3")
endif ()

# Test extra libraries
set(TEST_EXTRA_LIBRARIES "")

# Add tests
addModuleIntegrationTests(${MODULE_NAME} "${TESTS}" "${TEST_EXTRA_LIBRARIES}")
unset(TESTS)
