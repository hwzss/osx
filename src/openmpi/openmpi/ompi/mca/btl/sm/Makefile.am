#
# Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
#                         University Research and Technology
#                         Corporation.  All rights reserved.
# Copyright (c) 2004-2005 The University of Tennessee and The University
#                         of Tennessee Research Foundation.  All rights
#                         reserved.
# Copyright (c) 2004-2005 High Performance Computing Center Stuttgart, 
#                         University of Stuttgart.  All rights reserved.
# Copyright (c) 2004-2005 The Regents of the University of California.
#                         All rights reserved.
# $COPYRIGHT$
# 
# Additional copyrights may follow
# 
# $HEADER$
#

# Use the top-level Makefile.options



libmca_btl_sm_la_sources = \
    btl_sm.c \
    btl_sm.h \
    btl_sm_component.c \
    btl_sm_endpoint.h \
    btl_sm_fifo.h \
    btl_sm_frag.c \
    btl_sm_frag.h 

# Make the output library in this directory, and name it either
# mca_<type>_<name>.la (for DSO builds) or libmca_<type>_<name>.la
# (for static builds).

if OMPI_BUILD_btl_sm_DSO
component_noinst =
component_install = mca_btl_sm.la
else
component_noinst = libmca_btl_sm.la
component_install =
endif

# See ompi/mca/common/sm/Makefile.am for an explanation of
# libmca_common_sm.la.

mcacomponentdir = $(libdir)/openmpi
mcacomponent_LTLIBRARIES = $(component_install)
mca_btl_sm_la_SOURCES = $(libmca_btl_sm_la_sources)
mca_btl_sm_la_LDFLAGS = -module -avoid-version
mca_btl_sm_la_LIBADD = \
    $(top_ompi_builddir)/ompi/mca/common/sm/libmca_common_sm.la \
    $(top_ompi_builddir)/ompi/libmpi.la \
    $(top_ompi_builddir)/orte/libopen-rte.la \
    $(top_ompi_builddir)/opal/libopen-pal.la
        
noinst_LTLIBRARIES = $(component_noinst)
libmca_btl_sm_la_SOURCES = $(libmca_btl_sm_la_sources)
libmca_btl_sm_la_LDFLAGS = -module -avoid-version
libmca_btl_sm_la_LIBADD = 
