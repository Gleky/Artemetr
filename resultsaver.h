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
    void setDone();

private:
    QString _savePath;
    QString _sourcePath = "../sources/";

    int _crayfishCount = 0;
    int _eggsCount = 0;
    bool _lastResultSaved = true;
};

#endif // RESULTSAVER_H
