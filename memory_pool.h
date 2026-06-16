#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <cassert>
#include <cstddef>
#include <array>

#include <QDebug>


template <typename T, size_t Capacity>
class MemoryPool
{
public:
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

        MemoryPool* m_pool;
        size_t m_index;
    };

public:

    class Iterator
    {
    public:

        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Iterator(MemoryPool* pool, size_t index) : m_pool(pool), m_index(index)
        {
            skip_empty();
        }

        reference operator*()
        {
            return *reinterpret_cast<T*>(m_pool->m_buffer + m_index);
        }

        pointer operator->() const {
            return reinterpret_cast<T*>(m_pool->m_buffer + m_index);
        }

        Iterator& operator++()
        {
            ++m_index;
            skip_empty();
            return *this;
        }

        bool operator==(const Iterator& other) const {
            return m_index == other.m_index && m_pool == other.m_pool;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }


    private:

        void skip_empty()
        {
            while(m_index < m_pool->m_capacity && m_pool->m_cells[m_index])
            {
                m_index++;
            }
        }

        MemoryPool* m_pool;
        size_t m_index;
    };

    MemoryPool() :
        m_buffer(allocate(Capacity)),
        m_capacity(Capacity)
    {
        qDebug() << "Created pool\n";
        std::fill(m_cells.begin(), m_cells.end(), true);
    }

    ~MemoryPool()
    {
        for(size_t i = 0; i < m_capacity; ++i)
        {
            if(!m_cells[i])
            {
                reinterpret_cast<T*>(m_buffer + i)->~T();
            }
        }

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
            size_t index = static_cast<size_t>(std::distance(m_cells.begin(), iter));
            *iter = false;
            m_size++;
            return std::unique_ptr<T, PoolDeleter>(new (m_buffer + index) T(std::forward<Args>(args)...),
                                               PoolDeleter{this, index});
        }
        else
        {
            return nullptr;
        }

    }

    Iterator begin() noexcept
    {
        return Iterator{this, 0};
    }

    Iterator end() noexcept
    {
        return Iterator{this, m_capacity};
    }

    T* operator+(size_t offset) noexcept
    {
        assert(offset <= m_capacity);
        return m_buffer + offset;
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
