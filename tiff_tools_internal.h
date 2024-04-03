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

#ifndef _TIFF_TOOLS_INTERNAL_
#define _TIFF_TOOLS_INTERNAL_

#include "config.h"

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

#define TIFF_NON_EXISTENT_DIR_NUMBER UINT_MAX

#define streq(a, b) (strcmp(a, b) == 0)
#define strneq(a, b, n) (strncmp(a, b, n) == 0)

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

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
#define fseek(stream, offset, whence) fseeko(stream, offset, whence)
#define ftell(stream) ftello(stream)
#endif
#if defined(__WIN32__) && !(defined(_MSC_VER) && _MSC_VER < 1400) &&           \
    !(defined(__MSVCRT_VERSION__) && __MSVCRT_VERSION__ < 0x800)
typedef unsigned int TIFFIOSize_t;
#define _TIFF_lseek_f(fildes, offset, whence)                                  \
    _lseeki64(fildes, /* __int64 */ offset, whence)
#define _TIFF_fseek_f(stream, offset, whence)                                  \
    _fseeki64(stream, /* __int64 */ offset, whence)
#define _TIFF_fstat_f(fildes, stat_buff)                                       \
    _fstati64(fildes, /* struct _stati64 */ stat_buff)
#define _TIFF_stat_s struct _stati64
#define _TIFF_off_t __int64
#else
typedef size_t TIFFIOSize_t;
#define _TIFF_lseek_f(fildes, offset, whence) lseek(fildes, offset, whence)
#define _TIFF_fseek_f(stream, offset, whence) fseek(stream, offset, whence)
#define _TIFF_fstat_f(fildes, stat_buff) fstat(fildes, stat_buff)
#define _TIFF_stat_s struct stat
#define _TIFF_off_t off_t
#endif

/* Libtiff functions not in headers */
/* Verify that the signatures did not change */
extern void *_TIFFCheckMalloc(TIFF *, tmsize_t, tmsize_t, const char *);
extern uint32_t _TIFFClampDoubleToUInt32(double);
extern uint32_t _TIFFMultiply32(TIFF *, uint32_t, uint32_t, const char *);

#if !defined(__MINGW32__)
#  define TIFF_SIZE_FORMAT "zu"
#endif
#if SIZEOF_SIZE_T == 8
#  define TIFF_SSIZE_FORMAT PRId64
#  if defined(__MINGW32__)
#    define TIFF_SIZE_FORMAT PRIu64
#  endif
#elif SIZEOF_SIZE_T == 4
#  define TIFF_SSIZE_FORMAT PRId32
#  if defined(__MINGW32__)
#    define TIFF_SIZE_FORMAT PRIu32
#  endif
#else
#  error "Unsupported size_t size; please submit a bug report"
#endif

#endif /* _TIFFIOP_ */
