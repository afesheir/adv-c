struct Option {
    char sname;
    char * lname;
    char * desc;
    void (*action)(void *);
};

void argm_parse(struct Option * options, int argc, char * argv[]);