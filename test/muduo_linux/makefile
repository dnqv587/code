CC=gcc
RM=rm -rf
MKDIR=mkdir

DIR_BIN=bin
DIR_OBJ=objs
DIR_DEPS=deps

DIRS=$(DIR_BIN) $(DIR_OBJ) $(DIR_DEPS)
TARGET:=$(addprefix $(DIR_BIN)/,test)
SRCS=$(wildcard *.c)
OBJS:=$(addprefix $(DIR_OBJ)/,$(patsubst %.c,%.o,$(SRCS)))
DEPS:=$(addprefix $(DIR_DEPS)/,$(patsubst %.c,%.dep,$(SRCS)))

.PHONY:all 
all:$(DIRS) $(DEPS) $(TARGET)

 include $(DEPS)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(filter %.o,$^)
$(DIR_OBJ)/%.o:%.c
	$(CC) -o $@ -c $(filter %.c,$^)
$(DIRS):
	$(MKDIR) $@
$(DIR_DEPS)/%.dep:%.c
	@echo "making $@ ..."
	@set -e;\
	$(RM) $@.tmp;\
	$(CC) -E -MM $(filter %.c,$^) > $@.tmp;\
	sed 's,\(.*\)\.o[ :]*,$(DIR_OBJ)/\1.o: ,g' < $@.tmp > $@ ; \
	$(RM) $@.tmp

.PHONY:clean
clean:
	@$(RM) $(DIRS)
