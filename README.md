# ccsds
This is my side project to realize CCSDS(The Consultative Committee for Space Data Systems) SDKs.
Today, there are no useful SDKs to encode/decode CCSDS bit stream, so I want to create the one.


# How to build
```
cmake -S . -B build
cmake --build build
```
* binaries are generated in `build/bin/`
* libraries are generated in `build/lib/`

# How to develop
```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build/test --verbose
```
* [google/googletest](https://github.com/google/googletest)
* [GoogleTest User's Guide](https://google.github.io/googletest/)