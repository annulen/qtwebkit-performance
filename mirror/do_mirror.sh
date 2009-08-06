#!/bin/sh
# Websites we want to mirror

# English
WEBSITES="http://us.yahoo.com http://google.com/ncr"

# shopping
WEBSITES="$WEBSITES http://amazon.com http://ebay.com"

# news sites
WEBSITES="$WEBSITES http://bbc.co.uk http://www.guardian.co.uk http://google.com/news/?hl=en http://news.yahoo.com"

# English entertainment
WEBSITES="$WEBSITES http://flickr.com http://www.youtube.com/?hl=en_US http://myspace.com http://www.twitter.com"


# Korean pages
WEBSITES="$WEBSITES http://naver.com http://kr.yahoo.com http://www.google.co.kr/ http://www.auction.co.kr http://www.youtube.com/?hl=ko-KR"


for website in $WEBSITES;
do
    echo "Mirroring $website"
    ./mirror $website
done
