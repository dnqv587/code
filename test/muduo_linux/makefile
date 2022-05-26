CC=g++
RM=rm -rf
MKDIR=mkdir

EXE=test
DIR_BIN=bin
DIR_OBJ=objs
DIR_DEPS=deps
DIR_SOURCE=. base designPattern logger test thread

LIB=-l pthread
SUFFIX=.c .cc .cpp .cxx

SRCS:=$(foreach suf,$(SUFFIX),$(foreach dir,$(DIR_SOURCE),$(wildcard $(dir)/*$(suf))))
DIRS=$(DIR_BIN) $(DIR_OBJ) $(DIR_DEPS)
TARGET:=$(addprefix $(DIR_BIN)/,$(EXE))
OBJS:=$(filter %.o,$(foreach suf,$(SUFFIX),$(addprefix $(DIR_OBJ)/,$(notdir $(patsubst %$(suf),%.o,$(SRCS))))))
DEPS:=$(filter %.dep,$(foreach suf,$(SUFFIX),$(addprefix $(DIR_DEPS)/,$(notdir $(patsubst %$(suf),%.dep,$(SRCS))))))
TEST=$(filter main,$(SRCS))

.PHONY:all clean
all:$(TARGET)

sinclude $(DEPS)

$(DIRS):
	$(MKDIR) $@
$(TARGET):$(DIR_BIN) $(OBJS)
	$(CC) -o $@ $(filter %.o,$^) $(LIB)
$(DIR_OBJ)/%.o:$(DIR_OBJ)
	$(CC) -o $@ -c $(foreach n,$(foreach src,$(SRCS),$(if $(findstring  $(strip $(subst .o,,$(notdir $@))),$(strip $(basename $(notdir $(src))))),$(src))),$(if $(findstring  $(strip $(basename $(notdir $(n)))),$(strip $(subst .o,,$(notdir $@)))),$(n))) $(LIB)
$(DIR_DEPS)/%.dep:$(DIR_DEPS) 
	@echo "making $@ ..."
	@set -e;\
	$(RM) $@.tmp;\
	$(CC) -E -MM $(foreach n,$(foreach src,$(SRCS),$(if $(findstring  $(strip $(subst .dep,,$(notdir $@))),$(strip $(basename $(notdir $(src))))),$(src))),$(if $(findstring  $(strip $(basename $(notdir $(n)))),$(strip $(subst .dep,,$(notdir $@)))),$(n))) > $@.tmp;\
	sed 's,\(.*\)\.o[ :]*,$(DIR_OBJ)/\1.o: ,g' < $@.tmp > $@ ; \
	$(RM) $@.tmp

clean:
	@$(RM) $(DIRS)
