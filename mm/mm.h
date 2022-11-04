
#define MEM_SIZE(bsize, nblocks) ((bsize) + sizeof(struct mm_block_hdr)) * (nblocks)

struct mm_block_hdr {
    struct mm_block_hdr * next;
};

struct mm_pool {
    char * memory;
    struct mm_block_hdr * first_free;
    int nr_blocks;
    int nr_free_blocks;
    int block_size;
};

void mm_pool_init(struct mm_pool * pool);
void * mm_allocate(struct mm_pool * pool);
void mm_free(void * data, struct mm_pool * pool);
void mm_show_stats(struct mm_pool * pool);