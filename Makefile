CC = gcc

CPPFLAGS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_POSIX_C_SOURCE=2
CFLAGS   = -pipe -std=c99 -pedantic -Wall -Wno-deprecated-declarations -Os
CFLAGS += -fno-stack-protector

# Debugging flags
CFLAGS += -funsigned-char -g
CFLAGS += -fno-strict-aliasing -ffunction-sections -fdata-sections
CFLAGS += --param max-inline-insns-single=500
CFLAGS += -gstrict-dwarf
CFLAGS += -Wall -Wstrict-prototypes -Wmissing-prototypes -Werror-implicit-function-declaration -Wpointer-arith
CFLAGS += -Wchar-subscripts -Wcomment -Wformat=2 -Wimplicit-int -Wmain -Wparentheses -Wsequence-point -Wreturn-type
CFLAGS += -Wswitch -Wtrigraphs -Wunused -Wuninitialized -Wunknown-pragmas -Wfloat-equal -Wundef -Wshadow -Wbad-function-cast
CFLAGS += -Wwrite-strings -Wsign-compare -Wmissing-declarations -Wformat -Wmissing-format-attribute
CFLAGS += -Wno-deprecated-declarations -Wpacked -Wredundant-decls -Wnested-externs -Wunreachable-code -Wcast-align


timetracking : timetracking.c csvparser.c terminalinterface.c

clean :
	rm timetracking
