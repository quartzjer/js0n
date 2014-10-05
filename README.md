# js0n - Complete Zero-Footprint JSON Parser in C

A one-pass minimal overhead walk of the raw bytes, using each one as an index into a jump table to determine state transitions.

* public domain, single js0n.c file with one function
* more flexible than strict JSON and allows bare keys
* requires no memory/malloc, uses only what is passed in
* no memory copying
* optimized for high speed scanning/routing of small chunks of json
* finds a given key-value pair location in the string

Parsing this:

````json
{"foo":"bar","barbar":[1,2,3],"obj":{"a":"b"}}
````

Using `at = js0n("barbr", json, len, &vlen)` would return an at pointing to `[1,2,3]` and set vlen to `7`.


## History

See [v1.0] for the original `js0n` function that returned an index of offsets for all key/values in one pass.
