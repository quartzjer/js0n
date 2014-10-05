# js0n - Complete Zero-Footprint JSON Parser in C

A one-pass minimal overhead walk of the raw bytes, using each one as an index into a jump table to determine state transitions.

* public domain, single js0n.c file with one function
* no dependencies, embedded friendly
* more flexible than strict JSON and allows bare keys
* requires no memory/malloc or copying, uses only what is passed in
* optimized for high speed scanning/routing of small chunks of json
* easy to use, just returns a given key-value pair location in the json string

Parsing this:

````json
{"foo":"bar","barbar":[1,2,3],"obj":{"a":"b"}}
````

Using `at = js0n("barbar", &len, json, strlen(json))` would return an at pointing to `[1,2,3]` and set len to `7`.


## History

See [v1.0] for the original `js0n` function that returned an index of offsets for all key/values in one pass.
