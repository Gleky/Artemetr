#ifndef RESULTSAVER_H
#define RESULTSAVER_H

#include "struct_result.h"

#include <QObject>

class ResultSaver : public QObject
{
    Q_OBJECT
public:
    explicit ResultSaver(const QString &folderName);

public slots:
    void saveResult(CellImage result);
    void savePackResultsText();
    void saveSummaryResultsText();

private:
    const QString _savePath;
    const QString _subdirOriginal = "original";
    const QString _subdirAfterIodine = "after_iodine";
    const QString _subdirAfterChlorine = "after_chlorine";
};

#endif // RESULTSAVER_H
