#ifndef BOTTOM_GLOBAL_H
#define BOTTOM_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(BOTTOM_LIBRARY)
#  define BOTTOMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define BOTTOMSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // BOTTOM_GLOBAL_H
