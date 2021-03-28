#ifndef STRUCT_RESULT_H
#define STRUCT_RESULT_H

#include <QImage>

enum PhotoType {Original, AfterIodine, AfterChlorine};

struct CellImage
{
    int photoType = 0;
    QImage img;
};

Q_DECLARE_METATYPE(CellImage)

#endif // STRUCT_RESULT_H
