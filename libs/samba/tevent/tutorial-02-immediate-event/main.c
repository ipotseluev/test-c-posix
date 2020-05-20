#include <stdio.h>
#include <unistd.h>
#include <tevent.h>

struct info_struct {
    int counter;
};

static void foo(struct tevent_context *ev, struct tevent_immediate *im,
                void *private_data)
{
    struct info_struct *data = talloc_get_type_abort(private_data, struct info_struct);
    printf("Data value: %d\n", data->counter);
}

int main (void) {
    struct tevent_context *event_ctx;
    TALLOC_CTX *mem_ctx;
    struct tevent_immediate *im;
    printf("INIT\n");
    mem_ctx = talloc_new(NULL);
    event_ctx = tevent_context_init(mem_ctx);
    struct info_struct *data = talloc(mem_ctx, struct info_struct);
    // setting up private data
    data->counter = 1;
    // first immediate event
    im = tevent_create_immediate(mem_ctx);
    if (im == NULL) {
        fprintf(stderr, "FAILED\n");
        return EXIT_FAILURE;
    }
    tevent_schedule_immediate(im, event_ctx, foo, data);
    tevent_loop_wait(event_ctx);
    talloc_free(mem_ctx);
    return 0;
}
