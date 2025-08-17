cmake_minimum_required(VERSION 3.28.3)

set(PROJECT seeds)
set(TARGET main)
project(${PROJECT})

# Enable export compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Path variables
set(PROJECT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
cmake_path(NORMAL_PATH PROJECT_DIR)
set(SOURCE_DIR "${PROJECT_DIR}/src")
set(SUBMODULES_DIR "${PROJECT_DIR}/submodules")
set(OUTPUT_DIR "${PROJECT_DIR}/out")

# Get all files recursively
file(GLOB_RECURSE SOURCES_CPP "${SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE SOURCES_C "${SOURCE_DIR}/*.c")
file(GLOB_RECURSE HEADERS_CPP "${SOURCE_DIR}/*.hpp")
file(GLOB_RECURSE HEADERS_C "${SOURCE_DIR}/*.h")
set(SOURCES ${SOURCES_CPP} ${SOURCES_C})
set(HEADERS ${HEADERS_CPP} ${HEADERS_C})

# Set their directories
foreach (SOURCE_FILE ${SOURCES};${HEADERS})

	# Get relative path
	cmake_path(RELATIVE_PATH SOURCE_FILE BASE_DIRECTORY ${PROJECT_DIR})

	# Extract directory
	get_filename_component(SOURCE_FILE_DIR ${SOURCE_FILE} DIRECTORY)
	
	# Set source group
	string(REPLACE "/" "\\" SOURCE_FILE_DIR ${SOURCE_FILE_DIR})
	source_group("${SOURCE_FILE_DIR}" FILES ${SOURCE_FILE})

endforeach ()

add_subdirectory(${SUBMODULES_DIR}/freetype)

message(${EMSCRIPTEN_ROOT_PATH})

set(INCLUDE_DIRS
	"${SOURCE_DIR}/"
	"${SUBMODULES_DIR}/cglm/include/" # CGLM
	"${SUBMODULES_DIR}/stb/" # STB
	"${SUBMODULES_DIR}/freetype/include/" # Freetype
	)

# Sources
add_executable(${TARGET}
	${SOURCES}
	${HEADERS}
	${ADDITIONAL_FILES}
	)
target_link_libraries(${TARGET} freetype)

# Headers directories
include_directories(${INCLUDE_DIRS})

set(FLAGS
	-sWASM=1
	-sMIN_WEBGL_VERSION=2
	-sFULL_ES3=1
	-sALLOW_MEMORY_GROWTH=1
	-sUSE_ZLIB=1
	-sASSERTIONS=1
	-sRETAIN_COMPILER_SETTINGS=1
	-sSTACK_SIZE=1048576 #Mebibyte
	--preload-file ${PROJECT_DIR}/assets@/)
string(REPLACE ";" " " FLAGS "${FLAGS}")
set_target_properties(${TARGET} PROPERTIES LINK_FLAGS ${FLAGS})
set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_EXECUTABLE_SUFFIX ".js")


set(CLANGD_FILE ${CMAKE_BINARY_DIR}/clangd)
if(EMSCRIPTEN)
	execute_process(COMMAND emcc --cflags OUTPUT_VARIABLE CLANGD_FLAGS_TO_ADD)
else()
	file(WRITE ${CLANGD_FILE} "")
endif()


# Hide ZERO_CHECK and ALL_BUILD targets
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER meta)
