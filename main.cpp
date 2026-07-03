#include "MyMemoryPool.h"


// ####### 应用示例 ######

struct Praticle
{
	float x, y, z;
	int life;

	// 重载new和delete运算符，使用内存池进行Praticle对象的内存分配和释放
	static void* operator new(std::size_t size);
	static void operator delete(void* p) noexcept;

	void update()
	{
		++life;
	}
};

// 创建一个全局的内存池对象，管理Praticle对象的内存分配和释放
static MyMemoryPool m_memorypool(sizeof(Praticle), 4096);

// 重载new和delete运算符，类外实现
void* Praticle::operator new(std::size_t size)
{
	return m_memorypool.allocate();
}
void Praticle::operator delete(void* p) noexcept
{
	m_memorypool.deallocate(p);
}

int main()
{
	// 
	std::vector<Praticle*> particles;
	particles.reserve(10000);

	for (int i = 0; i < 10000; i++)
	{
		Praticle* p = new Praticle{ 0,0,0,0 };
		particles.push_back(p);
	}

	//// 执行业务
	for(auto p : particles)
	{
		delete p;
	}

	std::cout << "BlockSize = " << m_memorypool.getBlockSize()
		<< ", BlocksPerPage = " << m_memorypool.getBlocksPerPage() << std::endl;

	return 0;
}
