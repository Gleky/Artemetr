#include "imageanalyzer.h"

#include "opencv2/opencv.hpp"

#include <QtConcurrent/QtConcurrent>
#include <QImage>

ImageAnalyzer::ImageAnalyzer()
{}

void ImageAnalyzer::checkPackPresence(const QImage &img)
{
    QtConcurrent::run(this, &ImageAnalyzer::checkPresence,img);
}

void ImageAnalyzer::checkPresence(QImage img)
{
    bool presence = false;

#ifndef TEST
    QImage *src = &img;
    cv::Mat inputImg( src->height(), src->width(), CV_8UC4, src->bits(), src->bytesPerLine());

    cv::Mat forStudy;
    cvtColor(inputImg, forStudy, cv::COLOR_BGR2GRAY);

    QSettings filterSettings("filter.config", QSettings::IniFormat);
    const int threshold = filterSettings.value("presenceWhiteThreshold",110).toInt();
    cv::inRange(forStudy, threshold, 255, forStudy);

    auto totalPixelsCount = forStudy.size().area();
    auto pixelValue = forStudy.data;
    int darkPixelsCount = 0;

    for (int i = 0; i < totalPixelsCount; ++i, ++pixelValue)
    {
        if ( (*pixelValue) == 0 )
            ++darkPixelsCount;
    }

    double darkness = (darkPixelsCount*1.0) / totalPixelsCount;

    const double minDarkPart = filterSettings.value("presenceDarkPatr", 0.12).toDouble();
    if ( darkness > minDarkPart ) presence = true;
#else
    static int count = 0;
    if (count < 2) presence = true;
    ++count;
#endif
    emit packPresence( presence );
}
