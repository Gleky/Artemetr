#ifndef STRUCT_RESULT_H
#define STRUCT_RESULT_H

#include <QImage>

struct CellResult
{
    int crayfishCount = -1;
    int eggsCount = -1;
    QImage result;
    QImage source;
};

Q_DECLARE_METATYPE(CellResult)

#endif // STRUCT_RESULT_H
