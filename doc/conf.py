# -*- coding: utf-8 -*-
# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# import os
# import sys
# sys.path.insert(0, os.path.abspath('.'))

# Substitutions from external build system.
srcdir = None
builddir = None
ext_source_branch = None
ext_source_user = None


# -- Project information -----------------------------------------------------

project = 'LibTIFF'
copyright = '1988-2022, LibTIFF contributors'
author = 'LibTIFF contributors'

# The full version, including alpha/beta/rc tags
release = 'UNDEFINED'
version = 'UNDEFINED'


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    'sphinx.ext.extlinks'
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = []

extlinks = {
    # GitLab links
    'issue' : ('https://gitlab.com/libtiff/libtiff/-/issues/%s', 'issue #%s'),
    'merge' : ('https://gitlab.com/libtiff/libtiff/-/merge_requests/%s', 'merge request #%s'),
    'commit' : ('https://gitlab.com/libtiff/libtiff/-/commit/%s', 'commit %s'),
    'branch' : ('https://gitlab.com/libtiff/libtiff/-/tree/%s', 'branch %s'),
    'tag' : ('https://gitlab.com/libtiff/libtiff/-/tags/%s', 'tag %s'),

    # Old Bugzilla
    'bugzilla' : ('http://bugzilla.maptools.org/show_bug.cgi?id=%s', 'MapTools bugzilla #%s'),
    'bugzilla-rs' : ('http://bugzilla.remotesensing.org/show_bug.cgi?id=%s', 'Remote Sensing bugzilla #%s [no longer available]'),

    # GDAL
    'gdal-trac' : ('http://trac.osgeo.org/gdal/ticket/%s', 'GDAL trac #%s'),
    'oss-fuzz' : ('https://bugs.chromium.org/p/oss-fuzz/issues/detail?id=%s', 'OSS-Fuzz #%s'),

    # Security
    'cve' : ('https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-%s', 'CVE-%s'),
}


# -- Options for manual page output --------------------------------------------

# One entry per manual page. List of tuples
# (source start file, name, description, authors, manual section).
man_pages = [
    ('fax2ps', 'fax2ps', 'convert a TIFF facsimile to compressed PostScript™', author, 1),
    ('fax2tiff', 'fax2tiff', 'create a TIFF Class F fax file from raw fax data', author, 1),
    ('pal2rgb', 'pal2rgb', 'convert a palette color TIFF image to a full color image', author, 1),
    ('ppm2tiff', 'ppm2tiff', 'create a TIFF file from PPM, PGM and PBM image files', author, 1),
    ('raw2tiff', 'raw2tiff', 'create a TIFF file from a raw data file', author, 1),
    ('rgb2ycbcr', 'rgb2ycbcr', 'convert non-YCbCr TIFF images to a YCbCr TIFF image', author, 1),
    ('thumbnail', 'thumbnail', 'create a TIFF file with thumbnail images', author, 1),
    ('tiff2bw', 'tiff2bw', 'convert a color TIFF image to greyscale', author, 1),
    ('tiff2pdf', 'tiff2pdf', 'convert a TIFF image to a PDF document', author, 1),
    ('tiff2ps', 'tiff2ps', 'convert TIFF image to PostScript™', author, 1),
    ('tiff2rgba', 'tiff2rgba', 'convert a TIFF image to RGBA color space', author, 1),
    ('tiffcmp', 'tiffcmp', 'compare two TIFF files', author, 1),
    ('tiffcp', 'tiffcp', 'copy (and possibly convert) a TIFF file', author, 1),
    ('tiffcrop', 'tiffcrop', 'select, copy, crop, convert, extract, and/or process one or more TIFF files', author, 1),
    ('tiffdither', 'tiffdither', 'convert a greyscale TIFF image to bilevel using dithering', author, 1),
    ('tiffdump', 'tiffdump', 'print verbatim information about TIFF files', author, 1),
    ('tiffgt', 'tiffgt', 'display an image stored in a TIFF file (Silicon Graphics version)', author, 1),
    ('tiffinfo', 'tiffinfo', 'print information about TIFF files', author, 1),
    ('tiffmedian', 'tiffmedian', 'apply the median cut algorithm to data in a TIFF file', author, 1),
    ('tiffset', 'tiffset', 'set or unset a field in a TIFF header', author, 1),
    ('tiffsplit', 'tiffsplit', 'split a multi-image TIFF into single-image TIFF files', author, 1),
]

# If true, show URL addresses after external links.
man_show_urls = True
