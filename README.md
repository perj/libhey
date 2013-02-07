
libhey does one thing, it connects sockets, and only stream sockets at that.
But it does it using [Happy Eyeballs][] and it's dead easy to use.

## Synopsis

    #include <hey.h>

    int
    hey_connect(struct hey *hey, const char *host, const char *port, int timeoutms);

    const char *
    hey_strerror(int err, char *buf, size_t buflen)

## Description

So far, only basic Happy Eyeballs is supported. The `hey` argument should be `NULL`.

In basic mode, `hey_connect` will call `getaddrinfo` followed by `connect`.
Before calling `connect` it rearranges the addresses to alternate between IPv4 and IPv6.
There's a short timeout on each connect before another one is started in parallel.
The first socket to finish connecting is returned, while any others are closed.

If more than `timeoutms` milliseconds pass before a connection can be established, the
call is aborted.

libhey only supports `SOCK_STREAM` sockets, usually TCP.

## Return value

If successful, `hey_connect` will return the connected socket. Note that `O_NONBLOCK`
is always set. On failure a negative value is returned matching one of the errors
listed in `<hey.h>`. In the case of `HEY_ESYSTEM`, the system error is given in `errno`.

`hey_strerror` returns a textual representation of the libhey error `err`. Note that
it doessn't necessarily fill `buf`, the return value should be used.

## Example

    int s = hey_connect(NULL, "www.google.com", "80", 10000);
    char errbuf[256];
    
    if (s < 0)
        errx(1, "hey_connect: %s", hey_strerror(s, errbuf, sizeof (errbuf)));

## Compiling

libhey is build using [cmake][]. To install in the default prefix /usr/local:

    cmake .
    make
    sudo make install

You can change the prefix by adding `-DCMAKE_INSTALL_PREFIX`:

    cmake -DCMAKE_INSTALL_PREFIX=/usr .

You can also replace cmake with ccmake to interactively edit flags.

For development/hacking, it's recommended that you use a separate build
directory, and add the flag `-DCMAKE_BUILD_TYPE=Debug`:

    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Debug ..
    make

  [Happy Eyeballs]: http://tools.ietf.org/html/rfc6555
  [cmake]: http://www.cmake.org

## License
 
libhey is distributed under the BSD 2-clause license, see the various source
files for details.
