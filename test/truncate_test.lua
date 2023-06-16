local assert = require('assert')
local fileno = require('io.fileno')
local truncate = require('io.truncate')

local function test_truncate_file()
    -- confirm initial length
    local f = assert(io.tmpfile())
    assert.equal(f:seek('end'), 0)

    -- test that extend length to 10
    assert(truncate(f, 10))
    assert.equal(f:seek('end'), 10)

    -- test that shrink length to 5 and update position
    assert(truncate(f, 5))
    assert.equal(f:seek('end'), 5)
end
test_truncate_file()

local function test_truncate_fd()
    -- confirm initial length
    local f = assert(io.tmpfile())
    assert.equal(f:seek('end'), 0)

    -- test that extend length to 10
    assert(truncate(fileno(f), 10))
    assert.equal(f:seek('end'), 10)

    -- test that shrink length to 5 and update position
    assert(truncate(fileno(f), 5))
    assert.equal(f:seek('end'), 5)

    -- test that return error if fd is invalid
    f:close()
    local ok, err = truncate(fileno(f), 5)
    assert.is_false(ok)
    assert.re_match(err, 'bad file', 'i')
end
test_truncate_fd()

local function test_truncate_filename()
    -- confirm initial length
    local tmpfile = os.tmpname()
    local f = assert(io.open(tmpfile, 'w+'))
    assert.equal(f:seek('end'), 0)

    -- test that extend length to 10
    assert(truncate(tmpfile, 10))
    assert.equal(f:seek('end'), 10)

    -- test that shrink length to 5 and update position
    assert(truncate(tmpfile, 5))
    assert.equal(f:seek('end'), 5)

    -- test that return error if file not exists
    os.remove(tmpfile)
    local ok, err = truncate(tmpfile, 15)
    assert.is_false(ok)
    assert.re_match(err, 'no .* file', 'i')
end
test_truncate_filename()

local function test_truncate_with_invalid_arguments()
    -- test that throw error if first argument is invalid
    local err = assert.throws(truncate, {}, 10)
    assert.match(err, 'bad argument .* string, integer or file', false)

    -- test that throw error if second argument is invalid
    err = assert.throws(truncate, 'test.txt', {})
    assert.match(err, 'bad argument .*number expected', false)
end
test_truncate_with_invalid_arguments()

