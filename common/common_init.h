/*
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

#ifndef common_init_h
#define common_init_h

/**
 * routines to add test data...
 *
 * Created from do_mirror.sh with the following shell foo:
 *  for site in $WEBSITES; do echo 'QTest::newRow("rowXY") <<' QUrl(\"$site\"\);; done
 */
QList<QUrl> test_urls()
{
    QList<QUrl> urls;
    urls << QUrl("http://us.yahoo.com");
    urls << QUrl("http://www.google.com/ncr");
    urls << QUrl("http://maps.google.com");
    urls << QUrl("http://www.apple.com");
    urls << QUrl("http://amazon.com");
    urls << QUrl("http://ebay.com");
    urls << QUrl("http://bbc.co.uk");
    urls << QUrl("http://www.guardian.co.uk");
    urls << QUrl("http://www.google.com/news?ned=us");
    urls << QUrl("http://news.yahoo.com");
    urls << QUrl("http://flickr.com");
    urls << QUrl("http://www.youtube.com/?hl=en_US");
    urls << QUrl("http://myspace.com");
    urls << QUrl("http://www.twitter.com");
    urls << QUrl("http://twitter.com/BARACKOBAMA");
    urls << QUrl("http://reddit.com");
    urls << QUrl("http://www.facebook.com");
    urls << QUrl("http://www.gmail.com");
    urls << QUrl("http://mail.yahoo.com");
    urls << QUrl("http://hi5.com");
    urls << QUrl("http://www.dailymotion.com");
    urls << QUrl("http://youporn.com");
    urls << QUrl("http://www.microsoft.com/en/us/default.aspx");
    urls << QUrl("http://www.live.com");
    urls << QUrl("http://www.wikipedia.org");
    urls << QUrl("http://en.wikipedia.org");
    urls << QUrl("http://www.wordpress.com");
    urls << QUrl("http://www.rapidshare.com");
    urls << QUrl("http://www.blogger.com");
    urls << QUrl("http://www.bing.com");
    urls << QUrl("http://www.imdb.com");
    urls << QUrl("http://www.linkedin.com");
    urls << QUrl("http://en.wikipedia.org/wiki/Maxwell_equations");
    urls << QUrl("http://naver.com");
    urls << QUrl("http://kr.yahoo.com");
    urls << QUrl("http://www.google.co.kr/");
    urls << QUrl("http://www.auction.co.kr");
    urls << QUrl("http://www.youtube.com/?hl=ko-KR");

    return urls;
}

void add_test_urls()
{
    QList<QUrl> row = test_urls();
    QTest::addColumn<QUrl>("url");

    for (int i = 0; i < row.count(); ++i)
        QTest::newRow(QString("row%1").arg(i+1).toLatin1()) << row[i];

}

#endif
