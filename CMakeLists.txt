
#
# COMPILE USING c++ 2014
#
cmake_minimum_required (VERSION 3.1)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if(SRC)
  get_filename_component(projectname ${SRC} NAME_WE)
else()
  set(projectname IMT2531)
endif(SRC)

project(${projectname})
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )

message( "system name is: " ${CMAKE_SYSTEM} )
message( "system processor is: " ${CMAKE_SYSTEM_PROCESSOR} )

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  SET( LINUX 1)
  SET( APPLE 0)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  SET( LINUX 0)
  SET( APPLE 1 )
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  SET( WINDOWS 1)
endif()

find_package(OpenGL REQUIRED)


if(APPLE OR LINUX)
find_package(GLEW REQUIRED)
endif(APPLE OR LINUX)

set( GLFW_BUILD_EXAMPLES OFF CACHE BOOL "turn off build examples")
add_subdirectory(glfw)
link_directories(${PROJECT_BINARY_DIR})
link_directories(${GLFW_BINARY_DIR})

if(WINDOWS)
link_directories(${PROJECT_SOURCE_DIR}/glew/lib/)
endif(WINDOWS)

#
# WHATTA FAKK?
#
if(SRC)
  get_filename_component(target ${SRC} NAME_WE)
  add_executable(${target} ${SRC} )
    if(WINDOWS)
        target_link_libraries(${target} glfw glew32 ${GLFW_LIBRARIES} )
        else()
        target_link_libraries(${target} glfw ${GLEW_LIBRARY} ${GLFW_LIBRARIES} )
    endif(WINDOWS)

else()
  file( GLOB EXAMPLES src/*.cpp )
  foreach( srcfile ${EXAMPLES} )
    get_filename_component(target ${srcfile} NAME_WE)
    if(target)
    add_executable(${target} ${srcfile} )
    if(WINDOWS)
        target_link_libraries(${target} glfw glew32 ${GLFW_LIBRARIES} )
        else()
        target_link_libraries(${target} glfw ${GLEW_LIBRARY} ${GLFW_LIBRARIES} )
    endif(WINDOWS)

    endif(target)
  endforeach( srcfile ${EXAMPLES} )
endif()
#file (GLOB         PROJECT_DLL dll/glew32.dll)


#
# INCLUDE DIRECTORIES
#
include_directories(${PROJECT_SOURCE_DIR}/include/)
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/glfw/include/)
include_directories(${PROJECT_SOURCE_DIR}/glm/glm/)
include_directories(${PROJECT_SOURCE_DIR}/glew/include/)
#file(COPY  ${PROJECT_SOURCE_DIR}/include/vertex.vert DESTINATION ${PROJECT_BINARY_DIR})
#file(COPY  ${PROJECT_SOURCE_DIR}/include/fragment.frag DESTINATION ${PROJECT_BINARY_DIR})

if(WINDOWS)
include_directories(${PROJECT_SOURCE_DIR}/glew/include/)
file(COPY  ${PROJECT_SOURCE_DIR}/dll/glew32.dll DESTINATION ${PROJECT_BINARY_DIR})
endif(WINDOWS)

if(APPLE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${GLEW_INCLUDE_DIRS}")
endif(APPLE)



set(CMAKE_CXX_FLAGS "-Wall -g")
message(STATUS "Build type: ${CMAKE_CXX_FLAGS}")



get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY LINK_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "dir='${dir}'")
endforeach()
