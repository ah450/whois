whois
=====

A multithreaded basic whois commandline utility, because why not?

How to build
============
(assuming you are in repo root)

First create a build directory `mkdir build`

Change directory to the one you just created `cd build`

Run cmake `cmake ..`

Make it `make`

Then run whois `./whois <ip list>`

<ip list> is a list of space seperated IPs either ipv4 or ipv6 or mixed.
