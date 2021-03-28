#include "resultsaver.h"

#include <QDir>
#include <QDateTime>

ResultSaver::ResultSaver(const QString &folderName)
    :_savePath("../" + folderName + '/')
{
    QDir saveDir(_savePath);
    if ( !saveDir.exists() )
        saveDir.mkpath(_savePath);

    saveDir.mkdir(_subdirOriginal);
    saveDir.mkdir(_subdirAfterIodine);
    saveDir.mkdir(_subdirAfterChlorine);
}

void ResultSaver::saveResult(CellImage result)
{
//#ifndef TEST
    auto fileName = QDateTime::currentDateTime().toString("yyyy.MM.dd hh-mm-ss");
    QString subdir;
    if ( result.photoType == Original ) subdir = _subdirOriginal;
    else if ( result.photoType == AfterIodine ) subdir = _subdirAfterIodine;
    else subdir = _subdirAfterChlorine;
    subdir += '/';

    result.img.save(_savePath+subdir+fileName+".jpg", "JPEG", 100);
//#endif
}

void ResultSaver::savePackResultsText()
{

}

void ResultSaver::saveSummaryResultsText()
{

}
