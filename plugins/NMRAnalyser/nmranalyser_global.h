#ifndef NMRANALYSER_GLOBAL_H
#define NMRANALYSER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(NMRANALYSER_LIBRARY)
#  define NMRANALYSERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define NMRANALYSERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // NMRANALYSER_GLOBAL_H
