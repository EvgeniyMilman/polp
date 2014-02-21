#ifndef TRIPLETDNP_GLOBAL_H
#define TRIPLETDNP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TRIPLETDNP_LIBRARY)
#  define TRIPLETDNPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define TRIPLETDNPSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // TRIPLETDNP_GLOBAL_H
