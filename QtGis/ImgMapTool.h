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
    bool mDragging; // ����Ƿ������϶�
    QgsPointXY mLastPoint; // ��¼��һ�����λ��
    QgsMapCanvas* mMapCanvas; // ��ӵ�ͼ������ָ��
};

