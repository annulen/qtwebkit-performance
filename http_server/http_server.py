#!/usr/bin/env python

import BaseHTTPServer
import sqlite3, sys


def create_uri(headers, path):
    """Attempt to create the URI from the Host: tag and path"""

    for header in str(headers).split("\r\n"):
        if header.lower().startswith("host: "):
            host = header[len("host: "):]
            return "http://%s%s" % (host, path)

    return None

class HTTPHandler(BaseHTTPServer.BaseHTTPRequestHandler):

    def do_GET(self):
        uri = create_uri(self.headers, self.path)

        global connection
        cur = connection.execute("select header, data from responses where url = ?", (uri,))
        for row in cur:
            headers = row[0]
            data = row[1]

            if not headers and not data:
                return self.send_error(404, "Test server is broken??? '%s' '%s'" % (self.path, uri))

            # take the header apart to let it put together..
            self.send_response(200)
            for header in str(headers).split("\r\n"):
                if not header:
                    break

                split = header.split(':', 1)
                self.send_header(split[0][:-1], split[1])

            self.end_headers()
            self.wfile.write(data)
            return

    def do_POST(self):
        print "Post is not supported"
        self.send_error(404, "POST is not supported")

def main():
    global connection
    if len(sys.argv) >= 2:
        print "Using %s" % sys.argv[1]
        connection = sqlite3.connect(sys.argv[1])
    else:
        connection = sqlite3.connect("crawl_db.db")

    try:
        server = BaseHTTPServer.HTTPServer(('', 80), HTTPHandler)
        print 'Started httpserver for pageload test...'
        server.serve_forever()
    except KeyboardInterrupt:
        print '^C received, bye bye'
        server.socket.close()

if __name__ == '__main__':
    main()

