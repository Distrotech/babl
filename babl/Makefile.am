## Source directory

if PLATFORM_WIN32
no_undefined = -no-undefined
endif

SUBDIRS = base

c_sources =				\
	babl.c				\
	babl-component.c		\
	babl-conversion.c		\
	babl-core.c			\
	babl-db.c			\
	babl-extension.c		\
	babl-fish-path.c		\
	babl-fish-reference.c		\
	babl-fish-simple.c		\
	babl-fish-stats.c		\
	babl-fish.c			\
	babl-format.c			\
	babl-hash-table.c		\
	babl-image.c			\
	babl-internal.c			\
	babl-introspect.c		\
	babl-list.c			\
	babl-memory.c			\
	babl-model.c			\
	babl-mutex.c			\
	babl-palette.c    \
	babl-ref-pixels.c		\
	babl-sampling.c			\
	babl-sanity.c			\
	babl-type.c			\
	babl-util.c			\
	babl-cpuaccel.c			\
	babl-version.c

h_sources  =				\
	babl.h				\
	babl-class.h			\
	babl-classes.h			\
	babl-component.h		\
	babl-conversion.h		\
	babl-cpuaccel.h			\
	babl-db.h			\
	babl-extension.h		\
	babl-fish.h			\
	babl-format.h			\
	babl-hash-table.h		\
	babl-ids.h			\
	babl-image.h			\
	babl-internal.h			\
	babl-introspect.h		\
	babl-list.h			\
	babl-macros.h			\
	babl-memory.h			\
	babl-model.h			\
	babl-mutex.h			\
	babl-ref-pixels.h		\
	babl-sampling.h			\
	babl-type.h			\
	babl-types.h			\
	babl-util.h

library_includedir=$(includedir)/babl-$(BABL_API_VERSION)/babl
libinc_hdrs = \
	babl-introspect.h       \
	babl-macros.h           \
	babl-types.h            \
	babl.h
libinc_generated_hdrs = \
	babl-version.h
library_include_HEADERS = \
	$(libinc_hdrs) $(libinc_generated_hdrs)

AM_CPPFLAGS = \
	-DLIBDIR=\""$(libdir)"\"	\
	-I$(top_srcdir)			\
	-I$(top_srcdir)/babl/base

lib_LTLIBRARIES= libbabl-@BABL_API_VERSION@.la
libbabl_@BABL_API_VERSION@_la_SOURCES= $(h_sources) $(c_sources)
libbabl_@BABL_API_VERSION@_la_LIBADD=\
	base/libbase.la \
	@LTLIBOBJS@

libbabl_@BABL_API_VERSION@_la_LDFLAGS= 		\
	${no_undefined} $(MATH_LIB) $(THREAD_LIB) \
	-version-info $(BABL_LIBRARY_VERSION)

EXTRA_DIST = babl-ref-pixels.inc
# CLEANFILES =
