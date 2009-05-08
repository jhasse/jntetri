#ifndef _MAGICK_MAGICK_CONFIG_H
#define _MAGICK_MAGICK_CONFIG_H 1
 
/* magick/magick-config.h. Generated automatically at end of configure. */
/* config/config.h.  Generated from config.h.in by configure.  */
/* config/config.h.in.  Generated from configure.ac by autoheader.  */

/* Define if you have AUTOTRACE library */
/* #undef AUTOTRACE_DELEGATE */

/* Define if coders and filters are to be built as modules. */
#ifndef MAGICKCORE_BUILD_MODULES 
#define MAGICKCORE_BUILD_MODULES  1 
#endif

/* Define if you have the bzip2 library */
/* #undef BZLIB_DELEGATE */

/* Define if you have CAIRO library */
/* #undef CAIRO_DELEGATE */

/* permit enciphering and deciphering image pixels */
/* #undef CIPHER_SUPPORT */

/* Define to 1 if the `closedir' function returns void instead of `int'. */
/* #undef CLOSEDIR_VOID */

/* Location of coder modules */
#ifndef MAGICKCORE_CODER_PATH 
#define MAGICKCORE_CODER_PATH  "c:\\Programme\\CodeBlocks\\mingw\\lib\\ImageMagick-6.4.2\\modules-Q16\\coders\\" 
#endif

/* Subdirectory of lib where coder modules are installed */
#ifndef MAGICKCORE_CODER_RELATIVE_PATH 
#define MAGICKCORE_CODER_RELATIVE_PATH  "ImageMagick-6.4.2/modules-Q16/coders" 
#endif

/* Directory where architecture-dependent configuration files live. */
#ifndef MAGICKCORE_CONFIGURE_PATH 
#define MAGICKCORE_CONFIGURE_PATH  "c:\\Programme\\CodeBlocks\\mingw\\lib\\ImageMagick-6.4.2\\config\\" 
#endif

/* Subdirectory of lib where architecture-dependent configuration files live.
   */
#ifndef MAGICKCORE_CONFIGURE_RELATIVE_PATH 
#define MAGICKCORE_CONFIGURE_RELATIVE_PATH  "ImageMagick-6.4.2/config" 
#endif

/* Define if you have DJVU library */
/* #undef DJVU_DELEGATE */

/* Directory where ImageMagick documents live. */
#ifndef MAGICKCORE_DOCUMENTATION_PATH 
#define MAGICKCORE_DOCUMENTATION_PATH  "c:\\Programme\\CodeBlocks\\mingw\\share\\doc\\ImageMagick-6.4.2\\" 
#endif

/* Define if you have Display Postscript */
/* #undef DPS_DELEGATE */

/* Build self-contained, embeddable, zero-configuration ImageMagick
   (experimental) */
/* #undef EMBEDDABLE_SUPPORT */

/* exclude deprecated methods in MagickCore API */
/* #undef EXCLUDE_DEPRECATED */

/* Directory where executables are installed. */
#ifndef MAGICKCORE_EXECUTABLE_PATH 
#define MAGICKCORE_EXECUTABLE_PATH  "c:\\Programme\\CodeBlocks\\mingw\\bin\\" 
#endif

/* Location of filter modules */
#ifndef MAGICKCORE_FILTER_PATH 
#define MAGICKCORE_FILTER_PATH  "c:\\Programme\\CodeBlocks\\mingw\\lib\\ImageMagick-6.4.2\\modules-Q16\\filters\\" 
#endif

/* Subdirectory of lib where filter modules are installed */
#ifndef MAGICKCORE_FILTER_RELATIVE_PATH 
#define MAGICKCORE_FILTER_RELATIVE_PATH  "ImageMagick-6.4.2/modules-Q16/filters" 
#endif

/* Define if you have FONTCONFIG library */
/* #undef FONTCONFIG_DELEGATE */

/* Define if you have FlashPIX library */
/* #undef FPX_DELEGATE */

/* Define if you have FreeType (TrueType font) library */
/* #undef FREETYPE_DELEGATE */

/* Define if you have Ghostscript library or framework */
/* #undef GS_DELEGATE */

/* Define if you have GVC library */
/* #undef GVC_DELEGATE */

/* Define to 1 if you have the `argz_add' function. */
/* #undef HAVE_ARGZ_ADD */

/* Define to 1 if you have the `argz_append' function. */
/* #undef HAVE_ARGZ_APPEND */

/* Define to 1 if you have the `argz_count' function. */
/* #undef HAVE_ARGZ_COUNT */

/* Define to 1 if you have the `argz_create_sep' function. */
/* #undef HAVE_ARGZ_CREATE_SEP */

/* Define to 1 if you have the <argz.h> header file. */
/* #undef HAVE_ARGZ_H */

/* Define to 1 if you have the `argz_insert' function. */
/* #undef HAVE_ARGZ_INSERT */

/* Define to 1 if you have the `argz_next' function. */
/* #undef HAVE_ARGZ_NEXT */

/* Define to 1 if you have the `argz_stringify' function. */
/* #undef HAVE_ARGZ_STRINGIFY */

/* Define to 1 if you have the <arm/limits.h> header file. */
/* #undef HAVE_ARM_LIMITS_H */

/* Define to 1 if you have the `atexit' function. */
#ifndef MAGICKCORE_HAVE_ATEXIT 
#define MAGICKCORE_HAVE_ATEXIT  1 
#endif

/* define if bool is a built-in type */
#ifndef MAGICKCORE_HAVE_BOOL 
#define MAGICKCORE_HAVE_BOOL  /**/ 
#endif

/* Define to 1 if you have the `clock' function. */
#ifndef MAGICKCORE_HAVE_CLOCK 
#define MAGICKCORE_HAVE_CLOCK  1 
#endif

/* Define to 1 if you have the `clock_gettime' function. */
/* #undef HAVE_CLOCK_GETTIME */

/* Define to 1 if you have the `closedir' function. */
#ifndef MAGICKCORE_HAVE_CLOSEDIR 
#define MAGICKCORE_HAVE_CLOSEDIR  1 
#endif

/* Define to 1 if you have the declaration of `cygwin_conv_path', and to 0 if
   you don't. */
#ifndef MAGICKCORE_HAVE_DECL_CYGWIN_CONV_PATH 
#define MAGICKCORE_HAVE_DECL_CYGWIN_CONV_PATH  0 
#endif

/* Define to 1 if you have the declaration of `pread', and to 0 if you don't.
   */
#ifndef MAGICKCORE_HAVE_DECL_PREAD 
#define MAGICKCORE_HAVE_DECL_PREAD  0 
#endif

/* Define to 1 if you have the declaration of `pwrite', and to 0 if you don't.
   */
#ifndef MAGICKCORE_HAVE_DECL_PWRITE 
#define MAGICKCORE_HAVE_DECL_PWRITE  0 
#endif

/* Define to 1 if you have the declaration of `strlcpy', and to 0 if you
   don't. */
#ifndef MAGICKCORE_HAVE_DECL_STRLCPY 
#define MAGICKCORE_HAVE_DECL_STRLCPY  0 
#endif

/* Define to 1 if you have the declaration of `vsnprintf', and to 0 if you
   don't. */
#ifndef MAGICKCORE_HAVE_DECL_VSNPRINTF 
#define MAGICKCORE_HAVE_DECL_VSNPRINTF  1 
#endif

/* Define to 1 if you have the <dirent.h> header file, and it defines `DIR'.
   */
#ifndef MAGICKCORE_HAVE_DIRENT_H 
#define MAGICKCORE_HAVE_DIRENT_H  1 
#endif

/* Define if you have the GNU dld library. */
/* #undef HAVE_DLD */

/* Define to 1 if you have the <dld.h> header file. */
/* #undef HAVE_DLD_H */

/* Define to 1 if you have the `dlerror' function. */
/* #undef HAVE_DLERROR */

/* Define to 1 if you have the <dlfcn.h> header file. */
/* #undef HAVE_DLFCN_H */

/* Define to 1 if you have the <dl.h> header file. */
/* #undef HAVE_DL_H */

/* Define to 1 if you don't have `vprintf' but do have `_doprnt.' */
/* #undef HAVE_DOPRNT */

/* Define if you have the _dyld_func_lookup function. */
/* #undef HAVE_DYLD */

/* Define to 1 if you have the <errno.h> header file. */
#ifndef MAGICKCORE_HAVE_ERRNO_H 
#define MAGICKCORE_HAVE_ERRNO_H  1 
#endif

/* Define to 1 if the system has the type `error_t'. */
/* #undef HAVE_ERROR_T */

/* Define to 1 if you have the `execvp' function. */
#ifndef MAGICKCORE_HAVE_EXECVP 
#define MAGICKCORE_HAVE_EXECVP  1 
#endif

/* Define to 1 if you have the <fcntl.h> header file. */
#ifndef MAGICKCORE_HAVE_FCNTL_H 
#define MAGICKCORE_HAVE_FCNTL_H  1 
#endif

/* Define to 1 if you have the `floor' function. */
#ifndef MAGICKCORE_HAVE_FLOOR 
#define MAGICKCORE_HAVE_FLOOR  1 
#endif

/* Define to 1 if you have the `fork' function. */
/* #undef HAVE_FORK */

/* Define to 1 if fseeko (and presumably ftello) exists and is declared. */
/* #undef HAVE_FSEEKO */

/* Define to 1 if you have the <ft2build.h> header file. */
/* #undef HAVE_FT2BUILD_H */

/* Define to 1 if you have the `ftime' function. */
#ifndef MAGICKCORE_HAVE_FTIME 
#define MAGICKCORE_HAVE_FTIME  1 
#endif

/* Define to 1 if you have the `ftruncate' function. */
#ifndef MAGICKCORE_HAVE_FTRUNCATE 
#define MAGICKCORE_HAVE_FTRUNCATE  1 
#endif

/* Define to 1 if you have the `getcwd' function. */
#ifndef MAGICKCORE_HAVE_GETCWD 
#define MAGICKCORE_HAVE_GETCWD  1 
#endif

/* Define to 1 if you have the `getdtablesize' function. */
/* #undef HAVE_GETDTABLESIZE */

/* Define to 1 if you have the `getexecname' function. */
/* #undef HAVE_GETEXECNAME */

/* Define to 1 if you have the `getpagesize' function. */
#ifndef MAGICKCORE_HAVE_GETPAGESIZE 
#define MAGICKCORE_HAVE_GETPAGESIZE  1 
#endif

/* Define to 1 if you have the `getpid' function. */
#ifndef MAGICKCORE_HAVE_GETPID 
#define MAGICKCORE_HAVE_GETPID  1 
#endif

/* Define to 1 if you have the `gettimeofday' function. */
#ifndef MAGICKCORE_HAVE_GETTIMEOFDAY 
#define MAGICKCORE_HAVE_GETTIMEOFDAY  1 
#endif

/* Define to 1 if you have the <inttypes.h> header file. */
#ifndef MAGICKCORE_HAVE_INTTYPES_H 
#define MAGICKCORE_HAVE_INTTYPES_H  1 
#endif

/* Define if you have the <lcms.h> header file. */
/* #undef HAVE_LCMS_H */

/* Define if you have the <lcms/lcms.h> header file. */
/* #undef HAVE_LCMS_LCMS_H */

/* Define if you have the libdl library or equivalent. */
/* #undef HAVE_LIBDL */

/* Define if libdlloader will be built on this platform */
#ifndef MAGICKCORE_HAVE_LIBDLLOADER 
#define MAGICKCORE_HAVE_LIBDLLOADER  1 
#endif

/* Define to 1 if you have the `gcov' library (-lgcov). */
/* #undef HAVE_LIBGCOV */

/* Define to 1 if you have the <limits.h> header file. */
#ifndef MAGICKCORE_HAVE_LIMITS_H 
#define MAGICKCORE_HAVE_LIMITS_H  1 
#endif

/* Define to 1 if you have the <locale.h> header file. */
#ifndef MAGICKCORE_HAVE_LOCALE_H 
#define MAGICKCORE_HAVE_LOCALE_H  1 
#endif

/* Define to 1 if the type `long double' works and has more range or precision
   than `double'. */
#ifndef MAGICKCORE_HAVE_LONG_DOUBLE 
#define MAGICKCORE_HAVE_LONG_DOUBLE  1 
#endif

/* Define to 1 if the type `long double' works and has more range or precision
   than `double'. */
#ifndef MAGICKCORE_HAVE_LONG_DOUBLE_WIDER 
#define MAGICKCORE_HAVE_LONG_DOUBLE_WIDER  1 
#endif

/* Define to 1 if you have the `lstat' function. */
/* #undef HAVE_LSTAT */

/* define if the compiler implements L"widestring" */
#ifndef MAGICKCORE_HAVE_LSTRING 
#define MAGICKCORE_HAVE_LSTRING  /**/ 
#endif

/* Define this if a modern libltdl is already installed */
#ifndef MAGICKCORE_HAVE_LTDL 
#define MAGICKCORE_HAVE_LTDL  1 
#endif

/* Define to 1 if you have the <machine/param.h> header file. */
/* #undef HAVE_MACHINE_PARAM_H */

/* Define to 1 if you have the <mach-o/dyld.h> header file. */
/* #undef HAVE_MACH_O_DYLD_H */

/* Define to 1 if you have the <memory.h> header file. */
#ifndef MAGICKCORE_HAVE_MEMORY_H 
#define MAGICKCORE_HAVE_MEMORY_H  1 
#endif

/* Define to 1 if you have the `memset' function. */
#ifndef MAGICKCORE_HAVE_MEMSET 
#define MAGICKCORE_HAVE_MEMSET  1 
#endif

/* Define to 1 if you have the `mkstemp' function. */
/* #undef HAVE_MKSTEMP */

/* Define to 1 if you have a working `mmap' system call. */
/* #undef HAVE_MMAP */

/* Define to 1 if you have a working `mmap' system call. */
/* #undef HAVE_MMAP_FILEIO */

/* Define to 1 if you have the `munmap' function. */
/* #undef HAVE_MUNMAP */

/* define if the compiler implements namespaces */
#ifndef MAGICKCORE_HAVE_NAMESPACES 
#define MAGICKCORE_HAVE_NAMESPACES  /**/ 
#endif

/* Define to 1 if you have the <ndir.h> header file, and it defines `DIR'. */
/* #undef HAVE_NDIR_H */

/* Define to 1 if you have the `opendir' function. */
#ifndef MAGICKCORE_HAVE_OPENDIR 
#define MAGICKCORE_HAVE_OPENDIR  1 
#endif

/* Define to 1 if you have the `pclose' function. */
#ifndef MAGICKCORE_HAVE_PCLOSE 
#define MAGICKCORE_HAVE_PCLOSE  1 
#endif

/* Define to 1 if you have the `poll' function. */
/* #undef HAVE_POLL */

/* Define to 1 if you have the `popen' function. */
#ifndef MAGICKCORE_HAVE_POPEN 
#define MAGICKCORE_HAVE_POPEN  1 
#endif

/* Define to 1 if you have the `posix_fallocate' function. */
/* #undef HAVE_POSIX_FALLOCATE */

/* Define to 1 if you have the `pow' function. */
#ifndef MAGICKCORE_HAVE_POW 
#define MAGICKCORE_HAVE_POW  1 
#endif

/* Define to 1 if you have the `pread' function. */
/* #undef HAVE_PREAD */

/* Define if libtool can extract symbol lists from object files. */
#ifndef MAGICKCORE_HAVE_PRELOADED_SYMBOLS 
#define MAGICKCORE_HAVE_PRELOADED_SYMBOLS  1 
#endif

/* Define if you have POSIX threads libraries and header files. */
/* #undef HAVE_PTHREAD */

/* Define to 1 if you have the `pwrite' function. */
/* #undef HAVE_PWRITE */

/* Define to 1 if you have the `raise' function. */
#ifndef MAGICKCORE_HAVE_RAISE 
#define MAGICKCORE_HAVE_RAISE  1 
#endif

/* Define to 1 if you have the `rand_r' function. */
/* #undef HAVE_RAND_R */

/* Define to 1 if you have the `readdir' function. */
#ifndef MAGICKCORE_HAVE_READDIR 
#define MAGICKCORE_HAVE_READDIR  1 
#endif

/* Define to 1 if you have the `readlink' function. */
/* #undef HAVE_READLINK */

/* Define to 1 if you have the `realpath' function. */
/* #undef HAVE_REALPATH */

/* Define to 1 if you have the `seekdir' function. */
#ifndef MAGICKCORE_HAVE_SEEKDIR 
#define MAGICKCORE_HAVE_SEEKDIR  1 
#endif

/* Define to 1 if you have the `select' function. */
/* #undef HAVE_SELECT */

/* Define to 1 if you have the `setlocale' function. */
#ifndef MAGICKCORE_HAVE_SETLOCALE 
#define MAGICKCORE_HAVE_SETLOCALE  1 
#endif

/* Define to 1 if you have the `setvbuf' function. */
#ifndef MAGICKCORE_HAVE_SETVBUF 
#define MAGICKCORE_HAVE_SETVBUF  1 
#endif

/* X11 server supports shape extension */
/* #undef HAVE_SHAPE */

/* X11 server supports shared memory extension */
/* #undef HAVE_SHARED_MEMORY */

/* Define if you have the shl_load function. */
/* #undef HAVE_SHL_LOAD */

/* Define to 1 if you have the `sigaction' function. */
/* #undef HAVE_SIGACTION */

/* Define to 1 if you have the `sigemptyset' function. */
/* #undef HAVE_SIGEMPTYSET */

/* Define to 1 if you have the `sqrt' function. */
#ifndef MAGICKCORE_HAVE_SQRT 
#define MAGICKCORE_HAVE_SQRT  1 
#endif

/* Define to 1 if you have the `stat' function. */
#ifndef MAGICKCORE_HAVE_STAT 
#define MAGICKCORE_HAVE_STAT  1 
#endif

/* Define to 1 if you have the <stdarg.h> header file. */
#ifndef MAGICKCORE_HAVE_STDARG_H 
#define MAGICKCORE_HAVE_STDARG_H  1 
#endif

/* Define to 1 if stdbool.h conforms to C99. */
#ifndef MAGICKCORE_HAVE_STDBOOL_H 
#define MAGICKCORE_HAVE_STDBOOL_H  1 
#endif

/* Define to 1 if you have the <stdint.h> header file. */
#ifndef MAGICKCORE_HAVE_STDINT_H 
#define MAGICKCORE_HAVE_STDINT_H  1 
#endif

/* Define to 1 if you have the <stdlib.h> header file. */
#ifndef MAGICKCORE_HAVE_STDLIB_H 
#define MAGICKCORE_HAVE_STDLIB_H  1 
#endif

/* define if the compiler supports ISO C++ standard library */
#ifndef MAGICKCORE_HAVE_STD_LIBS 
#define MAGICKCORE_HAVE_STD_LIBS  /**/ 
#endif

/* define if the compiler supports the std namespace */
#ifndef MAGICKCORE_HAVE_STD_NAMESPACE 
#define MAGICKCORE_HAVE_STD_NAMESPACE  /**/ 
#endif

/* Define to 1 if you have the `strcasecmp' function. */
#ifndef MAGICKCORE_HAVE_STRCASECMP 
#define MAGICKCORE_HAVE_STRCASECMP  1 
#endif

/* Define to 1 if you have the `strcspn' function. */
#ifndef MAGICKCORE_HAVE_STRCSPN 
#define MAGICKCORE_HAVE_STRCSPN  1 
#endif

/* Define to 1 if you have the `strdup' function. */
#ifndef MAGICKCORE_HAVE_STRDUP 
#define MAGICKCORE_HAVE_STRDUP  1 
#endif

/* Define to 1 if you have the `strerror' function. */
#ifndef MAGICKCORE_HAVE_STRERROR 
#define MAGICKCORE_HAVE_STRERROR  1 
#endif

/* Define to 1 if cpp supports the ANSI # stringizing operator. */
#ifndef MAGICKCORE_HAVE_STRINGIZE 
#define MAGICKCORE_HAVE_STRINGIZE  1 
#endif

/* Define to 1 if you have the <strings.h> header file. */
#ifndef MAGICKCORE_HAVE_STRINGS_H 
#define MAGICKCORE_HAVE_STRINGS_H  1 
#endif

/* Define to 1 if you have the <string.h> header file. */
#ifndef MAGICKCORE_HAVE_STRING_H 
#define MAGICKCORE_HAVE_STRING_H  1 
#endif

/* Define to 1 if you have the `strlcat' function. */
/* #undef HAVE_STRLCAT */

/* Define to 1 if you have the `strlcpy' function. */
/* #undef HAVE_STRLCPY */

/* Define to 1 if you have the `strncasecmp' function. */
#ifndef MAGICKCORE_HAVE_STRNCASECMP 
#define MAGICKCORE_HAVE_STRNCASECMP  1 
#endif

/* Define to 1 if you have the `strpbrk' function. */
#ifndef MAGICKCORE_HAVE_STRPBRK 
#define MAGICKCORE_HAVE_STRPBRK  1 
#endif

/* Define to 1 if you have the `strspn' function. */
#ifndef MAGICKCORE_HAVE_STRSPN 
#define MAGICKCORE_HAVE_STRSPN  1 
#endif

/* Define to 1 if you have the `strstr' function. */
#ifndef MAGICKCORE_HAVE_STRSTR 
#define MAGICKCORE_HAVE_STRSTR  1 
#endif

/* Define to 1 if you have the `strtol' function. */
#ifndef MAGICKCORE_HAVE_STRTOL 
#define MAGICKCORE_HAVE_STRTOL  1 
#endif

/* Define to 1 if you have the `symlink' function. */
/* #undef HAVE_SYMLINK */

/* Define to 1 if you have the `sysconf' function. */
/* #undef HAVE_SYSCONF */

/* Define to 1 if you have the <sys/dir.h> header file, and it defines `DIR'.
   */
/* #undef HAVE_SYS_DIR_H */

/* Define to 1 if you have the <sys/dl.h> header file. */
/* #undef HAVE_SYS_DL_H */

/* Define to 1 if you have the <sys/ipc.h> header file. */
/* #undef HAVE_SYS_IPC_H */

/* Define to 1 if you have the <sys/ndir.h> header file, and it defines `DIR'.
   */
/* #undef HAVE_SYS_NDIR_H */

/* Define to 1 if you have the <sys/resource.h> header file. */
/* #undef HAVE_SYS_RESOURCE_H */

/* Define to 1 if you have the <sys/select.h> header file. */
/* #undef HAVE_SYS_SELECT_H */

/* Define to 1 if you have the <sys/socket.h> header file. */
/* #undef HAVE_SYS_SOCKET_H */

/* Define to 1 if you have the <sys/stat.h> header file. */
#ifndef MAGICKCORE_HAVE_SYS_STAT_H 
#define MAGICKCORE_HAVE_SYS_STAT_H  1 
#endif

/* Define to 1 if you have the <sys/syslimits.h> header file. */
/* #undef HAVE_SYS_SYSLIMITS_H */

/* Define to 1 if you have the <sys/timeb.h> header file. */
#ifndef MAGICKCORE_HAVE_SYS_TIMEB_H 
#define MAGICKCORE_HAVE_SYS_TIMEB_H  1 
#endif

/* Define to 1 if you have the <sys/times.h> header file. */
/* #undef HAVE_SYS_TIMES_H */

/* Define to 1 if you have the <sys/time.h> header file. */
#ifndef MAGICKCORE_HAVE_SYS_TIME_H 
#define MAGICKCORE_HAVE_SYS_TIME_H  1 
#endif

/* Define to 1 if you have the <sys/types.h> header file. */
#ifndef MAGICKCORE_HAVE_SYS_TYPES_H 
#define MAGICKCORE_HAVE_SYS_TYPES_H  1 
#endif

/* Define to 1 if you have the <sys/wait.h> header file. */
/* #undef HAVE_SYS_WAIT_H */

/* Define to 1 if you have the `telldir' function. */
#ifndef MAGICKCORE_HAVE_TELLDIR 
#define MAGICKCORE_HAVE_TELLDIR  1 
#endif

/* Define to 1 if you have the `tempnam' function. */
#ifndef MAGICKCORE_HAVE_TEMPNAM 
#define MAGICKCORE_HAVE_TEMPNAM  1 
#endif

/* Define to 1 if you have the <tiffconf.h> header file. */
/* #undef HAVE_TIFFCONF_H */

/* Define to 1 if you have the `TIFFIsCODECConfigured' function. */
/* #undef HAVE_TIFFISCODECCONFIGURED */

/* Define to 1 if you have the `TIFFMergeFieldInfo' function. */
/* #undef HAVE_TIFFMERGEFIELDINFO */

/* Define to 1 if you have the `TIFFReadEXIFDirectory' function. */
/* #undef HAVE_TIFFREADEXIFDIRECTORY */

/* Define to 1 if you have the `TIFFSetErrorHandlerExt' function. */
/* #undef HAVE_TIFFSETERRORHANDLEREXT */

/* Define to 1 if you have the `TIFFSetTagExtender' function. */
/* #undef HAVE_TIFFSETTAGEXTENDER */

/* Define to 1 if you have the `TIFFSetWarningHandlerExt' function. */
/* #undef HAVE_TIFFSETWARNINGHANDLEREXT */

/* Define to 1 if you have the `TIFFSwabArrayOfTriples' function. */
/* #undef HAVE_TIFFSWABARRAYOFTRIPLES */

/* Define to 1 if you have the `times' function. */
/* #undef HAVE_TIMES */

/* Define to 1 if you have the <unistd.h> header file. */
#ifndef MAGICKCORE_HAVE_UNISTD_H 
#define MAGICKCORE_HAVE_UNISTD_H  1 
#endif

/* Define to 1 if you have the `usleep' function. */
/* #undef HAVE_USLEEP */

/* Define to 1 if you have the `vprintf' function. */
#ifndef MAGICKCORE_HAVE_VPRINTF 
#define MAGICKCORE_HAVE_VPRINTF  1 
#endif

/* Define to 1 if you have the `vsnprintf' function. */
#ifndef MAGICKCORE_HAVE_VSNPRINTF 
#define MAGICKCORE_HAVE_VSNPRINTF  1 
#endif

/* Define to 1 if you have the `vsprintf' function. */
#ifndef MAGICKCORE_HAVE_VSPRINTF 
#define MAGICKCORE_HAVE_VSPRINTF  1 
#endif

/* Define to 1 if you have the `waitpid' function. */
/* #undef HAVE_WAITPID */

/* This value is set to 1 to indicate that the system argz facility works */
/* #undef HAVE_WORKING_ARGZ */

/* Define to 1 if the system has the type `_Bool'. */
#ifndef MAGICKCORE_HAVE__BOOL 
#define MAGICKCORE_HAVE__BOOL  1 
#endif

/* Define to 1 if you have the `_exit' function. */
#ifndef MAGICKCORE_HAVE__EXIT 
#define MAGICKCORE_HAVE__EXIT  1 
#endif

/* Define to 1 if you have the `_NSGetExecutablePath' function. */
/* #undef HAVE__NSGETEXECUTABLEPATH */

/* Define to 1 if you have the `_pclose' function. */
#ifndef MAGICKCORE_HAVE__PCLOSE 
#define MAGICKCORE_HAVE__PCLOSE  1 
#endif

/* Define to 1 if you have the `_popen' function. */
#ifndef MAGICKCORE_HAVE__POPEN 
#define MAGICKCORE_HAVE__POPEN  1 
#endif

/* Define to 1 if you have the `_wfopen' function. */
#ifndef MAGICKCORE_HAVE__WFOPEN 
#define MAGICKCORE_HAVE__WFOPEN  1 
#endif

/* accurately represent the wide range of intensity levels in real scenes */
/* #undef HDRI_SUPPORT */

/* Define if you have umem memory allocation library */
/* #undef HasUMEM */

/* ImageMagick is formally installed under prefix */
#ifndef MAGICKCORE_INSTALLED_SUPPORT 
#define MAGICKCORE_INSTALLED_SUPPORT  1 
#endif

/* Define if you have JBIG library */
/* #undef JBIG_DELEGATE */

/* Define if you have JPEG version 2 "Jasper" library */
/* #undef JP2_DELEGATE */

/* Define if you have JPEG library */
/* #undef JPEG_DELEGATE */

/* Define if you have LCMS library */
/* #undef LCMS_DELEGATE */

/* Directory where architecture-dependent files live. */
#ifndef MAGICKCORE_LIBRARY_PATH 
#define MAGICKCORE_LIBRARY_PATH  "c:\\Programme\\CodeBlocks\\mingw\\lib\\ImageMagick-6.4.2\\" 
#endif

/* Subdirectory of lib where ImageMagick architecture dependent files are
   installed */
#ifndef MAGICKCORE_LIBRARY_RELATIVE_PATH 
#define MAGICKCORE_LIBRARY_RELATIVE_PATH  "ImageMagick-6.4.2" 
#endif

/* Define if you have LQR library */
/* #undef LQR_DELEGATE */

/* Define if using libltdl to support dynamically loadable modules */
#ifndef MAGICKCORE_LTDL_DELEGATE 
#define MAGICKCORE_LTDL_DELEGATE  1 
#endif

/* Define if the OS needs help to load dependent libraries for dlopen(). */
#ifndef MAGICKCORE_LTDL_DLOPEN_DEPLIBS 
#define MAGICKCORE_LTDL_DLOPEN_DEPLIBS  1 
#endif

/* Define if dlopen(NULL) is able to resolve symbols from the main program. */
/* #undef LTDL_DLOPEN_SELF_WORKS */

/* Define to the system default library search path. */
#ifndef MAGICKCORE_LT_DLSEARCH_PATH 
#define MAGICKCORE_LT_DLSEARCH_PATH  "/lib:/usr/lib" 
#endif

/* Define to the extension used for runtime loadable modules, say, ".so". */
#ifndef MAGICKCORE_LT_MODULE_EXT 
#define MAGICKCORE_LT_MODULE_EXT  ".dll" 
#endif

/* Define to the name of the environment variable that determines the run-time
   module search path. */
#ifndef MAGICKCORE_LT_MODULE_PATH_VAR 
#define MAGICKCORE_LT_MODULE_PATH_VAR  "PATH" 
#endif

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#ifndef MAGICKCORE_LT_OBJDIR 
#define MAGICKCORE_LT_OBJDIR  ".libs/" 
#endif

/* Define to prepend to default font search path. */
/* #undef MAGICK_FONT_PATH */

/* Magick API method prefix */
/* #undef NAMESPACE_PREFIX */

/* Turn off assert statements */
/* #undef NDEBUG */

/* Define if dlsym() requires a leading underscore in symbol names. */
/* #undef NEED_USCORE */

/* Define to 1 if your C compiler doesn't accept -c and -o together. */
/* #undef NO_MINUS_C_MINUS_O */

/* Define if you have OPENEXR library */
/* #undef OPENEXR_DELEGATE */

/* Name of package */
#ifndef MAGICKCORE_PACKAGE 
#define MAGICKCORE_PACKAGE  "ImageMagick" 
#endif

/* Define to the address where bug reports for this package should be sent. */
#ifndef MAGICKCORE_PACKAGE_BUGREPORT 
#define MAGICKCORE_PACKAGE_BUGREPORT  "http://www.imagemagick.org" 
#endif

/* Define to the full name of this package. */
#ifndef MAGICKCORE_PACKAGE_NAME 
#define MAGICKCORE_PACKAGE_NAME  "ImageMagick" 
#endif

/* Define to the full name and version of this package. */
#ifndef MAGICKCORE_PACKAGE_STRING 
#define MAGICKCORE_PACKAGE_STRING  "ImageMagick 6.4.2" 
#endif

/* Define to the one symbol short name of this package. */
#ifndef MAGICKCORE_PACKAGE_TARNAME 
#define MAGICKCORE_PACKAGE_TARNAME  "ImageMagick" 
#endif

/* Define to the version of this package. */
#ifndef MAGICKCORE_PACKAGE_VERSION 
#define MAGICKCORE_PACKAGE_VERSION  "6.4.2" 
#endif

/* Define if you have PNG library */
#ifndef MAGICKCORE_PNG_DELEGATE 
#define MAGICKCORE_PNG_DELEGATE  1 
#endif

/* Define to necessary symbol if this constant uses a non-standard name on
   your system. */
/* #undef PTHREAD_CREATE_JOINABLE */

/* Pixel cache threshold in MB (defaults to available memory) */
/* #undef PixelCacheThreshold */

/* Number of bits in a pixel Quantum (8/16/32/64) */
#ifndef MAGICKCORE_QUANTUM_DEPTH 
#define MAGICKCORE_QUANTUM_DEPTH  16 
#endif

/* Define as the return type of signal handlers (`int' or `void'). */
#ifndef MAGICKCORE_RETSIGTYPE 
#define MAGICKCORE_RETSIGTYPE  void 
#endif

/* Define if you have RSVG library */
/* #undef RSVG_DELEGATE */

/* Define to the type of arg 1 for `select'. */
#ifndef MAGICKCORE_SELECT_TYPE_ARG1 
#define MAGICKCORE_SELECT_TYPE_ARG1  int 
#endif

/* Define to the type of args 2, 3 and 4 for `select'. */
#ifndef MAGICKCORE_SELECT_TYPE_ARG234 
#define MAGICKCORE_SELECT_TYPE_ARG234  (int *) 
#endif

/* Define to the type of arg 5 for `select'. */
#ifndef MAGICKCORE_SELECT_TYPE_ARG5 
#define MAGICKCORE_SELECT_TYPE_ARG5  (struct timeval *) 
#endif

/* Directory where architecture-independent configuration files live. */
#ifndef MAGICKCORE_SHARE_CONFIGURE_PATH 
#define MAGICKCORE_SHARE_CONFIGURE_PATH  "c:\\Programme\\CodeBlocks\\mingw\\share\\ImageMagick-6.4.2\\config\\" 
#endif

/* Subdirectory of lib where architecture-independent configuration files
   live. */
#ifndef MAGICKCORE_SHARE_CONFIGURE_RELATIVE_PATH 
#define MAGICKCORE_SHARE_CONFIGURE_RELATIVE_PATH  "ImageMagick-6.4.2/config" 
#endif

/* Directory where architecture-independent files live. */
#ifndef MAGICKCORE_SHARE_PATH 
#define MAGICKCORE_SHARE_PATH  "c:\\Programme\\CodeBlocks\\mingw\\share\\ImageMagick-6.4.2\\" 
#endif

/* The size of `off_t', as computed by sizeof. */
#ifndef MAGICKCORE_SIZEOF_OFF_T 
#define MAGICKCORE_SIZEOF_OFF_T  4 
#endif

/* The size of `signed int', as computed by sizeof. */
#ifndef MAGICKCORE_SIZEOF_SIGNED_INT 
#define MAGICKCORE_SIZEOF_SIGNED_INT  4 
#endif

/* The size of `signed long', as computed by sizeof. */
#ifndef MAGICKCORE_SIZEOF_SIGNED_LONG 
#define MAGICKCORE_SIZEOF_SIGNED_LONG  4 
#endif

/* The size of `signed long long', as computed by sizeof. */
#ifndef MAGICKCORE_SIZEOF_SIGNED_LONG_LONG 
#define MAGICKCORE_SIZEOF_SIGNED_LONG_LONG  8 
#endif

/* The size of `signed short', as computed by sizeof. */
#ifndef MAGICKCORE_SIZEOF_SIGNED_SHORT 
#define MAGICKCORE_SIZEOF_SIGNED_SHORT  2 
#endif

/* The size of `size_t', as computed by sizeof. */
#ifndef MAGICKCORE_SIZEOF_SIZE_T 
#define MAGICKCORE_SIZEOF_SIZE_T  4 
#endif

/* The size of `unsigned int', as computed by sizeof. */
#ifndef MAGICKCORE_SIZEOF_UNSIGNED_INT 
#define MAGICKCORE_SIZEOF_UNSIGNED_INT  4 
#endif

/* The size of `unsigned int*', as computed by sizeof. */
#ifndef MAGICKCORE_SIZEOF_UNSIGNED_INTP 
#define MAGICKCORE_SIZEOF_UNSIGNED_INTP  4 
#endif

/* The size of `unsigned long', as computed by sizeof. */
#ifndef MAGICKCORE_SIZEOF_UNSIGNED_LONG 
#define MAGICKCORE_SIZEOF_UNSIGNED_LONG  4 
#endif

/* The size of `unsigned long long', as computed by sizeof. */
#ifndef MAGICKCORE_SIZEOF_UNSIGNED_LONG_LONG 
#define MAGICKCORE_SIZEOF_UNSIGNED_LONG_LONG  8 
#endif

/* The size of `unsigned short', as computed by sizeof. */
#ifndef MAGICKCORE_SIZEOF_UNSIGNED_SHORT 
#define MAGICKCORE_SIZEOF_UNSIGNED_SHORT  2 
#endif

/* Define to 1 if the `S_IS*' macros in <sys/stat.h> do not work properly. */
/* #undef STAT_MACROS_BROKEN */

/* Define to 1 if you have the ANSI C header files. */
#ifndef MAGICKCORE_STDC_HEADERS 
#define MAGICKCORE_STDC_HEADERS  1 
#endif

/* Define if you have TIFF library */
/* #undef TIFF_DELEGATE */

/* Define to 1 if you can safely include both <sys/time.h> and <time.h>. */
#ifndef MAGICKCORE_TIME_WITH_SYS_TIME 
#define MAGICKCORE_TIME_WITH_SYS_TIME  1 
#endif

/* Define to 1 if your <sys/time.h> declares `struct tm'. */
/* #undef TM_IN_SYS_TIME */

/* Version number of package */
#ifndef MAGICKCORE_VERSION 
#define MAGICKCORE_VERSION  "6.4.2" 
#endif

/* Define to use the Windows GDI32 library */
#ifndef MAGICKCORE_WINGDI32_DELEGATE 
#define MAGICKCORE_WINGDI32_DELEGATE  1 
#endif

/* Define if using the dmalloc debugging malloc package */
/* #undef WITH_DMALLOC */

/* Define if you have wmflite library */
/* #undef WMFLITE_DELEGATE */

/* Define if you have wmf library */
/* #undef WMF_DELEGATE */

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel and VAX). */
#if defined __BIG_ENDIAN__
# define WORDS_BIGENDIAN 1
#elif ! defined __LITTLE_ENDIAN__
/* # undef WORDS_BIGENDIAN */
#endif

/* Location of X11 configure files */
#ifndef MAGICKCORE_X11_CONFIGURE_PATH 
#define MAGICKCORE_X11_CONFIGURE_PATH  "/home/jhasse/ImageMagick-6.4.2/winpath.sh: path "=" is not mounted" 
#endif

/* Define if you have X11 library */
/* #undef X11_DELEGATE */

/* Define if you have XML library */
/* #undef XML_DELEGATE */

/* Define to 1 if the X Window System is missing or not being used. */
#ifndef MAGICKCORE_X_DISPLAY_MISSING 
#define MAGICKCORE_X_DISPLAY_MISSING  1 
#endif

/* Define if you have zlib compression library */
#ifndef MAGICKCORE_ZLIB_DELEGATE 
#define MAGICKCORE_ZLIB_DELEGATE  1 
#endif

/* Number of bits in a file offset, on hosts where this is settable. */
/* #undef _FILE_OFFSET_BITS */

/* Define to 1 to make fseeko visible on some hosts (e.g. glibc 2.2). */
/* #undef _LARGEFILE_SOURCE */

/* Define for large files, on AIX-style hosts. */
/* #undef _LARGE_FILES */

/* Define to 1 if on MINIX. */
/* #undef _MINIX */

/* Define to 2 if the system does not provide POSIX.1 features except with
   this defined. */
/* #undef _POSIX_1_SOURCE */

/* Define to 1 if you need to in order for `stat' and other things to work. */
/* #undef _POSIX_SOURCE */

/* Define to 1 if type `char' is unsigned and you are not using gcc.  */
#ifndef __CHAR_UNSIGNED__
/* # undef __CHAR_UNSIGNED__ */
#endif

/* Enable extensions on AIX 3, Interix.  */
#ifndef _ALL_SOURCE
# define _ALL_SOURCE 1
#endif
/* Enable GNU extensions on systems that have them.  */
#ifndef _GNU_SOURCE
# define _GNU_SOURCE 1
#endif
/* Enable threading extensions on Solaris.  */
#ifndef _POSIX_PTHREAD_SEMANTICS
# define _POSIX_PTHREAD_SEMANTICS 1
#endif
/* Enable extensions on HP NonStop.  */
#ifndef _TANDEM_SOURCE
# define _TANDEM_SOURCE 1
#endif
/* Enable general extensions on Solaris.  */
#ifndef __EXTENSIONS__
# define __EXTENSIONS__ 1
#endif


/* Define so that glibc/gnulib argp.h does not typedef error_t. */
#ifndef MAGICKCORE___error_t_defined 
#define MAGICKCORE___error_t_defined  1 
#endif

/* Define to appropriate substitue if compiler does not have __func__ */
/* #undef __func__ */

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* Define to a type to use for `error_t' if it is not otherwise available. */
#ifndef _magickcore_error_t 
#define _magickcore_error_t  int 
#endif

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
/* #undef inline */
#endif

/* Define to `int' if <sys/types.h> does not define. */
/* #undef mode_t */

/* Define to `long int' if <sys/types.h> does not define. */
/* #undef off_t */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef pid_t */

/* Define to the equivalent of the C99 'restrict' keyword, or to
   nothing if this is not supported.  Do not define if restrict is
   supported directly.  */
#ifndef _magickcore_restrict 
#define _magickcore_restrict  __restrict 
#endif
/* Work around a bug in Sun C++: it does not support _Restrict, even
   though the corresponding Sun C compiler does, which causes
   "#define restrict _Restrict" in the previous line.  Perhaps some future
   version of Sun C++ will work with _Restrict; if so, it'll probably
   define __RESTRICT, just as Sun C does.  */
#if defined __SUNPRO_CC && !defined __RESTRICT
# define _Restrict
#endif

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef ssize_t */

/* Define to empty if the keyword `volatile' does not work. Warning: valid
   code using `volatile' can become incorrect without. Disable with care. */
/* #undef volatile */
 
/* once: _MAGICK_MAGICK_CONFIG_H */
#endif