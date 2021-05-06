# Commands.
RM ?= rm -f

# Flags.
CFLAGS ?= -Wall -Wextra -Werror

# Programs.
PROGRAMS := fbinfo fbmmap vconsole

# Make targets, leverage implicit rules.
.PHONY: all
all: $(PROGRAMS)

.PHONY: clean
clean:
	$(RM) $(PROGRAMS)
