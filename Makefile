OBJS_DIR = .objs

# define all of student executables
EXE1=dl_main
EXES_STUDENT=$(EXE1) $(EXE2)

EXE2=dl_server

# tests are defined a little differently for this assignment, see below

# list object file dependencies for each
OBJS_DL=$(EXE1).o event.o dl.o
OBJS_SERVER=$(EXE2).o event.o dl.o
# set up compiler
CC = clang
WARNINGS = -Wall -Wextra -Wno-tautological-compare -Wno-sign-compare -Werror -Wno-error=unused-parameter
CFLAGS_DEBUG   = -O0 $(WARNINGS) -g -std=c99 -c -MMD -MP -D_GNU_SOURCE -DDEBUG
CFLAGS_RELEASE = -O2 $(WARNINGS) -g -std=c99 -c -MMD -MP -D_GNU_SOURCE

# set up linker
LD = clang
LDFLAGS =

.PHONY: all
all: release

# build types
.PHONY: release
.PHONY: debug

release: $(EXES_STUDENT)
debug:   clean $(EXES_STUDENT:%=%-debug)


# include dependencies
# -include $(OBJS_DIR)/*.d

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

# patterns to create objects
# keep the debug and release postfix for object files so that we can always
# separate them correctly
$(OBJS_DIR)/%-debug.o: %.c | $(OBJS_DIR)
	$(CC) $(CFLAGS_DEBUG) $< -o $@

$(OBJS_DIR)/%-release.o: %.c | $(OBJS_DIR)
	$(CC) $(CFLAGS_RELEASE) $< -o $@

# exes
# you will need a pair of exe and exe-debug targets for each exe
$(EXE1)-debug: $(OBJS_DL:%.o=$(OBJS_DIR)/%-debug.o)
	$(LD) $^ $(LDFLAGS) -o deadline-debug

$(EXE1): $(OBJS_DL:%.o=$(OBJS_DIR)/%-release.o)
	$(LD) $^ $(LDFLAGS) -o deadline

$(EXE2)-debug: $(OBJS_SERVER:%.o=$(OBJS_DIR)/%-debug.o)
	$(LD) $^ $(LDFLAGS) -o dl_server-debug

$(EXE2): $(OBJS_SERVER:%.o=$(OBJS_DIR)/%-release.o)
	$(LD) $^ $(LDFLAGS) -o dl_server

.PHONY: clean
clean:
	rm -rf .objs $(EXES_STUDENT) $(EXES_STUDENT:%=%-debug) deadline deadline-debug
reset: 
	rm data.bin
	touch data.bin
