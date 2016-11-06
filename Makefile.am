DISTCLEAN_LOCAL_HOOKS =
EXTRA_DIST =
CLEANFILES = $(BUILT_FILES)
noinst_LTLIBRARIES =
bin_PROGRAMS =
BUILT_FILES =
ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}
AM_MAKEFLAGS = --no-print-directory



GCC_COLORS ?= 'yes'
export GCC_COLORS

AM_CPPFLAGS = \
	-include $(top_builddir)/config.h \
	-I$(top_srcdir) \
	-DSYSCONFDIR=\""$(sysconfdir)"\"

AM_CFLAGS = \
	-pipe \
	-Wall \
	-W \
	-Wextra \
	-Wno-inline \
	-Wundef \
	-Wformat=2 \
	-Wlogical-op \
	-Wsign-compare \
	-Wformat-security \
	-Wmissing-include-dirs \
	-Wformat-nonliteral \
	-Wold-style-definition \
	-Wpointer-arith \
	-Winit-self \
	-Wdeclaration-after-statement \
	-Wfloat-equal \
	-Wmissing-prototypes \
	-Wstrict-prototypes \
	-Wredundant-decls \
	-Wmissing-declarations \
	-Wmissing-noreturn \
	-Wshadow \
	-Wendif-labels \
	-Wstrict-aliasing=3 \
	-Wwrite-strings \
	-Wno-long-long \
	-Wno-overlength-strings \
	-Wno-unused-parameter \
	-Wno-missing-field-initializers \
	-Wno-unused-result \
	-Wnested-externs \
	-Wchar-subscripts \
	-Wtype-limits \
	-Wuninitialized \
	-fno-common \
	-fdiagnostics-show-option \
	-fvisibility=hidden \
	-ffunction-sections \
	-fdata-sections

AM_CXXFLAGS = \
	-pipe \
	-Wall \
	-W \
	-Wextra \
	-Wno-inline \
	-Wundef \
	-Wformat=2 \
	-Wlogical-op \
	-Wsign-compare \
	-Wformat-security \
	-Wmissing-include-dirs \
	-Wformat-nonliteral \
	-Wpointer-arith \
	-Winit-self \
	-Wfloat-equal \
	-Wredundant-decls \
	-Wmissing-declarations \
	-Wmissing-noreturn \
	-Wshadow \
	-Wendif-labels \
	-Wstrict-aliasing=3 \
	-Wwrite-strings \
	-Wno-long-long \
	-Wno-overlength-strings \
	-Wno-unused-parameter \
	-Wno-missing-field-initializers \
	-Wno-unused-result \
	-Wchar-subscripts \
	-Wtype-limits \
	-Wuninitialized \
	-fno-common \
	-fdiagnostics-show-option \
	-fvisibility=hidden \
	-ffunction-sections \
	-fdata-sections

AM_LDFLAGS = \
	-Wl,--as-needed \
	-Wl,--no-undefined \
	-Wl,--gc-sections

bin_PROGRAMS += mavlink_router
mavlink_router_SOURCES = \
	comm.cpp \
	comm.h \
	log.c \
	log.h \
	macro.h \
	main.cpp \
	util.c \
	util.h