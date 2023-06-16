/**
 *  Copyright (C) 2023 Masatoshi Fukunaga
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a
 *  copy of this software and associated documentation files (the "Software"),
 *  to deal in the Software without restriction, including without limitation
 *  the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *  and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 *  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *  DEALINGS IN THE SOFTWARE.
 */

#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
// lua
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

static inline FILE **checkfilep(lua_State *L, int idx)
{
#if LUA_VERSION_NUM >= 502
    luaL_Stream *stream = luaL_checkudata(L, idx, LUA_FILEHANDLE);
    return &stream->f;

#else
    return (FILE **)luaL_checkudata(L, idx, LUA_FILEHANDLE);
#endif
}

static inline int do_ftruncate_lua(lua_State *L, int fd, lua_Integer len)
{
    if (ftruncate(fd, len) != 0) {
        lua_pushboolean(L, 0);
        lua_pushstring(L, strerror(errno));
        lua_pushinteger(L, errno);
        return 3;
    }
    lua_pushboolean(L, 1);
    return 1;
}

static int truncate_lua(lua_State *L)
{
    switch (lua_type(L, 1)) {
    case LUA_TSTRING: {
        const char *path = lua_tostring(L, 1);
        lua_Integer len  = luaL_optinteger(L, 2, 0);
        if (truncate(path, len) != 0) {
            lua_pushboolean(L, 0);
            lua_pushstring(L, strerror(errno));
            lua_pushinteger(L, errno);
            return 3;
        }
        lua_pushboolean(L, 1);
        return 1;
    }

    case LUA_TNUMBER: {
        int fd          = luaL_checkinteger(L, 1);
        lua_Integer len = luaL_optinteger(L, 2, 0);
        return do_ftruncate_lua(L, fd, len);
    }

    case LUA_TUSERDATA: {
        FILE *fp        = *checkfilep(L, 1);
        lua_Integer len = luaL_optinteger(L, 2, 0);
        return do_ftruncate_lua(L, fileno(fp), len);
    }

    default:
        return luaL_error(L,
                          "bad argument #1: expected string, integer or file*");
    }
}

LUALIB_API int luaopen_io_truncate(lua_State *L)
{
    lua_pushcfunction(L, truncate_lua);
    return 1;
}
