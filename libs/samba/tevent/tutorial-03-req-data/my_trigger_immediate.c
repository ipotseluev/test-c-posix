#include <stdio.h>
#include <unistd.h>
#include <tevent.h>

struct foo_state {
    int x;
};

struct testA {
    int y;
};

static void foo_done(struct tevent_req *req) {
    // a->x contains 10 since it came from foo_send
    struct foo_state *a = tevent_req_data(req, struct foo_state);
    // b->y contains 9 since it came from run
    struct testA *b = tevent_req_callback_data(req, struct testA);
    // c->y contains 9 since it came from run we just used a different way
    // of getting it.
    struct testA *c = (struct testA *)tevent_req_callback_data_void(req);
    printf("a->x: %d\n", a->x);
    printf("b->y: %d\n", b->y);
    printf("c->y: %d\n", c->y);
}

struct tevent_req * foo_send(TALLOC_CTX *mem_ctx, struct tevent_context *event_ctx) {

    printf("_send\n");
    struct tevent_req *req;
    struct foo_state *state;
    req = tevent_req_create(event_ctx, &state, struct foo_state);
    if (!tevent_wakeup_recv(req))
        exit(EXIT_FAILURE);
    state->x = 10;
    return req;
}

static void run(struct tevent_context *ev, struct tevent_immediate *im,
                void *private_data) 
{
    struct tevent_req *req;
    struct testA *tmp = talloc(ev, struct testA);
    // Note that we did not use the private data passed in
    tmp->y = 9;
    req = foo_send(ev, ev);
    tevent_req_set_callback(req, foo_done, tmp);
    tevent_req_done(req);
}

int main (int argc, char **argv)
{
    struct tevent_context *event_ctx;
    struct testA *data;
    TALLOC_CTX *mem_ctx;
    struct tevent_immediate *im;
    mem_ctx = talloc_new(NULL); //parent
    if (mem_ctx == NULL)
        return EXIT_FAILURE;
    event_ctx = tevent_context_init(mem_ctx);
    if (event_ctx == NULL)
        return EXIT_FAILURE;
    data = talloc(mem_ctx, struct testA);
    data->y = 11;

    im = tevent_create_immediate(mem_ctx);
    if (im == NULL) {
        fprintf(stderr, " FAILED\n");
        return EXIT_FAILURE;
    }
    tevent_schedule_immediate(im, event_ctx, run, data);
    tevent_loop_once(event_ctx);
    talloc_free(mem_ctx);
    printf("Quit\n");
    return EXIT_SUCCESS;
}
