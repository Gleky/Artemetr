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
#ifndef TEST
    auto fileName = QDateTime::currentDateTime().toString("yyyy.MM.dd hh-mm-ss");

    result.result.save(_savePath+fileName+".jpg", "JPEG", 100);

    result.source.save(_sourcePath+fileName+" source.jpg", "JPEG", 100);
#endif

    _crayfishCount += result.crayfishCount;
    _eggsCount += result.eggsCount;

    if ( _resultSaved )
    {
        _resultSaved = false;
        writeTitle();
    }
}


void createSaveFileIfNotExists(QString filePath, char splitter);

void ResultSaver::savePackResultsText()
{
    const int currentCryfishCount = _crayfishCount - _lastCrayfishCount;
    const int currentEggCount = _eggsCount - _lastEggsCount;

    const QString fileName = "results_log";
    const QStringList line = {" ", QString::number(_crayfishCount), QString::number(_eggsCount)};

    saveResults(fileName, line, ',');
    saveResults(fileName+"_old_format", line, ';');

    _lastCrayfishCount = _crayfishCount;
    _lastEggsCount = _eggsCount;
}

void ResultSaver::saveSummaryResultsText()
{
    if ( _resultSaved )
        return;

    const QString fileName = "results_log";
    const QStringList line = {"TOTAL", QString::number(_crayfishCount), QString::number(_eggsCount)+'\n'};

    saveResults(fileName, line, ',');
    saveResults(fileName+"_old_format", line, ';');

    QMessageBox::information(nullptr, "Done", "Crayfish: " + QString::number(_crayfishCount)+'\n'+
                                              "Eggs: " + QString::number(_eggsCount));

    resetCounters();
}

void ResultSaver::saveResults(QString fileName, QStringList line, char splitter) const
{
    const QString saveFilePath = "../"+fileName+".csv";
    createSaveFileIfNotExists(saveFilePath, splitter);
    writeLine(saveFilePath, line, splitter);
}

void ResultSaver::writeTitle()
{
    const QString fileName = "results_log";
    const QString date = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    const QStringList line = {date, "Crayfish", "Eggs"};

    saveResults(fileName, line, ',');
    saveResults(fileName+"_old_format", line, ';');
}

void ResultSaver::writeLine(QString filePath, QStringList line, char splitter) const
{
    QFile saveFile( filePath );
    if ( !saveFile.open(QIODevice::Append) )
        return;

    QTextStream stream(&saveFile);

    for (const auto &cellText : line )
    {
        stream << cellText;
        stream << splitter;
    }
    stream << '\n';
    saveFile.close();
}

void ResultSaver::resetCounters()
{
    _crayfishCount = 0;
    _eggsCount = 0;
    _resultSaved = true;

    _lastCrayfishCount = 0;
    _lastEggsCount = 0;
}

void createSaveFileIfNotExists(QString filePath, char splitter)
{
    QFile saveFile( filePath );
    if ( saveFile.exists() )
        return;

    if ( !saveFile.open(QIODevice::Append) )
        return;
}
