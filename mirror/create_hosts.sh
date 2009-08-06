#!/bin/sh

IP=$1

for line in `sqlite3 crawl_db.db "select url from responses;" | sed  s,http://,,g | cut -d '/' -f 1 | sort -u`;
do
echo "$IP	$line"
done

