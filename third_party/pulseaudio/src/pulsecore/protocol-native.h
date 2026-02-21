#ifndef fooprotocolnativehfoo
#define fooprotocolnativehfoo

/***
  This file is part of PulseAudio.

  Copyright 2004-2006 Lennart Poettering
  Copyright 2006 Pierre Ossman <ossman@cendio.se> for Cendio AB

  PulseAudio is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published
  by the Free Software Foundation; either version 2.1 of the License,
  or (at your option) any later version.

  PulseAudio is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with PulseAudio; if not, see <http://www.gnu.org/licenses/>.
***/
// GCC does not warn for unused *static inline* functions, but clang does.
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"
#endif

#include <pulsecore/core.h>
#include <pulsecore/ipacl.h>
#include <pulsecore/auth-cookie.h>
#include <pulsecore/iochannel.h>
#include <pulsecore/module.h>
#include <pulsecore/modargs.h>
#include <pulsecore/strlist.h>
#include <pulsecore/hook-list.h>
#include <pulsecore/pstream.h>
#include <pulsecore/tagstruct.h>

typedef struct pa_native_protocol pa_native_protocol;

typedef struct pa_native_connection pa_native_connection;

typedef struct pa_native_options {
    PA_REFCNT_DECLARE;

    pa_module *module;

    bool auth_anonymous;
    bool srbchannel;
    char *auth_group;
    pa_ip_acl *auth_ip_acl;
    pa_auth_cookie *auth_cookie;
} pa_native_options;

typedef enum pa_native_hook {
    PA_NATIVE_HOOK_SERVERS_CHANGED,
    PA_NATIVE_HOOK_CONNECTION_PUT,
    PA_NATIVE_HOOK_CONNECTION_UNLINK,
    PA_NATIVE_HOOK_MAX
} pa_native_hook_t;


struct pa_native_protocol;

typedef struct record_stream {
    pa_msgobject parent;

    pa_native_connection *connection;
    uint32_t index;

    pa_source_output *source_output;
    pa_memblockq *memblockq;

    bool adjust_latency : 1;
    bool early_requests : 1;

    /* Requested buffer attributes */
    pa_buffer_attr buffer_attr_req;
    /* Fixed-up and adjusted buffer attributes */
    pa_buffer_attr buffer_attr;

    pa_atomic_t on_the_fly;
    pa_usec_t configured_source_latency;
    size_t drop_initial;

    /* Only updated after SOURCE_OUTPUT_MESSAGE_UPDATE_LATENCY */
    size_t on_the_fly_snapshot;
    pa_usec_t current_monitor_latency;
    pa_usec_t current_source_latency;
} record_stream;

#define RECORD_STREAM(o) (record_stream_cast(o))
PA_DEFINE_PRIVATE_CLASS(record_stream, pa_msgobject);

typedef struct output_stream {
    pa_msgobject parent;
} output_stream;

#define OUTPUT_STREAM(o) (output_stream_cast(o))
PA_DEFINE_PRIVATE_CLASS(output_stream, pa_msgobject);

typedef struct playback_stream {
    output_stream parent;

    pa_native_connection *connection;
    uint32_t index;

    pa_sink_input *sink_input;
    pa_memblockq *memblockq;

    bool adjust_latency : 1;
    bool early_requests : 1;

    bool is_underrun : 1;
    bool drain_request : 1;
    uint32_t drain_tag;
    uint32_t syncid;

    /* Optimization to avoid too many rewinds with a lot of small blocks */
    pa_atomic_t seek_or_post_in_queue;
    int64_t seek_windex;

    pa_atomic_t missing;
    pa_usec_t configured_sink_latency;
    /* Requested buffer attributes */
    pa_buffer_attr buffer_attr_req;
    /* Fixed-up and adjusted buffer attributes */
    pa_buffer_attr buffer_attr;

    /* Only updated after SINK_INPUT_MESSAGE_UPDATE_LATENCY */
    int64_t read_index, write_index;
    size_t render_memblockq_length;
    pa_usec_t current_sink_latency;
    uint64_t playing_for, underrun_for;
} playback_stream;

#define PLAYBACK_STREAM(o) (playback_stream_cast(o))
PA_DEFINE_PRIVATE_CLASS(playback_stream, output_stream);

typedef struct upload_stream {
    output_stream parent;

    pa_native_connection *connection;
    uint32_t index;

    pa_memchunk memchunk;
    size_t length;
    char *name;
    pa_sample_spec sample_spec;
    pa_channel_map channel_map;
    pa_proplist *proplist;
} upload_stream;

#define UPLOAD_STREAM(o) (upload_stream_cast(o))
PA_DEFINE_PRIVATE_CLASS(upload_stream, output_stream);

struct pa_native_connection {
    pa_msgobject parent;
    pa_native_protocol *protocol;
    pa_native_options *options;
    bool authorized : 1;
    bool is_local : 1;
    uint32_t version;
    pa_client *client;
    /* R/W mempool, one per client connection, for srbchannel transport.
     * Both server and client can write to this shm area.
     *
     * Note: This will be NULL if our connection with the client does
     * not support srbchannels */
    pa_mempool *rw_mempool;
    pa_pstream *pstream;
    pa_pdispatch *pdispatch;
    pa_idxset *record_streams, *output_streams;
    uint32_t rrobin_index;
    pa_subscription *subscription;
    pa_time_event *auth_timeout_event;
    pa_srbchannel *srbpending;
};

#define PA_NATIVE_CONNECTION(o) (pa_native_connection_cast(o))
PA_DEFINE_PRIVATE_CLASS(pa_native_connection, pa_msgobject);

struct pa_native_protocol {
    PA_REFCNT_DECLARE;

    pa_core *core;
    pa_idxset *connections;

    pa_strlist *servers;
    pa_hook hooks[PA_NATIVE_HOOK_MAX];

    pa_hashmap *extensions;
};

enum {
    SOURCE_OUTPUT_MESSAGE_UPDATE_LATENCY = PA_SOURCE_OUTPUT_MESSAGE_MAX
};

enum {
    SINK_INPUT_MESSAGE_POST_DATA = PA_SINK_INPUT_MESSAGE_MAX, /* data from main loop to sink input */
    SINK_INPUT_MESSAGE_DRAIN, /* disabled prebuf, get playback started. */
    SINK_INPUT_MESSAGE_FLUSH,
    SINK_INPUT_MESSAGE_TRIGGER,
    SINK_INPUT_MESSAGE_SEEK,
    SINK_INPUT_MESSAGE_PREBUF_FORCE,
    SINK_INPUT_MESSAGE_UPDATE_LATENCY,
    SINK_INPUT_MESSAGE_UPDATE_BUFFER_ATTR
};

enum {
    PLAYBACK_STREAM_MESSAGE_REQUEST_DATA,      /* data requested from sink input from the main loop */
    PLAYBACK_STREAM_MESSAGE_UNDERFLOW,
    PLAYBACK_STREAM_MESSAGE_OVERFLOW,
    PLAYBACK_STREAM_MESSAGE_DRAIN_ACK,
    PLAYBACK_STREAM_MESSAGE_STARTED,
    PLAYBACK_STREAM_MESSAGE_UPDATE_TLENGTH,
    PLAYBACK_STREAM_MESSAGE_UNDERFLOW_OHOS,
};

enum {
    RECORD_STREAM_MESSAGE_POST_DATA         /* data from source output to main loop */
};

enum {
    CONNECTION_MESSAGE_RELEASE,
    CONNECTION_MESSAGE_REVOKE
};

/* Called from IO context */
static void playback_stream_request_bytes(playback_stream *s)
{
    size_t m;

    playback_stream_assert_ref(s);

    m = pa_memblockq_pop_missing(s->memblockq);
    if (m <= 0) {
        return;
    }
    /* pa_log("request_bytes(%lu) (tlength=%lu minreq=%lu length=%lu really missing=%lli)", */
    /*    (unsigned long) m, */
    /*    pa_memblockq_get_tlength(s->memblockq), */
    /*    pa_memblockq_get_minreq(s->memblockq), */
    /*    pa_memblockq_get_length(s->memblockq), */
    /*    (long long) pa_memblockq_get_tlength(s->memblockq) - (long long) pa_memblockq_get_length(s->memblockq)); */
#ifdef PROTOCOL_NATIVE_DEBUG
    pa_log("request_bytes(%lu)", (unsigned long) m);
#endif

    if (pa_atomic_add(&s->missing, (int) m) <= 0) {
        pa_asyncmsgq_post(pa_thread_mq_get()->outq, PA_MSGOBJECT(s),
            PLAYBACK_STREAM_MESSAGE_REQUEST_DATA, NULL, 0, NULL, NULL);
    }
}

pa_native_protocol* pa_native_protocol_get(pa_core *core);
pa_native_protocol* pa_native_protocol_ref(pa_native_protocol *p);
void pa_native_protocol_unref(pa_native_protocol *p);
void pa_native_protocol_connect(pa_native_protocol *p, pa_iochannel *io, pa_native_options *a);
void pa_native_protocol_disconnect(pa_native_protocol *p, pa_module *m);

pa_hook *pa_native_protocol_hooks(pa_native_protocol *p);

void pa_native_protocol_add_server_string(pa_native_protocol *p, const char *name);
void pa_native_protocol_remove_server_string(pa_native_protocol *p, const char *name);
pa_strlist *pa_native_protocol_servers(pa_native_protocol *p);

typedef int (*pa_native_protocol_ext_cb_t)(
        pa_native_protocol *p,
        pa_module *m,
        pa_native_connection *c,
        uint32_t tag,
        pa_tagstruct *t);

int pa_native_protocol_install_ext(pa_native_protocol *p, pa_module *m, pa_native_protocol_ext_cb_t cb);
void pa_native_protocol_remove_ext(pa_native_protocol *p, pa_module *m);

pa_pstream* pa_native_connection_get_pstream(pa_native_connection *c);
pa_client* pa_native_connection_get_client(pa_native_connection *c);

pa_native_options* pa_native_options_new(void);
pa_native_options* pa_native_options_ref(pa_native_options *o);
void pa_native_options_unref(pa_native_options *o);
int pa_native_options_parse(pa_native_options *o, pa_core *c, pa_modargs *ma);

#endif
