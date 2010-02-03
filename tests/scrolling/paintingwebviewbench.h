#ifndef PAINTINGWEBVIEWBENCH_H
#define PAINTINGWEBVIEWBENCH_H

#include "benchmark.h"

#include <qwebview.h>
#include <QTest>

class PaintingWebViewBench : public QWebView
{
    Q_OBJECT
public:
    PaintingWebViewBench();

    bool testing;
    SubSectionBenchmarkController* controller;

signals:
    void painted();

protected:
    void paintEvent(QPaintEvent* event);
};

#endif // PAINTINGWEBVIEWBENCH_H
