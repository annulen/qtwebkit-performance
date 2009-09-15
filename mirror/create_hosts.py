#!/usr/bin/env python

import sqlite3, sys

name = "crawl_db.db"
connection = sqlite3.connect(name)

urls = []
cursor = connection.execute("SELECT url FROM responses")
for row in cursor:
    url = str(row[0]).split("/")[2]
    if not url in urls:
        urls.append(url)

for url in urls:
    print "%s\t%s" % (sys.argv[1], url)

