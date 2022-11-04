#include <stdio.h>
#include <string.h>
#include "argm.h"

void _help(void * h)
{
    struct Option * op = (struct Option *)h;
    while (op->sname != 0) {
        printf("-%c, --%s: %s\n", op->sname, op->lname, op->desc);
        op++;
    }
}

struct Option help = {'h', "help", NULL, _help};

static struct Option * _find_option_by_sname(struct Option * options, char sname)
{
    struct Option * op = options;
    if (sname == 'h') {
        return &help;
    }

    while (op->sname != 0) {
        if (op->sname == sname) {
            return op;
        }
        op++;
    }
    return NULL;
}

static struct Option * _find_option_by_lname(struct Option * options, char * lname)
{
    struct Option * op = options;
    if (!strcmp(lname, "help")) {
        return &help;
    }
    while (op->sname != 0) {
        if (!strcmp(op->lname, lname)) {
            return op;
        }
        op++;
    }
    return NULL;
}

void argm_parse(struct Option * options, int argc, char * argv[])
{
    int i = 1;
    for (; i < argc; i++) {
        struct Option * op = NULL;
        if (argv[i][0] == '-') {
            if (argv[i][1] == '-') {
                op = _find_option_by_lname(options, &argv[i][2]);
            }
            else {
              op = _find_option_by_sname(options, argv[i][1]);
            }

            if (op == &help) {
                op->action((void *)options);
            }
            else if (op != NULL) {
                if ((i <= argc - 2) && (argv[i + 1][0] != '-')) {
                    op->action((void *)argv[i + 1]);
                    i++;
                }
                else {
                    op->action(NULL);
                }
            }
            else {
                printf("Unrecognized option: %s\n", argv[i]);
            }
        }
        else {
            printf("Unrecognized option: %s\n", argv[i]);
        }
    }
}