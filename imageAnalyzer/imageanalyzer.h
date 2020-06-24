#ifndef IMAGEANALYZER_H
#define IMAGEANALYZER_H

#include "imageAnalyzer_global.h"

#include <QObject>
#include "../struct_result.h"

class IMAGEANALYZER_EXPORT ImageAnalyzer : public QObject
{
    Q_OBJECT
public:
    ImageAnalyzer();
    void checkPackPresence(const QImage &img);
    void getResult(const QImage &img);

signals:
    void packPresence( bool presence );
    void resultReady(CellResult);

private:
    void checkPresence(QImage img);
    void computeResult(QImage img);
};

Q_DECLARE_METATYPE(CellResult)

#endif // IMAGEANALYZER_H
