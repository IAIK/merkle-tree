Q = @
CFLAGS  +=-Wall -Werror -pedantic -std=gnu99 $(EXTRA_CFLAGS)

DEPENDFILE = .depend
LIB_SRC = mt_arr_list.c mt_impl.c mt_crypto.c sha224-256.c
PRG_SRC = mt_test.c
SRC = $(LIB_SRC) $(PRG_SRC)
LIB_OBJS = $(LIB_SRC:%.c=%.o)
PRG_OBJS = $(PRG_SRC:%.c=%.o)
OBJS = $(LIB_OBJS) $(PRG_OBJS)
LIB = libMerkleTree.a
BIN = mt-test

all: silent

debug: LDFLAGS +=-ggdb
debug: CFLAGS +=-ggdb
debug: dep $(BIN)

coverage: LDFLAGS +=-ggdb -coverage
coverage: CFLAGS +=-ggdb -fprofile-arcs -ftest-coverage
coverage: dep $(BIN)

#silent: CFLAGS +=-ggdb -pg
silent: CFLAGS +=-ggdb
silent: clean dep $(BIN)

release: CFLAGS +=-O3 -fgnu89-inline -DNDEBUG -Wno-error=unused-function
release: clean dep $(BIN)

.PHONY: clean doc
clean:
	$(Q)rm -Rf $(BIN) $(OBJS) $(LIB) html latex .depend *.gcov *.gcno *.gcda

doc:
	$(Q)doxygen

$(BIN) : $(PRG_OBJS) $(LIB)
	$(Q)$(CC) $(CFLAGS) -o $(BIN) $(PRG_OBJS) $(LIB) $(LDFLAGS)

$(LIB): $(LIB_OBJS)
	$(Q)$(AR) crvs $(LIB) $(LIB_OBJS)

dep: $(SRC)
	$(Q)$(CC) -MM $(SRC) > $(DEPENDFILE)

-include $(DEPENDFILE)
