#  Copyright 2015 Google Inc.
#
#  Use of this source code is governed by a BSD-style license that can be
#  found in the LICENSE file.

OUT=../out
OBJDIR=${OUT}/objects

TESTS=$(wildcard ../tests/*_test.cpp)
TEST_OBJECTS := $(addprefix $(OBJDIR)/, $(notdir $(TESTS:.cpp=.o)))
HEADERS=$(wildcard ../include/*.h)

.PHONY: test
test: $(OUT)/test

$(OUT)/test: $(OBJDIR)/gtest.o $(OBJDIR)/test.o  ${TEST_OBJECTS}
	${CXX} -g  -std=c++11 -pthread $^ -o $@

GTEST_DIR=../external/googletest

${GTEST_DIR}/src/gtest-all.cc:
	sh ./get_gtest.sh

$(OBJDIR)/gtest.o: ${GTEST_DIR}/src/gtest-all.cc
	mkdir -p ${OBJDIR}
	${CXX} -isystem ${GTEST_DIR}/include -I ${GTEST_DIR} -pthread -c $< -o $@

$(OBJDIR)/%.o: ../tests/%.cpp ${HEADERS}
	mkdir -p ${OBJDIR}
	${CXX} -isystem ${GTEST_DIR}/include -I ../include -g -std=c++11 -pthread -c $< -o $@

.PHONY: clean
clean:
	rm -f ${OBJDIR}/*.o ${OUT}/test
