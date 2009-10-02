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
void add_test_urls()
{
    QTest::addColumn<QUrl>("url");

    QTest::newRow("row01") << QUrl("http://us.yahoo.com");
    QTest::newRow("row02") << QUrl("http://google.com/ncr");
    QTest::newRow("row03") << QUrl("http://maps.google.com");
    QTest::newRow("row04") << QUrl("http://www.apple.com");
    QTest::newRow("row05") << QUrl("http://amazon.com");
    QTest::newRow("row06") << QUrl("http://ebay.com");
    QTest::newRow("row07") << QUrl("http://bbc.co.uk");
    QTest::newRow("row08") << QUrl("http://www.guardian.co.uk");
    QTest::newRow("row09") << QUrl("http://google.com/news/?hl=en");
    QTest::newRow("row10") << QUrl("http://news.yahoo.com");
    QTest::newRow("row11") << QUrl("http://flickr.com");
    QTest::newRow("row12") << QUrl("http://www.youtube.com/?hl=en_US");
    QTest::newRow("row13") << QUrl("http://myspace.com");
    QTest::newRow("row14") << QUrl("http://www.twitter.com");
    QTest::newRow("row15") << QUrl("http://twitter.com/BARACKOBAMA");
    QTest::newRow("row16") << QUrl("http://reddit.com");
    QTest::newRow("row17") << QUrl("http://www.facebook.com");
    QTest::newRow("row18") << QUrl("http://www.gmail.com");
    QTest::newRow("row19") << QUrl("http://mail.yahoo.com");
    QTest::newRow("row20") << QUrl("http://www.flickr.com");
    QTest::newRow("row21") << QUrl("http://hi5.com");
    QTest::newRow("row22") << QUrl("http://www.dailymotion.com");
    QTest::newRow("row23") << QUrl("http://youporn.com");
    QTest::newRow("row24") << QUrl("http://www.microsoft.com");
    QTest::newRow("row25") << QUrl("http://www.live.com");
    QTest::newRow("row26") << QUrl("http://www.wikipedia.org");
    QTest::newRow("row27") << QUrl("http://en.wikipedia.org");
    QTest::newRow("row28") << QUrl("http://www.wordpress.com");
    QTest::newRow("row29") << QUrl("http://www.rapidshare.com");
    QTest::newRow("row30") << QUrl("http://www.blogger.com");
    QTest::newRow("row31") << QUrl("http://www.bing.com");
    QTest::newRow("row32") << QUrl("http://www.imdb.com");
    QTest::newRow("row33") << QUrl("http://www.linkedin.com");
    QTest::newRow("row34") << QUrl("http://en.wikipedia.org/wiki/Maxwell_equations");
    QTest::newRow("row35") << QUrl("http://naver.com");
    QTest::newRow("row36") << QUrl("http://kr.yahoo.com");
    QTest::newRow("row37") << QUrl("http://www.google.co.kr/");
    QTest::newRow("row38") << QUrl("http://www.auction.co.kr");
    QTest::newRow("row39") << QUrl("http://www.youtube.com/?hl=ko-KR");
    QTest::newRow("row40") << QUrl("http://www.linuxtogo.org/~zecke/image-stress-test.html");
}

#endif
