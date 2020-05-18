#ifndef RESULTSAVER_H
#define RESULTSAVER_H

#include "struct_result.h"

#include <QObject>

class ResultSaver : public QObject
{
    Q_OBJECT
public:
    ResultSaver();
    void setFolder(QString folderName);

public slots:
    void saveResult(Result result);

private:
    QString _savePath;
    QString _sourcePath = "../sources/";
};

#endif // RESULTSAVER_H
