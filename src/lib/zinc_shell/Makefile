
NAMESPACE = zinc

COMPILE_MODE = lib

REQUIRE_C = 1

LYOBJ = shell_parser

THIRD_PARTY_LIBS = -lncurses

OBJECT_SOURCES =           \
	environment.c      \
	view.c             \
	shell_kernel.c     \
	command.c          \
	command_sequence.c \
	keywords.c         \
	shell.c            \
	$(NULL)

PUBLIC_HEADERS =           \
	shell.h            \
	$(NULL)

#DEPENDENCIES = \
#	-lprimitives \
#	$(NULL)

include ../../../common.make
