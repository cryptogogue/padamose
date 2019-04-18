
#ifndef REPROCXX_EXPORT_H
#define REPROCXX_EXPORT_H

#ifdef REPROCXX_STATIC_DEFINE
#  define REPROCXX_EXPORT
#  define REPROCXX_NO_EXPORT
#else
#  ifndef REPROCXX_EXPORT
#    ifdef reproc___EXPORTS
        /* We are building this library */
#      define REPROCXX_EXPORT 
#    else
        /* We are using this library */
#      define REPROCXX_EXPORT 
#    endif
#  endif

#  ifndef REPROCXX_NO_EXPORT
#    define REPROCXX_NO_EXPORT 
#  endif
#endif

#ifndef REPROCXX_DEPRECATED
#  define REPROCXX_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef REPROCXX_DEPRECATED_EXPORT
#  define REPROCXX_DEPRECATED_EXPORT REPROCXX_EXPORT REPROCXX_DEPRECATED
#endif

#ifndef REPROCXX_DEPRECATED_NO_EXPORT
#  define REPROCXX_DEPRECATED_NO_EXPORT REPROCXX_NO_EXPORT REPROCXX_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef REPROCXX_NO_DEPRECATED
#    define REPROCXX_NO_DEPRECATED
#  endif
#endif

#endif /* REPROCXX_EXPORT_H */
