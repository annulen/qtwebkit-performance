/*
 * Copyright (C) 2010 Viatcheslav Ostapenko, Nokia
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include <QtTest/QtTest>

#include "common_init.h"
#include "benchmark.h"
#include "databasenetworkaccessmanager.h"
#include "databasetests.h"
#include "webpage.h"

#include <qwebframe.h>
#include <qgraphicsview.h>
#include <qgraphicswebview.h>
#include <qpainter.h>

struct PaintingGraphicsWebViewBench : public QGraphicsWebView
{
    Q_OBJECT
public:
    PaintingGraphicsWebViewBench() : m_controller(0) {}

    void paint(QPainter* p, const QStyleOptionGraphicsItem* o, QWidget* w)
    {
        if(m_controller) {
            m_controller->startSubMeasure();
            QGraphicsWebView::paint(p, o, w);
            m_controller->stopSubMeasure();
        } else {
            QGraphicsWebView::paint(p, o, w);
        }
        emit painted();
    }

    void setController(SubSectionBenchmarkController* controller)
    {
        m_controller = controller;
    }
signals:
    void painted();

private:
    SubSectionBenchmarkController* m_controller;
};

class tst_GraphicsScrolling : public QObject
{
    Q_OBJECT

public:
    ~tst_GraphicsScrolling();

public Q_SLOTS:
    void initTestCase();
    void init();
    void cleanup();

private Q_SLOTS:
    void scroll_data();
    void scroll();
    void paintingSpeed_data();
    void paintingSpeed();

private:
    PaintingGraphicsWebViewBench* m_view;
    WebPage* m_page;
    QNetworkAccessManager* m_networkAccessManager;
    QGraphicsView* m_gView;
    QGraphicsScene* m_scene;
};

tst_GraphicsScrolling::~tst_GraphicsScrolling()
{
    benchmarkOutput();
}

void tst_GraphicsScrolling::initTestCase()
{
    if (QSqlDatabase::database().isValid())
        m_networkAccessManager = new DatabaseNetworkAccessManager;
    else
        m_networkAccessManager = 0;
}

void tst_GraphicsScrolling::init()
{
    m_scene = new QGraphicsScene();
    m_gView = new QGraphicsView(m_scene);
    m_view = new PaintingGraphicsWebViewBench();
    m_page = new WebPage(m_view);
    m_view->setPage(m_page);
    m_scene->addItem(m_view);

    const QSize viewportSize(1024, 768);
    m_page->setPreferredContentsSize(viewportSize);
    if (m_networkAccessManager)
        m_page->setNetworkAccessManager(m_networkAccessManager);

#if defined(Q_WS_MAEMO_5) || defined(Q_OS_SYMBIAN) || defined(Q_WS_QWS)
    m_gView->showFullScreen();
#else
    m_page->setViewportSize(viewportSize);
    m_gView->setFixedSize(viewportSize);
    m_gView->show();
#endif
    m_view->setGeometry(m_gView->viewport()->geometry());

    QTest::qWaitForWindowShown(m_gView);
}

void tst_GraphicsScrolling::cleanup()
{
    delete m_gView;
}

void tst_GraphicsScrolling::scroll_data()
{
    add_test_urls();
}

/*
 * We want to test how fast we can scroll. The scrolling
 * process starts with the QWebPage::scrollBy call, this
 * will generate DOM Events which will result in a
 * QWidget::update() call. QCoreApplication::processEvents
 * is used to test the scrolling.
 */
void tst_GraphicsScrolling::scroll()
{
    QFETCH(QUrl, url);

    m_view->load(url);
    ::waitForSignal(m_view, SIGNAL(loadFinished(bool)));

    // wait for Javascript's lazy loading of ressources
#if defined(Q_WS_MAEMO_5) || defined(Q_OS_SYMBIAN)
    QTest::qWait(1500);4
#else
    QTest::qWait(500);
#endif

    /* force a layout */
    QWebFrame* mainFrame = m_page->mainFrame();

    const int scrollIncrement = 30;
    WEB_BENCHMARK(url.toString()) {
        mainFrame->setScrollPosition(QPoint(0, 0));
        m_view->update();

        do {
            mainFrame->scroll(0, scrollIncrement);
            qApp->processEvents();
        } while(mainFrame->scrollBarValue(Qt::Vertical) < mainFrame->scrollBarMaximum(Qt::Vertical));

        do {
            mainFrame->scroll(0, -scrollIncrement);
            qApp->processEvents();
        } while(mainFrame->scrollBarValue(Qt::Vertical) > 0);
    }
}

void tst_GraphicsScrolling::paintingSpeed_data()
{
    add_test_urls();
}

/*
 * This benchmark measure the painting time of scrolling.
 * The page is scrolled to the bottom, and then to the top. The
 * total painting time is the value of interest.
 */
void tst_GraphicsScrolling::paintingSpeed()
{
    QFETCH(QUrl, url);


    m_view->load(url);
    ::waitForSignal(m_view, SIGNAL(loadFinished(bool)));

    // wait for Javascript's lazy loading of ressources
#if defined(Q_WS_MAEMO_5) || defined(Q_OS_SYMBIAN) || defined(Q_WS_QWS)
    QTest::qWait(1500);
#else
    QTest::qWait(500);
#endif

    QWebFrame* mainFrame = m_page->mainFrame();
    if (mainFrame->scrollBarValue(Qt::Vertical) == mainFrame->scrollBarMaximum(Qt::Vertical)) {
        QSKIP("No scrolling for this page", SkipSingle);
    }

    WEB_BENCHMARK_SUBSECTION(url.toString()) {
        m_view->setController(&web__controller);
        const int scrollIncrement = 30;
        while (mainFrame->scrollBarValue(Qt::Vertical) < mainFrame->scrollBarMaximum(Qt::Vertical)) { // scroll forward
            mainFrame->scroll(0, scrollIncrement);
            waitForSignal(m_view, SIGNAL(painted()));
        }
        QCoreApplication::processEvents();
        while (mainFrame->scrollBarValue(Qt::Vertical) > 0) { // then backward
            mainFrame->scroll(0, -scrollIncrement);
            waitForSignal(m_view, SIGNAL(painted()));
        }
    }

    // Give all paintings time to finish
    QCoreApplication::processEvents();
    QTest::qWait(50);
    m_view->setController(0);
}

DBWEBTEST_MAIN(tst_GraphicsScrolling)
#include "tst_graphicsscrolling.moc"
