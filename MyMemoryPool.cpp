/*
* * 内存池设计与实现
* 
* // page1[大块内存] -> 切分 -> [小块内存1] [小块内存2] [小块内存3] ... [小块内存n]
*                       ↑          ↓
*					  allocate()   deallocate()小块内存用完释放归还
* 
* // page2[大块内存] -> 切分 -> [小块内存1] [小块内存2] [小块内存3] ... [小块内存n]
* 
*/

#include "MyMemoryPool.h"

MyMemoryPool::MyMemoryPool(std::size_t blockSize, std::size_t blocks_per_page = 1024)
{
	m_blockSize = alignSize(blockSize);
	m_blocks_per_page = blocks_per_page;
	m_freeList = nullptr;
}

MyMemoryPool::~MyMemoryPool()
{
	for(auto page : m_pages) {
		::operator delete(page);
		//delete page; <==> 对应new T   -- 这里不能用delete，因为我们使用了::operator new分配内存
	}	
}

// 切分小块内存(从空闲链表拿走小块内存)
void* MyMemoryPool::allocate()
{
	// 如果空闲链表为空，则扩展内存池
	if(!m_freeList) {
		expand();
	}

	Node* head = m_freeList;
	m_freeList = head->next;

	return head;
}
// 归还小块内存到空闲链表
void MyMemoryPool::deallocate(void* ptr)
{
	if (!ptr) return;
	Node* node = static_cast<Node*>(ptr);
	node->next = m_freeList;
	m_freeList = node;
}

// 最小内存块大小，向上取整
std::size_t MyMemoryPool::alignSize(std::size_t size)
{
	std::size_t min = sizeof(void*);
	std::size_t alignedSize = align_up(size < min ? min : size, alignof(void*));
	return alignedSize;
}

// 扩展内存池，申请新的page,并将其切分为小块内存，加入空闲链表
void MyMemoryPool::expand()
{
	// 申请一页大块内存
	std::size_t pageSize = m_blockSize * m_blocks_per_page;
	char* newPage = static_cast<char*>(::operator new(pageSize));
	m_pages.push_back(newPage);

	// 将大块内存切分为小块内存，并串成空闲链表
	for(std::size_t i = 0; i < m_blocks_per_page; ++i) {
		char* blockPtr = newPage + i * m_blockSize;
		Node* node = reinterpret_cast<Node*>(blockPtr);
		node->next = m_freeList;
		m_freeList = node;
	}
}
