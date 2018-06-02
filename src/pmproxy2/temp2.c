#include <uv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <limits.h>
#include </home/prajwal/hiredis/hiredis.h>
#include </home/prajwal/hiredis/net.h>
#include </home/prajwal/hiredis/sds.h>
#include </home/prajwal/hiredis/async.h>
#include </home/prajwal/hiredis/adapters/libuv.h>

#define PORT 44323 /* TODO make port dynamic */

struct sockaddr_in addr;

typedef struct {
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;

int64_t ClientCounter = 0;

struct KeyNode {
    sds key;
    struct KeyNode *next;
    struct KeyNode *prev;
};

struct ClientRequestData {
    redisAsyncContext *redisContext;
    sds ip;
    uv_tcp_t *client;
    struct KeyNode *keys;
    int64_t ClientID;
};

FILE *fp;

sds
ProcessRedisReply(redisReply *reply) {
    sds             end_line;
    sds             sdsbuffer;
    end_line = sdsempty();
    end_line = sdscat(end_line, "\r\n");
    sdsbuffer = sdsempty();
    if (reply == NULL) sdsbuffer;
    switch ((reply->type)) {
        case 1 : {
            //REDIS_REPLY_STRING
            sds sometempbuf = sdsfromlonglong((strlen(reply->str)));
            sdsbuffer = sdscat(sdsbuffer, "$");
            sdsbuffer = sdscat(sdsbuffer, sometempbuf);
            sdsbuffer = sdscat(sdsbuffer, end_line);
            sdsbuffer = sdscat(sdsbuffer, reply->str);
            sdsbuffer = sdscat(sdsbuffer, end_line);
            sdsfree(sometempbuf);
            return sdsbuffer;
        }
        case 2 : {
            //REDIS_REPLY_ARRAY
            int64_t i;
            sds sometempbuf = sdsfromlonglong(((redisReply *) reply)->elements);
            sdsbuffer = sdscat(sdsbuffer, "*");
            sdsbuffer = sdscat(sdsbuffer, sometempbuf);
            sdsbuffer = sdscat(sdsbuffer, end_line);
            for (i = 0; i < ((redisReply *) reply)->elements; i++) {
                sdsbuffer = sdscat(sdsbuffer, ProcessRedisReply((redisReply *) reply->element[i]));
            }
            sdsfree(sometempbuf);
            return sdsbuffer;
        }
        case 3: {
            //REDIS_REPLY_INTEGER
            sds sometempbuf = sdsfromlonglong(reply->integer);
            sdsbuffer = sdscat(sdsbuffer, ":");
            sdsbuffer = sdscat(sdsbuffer, sometempbuf);
            sdsbuffer = sdscat(sdsbuffer, end_line);
            sdsfree(sometempbuf);
            return sdsbuffer;
        }
        case 4 : {
            //REDIS_REPLY_NULL
            sdsbuffer = sdscat(sdsbuffer, "");
            return sdsbuffer;
        }
        case 5: {
            //REDIS_REPLY_STATUS
            sdsbuffer = sdscat(sdsbuffer, "+");
            sdsbuffer = sdscat(sdsbuffer, reply->str);
            sdsbuffer = sdscat(sdsbuffer, end_line);
            return sdsbuffer;
        }
        default: {
            //REDIS_REPLY_ERROR
            sdsbuffer = sdscat(sdsbuffer, "-");
            sdsbuffer = sdscat(sdsbuffer, reply->str);
            sdsbuffer = sdscat(sdsbuffer, end_line);
            return sdsbuffer;
        }
    }
}

static void
on_close_cb(uv_handle_t *handle) {
    free(handle);
}

static void
after_write(uv_write_t *req, int status) {
    write_req_t *wr = (write_req_t *) req;
    if (wr->buf.base != NULL) {}
    free(wr);
    if (status == 0) return;
    fprintf(stderr, "uv_write error: %s\n", uv_strerror(status));
    if (status == UV_ECANCELED) return;
    assert(status == UV_EPIPE);
    uv_close((uv_handle_t *) req->handle, on_close_cb);
}

static void
after_shutdown(uv_shutdown_t *req, int status) {
    struct ClientRequestData        *data;

    data = req->handle->data;
    redisAsyncDisconnect(data->redisContext);
    if (status < 0) {
        fprintf(stderr, "%s\n", uv_strerror(status));
    }
    uv_close((uv_handle_t *) req->handle, on_close_cb);
    free(req);
}

static void
getCallback(redisAsyncContext *c, void *r, void *privdata){
    struct ClientRequestData    *data;
    int                         len;
    int                         i;

    sds s = sdsempty();
    redisReply *reply = r;
    write_req_t *wr = (write_req_t *) malloc(sizeof(*wr));

    if (reply == NULL) return;

    data = c->data;
    s = ProcessRedisReply(reply);
    printf("%s",s);
    wr->buf = uv_buf_init(s, sdslen(s));
    r = uv_write(&wr->req, data->client, &wr->buf, 1, after_write);
}
static void
after_read(uv_stream_t *handle, ssize_t nread, const uv_buf_t *buf) {
    struct ClientRequestData            *data;
    redisReader                         *reader;
    void                                *reply;
    int                                 ret;
    int                                 i;
    int                                 r;
    redisContext                        *c;
    write_req_t                         *wr;
    uv_shutdown_t                       *req;
    sds                                 sdsbuffer;
    sds                                 getkeysds;
    if (nread <= 0 && buf->base != NULL) {
        free(buf->base);
    }
    if (nread == 0) return;

    if (nread < 0) {
        fprintf(stderr, "err: %s\n", uv_strerror(nread));

        req = (uv_shutdown_t *) malloc(sizeof(*req));
        assert(req != NULL);

        r = uv_shutdown(req, handle, after_shutdown);
        assert(r == 0);

        return;
    }
    wr = (write_req_t *) malloc(sizeof(*wr));
    assert(wr != NULL);

    data = handle->data;
   // printf("Buffer : %s\n", buf->base);
   // printf("Buffer len : %d\n", strlen(buf->base));
    reader = redisReaderCreate();
    redisReaderFeed(reader, (char *) (buf->base), strlen(buf->base));
    ret = redisReaderGetReply(reader, &reply);

    assert(ret == REDIS_OK);

    sdsbuffer = sdsempty();
    getkeysds = sdsempty();

    for (i = 0; reply != NULL && i < ((redisReply *) reply)->elements; i++) {
        sdsbuffer = sdscat(sdsbuffer, ((redisReply *) reply)->element[i]->str);
        if (i != (((redisReply *) reply)->elements) - 1) sdsbuffer = sdscat(sdsbuffer, " ");
    }
    printf("Input command : %s\n", sdsbuffer);
    char *command = malloc(sdslen(sdsbuffer));
    strcpy(command, sdsbuffer);

    getkeysds = sdscat(getkeysds,"COMMAND GETKEYS");
    getkeysds = sdscat(getkeysds, command);

    //redisAsyncCommand(data->redisContext, getCallback, (char*)"end-1",getkeysds);

    redisAsyncCommand(data->redisContext, getCallback, (char*)"end-1",command);

    fp=fopen("/home/prajwal/CLionProjects/pcp/src/pmproxy2/LOG.txt","a+");
    fputs(command,fp);
    fputs("\n",fp);
    fclose(fp);
    sdsfree(sdsbuffer);
    sdsfree(getkeysds);
    freeReplyObject(reply);
}

static void
alloc_cb(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    buf->base = malloc(suggested_size);
    assert(buf->base != NULL);
    buf->len = suggested_size;
}

static void
connectCallback(const redisAsyncContext *c, int status) {
    if (status != REDIS_OK) {
        printf("Error: %s\n", c->errstr);
        return;
    }
    printf("Connected...\n");
}
static void
disconnectCallback(const redisAsyncContext *c, int status) {
    if (status != REDIS_OK)
    {
        printf("Error: %s\n", c->errstr);
        return;
    }
    printf("Disconnected...\n");
}
static void
on_connection(uv_stream_t *server, int status) {
    uv_tcp_t            *stream;
    int                 r;
    char                addr[16];
    char                client_ip[32];
    struct sockaddr_in name;
    struct ClientRequestData *data = malloc(sizeof(*data));

    int namelen = sizeof(name);
    ClientCounter++;
    redisAsyncContext *c = redisAsyncConnect("127.0.0.1", 6379);
    redisLibuvAttach(c, uv_default_loop());
    redisAsyncSetConnectCallback(c, connectCallback);
    if (c->err) {
        printf("Error: %s\n", c->errstr);
        return;
    }
    redisLibuvAttach(c,uv_default_loop());
    redisAsyncSetConnectCallback(c,connectCallback);
    redisAsyncSetDisconnectCallback(c,disconnectCallback);
    assert(status == 0);

    stream = malloc(sizeof(uv_tcp_t));
    assert(stream != NULL);

    r = uv_tcp_init(uv_default_loop(), stream);
    assert(r == 0);

    stream->data = server;
    data->redisContext = c;
    c->data = data;
    stream->data = data;
    data->client = stream;

    r = uv_accept(server, (uv_stream_t *) stream);
    assert(r == 0);

    uv_tcp_getpeername(&stream, (struct sockaddr *) &name, &namelen);
    uv_inet_ntop(AF_INET, &name.sin_addr, addr, sizeof(addr));
    snprintf(client_ip, sizeof(client_ip), "%s:%d", addr, ntohs(name.sin_port));
    data->ip = sdsnew(client_ip);
    data->ClientID = ClientCounter;
    redisAsyncCommand(data->redisContext,NULL, NULL,"COMMAND");

    r = uv_read_start((uv_stream_t *) stream, alloc_cb, after_read);
    assert(r == 0);
}

static int tcp_echo_server() {
    uv_tcp_t        *tcp_server;
    int             r;

    r = uv_ip4_addr("0.0.0.0", PORT, &addr);
    assert(r == 0);

    tcp_server = (uv_tcp_t *) malloc(sizeof(*tcp_server));
    assert(tcp_server != NULL);

    r = uv_tcp_init(uv_default_loop(), tcp_server);
    assert(r == 0);

    r = uv_tcp_bind(tcp_server, (const struct sockaddr *) &addr, 0);
    assert(r == 0);

    uv_tcp_keepalive(tcp_server, 1, 50);
    uv_tcp_simultaneous_accepts(tcp_server, 1);

    r = uv_listen((uv_stream_t *) tcp_server, SOMAXCONN, on_connection);
    if (r) {
        fprintf(stderr, "LISTEN ERROR %s\n", uv_strerror(r));
        return 1;
    }
    return 0;
}

int main() {
    int         r;

    r = tcp_echo_server();
    assert(r == 0);

    r = uv_run(uv_default_loop(), UV_RUN_DEFAULT);
    assert(r == 0);

    return 0;
}