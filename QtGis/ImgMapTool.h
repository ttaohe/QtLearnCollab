#pragma once
#include <QgsMapTool.h>
#include <QMouseEvent>
#include <QgsPoint.h>

class ImgMapTool : public QgsMapTool
{
    Q_OBJECT

public:
    explicit ImgMapTool(QgsMapCanvas* canvas);

protected:
    void canvasPressEvent(QgsMapMouseEvent* e) override;
    void canvasMoveEvent(QgsMapMouseEvent* e) override;
    void canvasReleaseEvent(QgsMapMouseEvent* e) override;

private:
    bool mDragging; // 标记是否正在拖动
    QgsPointXY mLastPoint; // 记录上一个点的位置
    QgsMapCanvas* mMapCanvas; // 添加地图画布的指针
};

