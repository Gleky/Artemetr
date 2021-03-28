#ifndef IMAGEANALYZER_H
#define IMAGEANALYZER_H

#include "imageAnalyzer_global.h"

#include <QObject>


class IMAGEANALYZER_EXPORT ImageAnalyzer : public QObject
{
    Q_OBJECT
public:
    ImageAnalyzer();

    void checkPackPresence(const QImage &img);

signals:
    void packPresence( bool presence );

private:
    void checkPresence(QImage img);
};

#endif // IMAGEANALYZER_H
