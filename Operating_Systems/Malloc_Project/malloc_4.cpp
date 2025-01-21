
#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <climits>

#define MAX_SIZE 100000000
#define MAX_ORDER 10
#define DEF_SIZE 131072
#define INIT_BLOCKS 32
#define MIN_SIZE 128
#define ALIGNMENT_FACTOR 4194304
#define TWO_MB 2097152      //EDITED
#define FOUR_MB 4194304     //EDITED


unsigned int random_number;     //EDITED

/**************************
   * CLASS DEFINITIONS *
 **************************/
class MallocMetadata {
public:
    unsigned int cookies;
    size_t size;
    bool is_free;
    MallocMetadata* next_of_order;
    MallocMetadata* prev_of_order;
    int order;

    MallocMetadata() : cookies(random_number) ,size(0), is_free(false), next_of_order(nullptr), prev_of_order(nullptr), order(-1) {}
    ///MallocMetadata(size_t s, bool free, MallocMetadata* n = nullptr, MallocMetadata* p = nullptr): size(s), is_free(free), next(n), prev(p) {}
    bool isMemoryFree(){return is_free;};
    MallocMetadata* findBuddy();
    bool isCorrupt(){return this->cookies != random_number;};    //TODO / EDITED we need to add this function everywhere if its good
};

size_t met_size = sizeof(MallocMetadata);

MallocMetadata *MallocMetadata::findBuddy() {
    unsigned long target = ((unsigned long)this) ^ (this->size + met_size);
    return (MallocMetadata*)target;
}

class Memory_Block_List{
public:
    MallocMetadata* BlockBank[MAX_ORDER+1];
    size_t free_blocks;
    size_t free_bytes;
    size_t used_blocks;
    size_t used_bytes;

    Memory_Block_List() : free_blocks(0), free_bytes(0), used_blocks(0) {}

    MallocMetadata* get_free_block(size_t size);
    void* ptr_to_block(MallocMetadata* ptr);
    MallocMetadata* splitter(MallocMetadata* ptr, int diff);
    MallocMetadata* merge_buddy(MallocMetadata* ptr);
    MallocMetadata* mmap_block(size_t size);
    void relist(MallocMetadata* ptr);
};

/**************************
     * CLASS METHODS *
 **************************/
Memory_Block_List mbl = Memory_Block_List();
int calcOrder(size_t size);


void generate_random_number() {     //EDITED
    random_number = rand() % UINT_MAX;
}

MallocMetadata *Memory_Block_List::get_free_block(size_t size) {
    int order = calcOrder(size);
    MallocMetadata* ret = nullptr;
    if(mbl.BlockBank[order] != nullptr){
        ret = BlockBank[order];

        ////cookies
        if(ret->isCorrupt()){
            exit(0xDEADBEEF);
        }///

        BlockBank[order] = ret->next_of_order;
    }
    else{  ///split
        int min_order = order+1;
        for(; min_order <= MAX_ORDER;){
            if(mbl.BlockBank[min_order] == nullptr){
                min_order++;
                continue;
            }
            break;
        }
        if(min_order > MAX_ORDER){
            return nullptr;
        }
        ret = splitter(BlockBank[min_order], min_order - order);

        ////cookies///
        if(ret->isCorrupt()){
            exit(0xDEADBEEF);
        }

    }

    ret->is_free = false;
    mbl.free_bytes -= ret->size;
    mbl.free_blocks--;
    mbl.used_blocks++;
    mbl.used_bytes += ret->size;
    BlockBank[order] = ret->next_of_order;

    return ret;
}

//// gets ptr to metadata and returns ptr to block beginning
void *Memory_Block_List::ptr_to_block(MallocMetadata *ptr){
    ////cookies
    if(ptr->isCorrupt()){
        exit(0xDEADBEEF);
    }///

    unsigned long address = (unsigned long)ptr;
    address += sizeof(MallocMetadata);
    void* ret = (void*)address;
    return ret;
}

MallocMetadata *Memory_Block_List::splitter(MallocMetadata *ptr, int diff) {
    ////cookies
    if(ptr->isCorrupt()){
        exit(0xDEADBEEF);
    }///

    if(diff == 0 || ptr->order == 0){//TODO / EDITED ptr->size < size (we need to give size in the args)
        return ptr;
    }
    unsigned long base_address = (unsigned long)ptr;
    size_t new_size = (ptr->size + met_size)/2 - met_size;
    MallocMetadata* buddy = (MallocMetadata*)(base_address+new_size+met_size);

    /// find place in previous list
    MallocMetadata* new_prev = nullptr;
    MallocMetadata* new_next = nullptr;
    MallocMetadata* itr = mbl.BlockBank[(ptr->order)-1];
    if(itr == nullptr){
        mbl.BlockBank[(ptr->order)-1] = ptr;
    }
    else{

        ////cookies
        if(itr->isCorrupt()){
            exit(0xDEADBEEF);
        }///

        if(ptr<itr){
            mbl.BlockBank[(ptr->order)-1] = ptr;
            new_next = itr;
        }
        else{
            while(ptr < itr->next_of_order){
                new_prev = itr;
                new_next = itr->next_of_order;
                itr = itr->next_of_order;
            }
            if(itr->next_of_order == nullptr){
                new_next = nullptr;
                new_prev = itr;
            }
        }
    }
    /// we have prev and next

    if(!ptr->prev_of_order){
        mbl.BlockBank[ptr->order] = ptr->next_of_order;
        if(ptr->next_of_order != nullptr){

            ////cookies
            if(ptr->next_of_order->isCorrupt()){
                exit(0xDEADBEEF);
            }///

            ptr->next_of_order->prev_of_order = nullptr;
        }
    }
    else{

        ////cookies
        if(ptr->prev_of_order->isCorrupt()){
            exit(0xDEADBEEF);
        }///

        ptr->prev_of_order->next_of_order = ptr->next_of_order;
        if(ptr->next_of_order != nullptr){

            ////cookies
            if(ptr->next_of_order->isCorrupt()){
                exit(0xDEADBEEF);
            }///

            ptr->next_of_order->prev_of_order = ptr->prev_of_order;
        }
    }
    ptr->prev_of_order = new_prev;
    if(new_prev){

        ////cookies
        if(new_prev->isCorrupt()){
            exit(0xDEADBEEF);
        }///

        new_prev->next_of_order = ptr;
    }
    buddy->next_of_order = new_next;
    if(new_next) {

        ////cookies
        if(new_next->isCorrupt()){
            exit(0xDEADBEEF);
        }///

        new_next->prev_of_order = buddy;
    }
    ptr->next_of_order = buddy;
    buddy->prev_of_order = ptr;
    buddy->is_free = true;
    ptr->size = new_size;
    buddy->size = new_size;
    ptr->order--;
    buddy->order = ptr->order;
    buddy->cookies = random_number; /// added

    mbl.free_bytes -= met_size;
    mbl.free_blocks++;

    return splitter(ptr, diff-1);
}

MallocMetadata *Memory_Block_List::merge_buddy(MallocMetadata *ptr) {
    ////cookies
    if(ptr->isCorrupt()){
        exit(0xDEADBEEF);
    }///

    MallocMetadata* buddy = ptr->findBuddy();

    ////cookies
    if(buddy->isCorrupt()){
        exit(0xDEADBEEF);
    }///

    MallocMetadata* ret = nullptr;
    if(buddy->size != ptr->size || !(buddy->is_free) || ptr->order == MAX_ORDER){
        ptr->is_free = true;
        return ptr;
    }
    else{
        if(buddy->prev_of_order == nullptr){
            mbl.BlockBank[buddy->order] = buddy->next_of_order;
        }
        else{

            ////cookies
            if(buddy->prev_of_order->isCorrupt()){
                exit(0xDEADBEEF);
            }///

            buddy->prev_of_order->next_of_order = buddy->next_of_order;
        }
        if(buddy->next_of_order != nullptr){

            ////cookies
            if(buddy->next_of_order->isCorrupt()){
                exit(0xDEADBEEF);
            }///

            buddy->next_of_order->prev_of_order = buddy->next_of_order;
        }
        /// buddy not in list
        if(ptr < buddy){
            ret = ptr;
        }
        else{
            ret = buddy;
        }
        ret->is_free = true;
        /// getting ret's buddy
        buddy = ret->findBuddy();
        ret->size = (ret->size + met_size)*2 - met_size;
        if(ret->prev_of_order == nullptr){
            if(ret->next_of_order == buddy){
                mbl.BlockBank[ret->order] = buddy->next_of_order;
            }
            else{
                mbl.BlockBank[ret->order] = ret->next_of_order;
            }
        }
        ret->order++;


        //// insert ret back into list
        if(mbl.BlockBank[ret->order] == nullptr){
            mbl.BlockBank[ret->order] = ret;
            ret->prev_of_order = nullptr;
            ret->next_of_order = nullptr;
        }
        else{

            ////cookies
            if(mbl.BlockBank[ret->order]->isCorrupt()){
                exit(0xDEADBEEF);
            }///

            MallocMetadata* new_next = nullptr;
            MallocMetadata* new_prev = nullptr;
            if(ret < mbl.BlockBank[ret->order]){
                new_next = mbl.BlockBank[ret->order];
                new_prev = nullptr;
                mbl.BlockBank[ret->order] = ret;
            }
            else{
                MallocMetadata* itr = mbl.BlockBank[ret->order];
                while(itr){

                    ////cookies
                    if(itr->isCorrupt()){
                        exit(0xDEADBEEF);
                    }///

                    if(ret < itr->next_of_order){
                        new_next = itr->next_of_order;
                        new_prev = itr;
                    }
                    else{
                        if(itr->next_of_order == nullptr){
                            new_next = nullptr;
                            new_prev = itr;
                        }
                        itr = itr->next_of_order;
                    }
                }
            }

            ret->next_of_order = new_next;
            if(new_next != nullptr){

                ////cookies
                if(new_next->isCorrupt()){
                    exit(0xDEADBEEF);
                }///

                new_next->prev_of_order = ret;
            }
            ret->prev_of_order = new_prev;
            if(new_prev != nullptr){

                ////cookies
                if(new_prev->isCorrupt()){
                    exit(0xDEADBEEF);
                }///

                new_prev->next_of_order = ret;
            }
        }
        mbl.free_bytes += met_size;
        mbl.free_blocks --;
        return ret;
    }
}

MallocMetadata *Memory_Block_List::mmap_block(size_t size) {
    MallocMetadata* ret = (MallocMetadata*)mmap(nullptr, size+met_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if(ret == nullptr){
        return ret;
    }
    ret->size = size;
    ret->is_free = false;
    ret->next_of_order = nullptr;
    ret->prev_of_order = nullptr;
    ret->cookies = random_number; /// added
    mbl.used_blocks++;
    mbl.used_bytes += ret->size;
    return ret;
}

void Memory_Block_List::relist(MallocMetadata *ptr) {

    ////cookies
    if(ptr->isCorrupt()){
        exit(0xDEADBEEF);
    }///

    if(mbl.BlockBank[ptr->order] == nullptr){
        mbl.BlockBank[ptr->order] = ptr;
        ptr->prev_of_order = nullptr;
        ptr->next_of_order = nullptr;
    }
    else{

        ////cookies
        if(mbl.BlockBank[ptr->order]->isCorrupt()){
            exit(0xDEADBEEF);
        }///

        MallocMetadata* new_next = nullptr;
        MallocMetadata* new_prev = nullptr;
        if(ptr < mbl.BlockBank[ptr->order]){
            new_next = mbl.BlockBank[ptr->order];
            new_prev = nullptr;
            mbl.BlockBank[ptr->order] = ptr;
        }
        else{
            MallocMetadata* itr = mbl.BlockBank[ptr->order];
            while(itr){

                ////cookies
                if(itr->isCorrupt()){
                    exit(0xDEADBEEF);
                }///

                if(ptr < itr->next_of_order){
                    new_next = itr->next_of_order;
                    new_prev = itr;
                }
                else{
                    if(itr->next_of_order == nullptr){
                        new_next = nullptr;
                        new_prev = itr;
                    }
                    itr = itr->next_of_order;
                }
            }
        }

        ptr->next_of_order = new_next;
        if(new_next != nullptr){

            ////cookies
            if(new_next->isCorrupt()){
                exit(0xDEADBEEF);
            }///

            new_next->prev_of_order = ptr;
        }
        ptr->prev_of_order = new_prev;
        if(new_prev != nullptr){

            ////cookies
            if(new_prev->isCorrupt()){
                exit(0xDEADBEEF);
            }///

            new_prev->next_of_order = ptr;
        }
    }
}


/**************************
         * MAIN *
 **************************/



void init(){
    unsigned long curr_addr = (unsigned long)sbrk(0);
    unsigned long remainder = curr_addr % (unsigned long)ALIGNMENT_FACTOR;
    if(remainder != 0){
        sbrk(ALIGNMENT_FACTOR-remainder);
    }
    /// aligned
    curr_addr = (unsigned long)sbrk(0);
    mbl.BlockBank[MAX_ORDER] = (MallocMetadata*)sbrk(32*DEF_SIZE); /// edited
    if(mbl.BlockBank[MAX_ORDER] == (void*)-1){
        return;
    }
    MallocMetadata* itr = mbl.BlockBank[MAX_ORDER];
    generate_random_number(); ////added
    for (int i=0; i<INIT_BLOCKS-1; i++){
        itr->cookies = random_number; ///added
        itr->size = DEF_SIZE-met_size;
        itr->is_free = true;
        itr->order = MAX_ORDER;
        unsigned long next_addr = (unsigned long)itr + (unsigned long)DEF_SIZE;
        itr->next_of_order = (MallocMetadata*)next_addr;/// edited
        if(itr->next_of_order == (void*)-1){
            return;
        }
        itr = itr->next_of_order;
    }
    itr->cookies = random_number; ///added
    itr->size = DEF_SIZE-met_size;
    itr->is_free = true;
    itr->order = MAX_ORDER;
    itr->next_of_order = nullptr;

    mbl.free_blocks = INIT_BLOCKS;
    mbl.free_bytes = INIT_BLOCKS * (DEF_SIZE - met_size);
    mbl.used_blocks = 0;
    mbl.used_bytes = 0;
}

int calcOrder(size_t size){
    int i=0;
    for (; i<=MAX_ORDER; i++){
        if(size < (MIN_SIZE*pow(2,i) - met_size)){
            break;
        }
    }
    return i;
}

//EDITED
MallocMetadata* expandAlloc(MallocMetadata* first, unsigned int size){

    ////cookies
    if(first->isCorrupt()){
        exit(0xDEADBEEF);
    }///

    while(first->size < size){
        first = mbl.merge_buddy(first);
        first->is_free = false;     //because we did the oppesite in merge_buddy
    }
    return first;
}

////// SMALLOC //////
void* smalloc(size_t size){
    if(mbl.free_blocks == 0 && mbl.used_blocks == 0){
        init();
    }
    if(size == 0 || size >= MAX_SIZE){
        return nullptr;
    }
    MallocMetadata* ret = nullptr;

    if(size >= FOUR_MB){        //EDITED HUGEPAGE
        ret = (MallocMetadata*)mmap(nullptr, size+met_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
        if(ret == nullptr){
            return ret;
        }
        ret->cookies = random_number;
        ret->size = size;
        ret->is_free = false;
        ret->next_of_order = nullptr;
        ret->prev_of_order = nullptr;
        mbl.used_blocks++;
        mbl.used_bytes += ret->size;
        return ret;
    }
    if(size > DEF_SIZE-met_size){
        ret = mbl.mmap_block(size);
    }
    else{
        ret = mbl.get_free_block(size);
    }
    if(ret == nullptr){
        return ret;
    }

    ////cookies
    if(ret->isCorrupt()){
        exit(0xDEADBEEF);
    }///

    return mbl.ptr_to_block(ret);
}

////// SCALLOC //////
void* scalloc(size_t num, size_t size){
    if(mbl.free_blocks == 0 && mbl.used_blocks == 0){
        init();
    }
    unsigned long total_size = num * size;
    if(total_size == 0 || total_size > MAX_SIZE){
        return nullptr;
    }
    MallocMetadata* ptr = nullptr;
    if(total_size >= TWO_MB){        //EDITED HUGEPAGE
        ptr = (MallocMetadata*)mmap(nullptr, total_size+met_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);
        if(ptr == nullptr){
            return ptr;
        }
        ptr->size = total_size;
        ptr->is_free = false;
        ptr->next_of_order = nullptr;
        ptr->prev_of_order = nullptr;
        mbl.used_blocks++;
        mbl.used_bytes += ptr->size;
    }else if(total_size > DEF_SIZE-met_size){
        ptr = mbl.mmap_block(size);
    }
    else{
        ptr = mbl.get_free_block(total_size);
    }
    if(ptr == nullptr){
        return ptr;
    }

    ////cookies
    if(ptr->isCorrupt()){
        exit(0xDEADBEEF);
    }///

    void* ret = mbl.ptr_to_block(ptr);
    memset(ret, 0, ptr->size);
    return ret;
}

////// SFREE //////
void sfree(void* p){
    if(mbl.free_blocks == 0 && mbl.used_blocks == 0){
        return;
    }
    if(p == nullptr){
        return;
    }

    unsigned long release_address = (unsigned long)p;

    MallocMetadata* met = (MallocMetadata*)(release_address - met_size);

    ////cookies
    if(met->isCorrupt()){
        exit(0xDEADBEEF);
    }///

    if(met->is_free){
        return;
    }

    if(met->size > DEF_SIZE - met_size){
        mbl.used_blocks--;
        mbl.used_bytes -= met->size;
        munmap(met, met->size + met_size);
        return;
    }

    mbl.free_blocks++;
    mbl.free_bytes += met->size;
    mbl.used_blocks--;
    mbl.used_bytes -= met->size;

    int old_order = met->order;
    met = mbl.merge_buddy(met);
    if(old_order != met->order){
        while(old_order != met->order){
            met = mbl.merge_buddy(met);
            old_order++;
        }
        return;
    }
    else{
        mbl.relist(met);
    }
}

////left
void* srealloc(void* oldp, size_t size){
    if(mbl.free_blocks == 0 && mbl.used_blocks == 0){
        return nullptr;
    }
    if(oldp == nullptr){
        return smalloc(size);
    }
    if(size == 0 || size >= MAX_SIZE){
        return nullptr;
    }

    MallocMetadata* met = (MallocMetadata*)((unsigned long)oldp - (unsigned long)sizeof(MallocMetadata));

    ////cookies
    if(met->isCorrupt()){
        exit(0xDEADBEEF);
    }///

    if(met->size >= size){
        return oldp;
    }

    //EDITED
    void* ret = nullptr;
    if(met->size > DEF_SIZE-met_size){
        ret = mbl.ptr_to_block(mbl.mmap_block(size));
    }
    else {
        MallocMetadata* first = met;
        MallocMetadata* buddy = first->findBuddy();

        ////cookies
        if(buddy->isCorrupt()){
            exit(0xDEADBEEF);
        }///

        unsigned int size_counter = met->size + buddy->size;
        while(size >= size_counter && buddy->is_free) {
            if(size <= size_counter){   //we can make the current space bigger
                return mbl.ptr_to_block(expandAlloc(met, size));
            }
            if (buddy < first) {
                first = buddy;
            }
            buddy = (MallocMetadata*)(((unsigned long)first) ^ (size_counter + met_size));
            size_counter = first->size + buddy->size;
        }

        ret = smalloc(size);
        if (ret == nullptr) {
            return nullptr;
        }
    }

    memmove(ret, oldp, met->size);
    sfree(oldp);
    return ret;
}

size_t _num_free_blocks(){
    return mbl.free_blocks;
}
size_t _num_free_bytes(){
    return mbl.free_bytes;
}
size_t _num_allocated_blocks(){
    return mbl.free_blocks + mbl.used_blocks;
}
size_t _num_allocated_bytes(){
    return mbl.free_bytes + mbl.used_bytes;
}

size_t _size_meta_data(){
    return sizeof(MallocMetadata);
}

size_t _num_meta_data_bytes(){
    return (_num_allocated_blocks() * _size_meta_data());
}