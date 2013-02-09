hey_connect(3) -- connect to server
===================================

## SYNOPSIS

    #include <hey.h>

    int
    hey_connect(struct hey *hey, const char *host, const char *port, int timeoutms);

## DESCRIPTION

So far, only basic Happy Eyeballs is supported. The `hey` argument should be `NULL`.

In basic mode, `hey_connect` will call `getaddrinfo` followed by `connect`.
Before calling `connect` it rearranges the addresses to alternate between IPv4 and IPv6.
There's a short timeout on each connect before another one is started in parallel.
The first socket to finish connecting is returned, while any others are closed.

If more than `timeoutms` milliseconds pass before a connection can be established, the
call is aborted.

libhey currently only supports TCP sockets.

## RETURN VALUES

If successful, `hey_connect` will return the connected socket. Note that `O_NONBLOCK`
is always set. On failure a negative value is returned matching one of the errors
listed in `<hey.h>`. In the case of `HEY_ESYSTEM`, the system error is given in `errno`.

## EXAMPLE

    int s = hey_connect(NULL, "www.google.com", "80", 10000);
    char errbuf[256];
    
    if (s < 0)
        errx(1, "hey_connect: %s", hey_strerror(s, errbuf, sizeof (errbuf)));
