#include "imageanalyzer.h"

#include "opencv2/opencv.hpp"

#include <QtConcurrent/QtConcurrent>

ImageAnalyzer::ImageAnalyzer()
{
    qRegisterMetaType<CellResult>();
}

void ImageAnalyzer::checkPackPresence(const QImage &img)
{
    QtConcurrent::run(this, &ImageAnalyzer::checkPresence,img);
}

void ImageAnalyzer::getResult(const QImage &img)
{
    QtConcurrent::run(this, &ImageAnalyzer::computeResult,img);
}



void ImageAnalyzer::checkPresence(QImage img)
{
    bool presence = false;

    QImage *src = &img;
    cv::Mat inputImg( src->height(), src->width(), CV_8UC4, src->bits(), src->bytesPerLine());

    cv::Mat forStudy;
    cvtColor(inputImg, forStudy, cv::COLOR_BGR2GRAY);

    int threshold = 100;
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

    if ( darkness > 0.12 )
        presence = true;

    emit packPresence( presence );
}


int crayfishFound(QSettings &settings, const std::vector< std::vector<cv::Point> > &contours, cv::Mat &outputImage);
int eggsFound(QSettings &settings, const std::vector< std::vector<cv::Point> > &contours, cv::Mat &outputImage);
void convert(const cv::Mat &input, QImage &output);

void ImageAnalyzer::computeResult(QImage img)
{
    qDebug() << "Preparing to analyze";
    CellResult result;
    result.source = img.copy();
    result.crayfishCount = 0;
    result.eggsCount = 0;

    QImage *src = &img;

    cv::Mat inputImg( src->height(), src->width(), CV_8UC4, src->bits(), src->bytesPerLine());
    cv::Mat forStudy, ret; // deep copy just in case (my lack of knowledge with open cv)
    cvtColor(inputImg, forStudy, cv::COLOR_BGR2GRAY);
    cvtColor(inputImg, ret, cv::COLOR_BGRA2RGB);

    QSettings filterSettings("filter.config", QSettings::IniFormat);

    const int magicFactor1 = filterSettings.value("magicFactor1",55).toInt();
    const int magicFactor2 = filterSettings.value("magicFactor2",41).toInt();

#ifndef TEST
    qDebug() << "Start analyze";
    cv::Mat temp;
    cv::bilateralFilter(forStudy,temp,30,12,200);
    forStudy = temp;

    cv::adaptiveThreshold(forStudy,forStudy, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, magicFactor1, magicFactor2);

    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(forStudy, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_TC89_KCOS);

    result.crayfishCount = crayfishFound(filterSettings, contours, ret);
    result.crayfishCount = eggsFound(filterSettings, contours, ret);

#else
    filterSettings.beginGroup("crayfish");
    const int fillPercentCrayfish = filterSettings.value("fillPercent",55).toInt();
    filterSettings.endGroup();

    filterSettings.beginGroup("eggs");
    const int fillPercentEggs = filterSettings.value("fillPercent",55).toInt();
    filterSettings.endGroup();

    result.crayfishCount = 1;
    result.eggsCount = 1;
#endif

    qDebug() << "Result ready, converting";

    convert(ret, result.result);

    qDebug() << "Result converted, send it";
    emit resultReady(result);
}



int crayfishFound(QSettings &settings, const std::vector< std::vector<cv::Point> > &contours, cv::Mat &outputImage)
{
    settings.beginGroup("crayfish");
    const int maxRatio = settings.value("fillPercent",55).toInt();
    const double delta = settings.value("maxAspectRatio", 1.4).toDouble();
    const int minArea = settings.value("minContourArea",86).toInt();
    const int maxArea = settings.value("maxContourArea",350).toInt();
    const int minRectArea = settings.value("minRectArea",200).toInt();
    const int maxRectArea = settings.value("maxRectArea",1370).toInt();
    settings.endGroup();

    int objectsFound = 0;

    for ( int k = 0; k < static_cast<int>( contours.size() ); ++k )
    {
        auto contour = contours[k];

        double contArea = fabs( contourArea(contour) );
        auto rect = cv::boundingRect(contour);
        auto rectArea = rect.area();

        int areaRatio = contArea/rectArea*100.;
        if ( areaRatio > maxRatio )
        {
            double aspectRatio = (rect.width*1.)/rect.height;
            if ( 1/delta < aspectRatio && aspectRatio < 1*delta )
                continue;
        }

        for (int i = 1; i < 3; ++i)
        {
            int contAreaLocal = contArea/i;
            int rectAreaLocal = rectArea/i;

            if ( minArea > contAreaLocal || contAreaLocal > maxArea )
                continue;

            if ( minRectArea > rectAreaLocal || rectAreaLocal > maxRectArea )
                continue;

            cv::rectangle(outputImage, rect, cv::Scalar(70,255,90), 1);
            objectsFound += i;
            break;
        }
    }
    return objectsFound;
}

int eggsFound(QSettings &settings, const std::vector< std::vector<cv::Point> > &contours, cv::Mat &outputImage)
{
    settings.beginGroup("eggs");
    const int maxRatio = settings.value("fillPercent",55).toInt();
    const double delta = settings.value("maxAspectRatio", 1.4).toDouble();
    const int minArea = settings.value("minContourArea",86).toInt();
    const int maxArea = settings.value("maxContourArea",350).toInt();
    const int minRectArea = settings.value("minRectArea",200).toInt();
    const int maxRectArea = settings.value("maxRectArea",1370).toInt();
    settings.endGroup();

    int objectsFound = 0;

    for ( int k = 0; k < static_cast<int>( contours.size() ); ++k )
    {
        auto contour = contours[k];

        double contArea = fabs( contourArea(contour) );
        auto rect = cv::boundingRect(contour);
        auto rectArea = rect.area();

        int areaRatio = contArea/rectArea*100.;
        if ( areaRatio > maxRatio )
        {
            double aspectRatio = (rect.width*1.)/rect.height;
            if ( 1/delta < aspectRatio && aspectRatio < 1*delta )
            {
                cv::rectangle(outputImage, rect, cv::Scalar(255,55,55), 1);
                objectsFound += 1;
                continue;
            }
        }

        for (int i = 1; i < 3; ++i)
        {
            int contAreaLocal = contArea/i;
            int rectAreaLocal = rectArea/i;

            if ( minArea > contAreaLocal || contAreaLocal > maxArea )
                continue;

            if ( minRectArea > rectAreaLocal || rectAreaLocal > maxRectArea )
                continue;

            break;
        }
    }

    return objectsFound;
}

void convert(const cv::Mat &input, QImage &output)
{
    switch ( input.type() )
    {
    // 8-bit, 4 channel
    case CV_8UC4:
    {
        QImage image( input.data,
                      input.cols, input.rows,
                      static_cast<int>(input.step),
                      QImage::Format_ARGB32 );
        output = image.copy();
        break;
    }

        // 8-bit, 3 channel
    case CV_8UC3:
    {
        QImage image( input.data,
                      input.cols, input.rows,
                      static_cast<int>(input.step),
                      QImage::Format_RGB888 );
        output = image.copy();
        break;
    }

        // 8-bit, 1 channel
    case CV_8UC1:
    {
        QImage image( input.data,
                      input.cols, input.rows,
                      static_cast<int>(input.step),
                      QImage::Format_Grayscale8 );
        output = image.copy();
        break;
    }
    }
}
