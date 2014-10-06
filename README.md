# js0n - Flexible Zero-Footprint JSON Parser in C

A one-pass minimal overhead walk of the raw bytes, using each one as an index into a jump table to determine state transitions.

* public domain, single js0n.c file with one function
* no dependencies, embedded friendly
* more flexible than strict JSON and allows bare keys
* requires no memory/malloc or copying, uses only what is passed in
* optimized for high speed scanning/routing of small chunks of json
* easy to use, just returns a given key-value pair location in the json string

Parsing this:

````json
{
    "barbar": [
        1,
        2,
        3
    ],
    "foo": "bar",
    "obj": {
        "a": "b"
    }
}

````

Using `val = js0n("barbar", 6, json, strlen(json), &vlen)` would return a val pointing to `[1,2,3]` and set vlen to `7`.

For more usage examples see the [test.json](test/test.json) and [test.c](test/test.c).

## History

See [v1.0](https://github.com/quartzjer/js0n/tree/v1.0) for the original `js0n` function that returned an index of offsets for all key/values in one pass.
