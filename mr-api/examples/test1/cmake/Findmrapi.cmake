# libcontrolAPI_INCLUDE_DIRS - the libcontrolAPI include directories
# libcontrolAPI_LIBS - link these to use libcontrolAPI

find_path(libmrapi_INCLUDE_DIR
  NAMES mrapi.h
  PATHS /usr/local/include/MRAPI
        $ENV{INCLUDE}
)

find_library(libmrapi
  NAMES mrapi
  PATHS /usr/local/lib/MRAPI
)

find_library(pcandriver   
  NAMES pcanbasic 
  PATHS /usr/lib  NO_DEFAULT_PATH)

set(libmrapi_LDFLAGS "-lrt -lpthread -lm -lpcap")
set(libmrapi_CFLAGS )

set(libmrapi_INCLUDE_DIRS ${libmrapi_INCLUDE_DIR} /usr/include)

set(libmrapi_LIBS ${libmrapi} ${pcandriver})

if(libmrapi_INCLUDE_DIRS)
        message(STATUS "Found Control API include dir: ${libmrapi_INCLUDE_DIRS}")
else(libmrapi_INCLUDE_DIRS)
        message(STATUS "Could NOT find Control API headers.")
endif(libmrapi_INCLUDE_DIRS)


if(libmrapi_LIBS)
  message(STATUS "Found Control API library: ${libmrapi_LIBS}")
else(libmrapi_LIBS)
  message(STATUS "Could NOT find libcontrolAPI library.")
endif(libmrapi_LIBS)

if(libmrapi_INCLUDE_DIRS AND libmrapi_LIBS)
  set(libmrapi_FOUND TRUE)
else(libmrapi_INCLUDE_DIRS AND libmrapi_LIBS)
  set(libmrapi_FOUND FALSE)
  if(libmrapi_FIND_REQUIRED)
    message(FATAL_ERROR "Could not find Control API.")
  endif(libmrapi_FIND_REQUIRED)
endif(libmrapi_INCLUDE_DIRS AND libmrapi_LIBS)
