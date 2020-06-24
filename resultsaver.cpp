#include "resultsaver.h"

#include <QDir>
#include <QDateTime>
#include <QTextStream>
#include <QMessageBox>

ResultSaver::ResultSaver(QString folderName)
    :_savePath("../" + folderName + '/')
{
    QDir saveDir(_savePath);
    if ( !saveDir.exists() )
        saveDir.mkpath(_savePath);

    QDir scourceDir(_sourcePath);
    if ( !scourceDir.exists() )
        scourceDir.mkpath(_sourcePath);
}

void ResultSaver::saveResult(CellResult result)
{
    auto fileName = QDateTime::currentDateTime().toString("yyyy.MM.dd hh-mm-ss");

    result.result.save(_savePath+fileName+".jpg", "JPEG", 100);

    result.source.save(_sourcePath+fileName+" source.jpg", "JPEG", 100);

    _crayfishCount += result.crayfishCount;
    _eggsCount += result.eggsCount;

    if ( _resultSaved )
        _resultSaved = false;
}


void createSaveFileIfNotExists(QString filePath, char splitter);

//void ResultSaver::savePackResultsText() const
//{
//    const int currentCryfishCount = _crayfishCount - _lastCrayfishCount;
//    const int currentEggCount = _eggsCount - _lastEggsCount;
//}

void ResultSaver::saveSummaryResultsText()
{
    if ( _resultSaved )
        return;

    const QString fileName = "results_log";

    saveResults(fileName,',');
    saveResults(fileName+"_old_format", ';');
    saveResults(fileName+"_tabtest", '\t');

    QMessageBox::information(nullptr, "Done", "Crayfish: " + QString::number(_crayfishCount)+'\n'+
                                              "Eggs: " + QString::number(_eggsCount));

    resetCounters();
}

void ResultSaver::saveResults(QString fileName, char splitter) const
{
    const QString saveFilePath = "../"+fileName+".csv";

    createSaveFileIfNotExists(saveFilePath, splitter);
    QFile saveFile( saveFilePath );

    if ( !saveFile.open(QIODevice::Append) )
        return;

    QTextStream stream(&saveFile);

    QString date = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    stream << date << splitter << QString::number(_crayfishCount) << splitter << QString::number(_eggsCount) << '\n';

    saveFile.close();
}

void ResultSaver::resetCounters()
{
    _crayfishCount = 0;
    _eggsCount = 0;
    _resultSaved = true;
}

void createSaveFileIfNotExists(QString filePath, char splitter)
{
    QFile saveFile( filePath );
    if ( saveFile.exists() )
        return;

    if ( !saveFile.open(QIODevice::ReadWrite) )
        return;

    QTextStream stream(&saveFile);

    stream << "Date" << splitter << "Crayfish" << splitter << "Eggs\n";

    saveFile.close();
}
