cmake_minimum_required(VERSION 3.11)

project(PaOpenSLESUnitTests)

# Add OpenSLES
find_path(OPENSLES_INCLUDES SLES/OpenSLES.h)
find_library(OPENSLES OpenSLES)

# Add liblog
find_path(LOG_INCLUDES android/log.h)
find_library(LOG log)

# Add googletest
include(FetchContent)
FetchContent_Declare(
  GOOGLETEST
  URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.tar.gz
)
FetchContent_MakeAvailable(GOOGLETEST)

# Add portaudio
set(PA_ARCHIVE_PATH "${PROJECT_SOURCE_DIR}/portaudio-archives/libportaudio-${ANDROID_PLATFORM_LEVEL}.a")
if (NOT EXISTS "${PA_ARCHIVE_PATH}")
  message(FATAL_ERROR "Could not find required portaudio archive ${PA_ARCHIVE_PATH}")
endif()

set(TARGET "pa-opensles-unit-tests-${ANDROID_PLATFORM_LEVEL}")


set(SOURCES
  sinetest.c
  )

add_executable("${TARGET}" "${SOURCES}")

target_include_directories("${TARGET}"
  PRIVATE include
  PRIVATE "${OPENSLES_INCLUDES}"
  PRIVATE "${LOG_INCLUDES}"
  )

target_link_libraries("${TARGET}"
  "${OPENSLES}"
  "${LOG}"
  "${GOOGLETEST}"
  "${PA_ARCHIVE_PATH}"
  )


