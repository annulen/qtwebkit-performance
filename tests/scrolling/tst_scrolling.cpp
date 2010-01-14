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

#include <qwebframe.h>
#include <qwebview.h>
#include <qpainter.h>

/**
 * Starts an event loop that runs until the given signal is received.
 Optionally the event loop
 * can return earlier on a timeout.
 *
 * \return \p true if the requested signal was received
 *         \p false on timeout
 */
static bool waitForSignal(QObject* obj, const char* signal, int timeout = 0)
{
    QEventLoop loop;
    QObject::connect(obj, signal, &loop, SLOT(quit()));
    QTimer timer;
    QSignalSpy timeoutSpy(&timer, SIGNAL(timeout()));
    if (timeout > 0) {
        QObject::connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
        timer.setSingleShot(true);
        timer.start(timeout);
    }
    loop.exec();
    return timeoutSpy.isEmpty();
}

class tst_Scrolling : public QObject
{
    Q_OBJECT

public:
    ~tst_Scrolling();

public Q_SLOTS:
    void initTestCase();
    void init();
    void cleanup();

private Q_SLOTS:
    void scroll_data();
    void scroll();

private:
    QWebView* m_view;
    QWebPage* m_page;
    QNetworkAccessManager* m_networkAccessManager;
};

tst_Scrolling::~tst_Scrolling()
{
    benchmarkOutput(*benchmark_parent);
}

void tst_Scrolling::initTestCase()
{
    if (QSqlDatabase::database().isValid())
        m_networkAccessManager = new DatabaseNetworkAccessManager;
    else
        m_networkAccessManager = 0;
}

void tst_Scrolling::init()
{
    m_view = new QWebView;
    const QSize viewportSize(1024, 768);
    m_page = m_view->page();
    m_page->setPreferredContentsSize(viewportSize);
    if (m_networkAccessManager)
        m_page->setNetworkAccessManager(m_networkAccessManager);

#if defined(Q_WS_MAEMO_5) || defined(Q_OS_SYMBIAN) || defined(Q_WS_QWS)
    m_view->showFullScreen();
#else
    m_page->setViewportSize(viewportSize);
    m_view->setFixedSize(viewportSize);
    m_view->show();
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

    m_view->load(url);
    ::waitForSignal(m_view, SIGNAL(loadFinished(bool)));

    // wait for Javascript's lazy loading of ressources
#if defined(Q_WS_MAEMO_5) || defined(Q_OS_SYMBIAN)
    QTest::qWait(1500);
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

DBWEBTEST_MAIN(tst_Scrolling)
#include "tst_scrolling.moc"
