#ifndef CSVFILEFORMAT_GLOBAL_H
#define CSVFILEFORMAT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CSVFILEFORMAT_LIBRARY)
#  define CSVFILEFORMATSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CSVFILEFORMATSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CSVFILEFORMAT_GLOBAL_H
