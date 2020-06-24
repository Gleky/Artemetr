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

#endif // STRUCT_RESULT_H
