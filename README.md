# Basic, Dummy, HTTP Server to browse PostgreSQL database

This project's aim is to learn C++. A moderately complex project is to offer an HTTP server to navigate a PostgreSQL database and view tables and table contents.

## Architecture

* `Server` is responsible for low-level socket and connection handling. As soon as a connection is received, the connection is passed to a callback which does the actual parsing. The remote connection is hidden by using a plain stream, which enables running testing the HTTP parser without using real sockets.
* `HttpParser` is responsible for HTTP parsing. It returns a tuple of `(method, path, params)`. `params` is a Hash-like structure, to enable updates.
* `HttpRouter` is responsible for handling the pair `(method, path)` and calling into application code.
* `PGBrowser` provides functions to browse a PostgreSQL database.
* `PGBrowserRouter` is the glue that connects the HTTP world with the `PGBrowser`: in Rails parlance, we would say this is the controller.
