#!/bin/sh
# Websites we want to mirror

# English
WEBSITES="http://us.yahoo.com http://google.com/ncr http://maps.google.com http://www.apple.com"

# shopping
WEBSITES="$WEBSITES http://amazon.com http://ebay.com"

# news sites
WEBSITES="$WEBSITES http://bbc.co.uk http://www.guardian.co.uk http://google.com/news/?hl=en http://news.yahoo.com"

# English entertainment
WEBSITES="$WEBSITES http://flickr.com http://www.youtube.com/?hl=en_US http://myspace.com http://www.twitter.com http://twitter.com/BARACKOBAMA"
WEBSITES="$WEBSITES http://reddit.com http://www.facebook.com http://www.gmail.com http://mail.yahoo.com http://www.flickr.com"
WEBSITES="$WEBSITES http://hi5.com http://www.dailymotion.com"

# From the top50 of webites... well adult content... it exists... no kidding... don't go there if you are legally not allowed
WEBSITES="$WEBSITES http://youporn.com"


# Korean pages
WEBSITES="$WEBSITES http://naver.com http://kr.yahoo.com http://www.google.co.kr/ http://www.auction.co.kr http://www.youtube.com/?hl=ko-KR"


for website in $WEBSITES;
do
    echo "Mirroring $website"
    ./mirror $website
done
