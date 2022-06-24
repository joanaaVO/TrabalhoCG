cmake_minimum_required(VERSION 3.5)

# Project Name
PROJECT(Fase4)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_executable(generator src/GENERATOR/generator.cpp src/GENERATOR/bezier.cpp src/GENERATOR/headers/bezier.hpp)
add_executable(engine src/ENGINE/engine.cpp src/ENGINE/Group.cpp src/ENGINE/headers/Group.hpp src/ENGINE/catmull.cpp src/ENGINE/headers/catmull.hpp src/ENGINE/Transformation.cpp src/ENGINE/headers/Transformation.hpp)

add_library(
		tinyxml2Lib
		src/tinyxml2/tinyxml2.cpp
)

target_link_libraries(engine tinyxml2Lib)

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

if  (WIN32)

	message(STATUS "Toolkits_DIR set to: " ${TOOLKITS_FOLDER})
	set(TOOLKITS_FOLDER "" CACHE PATH "Path to Toolkits folder")
	
	if (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")
		message(ERROR ": GLUT not found")
	endif (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")

	if (NOT EXISTS "${TOOLKITS_FOLDER}/glew/GL/glew.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glew/glew32.lib")
		message(ERROR ": GLEW not found")
	endif (NOT EXISTS "${TOOLKITS_FOLDER}/glew/GL/glew.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glew/glew32.lib")
	
	if (NOT EXISTS "${TOOLKITS_FOLDER}/devil/IL/il.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/devil/devIL.lib")
		message(ERROR ": DevIL not found")
	endif (NOT EXISTS "${TOOLKITS_FOLDER}/devil/IL/il.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/devil/devIL.lib")	

	
	include_directories(${TOOLKITS_FOLDER}/glut ${TOOLKITS_FOLDER}/glew ${TOOLKITS_FOLDER}/devil)
	target_link_libraries(engine ${OPENGL_LIBRARIES} 
										  ${TOOLKITS_FOLDER}/glut/glut32.lib
										  ${TOOLKITS_FOLDER}/devil/devIL.lib
										  ${TOOLKITS_FOLDER}/glew/glew32.lib )
	
	if (EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll" AND EXISTS "${TOOLKITS_FOLDER}/glew/glew32.dll" 
											AND EXISTS "${TOOLKITS_FOLDER}/devil/devil.dll")
		file(COPY ${TOOLKITS_FOLDER}/glut/glut32.dll DESTINATION ${CMAKE_BINARY_DIR})
		file(COPY ${TOOLKITS_FOLDER}/devil/devil.dll DESTINATION ${CMAKE_BINARY_DIR})
		file(COPY ${TOOLKITS_FOLDER}/glew/glew32.dll DESTINATION ${CMAKE_BINARY_DIR})
	endif(EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll" AND EXISTS "${TOOLKITS_FOLDER}/glew/glew32.dll"
										  	AND EXISTS "${TOOLKITS_FOLDER}/devil/devil.dll")
	
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT engine)
	
else (WIN32) #Linux and Mac

	set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} Wno-deprecated")
	
	find_package(GLUT REQUIRED)
	include_directories(${GLUT_INCLUDE_DIR})
	link_directories(${GLUT_LIBRARY_DIRS})
	add_definitions(${GLUT_DEFINITIONS})

	find_package(DevIL REQUIRED)
	link_libraries(${IL_LIBRARIES} )
	include_directories(${IL_INCLUDE_DIR})
	
	if (NOT APPLE)
		find_package(GLEW REQUIRED)
		include_directories(${GLEW_INCLUDE_DIRS})
		link_libraries(${GLEW_LIBRARIES})
		target_link_libraries(engine ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES} ${IL_LIBRARIES})
	else (NOT APPLE)
		target_link_libraries(engine ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${IL_LIBRARIES})
	endif(NOT APPLE)
	
	target_link_libraries(engine ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
	if(NOT GLUT_FOUND)
	   message(ERROR ": GLUT not found!")
	endif(NOT GLUT_FOUND)
	
endif(WIN32)








