#include "resultsaver.h"

#include <QDir>
#include <QDateTime>
#include <QTextStream>
#include <QMessageBox>

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
    auto fileName = QDateTime::currentDateTime().toString("yyyy.MM.dd hh-mm-ss");

    result.result.save(_savePath+fileName+".jpg", "JPEG", 100);

    result.source.save(_sourcePath+fileName+" source.jpg", "JPEG", 100);

    _crayfishCount += result.crayfishCount;
    _eggsCount += result.eggsCount;

    if ( _lastResultSaved )
        _lastResultSaved = false;
}


void createSaveFile();

void ResultSaver::setDone()
{
    if ( _lastResultSaved )
        return;

    QString saveFileName = "../results_log.csv";

    QFile saveFile( saveFileName );

    if ( !saveFile.exists() )
        createSaveFile();

    if ( !saveFile.open(QIODevice::Append) )
        return;

    QTextStream stream(&saveFile);

    QString date = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    stream << date << ',' << QString::number(_crayfishCount) << ',' << QString::number(_eggsCount) << '\n';

    QMessageBox::information(nullptr, "Done", "Crayfish: " + QString::number(_crayfishCount)+'\n'+
                                              "Eggs: " + QString::number(_eggsCount));

    saveFile.close();
    _crayfishCount = 0;
    _eggsCount = 0;
    _lastResultSaved = true;
}

void createSaveFile()
{
    QString saveFileName = "../results_log.csv";

    QFile saveFile( saveFileName );
    if ( !saveFile.open(QIODevice::ReadWrite) )
        return;

    QTextStream stream(&saveFile);

    stream << "Date, Crayfish, Eggs \n";

    saveFile.close();
}
