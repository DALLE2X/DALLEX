cmake_minimum_required(VERSION 3.0)

project(dream)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m64 -std=c99 -g")



set(CJSON_DIR ${PROJECT_SOURCE_DIR}/cJSON)
set(RAINBOW_DIR ${PROJECT_SOURCE_DIR}/Rainbow)
set(DREAMAI_DIR ${PROJECT_SOURCE_DIR}/DreamAi)
set(BASE64_DIR ${PROJECT_SOURCE_DIR}/base64)
set(SQLITE3_DIR ${PROJECT_SOURCE_DIR}/sqlite3)
set(LOGGER_DIR ${PROJECT_SOURCE_DIR}/logger)

include_directories(

	${CJSON_DIR}/include
	${RAINBOW_DIR}/include
	${BASE64_DIR}/include
	${SQLITE3_DIR}/include
	${LOGGER_DIR}/include
)
set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

file (GLOB source_files 
						"${CJSON_DIR}/src/*.c"
						"${RAINBOW_DIR}/src/*.c"
						"${BASE64_DIR}/src/*.c"
						"${SQLITE3_DIR}/src/*.c"
						)
#message(${source_files})
add_library(${PROJECT_NAME} SHARED ${source_files})
#add_library(${PROJECT_NAME} STATIC ${source_files})
#add_executable(main ${PB_FILES})
#target_link_libraries(${PROJECT_NAME} sdk)
