/*
 * Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies)
 * Copyright (C) 2009 Holger Hans Peter Freyther
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <QApplication>
#include <QBasicTimer>
#include <QDebug>

#include <QWebElement>
#include <QWebFrame>
#include <QWebView>
#include <QWebPage>

class WebPage : public QWebPage
{
    Q_OBJECT

public:
    WebPage(QWidget *parent)
        : QWebPage(parent)
        , m_blockJs(false)
    {}

    void setBlockJSPrompt(bool block)
    {
        m_blockJs = block;
    }

    virtual void javaScriptAlert(QWebFrame *originatingFrame, const QString& msg);
    virtual bool javaScriptConfirm(QWebFrame *originatingFrame, const QString& msg);
    virtual bool javaScriptPrompt(QWebFrame *originatingFrame, const QString& msg, const QString& defaultValue, QString* result);
    virtual void javaScriptConsoleMessage(const QString& message, int lineNumber, const QString& sourceID);

private:
    bool m_blockJs;
};

void WebPage::javaScriptAlert(QWebFrame *originatingFrame, const QString& msg)
{
    if (m_blockJs)
        return;

    return QWebPage::javaScriptAlert(originatingFrame, msg);
}

bool WebPage::javaScriptConfirm(QWebFrame *originatingFrame, const QString& msg)
{
    if (m_blockJs)
        return false;

    return QWebPage::javaScriptConfirm(originatingFrame, msg);
}

bool WebPage::javaScriptPrompt(QWebFrame *originatingFrame, const QString& msg, const QString& defaultValue, QString* result)
{
    if (m_blockJs)
        return false;

    return QWebPage::javaScriptPrompt(originatingFrame, msg, defaultValue, result);
}

void WebPage::javaScriptConsoleMessage(const QString& message, int lineNumber, const QString& sourceID)
{
    if (m_blockJs)
        return;

    return QWebPage::javaScriptConsoleMessage(message, lineNumber, sourceID);
}

// Some sort of a fuzzer. Start with a webpage and then navigate
// through the world wide web.
class Crawler : public QObject {
    Q_OBJECT
public:
    Crawler()
        : m_startUrl(QLatin1String("http://www.digg.com"))
        , m_webView(0)
    {}

    void setView(QWebView* view)
    {
        qobject_cast<WebPage*>(view->page())->setBlockJSPrompt(true);

        m_webView = view;
        connect(m_webView, SIGNAL(loadFinished(bool)),
                SLOT(loadFinished(bool)), Qt::QueuedConnection);
    }

public Q_SLOTS:
    void start()
    {
        navigate(m_startUrl);
    }

private Q_SLOTS:
    void loadFinished(bool finished)
    {
        if (!finished) {
            qWarning() << "Load failed trying the start";
            QMetaObject::invokeMethod(this, "start", Qt::QueuedConnection);
            return;
        }

        // pick a random link.
        QList<QWebElement> result = m_webView->page()->mainFrame()->findAllElements("a");
        if (result.isEmpty())
            return start();

        QString newTarget;
        const QUrl oldUrl = m_webView->url();
        int tries = 0;
        do {
            int rand = random() % result.size();
            QWebElement element = result.at(rand);
            QUrl href = element.attribute("href");
            QUrl target;
            if (href.isRelative())
                target = oldUrl.resolved(href);
            else if (href.scheme().startsWith("http"))
                target = href.toString();

            if (oldUrl.host() != target.host() || oldUrl.path() != target.path())
                newTarget = target.toString();

            if (++tries == 30)
                return start();
        } while(newTarget.isEmpty() || newTarget == m_lastTarget);

        navigate(newTarget);
    }

private:
    void navigate(const QString& target)
    {
        // timeout, in case we will get stuck
        m_timeout.start(3 * 60 * 1000, this);

        qDebug() << "New target" << target;
        m_lastTarget = target;
        m_webView->load(m_lastTarget);
    }

    void timerEvent(QTimerEvent* e)
    {
        if (m_timeout.timerId() == e->timerId())
            m_webView->stop();

        QObject::timerEvent(e);
    }

private:
    QString m_startUrl;
    QBasicTimer m_timeout;
    QWebView* m_webView;
    QString m_lastTarget;
};


int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, false);

    QStringList arguments = app.arguments();

    QWebView *view = new QWebView;
    view->setPage(new WebPage(view));

    Crawler crawler;
    view->show();
    crawler.setView(view);
    crawler.start();

    return app.exec();
}

#include "main.moc"
