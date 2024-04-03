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

docdir = $(LIBTIFF_DOCDIR)/manual

if SPHINX
doc-man:
	$(SPHINX_BUILD) \
	-D "release=$(LIBTIFF_VERSION)" \
	-D "version=$(LIBTIFF_MAJOR_VERSION).$(LIBTIFF_MINOR_VERSION)" \
	-d "$(builddir)/cache" \
	-b man \
	"$(srcdir)" "$(builddir)/man"

man1_MANS = $(TIFF_MAN1_GEN)

@TIFF_MAN_RULES@

all-local: doc-html doc-man

clean-local:
	rm -r html
	rm -r man
	rm -r cache
else
# Sphinx not available
doc-man:
	echo "Manual page documentation generation not configured (sphinx not available)"
	exit 1

man1_MANS = $(TIFF_MAN1_PREBUILT)
endif


install-data-local: all-local
	find $(html_installdir) -type d | sed -e 's;^$(html_installdir);;' | while read dir; do \
	  echo " $(MKDIR_P) '$(DESTDIR)$(docdir)$$dir'"; \
	  $(MKDIR_P) "$(DESTDIR)$(docdir)$$dir" || exit 1; \
	done
	find $(html_installdir) -type f | sed -e 's;^$(html_installdir);;' | while read docfile; do \
	  docsubdir=$$(dirname $$docfile); \
	  echo "$(INSTALL_DATA) '$(html_installdir)$$docfile' '$(DESTDIR)$(docdir)$$docsubdir'"; \
	  $(INSTALL_DATA) "$(html_installdir)$$docfile" "$(DESTDIR)$(docdir)$$docsubdir"; \
	done

uninstall-local:
	echo "rm -r $(DESTDIR)$(docdir)"
	rm -r $(DESTDIR)$(docdir)

rst_sources = \
	tools/tiffgt.rst \
	tools/tiffdither.rst \
	tools/tiff2pdf.rst \
	tools/tiff2ps.rst \
	tools/ppm2tiff.rst \
	tools/raw2tiff.rst \
	tools/fax2tiff.rst \
	tools/tiffcrop.rst \
	tools/tiff2bw.rst \
	tools/fax2ps.rst \
	tools/thumbnail.rst \
	tools/tiffcmp.rst \
	tools/tiffdump.rst \
	tools/tiff2rgba.rst \
	tools/tiffinfo.rst \
	tools/rgb2ycbcr.rst \
	tools/tiffset.rst \
	tools/tiffsplit.rst \
	tools/tiffmedian.rst \
	tools/pal2rgb.rst \
	tools/tiffcp.rst

EXTRA_DIST = \
	conf.py \
	$(rst_sources)

dist-hook: doc-html doc-man
	cp -r html $(distdir)/html-prebuilt
	cp -r man $(distdir)/man-prebuilt

.PHONY: doc-html doc-man
