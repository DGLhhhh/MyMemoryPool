/*
* * 内存池设计与实现
*
* // page1[大块内存] -> 切分 -> [小块内存1] [小块内存2] [小块内存3] ... [小块内存n]
*                       ↑          ↓
*					  allocate()   deallocate()小块内存用完释放归还
*
* // page2[大块内存] -> 切分 -> [小块内存1] [小块内存2] [小块内存3] ... [小块内存n]
* 
* * 【头插法】维护一个空闲链表，空闲链表的每个节点就是一个小块内存的地址。
*             当需要使用小块内存时，从空闲链表的头部取出一个小块内存的地址。
*			  小块内存归还时，将其地址插回到空闲链表的头部；
*
*/

#pragma once
#include <iostream>
#include <vector>

// 对齐内存大小,向上对齐到指定的对齐值(通常是指针大小的倍数)
static inline std::size_t align_up(std::size_t size, std::size_t alignment) {
	return (size + (alignment - 1)) & ~(alignment - 1);
}

class MyMemoryPool {
public:
	explicit MyMemoryPool(std::size_t blockSize, std::size_t blocks_per_page);
	~MyMemoryPool();

	void* allocate();     // 切分内存
	void deallocate(void* ptr);          // 归还内存

	std::size_t getBlockSize() const { return m_blockSize; }
	std::size_t getBlocksPerPage() const { return m_blocks_per_page; }

private:
	// 对齐内存大小,保证每个小块内存的最小大小是一个指针的倍数
	std::size_t alignSize(std::size_t size);

	// 扩展内存池，申请新的page,并将其切分为小块内存，加入空闲链表
	void expand();

	std::size_t m_blockSize;       // 每个小块内存的大小
	std::size_t m_blocks_per_page; // 每个大块内存(每页)中小块内存的数量
	std::vector<void*> m_pages;    // 存储page的容器，析构时统一释放所有page内存

	struct Node
	{
		Node* next;
	};
	Node* m_freeList;  // 单向空闲链表的头指针
};
