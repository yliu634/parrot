
# SPDX-License-Identifier: BSD-3-Clause
# Copyright(c) 2010-2014 Intel Corporation

# all source are stored in SRCS-x
SRCS_client := client.cc
SRCS_server := server.cc

# Build using pkg-config variables if possible
PKGCONF ?= pkg-config
ifneq ($(shell $(PKGCONF) --exists libdpdk && echo 0),0)
$(error "no installation of DPDK found")
endif

all: shared
#.PHONY: shared
shared: build/client	build/server

PC_FILE := $(shell $(PKGCONF) --path libdpdk 2>/dev/null)
CFLAGS += -I../ -std=c++17 -Wno-literal-suffix -O3 $(shell $(PKGCONF) --cflags libdpdk) 
CFLAGS += -DALLOW_EXPERIMENTAL_API
LDFLAGS_SHARED = $(shell $(PKGCONF) --libs libdpdk)

build/client: $(SRCS_client) Makefile $(PC_FILE) | build
	$(CXX) $(CFLAGS) $(SRCS_client) -o $@ $(LDFLAGS) $(LDFLAGS_SHARED)
build/server: $(SRCS_server) Makefile $(PC_FILE) | build
	$(CXX) $(CFLAGS) $(SRCS_server) -o $@ $(LDFLAGS) $(LDFLAGS_SHARED)

build:
	@mkdir -p $@

.PHONY: clean
clean:
	rm -f build/*
	test -d build && rmdir -p build || true

