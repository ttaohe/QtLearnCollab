#include "ImgMapTool.h"
#include <QgsMapCanvas.h>
#include <QgsMapMouseEvent.h>
#include <qgsmapcanvas.h>

ImgMapTool::ImgMapTool(QgsMapCanvas* canvas)
    : QgsMapTool(canvas), mDragging(false), mMapCanvas(canvas)    // 初始化成员变量
{
}

void ImgMapTool::canvasPressEvent(QgsMapMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        mDragging = true;
        mLastPoint = e->mapPoint(); // 获取当前鼠标位置的地图坐标
    }
}

void ImgMapTool::canvasMoveEvent(QgsMapMouseEvent* e)
{
    if (mDragging && (e->buttons() & Qt::LeftButton))
    {
        QgsPointXY currentPoint = e->mapPoint(); // 直接使用 QgsPointXY
        double deltaX = currentPoint.x() - mLastPoint.x();
        double deltaY = currentPoint.y() - mLastPoint.y();
        //mMapCanvas.pan(deltaX, deltaY); // 使用 pan 方法平移地图
        //mLastPoint = currentPoint; // 更新最后点的位置
        // 计算新的地图中心点
        QgsPointXY newCenter = mMapCanvas->mapSettings().visibleExtent().center() + QgsVector(deltaX, deltaY);

        // 设置新的地图中心点
        mMapCanvas->setCenter(newCenter);

        // 更新最后点的位置
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