import psycopg2
import os
import json
import http.server
import socketserver
"""
Feb 16th:
Have a database, query the db, print results on a webserver at localhost:8000
Data is static -> updates to db will not be reflected on the website (UNTIL YOU MOVE CURSOR CODE INTO DB GET)

Web Server
* nginx -> better for static data (express or django)
    -> fast because it's not trying to process things in any way (ex: index.html)
    -> reverse proxy (comes in on port 80 and then converts to something like 8000)

HAVE TO:
1. modify ~/.zshrc to set env vars
2. create role "myapp2" and grant SELECT permissions to client to the new role
"""

conn = psycopg2.connect(database = "example",
                        user = os.getenv("MYAPPUSER"),
                        host = 'localhost',
                        password = os.getenv("MYAPPPASSWORD"),
                        port = 5432)
cursor = conn.cursor()

#print(json.dumps(record))

PORT = 8000

class SimpleHTTPRequestHandler(http.server.BaseHTTPRequestHandler):

    def do_GET(self):
        cursor.execute("SELECT * from client")
        record = cursor.fetchall()
        #print("Result ", record)
        result=[]
        for row in record:
            result.append({"id":row[0], "first":row[1],"last":row[2],"address":row[3],"zip":row[4],"phone":row[5]})
        jsonStr = json.dumps(result)
        print(jsonStr)

        self.send_response(200)
        self.end_headers()
        #self.wfile.write(json.dumps(result).encode('unicode_escape')) # This also works!
        self.wfile.write(str.encode(jsonStr))
        #self.wfile.write(b'Hello World!') # this works too (don't want string want bytes-like object)

with socketserver.TCPServer(("", PORT), SimpleHTTPRequestHandler) as httpd:
    print("serving at port ", PORT)
    httpd.serve_forever()