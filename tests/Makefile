Q=@
VGRUN = valgrind --tool=memcheck

LDFLAGS += -lcppunit -ggdb $(EXTRA_LDFLAGS)
CXXFLAGS += -Wall -ggdb $(EXTRA_CXXFLAGS)
CPPFLAGS += -I ../src/

TST_DEP_FILE = .depend
TST_CXX_SRC = MerkleTreeTest.cpp MtTestRunner.cpp
TST_CXX_OBJS = $(TST_CXX_SRC:%.cpp=%.o)
TST_SRC = $(TST_CXX_SRC)
TST_OBJS = $(TST_CXX_OBJS)
TST_LIB = ../src/libMerkleTree.a
TST_BIN = merkle-tree-test

VGRUN = valgrind --tool=memcheck 
Q=@

all: dep $(TST_BIN) 

test: $(TST_BIN)
	$(Q)$(VGRUN) ./$(TST_BIN)

.PHONY: clean
clean:
	$(Q)rm -Rf $(TST_BIN) $(TST_OBJS) html latex .depend

$(TST_BIN) : $(TST_OBJS) $(TST_LIB)
	$(Q)$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $(TST_BIN) $(TST_OBJS) $(TST_LIB) $(LDFLAGS)

$(TST_LIB):
	$(Q)$(MAKE) -C ../src/ all

dep: $(TST_SRC)
	$(Q)$(CXX) $(CPPFLAGS) -MM $(TST_SRC) > $(TST_DEP_FILE)

-include $(TST_DEP_FILE)
