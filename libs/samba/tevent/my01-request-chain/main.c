#include <unistd.h>
#include <stdio.h>
#include <tevent.h>

struct run_state
{
    int a;
};

struct req_state
{
    int b;
};

struct callback_state
{
    int c;
};

void req_done(struct tevent_req *req)
{
    printf("req_done\n");
    // struct req_state *req_data = tevent_req_data(req, struct req_state);
    struct callback_state *cb_data = tevent_req_callback_data(req, struct callback_state);
    // printf("req_data: %d\n", req_data ? req_data->b : -1);
    printf("callback data: %d\n", cb_data ? cb_data->c : -1);
}

struct tevent_req* req_send(TALLOC_CTX* mem_ctx, struct tevent_context *ev)
{
    printf("req_send\n");
    struct req_state *state;
    struct tevent_req* req = tevent_wakeup_send(mem_ctx, ev, tevent_timeval_current_ofs(2, 0));
    if (!tevent_wakeup_recv(req))
        exit(EXIT_FAILURE);
    // struct tevent_req* req = tevent_req_create(ev, &state, struct req_state);
    if (req == NULL)
        exit(EXIT_FAILURE);

    // state->b = 21;
    return req;
}

void run(struct tevent_context* event_ctx, struct tevent_immediate *im, void* private_data)
{
    printf("run\n");
    struct tevent_req* req;
    struct callback_state* state;

    req = req_send(event_ctx, event_ctx);

    state = talloc(event_ctx, struct callback_state);
    if (state == NULL)
        exit(EXIT_FAILURE);
    state->c = 33;

    tevent_req_set_callback(req, req_done, state);
    // tevent_req_done(req);
}

int main()
{
    TALLOC_CTX *mem_ctx;
    mem_ctx = talloc_new(NULL);
    if (mem_ctx == NULL)
        return EXIT_FAILURE;

    struct tevent_context *event_ctx = tevent_context_init(mem_ctx);
    if (event_ctx == NULL)
        return EXIT_FAILURE;

    struct tevent_immediate *im = tevent_create_immediate(mem_ctx);
    if (im == NULL)
        return EXIT_FAILURE;

    struct run_state *data = talloc(mem_ctx, struct run_state);
    if (data == NULL)
        return EXIT_FAILURE;

    data->a = 42;

    tevent_schedule_immediate(im, event_ctx, run, data);

    // tevent_loop_once(event_ctx);
    tevent_loop_wait(event_ctx);
}

