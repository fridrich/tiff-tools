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

#include "tiff_tools_config.h"

#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include <string.h>

#ifdef HAVE_ASSERT_H
#include <assert.h>
#else
#define assert(x)
#endif

#include <tiffio.h>

#include <limits.h>

#ifndef TIFF_MAX_DIR_COUNT
#define TIFF_MAX_DIR_COUNT 1048576
#endif

#define TIFF_NON_EXISTENT_DIR_NUMBER UINT_MAX

#define streq(a, b) (strcmp(a, b) == 0)
#define strneq(a, b, n) (strncmp(a, b, n) == 0)

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

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
#define TIFF_FILLORDER 0x00003U   /* natural bit fill order for machine */
#define TIFF_DIRTYHEADER 0x00004U /* header must be written on close */
#define TIFF_DIRTYDIRECT 0x00008U /* current directory must be written */
#define TIFF_BUFFERSETUP 0x00010U /* data buffers setup */
#define TIFF_CODERSETUP 0x00020U  /* encoder/decoder setup done */
#define TIFF_BEENWRITING 0x00040U /* written 1+ scanlines to file */
#define TIFF_SWAB 0x00080U        /* byte swap file information */
#define TIFF_NOBITREV 0x00100U    /* inhibit bit reversal logic */
#define TIFF_MYBUFFER 0x00200U    /* my raw data buffer; free on close */
#define TIFF_ISTILED 0x00400U     /* file is tile, not strip- based */
#define TIFF_MAPPED 0x00800U      /* file is mapped into memory */
#define TIFF_POSTENCODE 0x01000U  /* need call to postencode routine */
#define TIFF_INSUBIFD 0x02000U    /* currently writing a subifd */
#define TIFF_UPSAMPLED 0x04000U   /* library is doing data up-sampling */
#define TIFF_HEADERONLY                                                        \
    0x10000U /* read header only, do not process the first directory */
#define TIFF_NOREADRAW                                                         \
    0x20000U /* skip reading of raw uncompressed image data */
#define TIFF_INCUSTOMIFD 0x40000U /* currently writing a custom IFD */
#define TIFF_BIGTIFF 0x80000U     /* read/write bigtiff */
#define TIFF_BUF4WRITE 0x100000U  /* rawcc bytes are for writing */
#define TIFF_DIRTYSTRIP 0x200000U /* stripoffsets/stripbytecount dirty*/
#define TIFF_PERSAMPLE 0x400000U  /* get/set per sample tags as arrays */
#define TIFF_BUFFERMMAP                                                        \
    0x800000U /* read buffer (tif_rawdata) points into mmap() memory */
#define TIFF_DEFERSTRILELOAD                                                   \
    0x1000000U /* defer strip/tile offset/bytecount array loading. */
#define TIFF_LAZYSTRILELOAD                                                    \
    0x2000000U /* lazy/ondemand loading of strip/tile offset/bytecount values. \
                  Only used if TIFF_DEFERSTRILELOAD is set and in read-only    \
                  mode */
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

#define isPseudoTag(t) (t > 0xffff) /* is tag value normal or pseudo */

#define isTiled(tif) (((tif)->tif_flags & TIFF_ISTILED) != 0)
#define isMapped(tif) (((tif)->tif_flags & TIFF_MAPPED) != 0)
#define isFillOrder(tif, o) (((tif)->tif_flags & (o)) != 0)
#define isUpSampled(tif) (((tif)->tif_flags & TIFF_UPSAMPLED) != 0)
#define TIFFReadFile(tif, buf, size)                                           \
    ((*(tif)->tif_readproc)((tif)->tif_clientdata, (buf), (size)))
#define TIFFWriteFile(tif, buf, size)                                          \
    ((*(tif)->tif_writeproc)((tif)->tif_clientdata, (buf), (size)))
#define TIFFSeekFile(tif, off, whence)                                         \
    ((*(tif)->tif_seekproc)((tif)->tif_clientdata, (off), (whence)))
#define TIFFCloseFile(tif) ((*(tif)->tif_closeproc)((tif)->tif_clientdata))
#define TIFFGetFileSize(tif) ((*(tif)->tif_sizeproc)((tif)->tif_clientdata))
#define TIFFMapFileContents(tif, paddr, psize)                                 \
    ((*(tif)->tif_mapproc)((tif)->tif_clientdata, (paddr), (psize)))
#define TIFFUnmapFileContents(tif, addr, size)                                 \
    ((*(tif)->tif_unmapproc)((tif)->tif_clientdata, (addr), (size)))

/*
 * Default Read/Seek/Write definitions.
 */
#ifndef ReadOK
#define ReadOK(tif, buf, size) (TIFFReadFile((tif), (buf), (size)) == (size))
#endif
#ifndef SeekOK
#define SeekOK(tif, off) _TIFFSeekOK(tif, off)
#endif
#ifndef WriteOK
#define WriteOK(tif, buf, size) (TIFFWriteFile((tif), (buf), (size)) == (size))
#endif

/* Safe multiply which returns zero if there is an *unsigned* integer overflow.
 * This macro is not safe for *signed* integer types */
#define TIFFSafeMultiply(t, v, m)                                              \
    ((((t)(m) != (t)0) && (((t)(((v) * (m)) / (m))) == (t)(v)))                \
         ? (t)((v) * (m))                                                      \
         : (t)0)

#define TIFFmax(A, B) ((A) > (B) ? (A) : (B))
#define TIFFmin(A, B) ((A) < (B) ? (A) : (B))

/*
  Support for large files.

  Windows read/write APIs support only 'unsigned int' rather than 'size_t'.
  Windows off_t is only 32-bit, even in 64-bit builds.
*/
#if defined(HAVE_FSEEKO)
/*
  Use fseeko() and ftello() if they are available since they use
  'off_t' rather than 'long'.  It is wrong to use fseeko() and
  ftello() only on systems with special LFS support since some systems
  (e.g. FreeBSD) support a 64-bit off_t by default.

  For MinGW, __MSVCRT_VERSION__ must be at least 0x800 to expose these
  interfaces. The MinGW compiler must support the requested version.  MinGW
  does not distribute the CRT (it is supplied by Microsoft) so the correct CRT
  must be available on the target computer in order for the program to run.
*/
#if defined(HAVE_FSEEKO)
#define fseek(stream, offset, whence) fseeko(stream, offset, whence)
#define ftell(stream, offset, whence) ftello(stream, offset, whence)
#endif
#endif
#if defined(__WIN32__) && !(defined(_MSC_VER) && _MSC_VER < 1400) &&           \
    !(defined(__MSVCRT_VERSION__) && __MSVCRT_VERSION__ < 0x800)
typedef unsigned int TIFFIOSize_t;
#define _TIFF_lseek_f(fildes, offset, whence)                                  \
    _lseeki64(fildes, /* __int64 */ offset, whence)
/* #define _TIFF_tell_f(fildes) /\* __int64 *\/ _telli64(fildes) */
#define _TIFF_fseek_f(stream, offset, whence)                                  \
    _fseeki64(stream, /* __int64 */ offset, whence)
#define _TIFF_fstat_f(fildes, stat_buff)                                       \
    _fstati64(fildes, /* struct _stati64 */ stat_buff)
/* #define _TIFF_ftell_f(stream) /\* __int64 *\/ _ftelli64(stream) */
/* #define _TIFF_stat_f(path,stat_buff) _stati64(path,/\* struct _stati64 *\/
 * stat_buff) */
#define _TIFF_stat_s struct _stati64
#define _TIFF_off_t __int64
#else
typedef size_t TIFFIOSize_t;
#define _TIFF_lseek_f(fildes, offset, whence) lseek(fildes, offset, whence)
/* #define _TIFF_tell_f(fildes) (_TIFF_lseek_f(fildes,0,SEEK_CUR)) */
#define _TIFF_fseek_f(stream, offset, whence) fseek(stream, offset, whence)
#define _TIFF_fstat_f(fildes, stat_buff) fstat(fildes, stat_buff)
/* #define _TIFF_ftell_f(stream) ftell(stream) */
/* #define _TIFF_stat_f(path,stat_buff) stat(path,stat_buff) */
#define _TIFF_stat_s struct stat
#define _TIFF_off_t off_t
#endif

#if defined(__cplusplus)
extern "C"
{
#endif
    extern void *_TIFFCheckMalloc(TIFF *, tmsize_t, tmsize_t, const char *);
    extern uint32_t _TIFFClampDoubleToUInt32(double);
    extern uint32_t _TIFFMultiply32(TIFF *, uint32_t, uint32_t, const char *);
#if defined(__cplusplus)
}
#endif
#endif /* _TIFFIOP_ */
