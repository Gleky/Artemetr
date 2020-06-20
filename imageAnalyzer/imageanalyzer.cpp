#include "imageanalyzer.h"

#include "opencv2/opencv.hpp"

#include <QtConcurrent/QtConcurrent>

ImageAnalyzer::ImageAnalyzer()
{
    qRegisterMetaType<Result>();
}

void ImageAnalyzer::getResult(const QImage &img)
{
    QtConcurrent::run(this, &ImageAnalyzer::compute,img);
}

double median( cv::Mat channel );

void ImageAnalyzer::compute(QImage img)
{
    qDebug() << "Preparing to analyze";
    Result result;
    result.source = img.copy();

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


double median( cv::Mat channel )
{
    double m = (channel.rows*channel.cols) / 2;
    int bin = 0;
    double med = -1.0;

    int histSize = 256;
    float range[] = { 0, 256 };
    const float* histRange = { range };
    bool uniform = true;
    bool accumulate = false;
    cv::Mat hist;
    cv::calcHist( &channel, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange, uniform, accumulate );

    for ( int i = 0; i < histSize && med < 0.0; ++i )
    {
        bin += cvRound( hist.at< float >( i ) );
        if ( bin > m && med < 0.0 )
            med = i;
    }

    return med;
}
