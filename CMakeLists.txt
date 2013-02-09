# Copyright (c) 2013 Per Johansson
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
# NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

cmake_minimum_required(VERSION 2.6)
project(libhey C)
SET(CPACK_PACKAGE_VENDOR "Per Johansson")
SET(CPACK_PACKAGE_VERSION "1.0")
SET(API_VERSION "1.0")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Happy Eyeballs library")

SET(CMAKE_C_FLAGS "-g -Os -std=c99 -D_POSIX_C_SOURCE=200809L")
SET(CMAKE_C_FLAGS_DEBUG "-g -Os -Wall -Wwrite-strings -Wformat-security -Wempty-body -Werror -std=c99 -D_POSIX_C_SOURCE=200809L")

INCLUDE(CPack)
INCLUDE(CheckFunctionExists)

CHECK_FUNCTION_EXISTS(kqueue HAVE_KQUEUE)
CHECK_FUNCTION_EXISTS(epoll_ctl HAVE_EPOLL)

IF(HAVE_KQUEUE)
	SET(poller poller_kqueue.c)
ELSEIF(HAVE_EPOLL)
	SET(poller poller_epoll.c)
ELSE(HAVE_KQUEUE)
	SET(poller poller_poll.c)
ENDIF(HAVE_KQUEUE)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
CONFIGURE_FILE(config.h.in config.h)

add_library(hey SHARED hey.c lookup.c connect.c timefuncs.c ${poller})
set_target_properties(hey PROPERTIES VERSION ${CPACK_PACKAGE_VERSION} SOVERSION ${API_VERSION})

add_executable(test_lookup test_lookup.c)
target_link_libraries(test_lookup hey)

add_executable(test_connect test_connect.c)
target_link_libraries(test_connect hey)

add_executable(test_hey test_hey.c)
target_link_libraries(test_hey hey)

install(TARGETS hey LIBRARY DESTINATION lib)
install(FILES hey.h DESTINATION include)
install(FILES doc/hey_connect.3 doc/hey_strerror.3 DESTINATION share/man/man3)
