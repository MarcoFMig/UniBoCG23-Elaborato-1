cmake_minimum_required(VERSION 3.5.0)
project(CGExperiments VERSION 0.0.4 LANGUAGES C CXX)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_link_options(-static-libgcc -static-libstdc++ -static -lpthread)
set(ASSIMP_WARNINGS_AS_ERRORS OFF)
set(FT_DISABLE_BROTLI ON)
set(FT_DISABLE_HARFBUZZ ON)
#find_package(PkgConfig REQUIRED)
#pkg_check_modules(HARFBUZZ REQUIRED IMPORTED_TARGET GLOBAL harfbuzz)
#pkg_check_modules(BROTLIDEC REQUIRED IMPORTED_TARGET GLOBAL libbrotlidec)
#pkg_check_modules(BROTLICOMMON REQUIRED IMPORTED_TARGET GLOBAL libbrotlicommon)

#message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
#get_filename_component(MinGW64_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)
#get_filename_component(MinGW64_DIR "${MinGW64_DIR}/../.." ABSOLUTE)
#message(STATUS "MinGW64 directory: ${MinGW64_DIR}")

#set(MANUALLY_ADDED_LIBRARIES ${MinGW64_DIR}/mingw64/lib/libharfbuzz.a
#  ${MinGW64_DIR}/mingw64/lib/libbrotlidec.a)

message(STATUS "Manually added dependencies: ${MANUALLY_ADDED_LIBRARIES}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}"/bin/")
file(GLOB AUTOMATIC_SOURCES "${PROJECT_SOURCE_DIR}/src/shaderloader/*.cpp"
  "${PROJECT_SOURCE_DIR}/src/simplemath/*.cpp"
  "${PROJECT_SOURCE_DIR}/src/behaviours/data/*.cpp"
  "${PROJECT_SOURCE_DIR}/src/exceptions/*.cpp"
  "${PROJECT_SOURCE_DIR}/src/shaderloader/exceptions/*.cpp"
  "${PROJECT_SOURCE_DIR}/src/behaviours/*.cpp"
  "${PROJECT_SOURCE_DIR}/src/geometry/*.cpp"
  "${PROJECT_SOURCE_DIR}/src/behaviours/physics/*.cpp"
  "${PROJECT_SOURCE_DIR}/src/geometry/exceptions/*.cpp"
  "${PROJECT_SOURCE_DIR}/src/lightweightutils/exceptions/*.cpp"
  "${PROJECT_SOURCE_DIR}/src/lightweightutils/*.cpp"
  "${PROJECT_SOURCE_DIR}/src/inputhandler/*.cpp"
  "${PROJECT_SOURCE_DIR}/src/logger/*.cpp"
  "${PROJECT_SOURCE_DIR}/src/physics/*.cpp")
set(MANUAL_SOURCES ${PROJECT_SOURCE_DIR}"/src/main.cpp"
                    ${PROJECT_SOURCE_DIR}"/src/scene.cpp"
                    ${PROJECT_SOURCE_DIR}"/src/engine.cpp"
#                    ${PROJECT_SOURCE_DIR}"/src/behaviour.cpp"
                    ${PROJECT_SOURCE_DIR}"/src/physicsmanager.cpp"
                    ${PROJECT_SOURCE_DIR}"/src/context.cpp"
                    ${PROJECT_SOURCE_DIR}"/src/object.cpp")
set(SOURCES ${MANUAL_SOURCES} ${AUTOMATIC_SOURCES})
set(MANUAL_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/lib/glew/include
  ${PROJECT_SOURCE_DIR}/lib/freeglut/include
  ${PROJECT_SOURCE_DIR}/lib/PlasmaMath/include)

include(CTest)
enable_testing()

add_subdirectory("${PROJECT_SOURCE_DIR}/lib/glew/build/cmake" EXCLUDE_FROM_ALL)
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/glm" EXCLUDE_FROM_ALL)
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/freetype" EXCLUDE_FROM_ALL)
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/freeglut" EXCLUDE_FROM_ALL)
add_subdirectory("${PROJECT_SOURCE_DIR}/lib/assimp" EXCLUDE_FROM_ALL)

add_executable(CGExperiments ${MANUAL_SOURCES} ${AUTOMATIC_SOURCES})

set_target_properties(CGExperiments PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/")

target_link_libraries(CGExperiments PUBLIC glm)
target_link_libraries(CGExperiments PUBLIC glew)
target_link_libraries(CGExperiments PUBLIC freetype)
target_link_libraries(CGExperiments PUBLIC freeglut)
target_link_libraries(CGExperiments PUBLIC assimp)
#target_link_libraries(CGExperiments PUBLIC PkgConfig::HARFBUZZ)
#target_link_libraries(CGExperiments PUBLIC PkgConfig::BROTLIDEC)
#target_link_libraries(CGExperiments PUBLIC PkgConfig::BROTLICOMMON)
#target_link_libraries(CGExperiments PUBLIC ${MANUALLY_ADDED_LIBRARIES})

target_include_directories(CGExperiments PUBLIC ${MANUAL_INCLUDE_DIRS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_custom_command(TARGET CGExperiments POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:CGExperiments> $<TARGET_FILE_DIR:CGExperiments>
  COMMAND_EXPAND_LISTS)
