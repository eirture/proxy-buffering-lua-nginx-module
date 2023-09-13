
# Proxy Buffering Module

set `proxy_buffering` and `proxy_request_buffering` in lua

## Build

You can use the `--add-module=/path/proxy-buffering-lua-nginx-module` options of OpenResty's `./configure` tool to add this NGINX C module. see more details in [OpenResty: building from source](https://openresty.org/en/installation.html#building-from-source)

## Usage

```nginx
server {
  listen 80;
  server_name ~.*;

  location / {
    client_max_body_size 1024m;
    client_body_buffer_size 8k;
    client_body_in_file_only on;
    client_body_in_single_buffer on;

    access_by_lua_block {
        local h, err = ngx.req.get_headers()
        if err == "trucated" then
            ngx.log(ngx.ERR, "read headers failed")
        end
        if h["X-No-Request-Buffering"] then
            local pb = require 'proxy.buffering'
            pb.set_proxy_request_buffering(false)
            pb.set_proxy_buffering(false)
        end
    }

    proxy_redirect off;
    proxy_set_header Host $http_host;
    proxy_pass http://backend;
  }
}
```

## License

proxy-buffering-lua-nginx-module is released under the Apache 2.0 license. See LICENSE.txt
