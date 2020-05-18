#include "resultsaver.h"

#include <QDir>
#include <QDateTime>

ResultSaver::ResultSaver()
{

}

void ResultSaver::setFolder(QString folderName)
{
    _savePath = "../" + folderName + '/';

    QDir saveDir(_savePath);
    if ( !saveDir.exists() )
        saveDir.mkpath(_savePath);

    QDir scourceDir(_sourcePath);
    if ( !scourceDir.exists() )
        scourceDir.mkpath(_sourcePath);
}

void ResultSaver::saveResult(Result result)
{
    auto fileName = QDateTime::currentDateTime().toString("dd.MM.yyyy hh-mm-ss");

    result.result.save(_savePath+fileName+".jpg", "JPEG", 100);

    result.source.save(_sourcePath+fileName+" source.jpg", "JPEG", 100);
}
