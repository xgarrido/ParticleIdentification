# CMAKE_COMPILER
# - Basic CMake setup
# Check version meets ou requirements
# Declare project, which will configure compiler for us
cmake_minimum_required(VERSION 2.8)
project(FalaiseParticleIdentificationPlugin)

# Ensure our code can see the Falaise headers
#include_directories(${Falaise_INCLUDE_DIRS})
include_directories(${Falaise_BUILDPRODUCT_DIR}/include)
include_directories(${Falaise_BUILDPRODUCT_DIR}/include/falaise)
include_directories(${Bayeux_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/source)

############################################################################################
# - GammaTracking modules:
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/source/falaise)

# - Headers:
list(APPEND FalaiseParticleIdentificationPlugin_HEADERS source/falaise/snemo/reconstruction/particle_identification_driver.h)
list(APPEND FalaiseParticleIdentificationPlugin_HEADERS source/falaise/snemo/reconstruction/particle_identification_module.h)
list(APPEND FalaiseParticleIdentificationPlugin_HEADERS source/falaise/snemo/cuts/topology_cut.h)

# - Sources:
list(APPEND FalaiseParticleIdentificationPlugin_SOURCES source/falaise/snemo/reconstruction/particle_identification_driver.cc)
list(APPEND FalaiseParticleIdentificationPlugin_SOURCES source/falaise/snemo/reconstruction/particle_identification_module.cc)
list(APPEND FalaiseParticleIdentificationPlugin_SOURCES source/falaise/snemo/cuts/topology_cut.cc)

############################################################################################

# Build a dynamic library from our sources
add_library(Falaise_ParticleIdentification SHARED
  ${FalaiseParticleIdentificationPlugin_HEADERS}
  ${FalaiseParticleIdentificationPlugin_SOURCES})

target_link_libraries(Falaise_ParticleIdentification Falaise)

# Apple linker requires dynamic lookup of symbols, so we
# add link flags on this platform
if(APPLE)
  set_target_properties(Falaise_ParticleIdentification
    PROPERTIES LINK_FLAGS "-undefined dynamic_lookup"
    )
endif()

# Install it:
install(TARGETS Falaise_ParticleIdentification DESTINATION ${CMAKE_INSTALL_LIBDIR}/Falaise/modules)

# # Test support:
# option(FalaiseParticleIdentificationPlugin_ENABLE_TESTING "Build unit testing system for FalaiseParticleIdentification" ON)
# if(FalaiseParticleIdentificationPlugin_ENABLE_TESTING)
#   enable_testing()
#   add_subdirectory(testing)
# endif()
