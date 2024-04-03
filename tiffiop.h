/*
 * Copyright (c) 1988-1997 Sam Leffler
 * Copyright (c) 1991-1997 Silicon Graphics, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the names of
 * Sam Leffler and Silicon Graphics may not be used in any advertising or
 * publicity relating to the software without the specific, prior written
 * permission of Sam Leffler and Silicon Graphics.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 *
 * IN NO EVENT SHALL SAM LEFFLER OR SILICON GRAPHICS BE LIABLE FOR
 * ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF
 * LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THIS SOFTWARE.
 */

#ifndef _TIFFIOP_
#define _TIFFIOP_
/*
 * ``Library-private'' definitions.
 */

#include "tiff_tools_internal.h"

typedef struct client_info
{
    struct client_info *next;
    void *data;
    char *name;
} TIFFClientInfoLink;

/*
 * Typedefs for ``method pointers'' used internally.
 * these are deprecated and provided only for backwards compatibility.
 */
typedef unsigned char tidataval_t; /* internal image data value type */
typedef tidataval_t *tidata_t;     /* reference to internal image data */

typedef void (*TIFFVoidMethod)(TIFF *);
typedef int (*TIFFBoolMethod)(TIFF *);
typedef int (*TIFFPreMethod)(TIFF *, uint16_t);
typedef int (*TIFFCodeMethod)(TIFF *tif, uint8_t *buf, tmsize_t size,
                              uint16_t sample);
typedef int (*TIFFSeekMethod)(TIFF *, uint32_t);
typedef void (*TIFFPostMethod)(TIFF *tif, uint8_t *buf, tmsize_t size);
typedef uint32_t (*TIFFStripMethod)(TIFF *, uint32_t);
typedef void (*TIFFTileMethod)(TIFF *, uint32_t *, uint32_t *);

struct TIFFOffsetAndDirNumber
{
    uint64_t offset;
    tdir_t dirNumber;
};
typedef struct TIFFOffsetAndDirNumber TIFFOffsetAndDirNumber;

struct tiff
{
    char *tif_name; /* name of open file */
    int tif_fd;     /* open file descriptor */
    int tif_mode;   /* open mode (O_*) */
    uint32_t tif_flags;
    uint64_t tif_diroff;     /* file offset of current directory */
    uint64_t tif_nextdiroff; /* file offset of following directory */
    uint64_t tif_lastdiroff; /* file offset of last directory written so far */
    int tif_setdirectory_force_absolute; /* switch between relative and absolute
                                            stepping in TIFFSetDirectory() */
    union
    {
        TIFFHeaderCommon common;
        TIFFHeaderClassic classic;
        TIFFHeaderBig big;
    } tif_header;
    uint16_t tif_header_size;  /* file's header block and its length */
    uint32_t tif_row;          /* current scanline */
    tdir_t tif_curdir;         /* current directory (index) */
    uint32_t tif_curstrip;     /* current strip for read/write */
    uint64_t tif_curoff;       /* current offset for read/write */
    uint64_t tif_lastvalidoff; /* last valid offset allowed for rewrite in
                                  place. Used only by TIFFAppendToStrip() */
    uint64_t tif_dataoff;      /* current offset for writing dir */
    /* SubIFD support */
    uint16_t tif_nsubifd;   /* remaining subifds to write */
    uint64_t tif_subifdoff; /* offset for patching SubIFD link */
    /* tiling support */
    uint32_t tif_col;      /* current column (offset by row too) */
    uint32_t tif_curtile;  /* current tile for read/write */
    tmsize_t tif_tilesize; /* # of bytes in a tile */
    /* compression scheme hooks */
    int tif_decodestatus;
    TIFFBoolMethod tif_fixuptags;   /* called in TIFFReadDirectory */
    TIFFBoolMethod tif_setupdecode; /* called once before predecode */
    TIFFPreMethod tif_predecode;    /* pre- row/strip/tile decoding */
    TIFFBoolMethod tif_setupencode; /* called once before preencode */
    int tif_encodestatus;
    TIFFPreMethod tif_preencode;      /* pre- row/strip/tile encoding */
    TIFFBoolMethod tif_postencode;    /* post- row/strip/tile encoding */
    TIFFCodeMethod tif_decoderow;     /* scanline decoding routine */
    TIFFCodeMethod tif_encoderow;     /* scanline encoding routine */
    TIFFCodeMethod tif_decodestrip;   /* strip decoding routine */
    TIFFCodeMethod tif_encodestrip;   /* strip encoding routine */
    TIFFCodeMethod tif_decodetile;    /* tile decoding routine */
    TIFFCodeMethod tif_encodetile;    /* tile encoding routine */
    TIFFVoidMethod tif_close;         /* cleanup-on-close routine */
    TIFFSeekMethod tif_seek;          /* position within a strip routine */
    TIFFVoidMethod tif_cleanup;       /* cleanup state routine */
    TIFFStripMethod tif_defstripsize; /* calculate/constrain strip size */
    TIFFTileMethod tif_deftilesize;   /* calculate/constrain tile size */
    uint8_t *tif_data;                /* compression scheme private data */
    /* input/output buffering */
    tmsize_t tif_scanlinesize;  /* # of bytes in a scanline */
    tmsize_t tif_scanlineskew;  /* scanline skew for reading strips */
    uint8_t *tif_rawdata;       /* raw data buffer */
    tmsize_t tif_rawdatasize;   /* # of bytes in raw data buffer */
    tmsize_t tif_rawdataoff;    /* rawdata offset within strip */
    tmsize_t tif_rawdataloaded; /* amount of data in rawdata */
    uint8_t *tif_rawcp;         /* current spot in raw buffer */
    tmsize_t tif_rawcc;         /* bytes unread from raw buffer */
    /* memory-mapped file support */
    uint8_t *tif_base; /* base of mapped file */
    tmsize_t tif_size; /* size of mapped file region (bytes, thus tmsize_t) */
    TIFFMapFileProc tif_mapproc;     /* map file method */
    TIFFUnmapFileProc tif_unmapproc; /* unmap file method */
    /* input/output callback methods */
    thandle_t tif_clientdata;        /* callback parameter */
    TIFFReadWriteProc tif_readproc;  /* read method */
    TIFFReadWriteProc tif_writeproc; /* write method */
    TIFFSeekProc tif_seekproc;       /* lseek method */
    TIFFCloseProc tif_closeproc;     /* close method */
    TIFFSizeProc tif_sizeproc;       /* filesize method */
    /* post-decoding support */
    TIFFPostMethod tif_postdecode; /* post decoding routine */
    /* tag support */
    TIFFField **tif_fields;          /* sorted table of registered tags */
    size_t tif_nfields;              /* # entries in registered tag table */
    const TIFFField *tif_foundfield; /* cached pointer to already found tag */
    TIFFTagMethods tif_tagmethods;   /* tag get/set/print routines */
    TIFFClientInfoLink *tif_clientinfo; /* extra client information. */
    /* Backward compatibility stuff. We need these two fields for
     * setting up an old tag extension scheme. */
    TIFFFieldArray *tif_fieldscompat;
    size_t tif_nfieldscompat;
    /* Error handler support */
    TIFFErrorHandlerExtR tif_errorhandler;
    void *tif_errorhandler_user_data;
    TIFFErrorHandlerExtR tif_warnhandler;
    void *tif_warnhandler_user_data;
    tmsize_t tif_max_single_mem_alloc; /* in bytes. 0 for unlimited */
};

#endif /* _TIFFIOP_ */
