# js0n - Flexible Zero-Footprint JSON Parser in C

A one-pass minimal overhead walk of the raw bytes, using each one as an index into a jump table to determine state transitions.

* public domain or MIT license, single js0n.c file with one function
* easy to use, just returns a given key-value pair location in the JSON string
* no dependencies, embedded friendly
* requires no memory/malloc or copying, uses only what is passed in (up to `int` max length)
* more flexible than strict JSON and allows bare keys (non-validating)
* optimized for high speed scanning/routing of small chunks of json, stops parsing upon match
* safely errors on anything really bad (binary, NULLs, etc)

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

For arrays, pass `NULL` as the key, and the array offset as the second argument.

When the value is not found `NULL` is returned, if there were any parsing errors then vlen will be set to `-1`.

To determine if the returned value is an actual JSON string or a bare value (like `"true"` vs `true`), simply check if it starts with a quote character via `if(val && *(val-1) == '"')`.

For more usage examples see the [test.json](test/test.json) and [test.c](test/test.c).

## History

See [v1.0](https://github.com/quartzjer/js0n/tree/v1.0) for the original `js0n` function that returned an index of offsets for all key/values in one pass.
