DIR_SOURCE=. base designPattern logger test thread

LIB=-l pthread
SUFFIX=.c .cc .cpp .cxx .h
SRCS:=$(foreach suf,$(SUFFIX),$(foreach dir,$(DIR_SOURCE),$(wildcard $(dir)/*$(suf))))

all:
	@echo $(SRCS)
