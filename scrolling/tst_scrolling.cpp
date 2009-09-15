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

public Q_SLOTS:
    void init();
    void cleanup();

private Q_SLOTS:
    void scroll_data();
    void scroll();

private:
    QWebView* m_view;
    QWebPage* m_page;
};

void tst_Scrolling::init()
{
    m_view = new QWebView;
    m_view->show();
    m_page = m_view->page();

    QSize viewportSize(1024, 768);
    m_view->setFixedSize(viewportSize);
    m_page->setViewportSize(viewportSize);
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

    /* force a layout */
    QWebFrame* mainFrame = m_page->mainFrame();

    QBENCHMARK {
        mainFrame->setScrollPosition(QPoint(0, 0));
        m_view->update();

        int offset = 0;
        do {
            mainFrame->scroll(0, 1);
            ++offset;
            qApp->processEvents();
        } while(offset + m_page->viewportSize().height() < mainFrame->contentsSize().height());

        do {
            mainFrame->scroll(0, -1);
            --offset;
            qApp->processEvents();
        } while(offset >= 0);
    }
}

QTEST_MAIN(tst_Scrolling)
#include "tst_scrolling.moc"
