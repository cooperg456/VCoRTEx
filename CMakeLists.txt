cmake_minimum_required(VERSION 3.16...3.27 FATAL_ERROR)
project(VoRTEx VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB src ${CMAKE_SOURCE_DIR}/src/*.cpp)

file(GLOB mac ${CMAKE_SOURCE_DIR}/macros/*.mac)
file(COPY ${mac} DESTINATION ${CMAKE_BINARY_DIR}/)

add_executable(vcortex_sim)
target_sources(vcortex_sim PRIVATE ${src})

find_package(Geant4 REQUIRED ui_all vis_all)
target_link_libraries(vcortex_sim PRIVATE ${Geant4_LIBRARIES})

target_compile_definitions(vcortex_sim PRIVATE $<$<CONFIG:Debug>:DEBUG>)

add_custom_target(VoRTEx DEPENDS vcortex_sim)
