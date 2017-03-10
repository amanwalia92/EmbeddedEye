#include "qembeddedeye.h"
#include <QApplication>
#include <qopencvincludables.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QEmbeddedEye w;
    w.show();
    return a.exec();


    //cv::VideoCapture v(2);
    //
    //cv::Mat img;
    //
    //while(v.read(img)){
    //    cv::imshow("img",img);
    //    cv::waitKey(1);
    //}

    //return 0;
}
