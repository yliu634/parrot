# SPDX-License-Identifier: BSD-3-Clause
# Copyright(c) 2010-2014 Intel Corporation


# all source are stored in SRCS-x
SRCS_l2fwd := l2fwd.cc
SRCS_ludo_l2fwd := ludo_l2fwd.cc
SRCS_parrot_l2fwd := parrot_l2fwd.cc
SRCS_othello_l2fwd := othello_l2fwd.cc
SRCS_learned_l2fwd := learned_l2fwd.cc

# Build using pkg-config variables if possible
PKGCONF ?= pkg-config
ifneq ($(shell $(PKGCONF) --exists libdpdk && echo 0),0)
$(error "no installation of DPDK found")
endif

all: shared
#.PHONY: shared 
shared: build/l2fwd build/othello_l2fwd build/ludo_l2fwd build/parrot_l2fwd build/learned_l2fwd


PC_FILE := $(shell $(PKGCONF) --path libdpdk 2>/dev/null)
#CFLAGS += -I../   used for include trait.hpp 
CFLAGS += -I../ -std=c++17 -Wno-literal-suffix -O3 $(shell $(PKGCONF) --cflags libdpdk) 
CFLAGS += -DALLOW_EXPERIMENTAL_API
LDFLAGS_SHARED = $(shell $(PKGCONF) --libs libdpdk)
LOG_FILES = ../utils/ludo/*.cc


build/l2fwd: $(SRCS_l2fwd) Makefile $(PC_FILE) | build
	$(CXX) $(CFLAGS) $(SRCS_l2fwd) -o $@ $(LDFLAGS) $(LDFLAGS_SHARED)

build/othello_l2fwd: $(SRCS_othello_l2fwd) Makefile $(PC_FILE) | build
	$(CXX) $(CFLAGS) $(SRCS_othello_l2fwd) $(LOG_FILES) -o $@ $(LDFLAGS) $(LDFLAGS_SHARED)

build/ludo_l2fwd: $(SRCS_ludo_l2fwd) Makefile $(PC_FILE) | build
	$(CXX) $(CFLAGS) $(SRCS_ludo_l2fwd) $(LOG_FILES) -o $@ $(LDFLAGS) $(LDFLAGS_SHARED)

build/parrot_l2fwd: $(SRCS_parrot_l2fwd) Makefile $(PC_FILE) | build
	$(CXX) $(CFLAGS) $(SRCS_parrot_l2fwd) $(LOG_FILES) -o $@ $(LDFLAGS) $(LDFLAGS_SHARED) -labsl

build/learned_l2fwd: $(SRCS_learned_l2fwd) Makefile $(PC_FILE) | build
	$(CXX) $(CFLAGS) $(SRCS_learned_l2fwd) $(LOG_FILES) -o $@ $(LDFLAGS) $(LDFLAGS_SHARED)

build:
	@mkdir -p $@

.PHONY: clean
clean:
	rm -f build/l2fwd
	test -d build && rmdir -p build || true
