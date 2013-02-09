hey_strerror(3) -- libhey error messages
========================================

## SYNOPSIS

    #include <hey.h>

    const char *
    hey_strerror(int err, char *buf, size_t buflen);

## DESCRIPTION

`hey_strerror` returns a textual representation of the libhey error `err`.
Note that it doessn't necessarily fill `buf`, the return value should be used.

In the case of `HEY_ESYSTEM`, `hey_strerror` will call `strerror_r` to find
the message.

## RETURN VALUES

A string pointer is returned, containing a description of the error.

