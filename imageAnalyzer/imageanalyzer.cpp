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

void ImageAnalyzer::compute(QImage img)
{
    Result result;
    result.source = img;
    result.crayfishCount = 14;
    result.eggsCount = 88;

    QImage *src = &(result.source);

    cv::Mat inputImg( src->height(), src->width(), CV_8UC4, src->bits(), src->bytesPerLine());
    cv::Mat forSearch,ret; // deep copy just in case (my lack of knowledge with open cv)
    cvtColor(inputImg, forSearch,cv::COLOR_BGR2GRAY);

    for (int i = 0; i < 1; ++i)
        cv::GaussianBlur(forSearch,forSearch,cv::Size(3,3),0);

//    cv::inRange(forSearch, 0, 110, forSearch);
////   Canny(gray,bin, h, H, 3);
    Canny(forSearch,ret, 160, 30, 3);

//    Mat kernel = getStructuringElement(MORPH_RECT, Size(2,2));
//    morphologyEx(bin, bin, MORPH_CLOSE, kernel);

//    std::vector<std::vector<Point> > contours;
//    findContours(bin, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

//    int i = 0;
//    int found = 0;
//    for (auto contour : contours) {
//        double s = fabs(contourArea(contour));
////        double p = arcLength(contour, false);
//        if (s > 35 && s < 180) {
//            drawContours(img,contours,i,Scalar(0,255,0), 1);
//            ++found;
//        }
//        ++i;
//    }

//    QImage imgOriginal(img.data, img.cols, img.rows, static_cast<int>(img.step), QImage::Format_RGB888);

//    Result res;
//    emit resultReady(res);
//    _ui->labelImg->setPixmap(QPixmap::fromImage(imgOriginal));s

//    _ui->statusBar->showMessage("Найдено рачков: " + QString::number(found));


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
       }

       // 8-bit, 3 channel
       case CV_8UC3:
       {
          QImage image( ret.data,
                        ret.cols, ret.rows,
                        static_cast<int>(ret.step),
                        QImage::Format_RGB888 );
          result.result = image.copy();
       }

       // 8-bit, 1 channel
       case CV_8UC1:
       {
          QImage image( ret.data,
                        ret.cols, ret.rows,
                        static_cast<int>(ret.step),
                        QImage::Format_Grayscale8 );
          result.result = image.copy();
       }
    }

    emit resultReady(result);
}

//QImage Mat2QImage(cv::Mat const& src)
//{
//     cv::Mat temp; // make the same cv::Mat
//     cvtColor(src, temp, cv::COLOR_BGR2RGB); // cvtColor Makes a copt, that what i need
//     QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
//     dest.bits(); // enforce deep copy, see documentation
//     // of QImage::QImage ( const uchar * data, int width, int height, Format format )
//     return dest;
//}

//cv::Mat QImage2Mat(QImage const& src)
//{
//     cv::Mat tmp(src.height(),src.width(),CV_8UC3,(uchar*)src.bits(),src.bytesPerLine());
//     cv::Mat result; // deep copy just in case (my lack of knowledge with open cv)
//     cvtColor(tmp, result,cv::COLOR_BGR2RGB);
//     return result;
//}
