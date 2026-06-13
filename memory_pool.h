#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <cassert>
#include <cstddef>
#include <array>

#include <QDebug>


template <typename T, size_t Capacity>
class MemoryPool
{
    struct PoolDeleter
    {

        void operator()(T* obj) const
        {
            if(obj != nullptr)
            {
                obj->~T();
                m_pool->m_cells[m_index] = true;
            }
        }

    public:
        MemoryPool* m_pool;
        size_t m_index;
    };

public:
    MemoryPool() :
        m_buffer(allocate(Capacity)),
        m_capacity(Capacity)
    {
        qDebug() << "Created pool\n";
        std::fill(m_cells.begin(), m_cells.end(), true);
    }

    ~MemoryPool()
    {
        qDebug() << "Killed pool\n";
        deallocate(m_buffer);
    }

    MemoryPool(const MemoryPool& other) = delete;
    MemoryPool(MemoryPool&& other) = delete;

    MemoryPool& operator=(const MemoryPool& other) = delete;
    MemoryPool&& operator=(MemoryPool&& other) = delete;


    template<typename... Args>
    std::unique_ptr<T, PoolDeleter> create(Args&&... args)
    {
        auto iter = std::find(m_cells.begin(), m_cells.end(), true);
        if(iter != m_cells.end())
        {
            auto index = std::distance(m_cells.begin(), iter);
            *iter = false;
            return std::unique_ptr<T, PoolDeleter>(new (m_buffer + index) T(std::forward<Args>(args)...),
                                               PoolDeleter{this, index});
        }
        else
        {
            return nullptr;
        }

    }


private:


    T* m_buffer = nullptr;
    size_t m_capacity = 0;
    size_t m_size = 0;

    std::array<bool, Capacity> m_cells;

    static T* allocate(size_t capacity)
    {
        return capacity != 0 ? static_cast<T*>(operator new(sizeof(T) * capacity,
                                                static_cast<std::align_val_t>(alignof(T)))) : nullptr;
    }
    static void deallocate(T* buff) noexcept
    {
        operator delete(buff, static_cast<std::align_val_t>(alignof(T)));
    }


};


#endif // MEMORY_POOL_H
