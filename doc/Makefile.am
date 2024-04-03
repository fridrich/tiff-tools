#
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

if SPHINX
doc-man:
	$(SPHINX_BUILD) \
	-D "release=$(TIFF_TOOLS_VERSION)" \
	-D "version=$(TIFF_TOOLS_MAJOR_VERSION).$(TIFF_TOOLS_MINOR_VERSION)" \
	-d "$(builddir)/cache" \
	-b man \
	"$(srcdir)" "$(builddir)/man"

man1_MANS = $(TIFF_MAN1_GEN)

@TIFF_MAN_RULES@

all-local: doc-html doc-man

clean-local:
	rm -r man
	rm -r cache
else
# Sphinx not available
doc-man:
	echo "Manual page documentation generation not configured (sphinx not available)"
	exit 1

man1_MANS = $(TIFF_MAN1_PREBUILT)
endif

rst_sources = \
	tiffgt.rst \
	tiffdither.rst \
	tiff2pdf.rst \
	tiff2ps.rst \
	ppm2tiff.rst \
	raw2tiff.rst \
	fax2tiff.rst \
	tiffcrop.rst \
	tiff2bw.rst \
	fax2ps.rst \
	thumbnail.rst \
	tiffcmp.rst \
	tiffdump.rst \
	tiff2rgba.rst \
	tiffinfo.rst \
	rgb2ycbcr.rst \
	tiffset.rst \
	tiffsplit.rst \
	tiffmedian.rst \
	pal2rgb.rst \
	tiffcp.rst

EXTRA_DIST = \
	conf.py \
	index.rst \
	$(rst_sources)

dist-hook: doc-html doc-man
	cp -r man $(distdir)/man-prebuilt

.PHONY: doc-html doc-man
