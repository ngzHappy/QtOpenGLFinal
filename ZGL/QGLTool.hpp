#if !defined(__HPP__QGLTOOL__HPP__)
#define __HPP__QGLTOOL__HPP__

#include "glew.h"
#include <cstddef>
#include <cstdlib>
#include <utility>
#include <memory>
#include <QString>
#include <QList>

class QGLWidget;
extern bool qGLWidgetInitializeGlew(QGLWidget * const ptr);
extern QString readGLSLFile( const QString & fileName,const QList<QString> & filePath );

#endif


