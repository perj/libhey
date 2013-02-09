
[libhey][] does one thing, it connects sockets, and only stream sockets at that.
But it does it using [Happy Eyeballs][] and it's dead easy to use.

## Functions

* [hey_connect][]
* [hey_strerror][]

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

  [libhey]: http://www.libhey.org
  [hey_connect]: doc/hey_connect.3.html
  [hey_strerror]: doc/hey_strerror.3.html
  [Happy Eyeballs]: http://tools.ietf.org/html/rfc6555
  [cmake]: http://www.cmake.org

## License

libhey is distributed under the BSD 2-clause license, see the various source
files for details.
