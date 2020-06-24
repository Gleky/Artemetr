#ifndef RESULTSAVER_H
#define RESULTSAVER_H

#include "struct_result.h"

#include <QObject>

class ResultSaver : public QObject
{
    Q_OBJECT
public:
    ResultSaver(QString folderName);

public slots:
    void saveResult(CellResult result);
    void savePackResultsText();
    void saveSummaryResultsText();


private:
    const QString _savePath;
    const QString _sourcePath = "../sources/";

    int _crayfishCount = 0;
    int _eggsCount = 0;
    bool _resultSaved = true;

    int _lastCrayfishCount = 0;
    int _lastEggsCount = 0;

    void saveResults(QString fileName, QStringList line, char splitter) const;

    void writeTitle();
    void writeLine(QString filePath, QStringList line, char splitter) const;
    void resetCounters();
};

#endif // RESULTSAVER_H
