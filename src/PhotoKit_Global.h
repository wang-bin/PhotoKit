/******************************************************************************
	PhotoKit_Global.h: macros
	Copyright (C) 2012 Wang Bin <wbsecg1@gmail.com>
	
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.
	
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
******************************************************************************/


#ifndef PHOTOKIT_GLOBAL_H
#define PHOTOKIT_GLOBAL_H

#define BEGIN_NAMESPACE_PHOTOKIT namespace PhotoKit {
#define END_NAMESPACE_PHOTOKIT } //namespace PhotoKit


#undef PHOTOKIT_VERSION //0x000309

#define PHOTOKIT_VERSION_MAJOR 0	//((PHOTOKIT_VERSION&0xff0000)>>16)
#define PHOTOKIT_VERSION_MINOR 3	//((PHOTOKIT_VERSION&0xff00)>>8)
#define PHOTOKIT_VERSION_PATCH 9	//(PHOTOKIT_VERSION&0xff)

#define VERSION_CHK(major, minor, patch) \
    (((major&0xff)<<16) | ((minor&0xff)<<8) | (patch&0xff))

#define PHOTOKIT_VERSION VERSION_CHK(PHOTOKIT_VERSION_MAJOR, PHOTOKIT_VERSION_MINOR, PHOTOKIT_VERSION_PATCH)

/*! Stringify \a x. */
#define _TOSTR(x)   #x
/*! Stringify \a x, perform macro expansion. */
#define TOSTR(x)  _TOSTR(x)

static const char* const k_PhotoKit_version_string = TOSTR(PHOTOKIT_VERSION_MAJOR)"."TOSTR(PHOTOKIT_VERSION_MINOR)"."TOSTR(PHOTOKIT_VERSION_PATCH)"(" __DATE__", "__TIME__")";
#define PHOTOKIT_VERSION_STR		TOSTR(PHOTOKIT_VERSION_MAJOR)"."TOSTR(PHOTOKIT_VERSION_MINOR)"."TOSTR(PHOTOKIT_VERSION_PATCH)
#define PHOTOKIT_VERSION_STR_LONG	PHOTOKIT_VERSION_STR"(" __DATE__", "__TIME__")"

/***from webkit/JavascriptCore/wtf***/
#define COMPILER(WTF_FEATURE) (defined WTF_COMPILER_##WTF_FEATURE  && WTF_COMPILER_##WTF_FEATURE)

/* COMPILER(MSVC) Microsoft Visual C++ */
/* COMPILER(MSVC7_OR_LOWER) Microsoft Visual C++ 2003 or lower*/
/* COMPILER(MSVC9_OR_LOWER) Microsoft Visual C++ 2008 or lower*/
#if defined(_MSC_VER)
#define WTF_COMPILER_MSVC 1
#if _MSC_VER < 1400
#define WTF_COMPILER_MSVC7_OR_LOWER 1
#elif _MSC_VER < 1600
#define WTF_COMPILER_MSVC9_OR_LOWER 1
#endif
#endif

/* COMPILER(GCC) - GNU Compiler Collection */
/* --gnu option of the RVCT compiler also defines __GNUC__ */
#if defined(__GNUC__) && !COMPILER(RVCT)
#define WTF_COMPILER_GCC 1
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#define GCC_VERSION_AT_LEAST(major, minor, patch) (GCC_VERSION >= (major * 10000 + minor * 100 + patch))
#else
/* Define this for !GCC compilers, just so we can write things like GCC_VERSION_AT_LEAST(4, 1, 0). */
#define GCC_VERSION_AT_LEAST(major, minor, patch) 0
#endif


/* COMPILER(RVCT)  - ARM RealView Compilation Tools */
/* COMPILER(RVCT4_OR_GREATER) - ARM RealView Compilation Tools 4.0 or greater */
#if defined(__CC_ARM) || defined(__ARMCC__)
#define WTF_COMPILER_RVCT 1
#define RVCT_VERSION_AT_LEAST(major, minor, patch, build) (__ARMCC_VERSION >= (major * 100000 + minor * 10000 + patch * 1000 + build))
#else
/* Define this for !RVCT compilers, just so we can write things like RVCT_VERSION_AT_LEAST(3, 0, 0, 0). */
#define RVCT_VERSION_AT_LEAST(major, minor, patch, build) 0
#endif

/* COMPILER(MINGW) - MinGW GCC */
/* COMPILER(MINGW64) - mingw-w64 GCC - only used as additional check to exclude mingw.org specific functions */
#if defined(__MINGW32__)
#define WTF_COMPILER_MINGW 1
#endif /* __MINGW32__ */

#ifdef __GNUC__
#    define AV_GCC_VERSION_AT_LEAST(x,y) (__GNUC__ > x || __GNUC__ == x && __GNUC_MINOR__ >= y)
#else
#    define AV_GCC_VERSION_AT_LEAST(x,y) 0
#endif


#ifndef ALWAYS_INLINE
#if COMPILER(GCC) && AV_GCC_VERSION_AT_LEAST(3,1) && !COMPILER(MINGW)
#define ALWAYS_INLINE inline __attribute__((__always_inline__))
#elif (COMPILER(MSVC) || COMPILER(RVCT))
#define ALWAYS_INLINE __forceinline
#else
#define ALWAYS_INLINE inline
#endif
#endif //ALWAYS_INLINE

#ifndef NEVER_INLINE
#if COMPILER(GCC) && AV_GCC_VERSION_AT_LEAST(3,1)
#define NEVER_INLINE __attribute__((__noinline__))
#elif COMPILER(RVCT)
#define NEVER_INLINE __declspec(noinline)
#else
#define NEVER_INLINE
#endif
#endif //NEVER_INLINE

#ifndef UNLIKELY
#if COMPILER(GCC) || (RVCT_VERSION_AT_LEAST(3, 0, 0, 0) && defined(__GNUC__))
#define UNLIKELY(x) __builtin_expect((x), 0)
#else
#define UNLIKELY(x) (x)
#endif
#endif //UNLIKELY

#ifndef LIKELY
#if COMPILER(GCC) || (RVCT_VERSION_AT_LEAST(3, 0, 0, 0) && defined(__GNUC__))
#define LIKELY(x) __builtin_expect((x), 1)
#else
#define LIKELY(x) (x)
#endif
#endif //LIKELY

#include <qglobal.h>

//e.g. qt4.7.0
#ifndef Q_UNLIKELY
#define Q_UNLIKELY(x) UNLIKELY(x)
#endif

#ifndef Q_LIKELY
#define Q_LIKELY(x) LIKELY(x)
#endif

#endif // PHOTOKIT_GLOBAL_H
