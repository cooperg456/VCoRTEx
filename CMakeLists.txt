cmake_minimum_required(VERSION 3.16...3.27 FATAL_ERROR)
project(VoRTEx VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Geant4 REQUIRED ui_all vis_all)
find_package(ROOT REQUIRED COMPONENTS Core Tree)

file(GLOB src ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE headers ${CMAKE_SOURCE_DIR}/include/**/*.hpp)
file(GLOB mac ${CMAKE_SOURCE_DIR}/macros/*.mac)
file(COPY ${mac} DESTINATION ${CMAKE_BINARY_DIR}/)

foreach(header ${headers})
    get_filename_component(dir ${header} DIRECTORY)
    list(APPEND include_dirs ${dir})
endforeach()
list(REMOVE_DUPLICATES include_dirs)

add_executable(vcortex_sim)
target_sources(vcortex_sim PRIVATE ${src} ${headers})
target_include_directories(vcortex_sim PRIVATE ${include_dirs} ${Geant4_INCLUDE_DIRS})
target_link_libraries(vcortex_sim PRIVATE ${Geant4_LIBRARIES} ROOT::Core ROOT::Tree)

target_compile_definitions(vcortex_sim PRIVATE $<$<CONFIG:Debug>:DEBUG>)

add_custom_target(VoRTEx DEPENDS vcortex_sim)