# lua-io-truncate

[![test](https://github.com/mah0x211/lua-io-truncate/actions/workflows/test.yml/badge.svg)](https://github.com/mah0x211/lua-io-truncate/actions/workflows/test.yml)
[![codecov](https://codecov.io/gh/mah0x211/lua-io-truncate/branch/master/graph/badge.svg)](https://codecov.io/gh/mah0x211/lua-io-truncate)

truncate or extend a file to a specified length.

## Installation

```sh
luarocks install io-truncate
```

---

## ok, err, errno = truncate( file, length )

truncate or extend a file to a specified length.

**Parameters**

- `file:string|file*|integer`: filepath or file object or file descriptor.
- `length:number`: file length.

**Returns**

- `ok:boolean`: `true` on success, or `false` on failure.
- `err:string`: error message.
- `errno:number`: error number.

**Example**

```lua
local truncate = require('io.truncate')
local f = io.tmpfile()
f:write('hello')
f:flush() -- must be called before truncate
truncate(f, 3) -- shrink to 3 bytes
f:seek('set')
print(f:read('*a')) -- hel
f:close()
```
