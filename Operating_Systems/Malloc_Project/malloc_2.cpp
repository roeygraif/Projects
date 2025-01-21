
#include <iostream>
#include <unistd.h>
#include <cstring>

#define MAX_SIZE 100000000


/**************************
   * CLASS DEFINITIONS *
 **************************/

class MallocMetadata {
public:
    size_t size;
    bool is_free;
    MallocMetadata* next;
    MallocMetadata* prev;

    MallocMetadata() : size(0), is_free(false), next(nullptr), prev(nullptr) {}
    ///MallocMetadata(size_t s, bool free, MallocMetadata* n = nullptr, MallocMetadata* p = nullptr): size(s), is_free(free), next(n), prev(p) {}
    bool isMemoryFree(){return is_free;};
};

class Memory_Block_List{
public:
    MallocMetadata* head;
    MallocMetadata* tail;
    size_t free_blocks;
    size_t free_bytes;
    size_t used_blocks;
    size_t used_bytes;

    Memory_Block_List() : head(nullptr), free_blocks(0), free_bytes(0), used_blocks(0) {}

    MallocMetadata* get_free_block(size_t size);
    void* ptr_to_block(MallocMetadata* ptr);
    MallocMetadata* get_or_allocate(size_t size);
};



/**************************
     * CLASS METHODS *
 **************************/

MallocMetadata *Memory_Block_List::get_free_block(size_t size) {
        MallocMetadata* itr = this->head;
        MallocMetadata* ret = nullptr;
        for(unsigned int i = 0; i < this->free_blocks;) {
            if(itr == nullptr){
                break;
            }
            if(itr->is_free){
                i++;
                if (itr->size >= size) {
                    ret = itr;
                    break;
                }
            }
            itr = itr->next;
        }
        if(ret != nullptr){
            this->free_blocks--;
            this->used_blocks++;
            this->free_bytes -= ret->size;
            this->used_bytes += ret->size;
            ret->is_free = false;
        }
        return ret;
}

//// gets ptr to metadata and returns ptr to block beginning
void *Memory_Block_List::ptr_to_block(MallocMetadata *ptr){
    unsigned long address = (unsigned long)ptr;
    address += sizeof(MallocMetadata);
    void* ret = (void*)address;
    return ret;
}

MallocMetadata *Memory_Block_List::get_or_allocate(size_t size) {
    MallocMetadata* ret = this->get_free_block(size);
    if(ret != nullptr){
        return ret;
    }

    /// no block found, ret = nullptr, allocating...

    ret = (MallocMetadata*)sbrk(size + sizeof(MallocMetadata));
    if(ret == (void*)-1){
        return nullptr;
    }
    ret->size = size;
    ret->is_free = false;
    ret->next = nullptr;
    if(this->head == nullptr){
        this->head = ret;
        this->tail = ret;
        ret->prev = nullptr;
    }
    else{
        ret->prev = this->tail;
        this->tail->next = ret;
        this->tail = ret;
    }
    this->used_blocks++;
    this->used_bytes += ret->size;
    return ret;
}


/**************************
         * MAIN *
 **************************/

Memory_Block_List mbl = Memory_Block_List();

////// SMALLOC //////
void* smalloc(size_t size){
    if(size == 0 || size > MAX_SIZE){
        return nullptr;
    }
    MallocMetadata* ret = mbl.get_or_allocate(size);
    if(ret == nullptr){
        return ret;
    }
    return mbl.ptr_to_block(ret);
}

////// SCALLOC //////
void* scalloc(size_t num, size_t size){
    unsigned long total_size = num * size;
    if(total_size == 0 || total_size > MAX_SIZE){
        return nullptr;
    }
    MallocMetadata* ptr = mbl.get_or_allocate(total_size);
    if(ptr == nullptr){
        return ptr;
    }
    void* ret = mbl.ptr_to_block(ptr);

    memset(ret, 0, ptr->size);

    return ret;
}

////// SFREE //////
void sfree(void* p){
    if(p == nullptr){
        return;
    }
    unsigned long release_address = (unsigned long)p;
    unsigned long meta_size = sizeof(MallocMetadata);

    MallocMetadata* met = (MallocMetadata*)(release_address - meta_size);
    mbl.free_blocks++;
    mbl.free_bytes += met->size;
    mbl.used_blocks--;
    mbl.used_bytes -= met->size;
    met->is_free = true;
    return;
}

void* srealloc(void* oldp, size_t size){
    if(size == 0 || size > MAX_SIZE){
        return nullptr;
    }
    if(oldp == nullptr){
        return smalloc(size);
    }

    MallocMetadata* met = (MallocMetadata*)((unsigned long)oldp - (unsigned long)sizeof(MallocMetadata));
    if(met->size >= size){
        return oldp;
    }

    void* ret = smalloc(size);
    if(ret == nullptr){
        return nullptr;
    }
    memcpy(ret, oldp, met->size);
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