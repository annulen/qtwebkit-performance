#include "paintingwebviewbench.h"
#include <QTime>

PaintingWebViewBench::PaintingWebViewBench()
    : testing(false)
    , controller(0)
{
}

void PaintingWebViewBench::paintEvent(QPaintEvent* event)
{
    if (testing) {
        controller->startSubMeasure();
        QWebView::paintEvent(event);
        controller->stopSubMeasure();
    } else {
        QWebView::paintEvent(event);
    }
    emit painted();
}
