cmake_minimum_required(VERSION 3.5.0)
project(UniboCG23-Elaborato-1 VERSION 0.0.5 LANGUAGES C CXX)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static")
set(ASSET_DIR "${CMAKE_SOURCE_DIR}/assets")

add_link_options(-static-libgcc -static-libstdc++ -static -lpthread)
set(ASSIMP_WARNINGS_AS_ERRORS OFF)
set(FT_DISABLE_BROTLI ON)
set(FT_DISABLE_HARFBUZZ ON)

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

add_executable(UniboCG23-Elaborato-1 ${MANUAL_SOURCES} ${AUTOMATIC_SOURCES})

set_target_properties(UniboCG23-Elaborato-1 PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/"
  LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/"
  RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/")

target_link_libraries(UniboCG23-Elaborato-1 PUBLIC glm)
target_link_libraries(UniboCG23-Elaborato-1 PUBLIC glew)
target_link_libraries(UniboCG23-Elaborato-1 PUBLIC freetype)
target_link_libraries(UniboCG23-Elaborato-1 PUBLIC freeglut)
target_link_libraries(UniboCG23-Elaborato-1 PUBLIC assimp)

target_include_directories(UniboCG23-Elaborato-1 PUBLIC ${MANUAL_INCLUDE_DIRS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_custom_command(TARGET UniboCG23-Elaborato-1 POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:UniboCG23-Elaborato-1> $<TARGET_FILE_DIR:UniboCG23-Elaborato-1>
  COMMAND_EXPAND_LISTS)

add_custom_command(
  TARGET UniboCG23-Elaborato-1 POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${ASSET_DIR} "${PROJECT_SOURCE_DIR}/bin"
  COMMENT "Copying assets"
)
