#include "ImgMapTool.h"
#include <QgsMapCanvas.h>
#include <QgsMapMouseEvent.h>
#include <qgsmapcanvas.h>

ImgMapTool::ImgMapTool(QgsMapCanvas* canvas)
    : QgsMapTool(canvas), mDragging(false), mMapCanvas(canvas)    // ��ʼ����Ա����
{
}

void ImgMapTool::canvasPressEvent(QgsMapMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        mDragging = true;
        mLastPoint = e->mapPoint(); // ��ȡ��ǰ���λ�õĵ�ͼ����
    }
}

void ImgMapTool::canvasMoveEvent(QgsMapMouseEvent* e)
{
    if (mDragging && (e->buttons() & Qt::LeftButton))
    {
        QgsPointXY currentPoint = e->mapPoint(); // ֱ��ʹ�� QgsPointXY
        double deltaX = currentPoint.x() - mLastPoint.x();
        double deltaY = currentPoint.y() - mLastPoint.y();
        //mMapCanvas.pan(deltaX, deltaY); // ʹ�� pan ����ƽ�Ƶ�ͼ
        //mLastPoint = currentPoint; // ���������λ��
        // �����µĵ�ͼ���ĵ�
        QgsPointXY newCenter = mMapCanvas->mapSettings().visibleExtent().center() + QgsVector(deltaX, deltaY);

        // �����µĵ�ͼ���ĵ�
        mMapCanvas->setCenter(newCenter);

        // ���������λ��
        mLastPoint = currentPoint;
    }
}

void ImgMapTool::canvasReleaseEvent(QgsMapMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        mDragging = false;
    }
}