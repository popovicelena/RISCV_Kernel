#include "../h/memoryallocator.hpp"

MemoryAllocator::BlockHeader* MemoryAllocator::fmem_head = nullptr;
MemoryAllocator::BlockHeader* MemoryAllocator::amem_head = nullptr;

void MemoryAllocator::initializeHead() {
    fmem_head = (BlockHeader*)HEAP_START_ADDR;
    fmem_head->next = nullptr;
    fmem_head->prev = nullptr;
    fmem_head->size = (char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR - sizeof(BlockHeader);
    amem_head = nullptr;
}

void* MemoryAllocator::kmem_alloc(size_t size){

    size_t blkcnt = size % MEM_BLOCK_SIZE == 0 ? size/MEM_BLOCK_SIZE : size/MEM_BLOCK_SIZE + 1;
    size_t blksize=blkcnt * MEM_BLOCK_SIZE;//velicina bloka zaokruzena na blokove velicine MEM_BLOCK_SIZE

    for(BlockHeader* cur = fmem_head; cur != 0;cur = cur->next){
        if(cur->size<blksize) continue;
        //Found

        size_t remainingSize = cur->size - blksize;
        if(remainingSize > sizeof(BlockHeader)){//sizeof(BlockHeader)+MEM_BLOCK_SIZE ispravnije
            cur->size = blksize;
            size_t offset = sizeof (BlockHeader) + blksize;
            BlockHeader* newBlk = (BlockHeader*)((char*)cur + offset);
            newBlk->prev = nullptr;
            newBlk->next = nullptr;
            if(cur->prev){
                cur->prev->next = newBlk;
            }else{
                fmem_head = newBlk;

            }

            if(cur->next) cur->next->prev=newBlk;

            newBlk->next = cur->next;
            newBlk->prev = cur->prev;
            newBlk->size = remainingSize - sizeof(BlockHeader);
        }else{
            //No remaining fragment
            if(cur->prev) cur->prev->next = cur->next;
            else fmem_head = cur->next;
            if(cur->next) cur->next->prev = cur->prev;
        }

        cur->prev= nullptr;
        cur->next = amem_head;
        if(cur->next) cur->next->prev = cur;
        amem_head = cur;
        return (char*)cur + sizeof (BlockHeader);

    }
    //Not enough space
    return nullptr;

}

int MemoryAllocator::kmem_free(void* addr){
    BlockHeader* address = (BlockHeader*)((char*)addr - sizeof(BlockHeader));
    for(BlockHeader* cur = amem_head;cur!=0;cur=cur->next){
        if(cur == address){
            if(cur->prev) cur->prev->next = cur->next;
            else amem_head = cur->next;
            if(cur->next) cur->next->prev = cur->prev;

            cur->prev = nullptr;
            cur->next = fmem_head;
            if(cur->next)cur->next->prev = cur;
            fmem_head=cur;
            return 0;
        }
    }

    return -1;
//    if(addr == nullptr) return -1;
//    BlockHeader* header = (BlockHeader*)((char*)addr - sizeof(BlockHeader));
//    header->prev= nullptr;
//    header->next = fmem_head;
//    fmem_head = header;
//    return 0;

}

