#include "MyMemoryPool.h"


// ####### 应用示例 ######

struct Particle
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
static MyMemoryPool m_memorypool(sizeof(Particle), 4096);

// 重载new和delete运算符，类外实现
void* Particle::operator new(std::size_t size)
{
	return m_memorypool.allocate();
}
void Particle::operator delete(void* p) noexcept
{
	m_memorypool.deallocate(p);
}

int main()
{
	// 
	std::vector<Particle*> particles;
	particles.reserve(10000);

	for (int i = 0; i < 10000; i++)
	{
		Particle* p = new Particle{ 0,0,0,0 };
		particles.push_back(p);
	}

	/*for (std::vector<Particle*>::iterator it = particles.begin();
		it != particles.end(); ++it) {
		delete* it;         // 标准迭代器形式
	}*/
	/*for (auto it = particles.begin(); it != particles.end(); it++)
	{
		delete *it;         // 迭代器形式
	}*/
	/*for (size_t i = 0; i < particles.size(); ++i) {
		delete particles[i]; // 下标形式
	}*/
	
	//// 执行业务
	for(auto p : particles)
	{
		delete p;
	}

	std::cout << "BlockSize = " << m_memorypool.getBlockSize()
		<< ", BlocksPerPage = " << m_memorypool.getBlocksPerPage() << std::endl;

	return 0;
}
