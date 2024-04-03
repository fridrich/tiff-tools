# Tag Image File Format (TIFF) Software
#
# Copyright (C) 2004, Andrey Kiselev <dron@ak4719.spb.edu>
#
# Permission to use, copy, modify, distribute, and sell this software and 
# its documentation for any purpose is hereby granted without fee, provided
# that (i) the above copyright notices and this permission notice appear in
# all copies of the software and related documentation, and (ii) the names of
# Sam Leffler and Silicon Graphics may not be used in any advertising or
# publicity relating to the software without the specific, prior written
# permission of Sam Leffler and Silicon Graphics.
# 
# THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
# EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
# WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
# 
# IN NO EVENT SHALL SAM LEFFLER OR SILICON GRAPHICS BE LIABLE FOR
# ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
# OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
# WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF 
# LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 
# OF THIS SOFTWARE.

# Process this file with automake to produce Makefile.in.

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = doc

EXTRA_DIST = \
	list-manpage-dependencies.py \
	list-manpages.py \
	tif_tools_versioninfo.rc

bin_PROGRAMS = \
	fax2ps \
	fax2tiff \
	pal2rgb \
	ppm2tiff \
	raw2tiff \
	rgb2ycbcr \
	thumbnail \
	tiff2bw \
	tiff2pdf \
	tiff2ps \
	tiff2rgba \
	tiffcmp \
	tiffcp \
	tiffcrop \
	tiffdither \
	tiffdump \
	tiffinfo \
	tiffmedian \
	tiffset \
	tiffsplit
if HAVE_OPENGL
bin_PROGRAMS += tiffgt
endif

noinst_HEADERS = \
	tiffiop.h \
	tiff_tools_internal.h

fax2ps_SOURCES = fax2ps.c
fax2ps_LDADD = $(LIBTIFF_LIBS)

fax2tiff_SOURCES = fax2tiff.c
fax2tiff_LDADD = $(LIBTIFF_LIBS)

pal2rgb_SOURCES = pal2rgb.c
pal2rgb_LDADD = $(LIBTIFF_LIBS)

ppm2tiff_SOURCES = ppm2tiff.c
ppm2tiff_LDADD = $(LIBTIFF_LIBS)

raw2tiff_SOURCES = raw2tiff.c
raw2tiff_LDADD = $(LIBTIFF_LIBS)

rgb2ycbcr_SOURCES = rgb2ycbcr.c
rgb2ycbcr_LDADD = $(LIBTIFF_LIBS)

thumbnail_SOURCES = thumbnail.c
thumbnail_LDADD = $(LIBTIFF_LIBS)

tiff2bw_SOURCES = tiff2bw.c
tiff2bw_LDADD = $(LIBTIFF_LIBS)

tiff2pdf_SOURCES = tiff2pdf.c
tiff2pdf_LDADD = $(LIBTIFF_LIBS)

tiff2ps_SOURCES = tiff2ps.c
tiff2ps_LDADD = $(LIBTIFF_LIBS)

tiff2rgba_SOURCES = tiff2rgba.c
tiff2rgba_LDADD = $(LIBTIFF_LIBS)

tiffcmp_SOURCES = tiffcmp.c
tiffcmp_LDADD = $(LIBTIFF_LIBS)

tiffcp_SOURCES = tiffcp.c
tiffcp_LDADD = $(LIBTIFF_LIBS)

tiffcrop_SOURCES = tiffcrop.c
tiffcrop_LDADD = $(LIBTIFF_LIBS)

tiffdither_SOURCES = tiffdither.c
tiffdither_LDADD = $(LIBTIFF_LIBS)

tiffdump_SOURCES = tiffdump.c
tiffdump_LDADD = $(LIBTIFF_LIBS)

tiffinfo_SOURCES = tiffinfo.c
tiffinfo_LDADD = $(LIBTIFF_LIBS)

tiffmedian_SOURCES = tiffmedian.c
tiffmedian_LDADD = $(LIBTIFF_LIBS)

tiffset_SOURCES = tiffset.c
tiffset_LDADD = $(LIBTIFF_LIBS)

tiffsplit_SOURCES = tiffsplit.c
tiffsplit_LDADD = $(LIBTIFF_LIBS)

tiffgt_SOURCES = tiffgt.c
tiffgt_CFLAGS = $(CFLAGS) $(GLUT_CFLAGS) $(AM_CFLAGS)
tiffgt_LDADD = $(LIBTIFF_LIBS) $(X_LIBS) $(GLUT_LIBS)

AM_CPPFLAGS = $(LIBTIFF_CFLAGS)

echo:
	(echo $(CFLAGS))
	(echo $(tiffgt_CFLAGS))
	(echo $(GL_CFLAGS))
	(echo $(GLU_CFLAGS))
	(echo $(GLUT_CFLAGS))
