
/*---------------------------------------------------------------------------*/
/*                Copyright (c) 2021 by Kevin L. Esteb                       */
/*                                                                           */
/*  Permission to use, copy, modify, and distribute this software and its    */
/*  documentation for any purpose and without fee is hereby granted,         */
/*  provided that this copyright notice appears in all copies. The author    */
/*  makes no representations about the suitability of this software for      */
/*  any purpose. It is provided "as is" without express or implied warranty. */
/*                                                                           */
/*---------------------------------------------------------------------------*/

#include <stdio.h>

#include "common.h"
#include "interfaces.h"

#include "container.h"
#include "component.h"
#include "components/inkey.h"
#include "components/prompt.h"
#include "components/textarea.h"
#include "containers/scrollable.h"
#include "containers/horizontal.h"

window_t *screen = NULL;
component_t *inkey = NULL;
component_t *label = NULL;
component_t *textarea = NULL;
container_t *scrollable = NULL;
container_t *horizontal = NULL;


int doHelp(void *data) {

    fprintf(stderr, "doHelp()\n");

    return OK;

}

int bbs_create_screen(error_trace_t *errors) {

    int stat = OK;
    char prompt[32]; 
    int row = getbegy(stdscr);
    int col = getbegx(stdscr);
    int width = getmaxx(stdscr) - 2;
    int height = getmaxy(stdscr) - 4;
    char *value = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer aliquam eget justo ut accumsan. Curabitur commodo velit eu ipsum venenatis, vitae bibendum leo convallis. Vestibulum ac blandit turpis. Praesent interdum urna sapien, sit amet vestibulum enim euismod non. Fusce hendrerit ultricies interdum. Morbi dictum mauris at elit sollicitudin, a faucibus odio fringilla. Aliquam feugiat sem quis quam accumsan, ut hendrerit tortor gravida. Nam gravida ipsum vel dictum vehicula. Aliquam condimentum erat et massa auctor commodo. Aliquam vel neque et nulla luctus interdum. Donec ac nisl dapibus, ullamcorper quam vehicula, congue orci. Aliquam ac egestas nisi. Morbi non arcu in lorem dictum pharetra. Nam purus orci, egestas bibendum lacinia et, ultricies porttitor sapien. Pellentesque blandit, ante nec placerat eleifend, nisi sapien fringilla augue, ut tempus nisl libero a ligula. Ut libero sapien, semper eget mauris sed, aliquet tristique lacus.";

    when_error_in {

fprintf(stderr, "row = %d, col = %d, height = %d, width = %d\n", row, col, height, width);

        screen = window_create(row, col, height, width);
        check_creation(screen);
fprintf(stderr, "after window_create()\n");

        stat = window_box(screen, "testing");
        check_return(stat, screen);
fprintf(stderr, "after window_box()\n");

        textarea = textarea_create(width - 4, value, strlen(value));
        check_creation(textarea);
fprintf(stderr, "after textarea_create()\n");

        row += 2;
        col += 2;
        height -= 4;
        width -= 4;
fprintf(stderr, "row = %d, col = %d, height = %d, width = %d\n", row, col, height, width);
        
        scrollable = scrollable_create(row, col, height, width);
        check_creation(scrollable);
fprintf(stderr, "after scrollable_create()\n");

        stat = container_add_component(scrollable, textarea);
        check_return(stat, scrollable);
fprintf(stderr, "after add component textarea to scrollable\n");

        stat = window_add_container(screen, scrollable);
        check_return(stat, screen);
fprintf(stderr, "after add scrollable to screen\n");

        /* row = height - 1; */
        /* horizontal = horizontal_create(row, col, 2, width - 1); */
        /* check_creation(horizontal); */
/* fprintf(stderr, "after horizontal_create()\n"); */

        /* memset(prompt, '\0', 32); */
        /* strncpy(prompt, qroom.name, 31); */

        /* label = prompt_create(prompt, strlen(prompt)); */
        /* check_creation(label); */
/* fprintf(stderr, "after prompt_create()\n"); */

        /* inkey = inkey_create(keys); */
        /* check_creation(inkey); */
/* fprintf(stderr, "after inkey_create()\n"); */

        /* stat = container_add_component(horizontal, label); */
        /* check_return(stat, horizontal); */
/* fprintf(stderr, "after add component label to horizontal\n"); */

        /* stat = container_add_component(horizontal, inkey); */
        /* check_return(stat, horizontal); */
/* fprintf(stderr, "after add component inkey to horizontal\n"); */

        /* stat = window_add_container(screen, horizontal); */
        /* check_return(stat, prompt); */
/* fprintf(stderr, "after add container horizontal to screen\n"); */

        exit_when;

    } use {

        stat = ERR;
        copy_error(errors);
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

int bbs_init_terminal(error_trace_t *errors) {

    int stat = OK;
    error_trace_t error;

    when_error_in {

        stat = bbs_create_screen(&error);
        check_status2(stat, OK, error);

        /* stat = bbs_create_prompt("Lobby> ", &error); */
        /* check_status2(stat, OK, errors); */

        /* stat = workbench_add_window(workbench, screen); */
        /* check_return(stat, workbench); */

        /* stat = workbench_add_window(workbench, prompt); */
        /* check_return(stat, workbench); */

        /* stat = workbench_refresh(workbench); */
        /* check_return(stat, workbench); */

        exit_when;

    } use {

        stat = ERR;
        copy_error(errors);
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

int read_stdin(void *data) {

    workbench_capture(workbench);
    return OK;

}

int bbs_run(error_trace_t *errors) {

    int stat = OK;
    error_trace_t error;

    when_error_in {

        stat = bbs_init_terminal(&error);
        check_status2(stat, OK, error);

        stat = event_register_input(events, fileno(stdin), read_stdin, NULL);
        check_return(stat, events);

        stat = event_loop(events);
        check_return(stat, events);

        exit_when;

    } use {

        stat = ERR;
        copy_error(errors);
        capture_trace(dump);
        clear_error();

    } end_when;

    return stat;

}

