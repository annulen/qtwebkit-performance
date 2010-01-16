/*
 * Copyright (C) 2009 Holger Hans Peter Freyther
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

#include <qapplication.h>
#include <qdesktopwidget.h>
#include <qwebelement.h>
#include <qwebframe.h>
#include <qwebview.h>
#include <qpaintengine.h>
#include <qpainter.h>

class tst_Painting : public QObject
{
    Q_OBJECT

public:
    ~tst_Painting();

public Q_SLOTS:
    void init();
    void cleanup();

private Q_SLOTS:
    void paint_data();
    void paint();

    void fullPagePaint_data();
    void fullPagePaint();

private:
    QWebView* m_view;
    QWebPage* m_page;
};

tst_Painting::~tst_Painting()
{
    benchmarkOutput(*benchmark_parent);
}

void tst_Painting::init()
{
    m_view = new QWebView;
    m_page = m_view->page();

    QSize viewportSize(1024, 768);
#if defined(Q_WS_MAEMO_5) || defined(Q_OS_SYMBIAN) || defined(Q_WS_QWS)
    // screensize is a more typical rendering unit for embedded
    m_page->setPreferredContentsSize(viewportSize);
    const QSize screenSize = QApplication::desktop()->geometry().size();
    m_view->setFixedSize(screenSize);
    m_page->setViewportSize(screenSize);
#else
    m_view->setFixedSize(viewportSize);
    m_page->setViewportSize(viewportSize);
#endif

    if (QSqlDatabase::database().isValid())
        m_page->setNetworkAccessManager(new DatabaseNetworkAccessManager);
}

void tst_Painting::cleanup()
{
    delete m_view;
}

void tst_Painting::paint_data()
{
    add_test_urls();
}

void tst_Painting::paint()
{
    QFETCH(QUrl, url);

    m_view->load(url);
    ::waitForSignal(m_view, SIGNAL(loadFinished(bool)));

    /* force a layout */
    QWebFrame* mainFrame = m_page->mainFrame();
    mainFrame->toPlainText();

    QPixmap pixmap(m_page->viewportSize());
#if defined(Q_WS_X11)
    const bool needToSync = pixmap.paintEngine()->type() != QPaintEngine::Raster;
#endif
    WEB_BENCHMARK(url.toString()) {
        QPainter painter(&pixmap);
        mainFrame->render(&painter, QRect(QPoint(0, 0), m_page->viewportSize()));
        painter.end();

#if defined(Q_WS_X11)
        // force badness... to have some reliable result on non raster..
        // this will make the result "unreal" in some ways but might be
        // better than just using the raster engine.
        if (needToSync)
            QApplication::syncX();
#endif
    }
}

void tst_Painting::fullPagePaint_data()
{
    add_test_urls();
}

void tst_Painting::fullPagePaint()
{
    QFETCH(QUrl, url);

    m_view->load(url);
    ::waitForSignal(m_view, SIGNAL(loadFinished(bool)));

    /* force a layout */
    QWebFrame* mainFrame = m_page->mainFrame();
    mainFrame->toPlainText();

    QWebElement rootElement = mainFrame->documentElement();
    QPixmap pixmap(rootElement.geometry().size());
    if (pixmap.isNull())
        QSKIP("Invalid pixmap", SkipSingle);

    {
        QPainter painter(&pixmap);
        if (!painter.paintEngine() || !painter.paintEngine()->paintDevice())
            QSKIP("Invalid engine for pixmap, probably short on memory", SkipSingle);
    }
#if defined(Q_WS_X11)
    const bool needToSync = pixmap.paintEngine()->type() != QPaintEngine::Raster;
#endif
    WEB_BENCHMARK(url.toString()) {
        QPainter painter(&pixmap);
        rootElement.render(&painter);
        painter.end();

#if defined(Q_WS_X11)
        // force badness... to have some reliable result on non raster..
        // this will make the result "unreal" in some ways but might be
        // better than just using the raster engine.
        if (needToSync)
            QApplication::syncX();
#endif
    }
}

DBWEBTEST_MAIN(tst_Painting)
#include "tst_painting.moc"
