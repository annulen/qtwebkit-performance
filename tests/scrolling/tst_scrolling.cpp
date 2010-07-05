/*
 * Copyright (C) 2009 Holger Hans Peter Freyther
 * Copyright (C) 2010 Benjamin Poulain, Nokia
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
#include "paintingwebviewbench.h"
#include "webpage.h"

#include <qwebframe.h>
#include <qwebview.h>

static void loadUrl(QWebView* view, const QUrl& url) {
    view->load(url);
    ::waitForSignal(view, SIGNAL(loadFinished(bool)));

    // wait for Javascript's lazy loading of ressources
#if defined(Q_WS_MAEMO_5) || defined(Q_OS_SYMBIAN)
    QTest::qWait(1500);
#else
    QTest::qWait(500);
#endif
}

static bool canScroll(QWebFrame* mainFrame) {
    if (mainFrame->scrollBarValue(Qt::Vertical) == mainFrame->scrollBarMaximum(Qt::Vertical))
        return false;
    return true;
}

class tst_Scrolling : public QObject
{
    Q_OBJECT

public:
    ~tst_Scrolling();

public Q_SLOTS:
    void init();
    void cleanup();

private Q_SLOTS:
    void scroll_data();
    void scroll();

    void paintingSpeed_data();
    void paintingSpeed();

private:
    PaintingWebViewBench* m_view;
};

tst_Scrolling::~tst_Scrolling()
{
    benchmarkOutput();
}

void tst_Scrolling::init()
{
    m_view = new PaintingWebViewBench();

    QWebPage* page = new WebPage(m_view);
    if (QSqlDatabase::database().isValid())
        page->setNetworkAccessManager(new DatabaseNetworkAccessManager);

    m_view->setPage(page);

#if defined(Q_WS_MAEMO_5) || defined(Q_OS_SYMBIAN) || defined(Q_WS_QWS)
    const QSize viewportSize(1024, 768);
    page->setPreferredContentsSize(viewportSize);
    m_view->showFullScreen();
    m_view->window()->raise();
#else
    m_view->showMaximized();
#endif
    QTest::qWaitForWindowShown(m_view);
}

void tst_Scrolling::cleanup()
{
    delete m_view;
}

void tst_Scrolling::scroll_data()
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
void tst_Scrolling::scroll()
{
    QFETCH(QUrl, url);

    loadUrl(m_view, url);

    QWebFrame* mainFrame = m_view->page()->mainFrame();
    // first rendering outside of benchmarks
    mainFrame->setScrollPosition(QPoint(0, 0));
    m_view->update();
    qApp->processEvents();

    if (!canScroll(mainFrame))
        QSKIP("No scrolling for this page", SkipSingle);

    WEB_BENCHMARK_TIME_PER_FRAME("scrolling::scroll", url.toString()) {
        const int scrollIncrement = 30;
        do {
            web__controller.newFrame();
            mainFrame->scroll(0, scrollIncrement);
            qApp->processEvents();
        } while (mainFrame->scrollBarValue(Qt::Vertical) < mainFrame->scrollBarMaximum(Qt::Vertical));

        do {
            web__controller.newFrame();
            mainFrame->scroll(0, -scrollIncrement);
            qApp->processEvents();
        } while (mainFrame->scrollBarValue(Qt::Vertical) > 0);
    }
}

void tst_Scrolling::paintingSpeed_data()
{
    add_test_urls();
}

/*
 * This benchmark measure the painting time of scrolling.
 * The page is scrolled to the bottom, and then to the top. The
 * total painting time is the value of interest.
 */
void tst_Scrolling::paintingSpeed()
{
    QFETCH(QUrl, url);

    loadUrl(m_view, url);

    QWebFrame* mainFrame = m_view->page()->mainFrame();
    if (!canScroll(mainFrame))
        QSKIP("No scrolling for this page", SkipSingle);

    // first rendering outside of scope
    mainFrame->setScrollPosition(QPoint(0, 0));
    m_view->update();
    qApp->processEvents();

    WEB_BENCHMARK_SUBSECTION("scrolling::paintingSpeed", url.toString()) {
        m_view->controller = &web__controller;
        m_view->testing = true;
        const int scrollIncrement = 30;
        while (mainFrame->scrollBarValue(Qt::Vertical) < mainFrame->scrollBarMaximum(Qt::Vertical)) { // scroll forward
            mainFrame->scroll(0, scrollIncrement);
            waitForSignal(m_view, SIGNAL(painted()));
        }

        while (mainFrame->scrollBarValue(Qt::Vertical) > 0) { // then backward
            mainFrame->scroll(0, -scrollIncrement);
            waitForSignal(m_view, SIGNAL(painted()));
        }
    }
}

DBWEBTEST_MAIN(tst_Scrolling)
#include "tst_scrolling.moc"
