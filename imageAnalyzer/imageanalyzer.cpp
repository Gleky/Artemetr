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
    auto v = median(forStudy);
    double sigma = 0.48;
    int lower = int( std::max(0., (1-sigma)*v) );
    int upper = int( std::min(255., (1+sigma)*v) );

    cv::Canny(forStudy,forStudy, lower,upper);

    cv::Mat kerne = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2,2));
    cv::morphologyEx(forStudy, forStudy, cv::MORPH_GRADIENT, kerne);
    cv::morphologyEx(forStudy, forStudy, cv::MORPH_CLOSE, kerne);


    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(forStudy, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_TC89_KCOS);

    int found = 0;
    for ( int k = 0; k < contours.size(); ++k )
    {
        auto contour = contours[k];

        double s = fabs( contourArea(contour) );
        double p = arcLength( contour, false )/s;

            if ( 190 > s || s > 524 )
                continue;

            if ( 0.26 > p || p > 0.53 )
                continue;

        auto rect = cv::boundingRect(contour);
        cv::rectangle(ret, rect, cv::Scalar(70,255,90), 2);
        ++found;
    }
    result.crayfishCount = found;
    result.eggsCount = 0;

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
