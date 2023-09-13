# vim:set ft= ts=4 sw=4 et fdm=marker:

use Test::Nginx::Socket;

plan tests => repeat_each() * (blocks() * 3);

no_long_string();

run_tests();

__DATA__

=== TEST 1: example echo
--- config
    location = /test {
        content_by_lua '
            local buffering = require "proxy.buffering"
            buffering.set_proxy_buffering(false)
            buffering.set_proxy_request_buffering(false)
            ngx.say("hello")
        ';
    }
--- request
GET /test
--- response_body
hello
--- no_error_log
[error]