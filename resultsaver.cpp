#include "resultsaver.h"

#include <QDir>
#include <QDateTime>

ResultSaver::ResultSaver()
{

}

void ResultSaver::setFolder(QString folderName)
{
    _savePath = folderName + '/';

    QDir saveDir(_savePath);
    if ( !saveDir.exists() )
        saveDir.mkpath(_savePath);

    if ( !saveDir.exists(_sourceSubfolder) )
        saveDir.mkdir(_sourceSubfolder);
}

void ResultSaver::saveResult(Result result)
{
    auto fileName = QDateTime::currentDateTime().toString("dd.MM.yyyy hh-mm-ss");

    result.result.save(_savePath+fileName+".jpg", "JPEG", 100);

    result.source.save(_savePath+_sourceSubfolder+fileName+" source.jpg", "JPEG", 100);
}
