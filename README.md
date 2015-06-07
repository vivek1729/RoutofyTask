# RoutofyTask
A simple web crawler that extracts bus information from http://www.travelyaari.com/ and updates in redis server (C++)

I use libcurl to make the http request. The redis client I use https://github.com/brianwatling/redispp depends on boost library.

Do the following steps to run the program:

* sudo apt-get install libboost-all-dev (one time to esnure dependcies)
* make
* Start the redis server before running the executable (redis-server)
* ./app
