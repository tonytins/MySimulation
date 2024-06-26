#########################################
#### CMake generator file for Niotso ####
project(Niotso)
cmake_minimum_required(VERSION 2.6)

enable_language(ASM)

option(BUILD_EXAMPLES "Build the render demos" ON)
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
if(BUILD_SHARED_LIBS)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

if(WIN32)
    set(64BIT 1)
else()
    set(64BIT 1)
endif()

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    # Base options
    set(CFLAGS  "-Wall -Wextra -Wabi -pedantic -mmmx -msse -msse2 -msse3 -fvisibility=hidden")
    set(LDFLAGS "-static-libgcc")
    set(RCFLAGS "")

    set(CFLAGS_LANG_C   "-ansi")
    set(CFLAGS_LANG_CPP "-fvisibility-inlines-hidden -fno-exceptions -fno-rtti -fno-threadsafe-statics -D__STDC_LIMIT_MACROS")

    if(64BIT)
        set(CFLAGS "-m64 ${CFLAGS}")
        set(LDFLAGS "-m64 ${LDFLAGS}")
        set(RCFLAGS "${RCFLAGS} -F pe-x86-64")
    else()
        set(CFLAGS "-m32 ${CFLAGS}")
        set(LDFLAGS "-m32 ${LDFLAGS}")
        set(RCFLAGS "${RCFLAGS} -F pe-i386")
    endif()

    ####
    ## [Profiles]

    if(NOT (CMAKE_BUILD_TYPE MATCHES "Debug"))
        if(CMAKE_BUILD_TYPE MATCHES "Release-MakeProfile")
            set(CFLAGS "${CFLAGS} -fprofile-generate")
            set(LDFLAGS "${LDFLAGS} -lgcov")
        elseif(CMAKE_BUILD_TYPE MATCHES "Release-UseProfile")
            set(CFLAGS "${CFLAGS} -fprofile-use")
        endif()

        # Size
        set(CFLAGS_SIZE "${CFLAGS} -Os -g0 -fomit-frame-pointer -mfpmath=both -msahf -malign-double -mpc32 -ffast-math -fmerge-all-constants -funsafe-loop-optimizations -fsched-pressure -mstringop-strategy=rep_byte -fno-stack-protector")
        set(LDFLAGS_SIZE "${LDFLAGS} -s -fwhole-program -flto -fno-stack-protector")

        # Speed
        set(CFLAGS_SPEED "${CFLAGS} -O3 -g0 -fomit-frame-pointer -mfpmath=both -msahf -malign-double -mpc32 -ffast-math -fmerge-all-constants -funsafe-loop-optimizations -fsched-pressure -fno-stack-protector -fmodulo-sched -fmodulo-sched-allow-regmoves -fgcse-sm -fgcse-las -fsched-spec-load -fsched-spec-load-dangerous -fsched-stalled-insns=0 -fsched-stalled-insns-dep -fsched2-use-superblocks -fipa-pta -fipa-matrix-reorg -ftree-loop-linear -floop-interchange -floop-strip-mine -floop-block -fgraphite-identity -floop-parallelize-all -ftree-loop-distribution -ftree-loop-im -ftree-loop-ivcanon -fivopts -fvect-cost-model -fvariable-expansion-in-unroller -fbranch-target-load-optimize -maccumulate-outgoing-args -flto")
        set(LDFLAGS_SPEED "${LDFLAGS} -s -fwhole-program -flto -fno-stack-protector")
    else()
        # Debug
        set(CFLAGS_DEBUG "${CFLAGS} -O0 -g3 -fstack-protector-all -D_FORTIFY_SOURCE=2 -DDEBUG")
        set(LDFLAGS_DEBUG "${LDFLAGS} -fstack-protector-all")
            set(CFLAGS_SIZE "${CFLAGS_DEBUG}")
            set(LDFLAGS_SIZE "${LDFLAGS_DEBUG}")
            set(CFLAGS_SPEED "${CFLAGS_DEBUG}")
            set(LDFLAGS_SPEED "${LDFLAGS_DEBUG}")
    endif()

    set(CMAKE_C_FLAGS "${CFLAGS_LANG_C} ${CFLAGS_SIZE}")
    set(CMAKE_CXX_FLAGS "${CFLAGS_LANG_CPP} ${CFLAGS_SIZE}")
    if(64BIT)
        set(CMAKE_SHARED_LIBRARY_C_FLAGS "-fpic")
        set(CMAKE_SHARED_LIBRARY_CXX_FLAGS "-fpic")
        set(CMAKE_SHARED_LIBRARY_ASM_FLAGS "-fpic")
    else()
        set(CMAKE_SHARED_LIBRARY_C_FLAGS "")
        set(CMAKE_SHARED_LIBRARY_CXX_FLAGS "")
        set(CMAKE_SHARED_LIBRARY_ASM_FLAGS "")
    endif()
    set(CMAKE_SHARED_LINKER_FLAGS "-shared ${LDFLAGS} ${LDFLAGS_SIZE}")
    set(CMAKE_EXE_LINKER_FLAGS "${LDFLAGS} ${LDFLAGS_SIZE}")
    set(CMAKE_RC_FLAGS "${RCFLAGS}")
    set(CMAKE_ASM_FLAGS "${CFLAGS}")

    if(WIN32)
        set(DIST_NAME "windows" CACHE STRING "Output folder name for the _dist folder (no start or end slash)")
    elseif(APPLE)
        set(DIST_NAME "mac" CACHE STRING "Output folder name for the _dist folder (no start or end slash)")
    elseif(UNIX)
        set(DIST_NAME "linux" CACHE STRING "Output folder name for the _dist folder (no start or end slash)")
    else()
        set(DIST_NAME "unknown" CACHE STRING "Output folder name for the _dist folder (no start or end slash)")
    endif()

endif()

#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/_dist/${DIST_NAME}") (-flto means our archive files should not be redistributed)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/_dist/${DIST_NAME}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/_dist/${DIST_NAME}")

if(WIN32)
    set(FREETYPE_INCLUDE ${CMAKE_SOURCE_DIR}/_deps/freetype/include ${CMAKE_SOURCE_DIR}/_deps/freetype/include/freetype/config)
    #set(LIBJPEGTURBO_INCLUDE ${CMAKE_SOURCE_DIR}/_deps/libjpeg-turbo)
    set(LIBMPG123_INCLUDE ${CMAKE_SOURCE_DIR}/_deps/libmpg123)
    set(LIBPNG_INCLUDE ${CMAKE_SOURCE_DIR}/_deps/libpng)
    #set(LIBPQ_INCLUDE ${CMAKE_SOURCE_DIR}/_deps/libpq ${CMAKE_SOURCE_DIR}/_deps/libpq/include ${CMAKE_SOURCE_DIR}/_deps/libpq/include/port/win32 ${CMAKE_SOURCE_DIR}/_deps/libpq/include/port)
    set(ZLIB_INCLUDE ${CMAKE_SOURCE_DIR}/_deps/zlib)

    add_subdirectory(_deps/freetype)
    add_subdirectory(_deps/libjpeg-turbo)
    #add_subdirectory(_deps/libmpg123)
    add_subdirectory(_deps/libpng)
    #add_subdirectory(_deps/libpq)
    add_subdirectory(_deps/zlib)

    set(FREETYPE_LINK freetype_shared)
    set(LIBJPEG_LINK jpegturbo_static)
    #set(LIBMPG123_LINK libmpg123_static)
    set(LIBPNG_LINK libpng_static)
    #set(LIBPQ_LINK libpq_shared)
    set(ZLIB_LINK zlib_static)
else()
    set(FREETYPE_LINK freetype)
    set(LIBJPEG_LINK jpeg)
    #set(LIBMPG123_LINK mpg123)
    set(LIBPNG_LINK png)
    #set(LIBPQ_LINK pq)
    set(ZLIB_LINK z)
endif()

set(FILEHANDLER_INCLUDE ${CMAKE_SOURCE_DIR}/formats)
set(LIBGLDEMO_INCLUDE ${CMAKE_SOURCE_DIR}/libgldemo)
set(LIBVITABOY_INCLUDE ${CMAKE_SOURCE_DIR}/libvitaboy)

if(WIN32)
    set(GLDEMO_EXE WIN32)
    set(GLDEMO_LIBS mingw32 libgldemo_static opengl32 glu32)
else()
    set(GLDEMO_EXE "")
    set(GLDEMO_LIBS libgldemo_static Xxf86vm rt Xext X11 GL GLU)
endif()

add_subdirectory(formats)
add_subdirectory(libgldemo)
add_subdirectory(libvitaboy)
add_subdirectory(tools)

if(BUILD_EXAMPLES)
    add_subdirectory(rlgldemo)
endif()

unset(BUILD_SHARED_LIBS CACHE)
unset(BUILD_EXAMPLES CACHE)
