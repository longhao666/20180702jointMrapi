#ifndef MOVE_GLOBAL_H
#define MOVE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MOVE_LIBRARY)
#  define MOVESHARED_EXPORT Q_DECL_EXPORT
#else
#  define MOVESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MOVE_GLOBAL_H
