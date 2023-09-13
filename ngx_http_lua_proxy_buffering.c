#include <ngx_core.h>
#include <ngx_http.h>
#include "ngx_http_lua_api.h"
#include "lauxlib.h"

ngx_module_t ngx_http_lua_proxy_buffering_module;

static ngx_int_t ngx_http_lua_proxy_buffering_init(ngx_conf_t *cf);

static ngx_http_module_t ngx_http_lua_proxy_buffering_ctx = {
    NULL,                              /* preconfiguration */
    ngx_http_lua_proxy_buffering_init, /* postconfiguration */
    NULL,                              /* create main configuration */
    NULL,                              /* init main configuration */
    NULL,                              /* create server configuration */
    NULL,                              /* merge server configuration */
    NULL,                              /* create location configuration */
    NULL                               /* merge location configuration */
};

ngx_module_t ngx_http_lua_proxy_buffering_module = {
    NGX_MODULE_V1,
    &ngx_http_lua_proxy_buffering_ctx, /* module context */
    NULL,                              /* module directives */
    NGX_HTTP_MODULE,                   /* module type */
    NULL,                              /* init master */
    NULL,                              /* init module */
    NULL,                              /* init process */
    NULL,                              /* init thread */
    NULL,                              /* exit thread */
    NULL,                              /* exit process */
    NULL,                              /* exit master */
    NGX_MODULE_V1_PADDING};

static int set_proxy_request_buffering(lua_State *L)
{
    ngx_http_request_t *r;
    int n;
    int buffering;
    n = lua_gettop(L);

    r = ngx_http_lua_get_request(L);
    if (n != 1)
    {
        return luaL_error(L, "expecting 1 arguments but seen %d", n);
    }

    buffering = lua_toboolean(L, 1);

    r = ngx_http_lua_get_request(L);
    r->request_body_no_buffering = !buffering;
    return 0;
}

static int set_proxy_buffering(lua_State *L)
{
    ngx_http_request_t *r;
    ngx_http_upstream_t *u;
    int n;
    int buffering;
    n = lua_gettop(L);

    if (n != 1)
    {
        return luaL_error(L, "expecting 1 arguments but seen %d", n);
    }

    buffering = lua_toboolean(L, 1);

    r = ngx_http_lua_get_request(L);
    u = r->upstream;

    if (u == NULL)
    {
        return NGX_ERROR;
    }
    u->buffering = buffering;
    return 0;
}

static int luaopen_proxy_buffering(lua_State *L)
{
    lua_createtable(L, 0, 1);
    lua_pushcfunction(L, set_proxy_request_buffering);
    lua_setfield(L, -2, "set_proxy_request_buffering");
    lua_pushcfunction(L, set_proxy_buffering);
    lua_setfield(L, -2, "set_proxy_buffering");

    return 1;
}

static ngx_int_t
ngx_http_lua_proxy_buffering_init(ngx_conf_t *cf)
{
    if (ngx_http_lua_add_package_preload(cf, "proxy.buffering",
                                         luaopen_proxy_buffering) != NGX_OK)
    {
        return NGX_ERROR;
    }

    return NGX_OK;
}