from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer


class Handler(SimpleHTTPRequestHandler):
    extensions_map = {
        **SimpleHTTPRequestHandler.extensions_map,
        ".js": "application/javascript",
        ".css": "text/css",
        ".csv": "text/csv",
    }


if __name__ == "__main__":
    server = ThreadingHTTPServer(("127.0.0.1", 8501), Handler)
    print("Dashboard running at http://127.0.0.1:8501/")
    server.serve_forever()
