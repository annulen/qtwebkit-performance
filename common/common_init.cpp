#include "common_init.h"

void add_url_to_test_urls(const QUrl &url) {
    globalUrlList << url;
}

QList<QUrl> test_urls()
{
    if (globalUrlList.isEmpty()) {
        // populate with the old standard test set
        /**
         * Created from do_mirror.sh with the following shell foo:
         *  for site in $WEBSITES; do echo 'QTest::newRow("rowXY") <<' QUrl(\"$site\"\);; done
         */

        globalUrlList << QUrl("http://us.yahoo.com");
        globalUrlList << QUrl("http://www.google.com/ncr");
        globalUrlList << QUrl("http://maps.google.com");
        globalUrlList << QUrl("http://www.apple.com");
        globalUrlList << QUrl("http://amazon.com");
        globalUrlList << QUrl("http://ebay.com");
        globalUrlList << QUrl("http://bbc.co.uk");
        globalUrlList << QUrl("http://www.guardian.co.uk");
        globalUrlList << QUrl("http://www.google.com/news?ned=us");
        globalUrlList << QUrl("http://news.yahoo.com");
        globalUrlList << QUrl("http://flickr.com");
        globalUrlList << QUrl("http://www.youtube.com/?hl=en_US");
        globalUrlList << QUrl("http://myspace.com");
        globalUrlList << QUrl("http://www.twitter.com");
        globalUrlList << QUrl("http://twitter.com/BARACKOBAMA");
        globalUrlList << QUrl("http://reddit.com");
        globalUrlList << QUrl("http://www.facebook.com");
        globalUrlList << QUrl("http://www.gmail.com");
        globalUrlList << QUrl("http://mail.yahoo.com");
        globalUrlList << QUrl("http://hi5.com");
        globalUrlList << QUrl("http://www.dailymotion.com");
        globalUrlList << QUrl("http://youporn.com");
        globalUrlList << QUrl("http://www.microsoft.com/en/us/default.aspx");
        globalUrlList << QUrl("http://www.live.com");
        globalUrlList << QUrl("http://www.wikipedia.org");
        globalUrlList << QUrl("http://en.wikipedia.org");
        globalUrlList << QUrl("http://www.wordpress.com");
        globalUrlList << QUrl("http://www.rapidshare.com");
        globalUrlList << QUrl("http://www.blogger.com");
        globalUrlList << QUrl("http://www.bing.com");
        globalUrlList << QUrl("http://www.imdb.com");
        globalUrlList << QUrl("http://www.linkedin.com");
        globalUrlList << QUrl("http://en.wikipedia.org/wiki/Maxwell_equations");
        globalUrlList << QUrl("http://naver.com");
        globalUrlList << QUrl("http://kr.yahoo.com");
        globalUrlList << QUrl("http://www.google.co.kr/");
        globalUrlList << QUrl("http://www.auction.co.kr");
        globalUrlList << QUrl("http://www.youtube.com/?hl=ko-KR");
    }
    return globalUrlList;
}

void add_test_urls()
{
    QList<QUrl> row = test_urls();
    QTest::addColumn<QUrl>("url");

    for (int i = 0; i < row.count(); ++i)
        QTest::newRow(QString("row%1").arg(i+1).toLatin1()) << row[i];

}
