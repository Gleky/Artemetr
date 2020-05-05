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
    void getResult(const QImage &img);

signals:
    void resultReady(Result);

private:
    void compute(QImage img);
};

Q_DECLARE_METATYPE(Result)

#endif // IMAGEANALYZER_H
