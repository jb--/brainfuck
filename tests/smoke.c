#include <stdio.h>
#include <stdlib.h>
#include <html.h>

/**
 * Smoke test running a simple "Hello World" html program.
 */
int main() {
    HtmlState *state = html_state();
    HtmlExecutionContext *context = html_context(HTML_TAPE_SIZE);
    HtmlInstruction *instruction = html_parse_string("tttttttthLtttthLttLtttLtttLtHHHHmlLtLtLmLLthHlHmlLLTLmmmTtttttttTTtttTLLTHmTHTtt"
                                                     "tTmmmmmmTmmmmmmmmTLLtTLttT");
    html_add(state, instruction);
    html_execute(state->root, context);
    html_destroy_context(context);
    html_destroy_state(state);
    return EXIT_SUCCESS;
}
