#include "qframedisplay.h"
#include <QDebug>

QFrameDisplay::QFrameDisplay(int type)
{

    mType = type;

    if(type == PAN_DISPLAY)
    {
        this->setWindowTitle("Panoramic View");
        this->setFixedHeight(PAN_HEIGHT);
        this->setFixedWidth(PAN_WIDTH);
        this->setMouseTracking(true);
        this->setCursor(Qt::CrossCursor);
    }
    else if (type == FOVEAL_DISPLAY)
    {
        this->setWindowTitle("Foveal View");
        this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        this->setFixedHeight(FOVEAL_HEIGHT);
        this->setFixedWidth(FOVEAL_WIDTH);
        // probably we should keep this resizable for the foveal display
    }

}

////////////////////////////////////////////////////////////////////////////////////////////////
void QFrameDisplay::mouseMoveEvent(QMouseEvent *ev){

    if(mType == PAN_DISPLAY) // only the pan view will be able to do this
    {
        QPoint currentMousePos = ev->pos();

        if (ev->x() <= this->width() && ev->x() >= 0)
        {
            if (ev->y() <= this->height() && ev->y() >= 0)
            {
                emit sendMousePosition(currentMousePos);
            }
        }
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////
void QFrameDisplay::mousePressEvent(QMouseEvent *ev){
    if(mType == PAN_DISPLAY) // only the pan view will be able to do this
    {
        QPoint currentMousePos = ev->pos();

        if (ev->x() <= this->width() && ev->x() >= 0)
        {
            if (ev->y() <= this->height() && ev->y() >= 0)
            {
                emit sendClickedMousePosition(currentMousePos);
            }
        }
    }

}
////////////////////////////////////////////////////////////////////////////////////////////////
void QFrameDisplay::receiveFrameCaptured(cv::Mat *frm){

    //qDebug() << "Here";
    if(this->isVisible())
    {
        if(frm != NULL && frm->data)
        {
            //cv::cvtColor(*frm,*frm, CV_BGR2RGB); // convert to rgb
            //if(mType == PAN_DISPLAY)
            //{
            //    mPixMap = QPixmap::fromImage(QImage(frm->data, frm->cols, frm->rows,frm->step, QImage::Format_RGB888));
            //    this->setPixmap(mPixMap);
            //}
            //else if(mType == FOVEAL_DISPLAY)
            //{
            //    mPixMap = QPixmap::fromImage(QImage(frm->data, frm->cols, frm->rows,frm->step, QImage::Format_RGB888));
            //    //this->setPixmap(mPixMap.scaled(this->width(), this->height())); // use Qt::KeepAspectRatio to maintain aspect ratio
            //}


            mPixMap = QPixmap::fromImage(QImage(frm->data, frm->cols, frm->rows,frm->step, QImage::Format_RGB888));
            this->setPixmap(mPixMap);
            emit sendFrameDisplayed();
        }
    }
    // qDebug() << "Receiving Frame "<<mType << " num:" << mNum++;
}
////////////////////////////////////////////////////////////////////////////////////////////////

