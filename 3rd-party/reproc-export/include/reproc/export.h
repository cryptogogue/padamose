
#ifndef REPROC_EXPORT_H
#define REPROC_EXPORT_H

#ifdef REPROC_STATIC_DEFINE
#  define REPROC_EXPORT
#  define REPROC_NO_EXPORT
#else
#  ifndef REPROC_EXPORT
#    ifdef reproc_EXPORTS
        /* We are building this library */
#      define REPROC_EXPORT 
#    else
        /* We are using this library */
#      define REPROC_EXPORT 
#    endif
#  endif

#  ifndef REPROC_NO_EXPORT
#    define REPROC_NO_EXPORT 
#  endif
#endif

#ifndef REPROC_DEPRECATED
#  define REPROC_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef REPROC_DEPRECATED_EXPORT
#  define REPROC_DEPRECATED_EXPORT REPROC_EXPORT REPROC_DEPRECATED
#endif

#ifndef REPROC_DEPRECATED_NO_EXPORT
#  define REPROC_DEPRECATED_NO_EXPORT REPROC_NO_EXPORT REPROC_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef REPROC_NO_DEPRECATED
#    define REPROC_NO_DEPRECATED
#  endif
#endif

#endif /* REPROC_EXPORT_H */
