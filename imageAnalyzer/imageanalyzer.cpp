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

    auto val = cv::mean(forStudy);
    double average = val[0];

    if ( average < 200 )
        presence = true;

    emit packPresence( presence );
}



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

    qDebug() << "Start analyze";
    cv::Mat temp;
    cv::bilateralFilter(forStudy,temp,30,12,200);
    forStudy = temp;

    cv::adaptiveThreshold(forStudy,forStudy, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 55, 41);

    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(forStudy, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_TC89_KCOS);

    for ( int k = 0; k < contours.size(); ++k )
    {
        auto contour = contours[k];

        double contArea = fabs( contourArea(contour) );
        auto rect = cv::boundingRect(contour);
        auto rectArea = rect.area();

        int areaRatio = contArea/rectArea*100.;
        int maxRatio = 55;
        if ( areaRatio > maxRatio )
        {
            double aspectRatio = (rect.width*1.)/rect.height;
            double delta = 1.4;
            if ( 1/delta < aspectRatio && aspectRatio < 1*delta )
            {
                cv::rectangle(ret, rect, cv::Scalar(255,55,55), 1);
                result.eggsCount += 1;
                continue;
            }
        }

        for (int i = 1; i < 3; ++i)
        {
            int contAreaLocal = contArea/i;
            int rectAreaLocal = rectArea/i;

            int minArea = 86;
            int maxArea = 350;
            if ( minArea > contAreaLocal || contAreaLocal > maxArea )
                continue;

            int minRectArea = 200;
            int maxRectArea = 1370;
            if ( minRectArea > rectAreaLocal || rectAreaLocal > maxRectArea )
                continue;

            cv::rectangle(ret, rect, cv::Scalar(70,255,90), 1);
            result.crayfishCount += i;
            break;
        }
    }

    qDebug() << "Result ready, converting";

    switch ( ret.type() )
    {
    // 8-bit, 4 channel
    case CV_8UC4:
    {
        QImage image( ret.data,
                      ret.cols, ret.rows,
                      static_cast<int>(ret.step),
                      QImage::Format_ARGB32 );
        result.result = image.copy();
        break;
    }

        // 8-bit, 3 channel
    case CV_8UC3:
    {
        QImage image( ret.data,
                      ret.cols, ret.rows,
                      static_cast<int>(ret.step),
                      QImage::Format_RGB888 );
        result.result = image.copy();
        break;
    }

        // 8-bit, 1 channel
    case CV_8UC1:
    {
        QImage image( ret.data,
                      ret.cols, ret.rows,
                      static_cast<int>(ret.step),
                      QImage::Format_Grayscale8 );
        result.result = image.copy();
        break;
    }
    }

    qDebug() << "Result converted, send it";
    emit resultReady(result);
}
