#ifndef DUCKLIKEOBJECT
#define DUCKLIKEOBJECT

#include <memory>

//Magic happens here
class DuckLikeObject
{
    class ContainerBase
    {
    public:
        virtual ~ContainerBase() {}
        virtual ContainerBase *clone() const = 0;
        virtual void quack() = 0;
    };

    template <class T>
    class Container : public ContainerBase
    {
    public:
        Container(T value) : m_value(value) {}

        virtual ContainerBase *clone() const override
        {
            return new Container<T>(m_value);
        }

        virtual void quack() override
        {
            m_value.quack();
        }

    private:
        T m_value;
    };

public:
    DuckLikeObject() = default;
    DuckLikeObject(const DuckLikeObject &other)
    {
        m_data = std::unique_ptr<ContainerBase>(other.m_data->clone());
    }

    DuckLikeObject & operator =(const DuckLikeObject &other)
    {
        DuckLikeObject tmp(other);
        std::swap(tmp, *this);
        return *this;
    }

    template <class T> DuckLikeObject(T value)
    {
        m_data = std::unique_ptr<ContainerBase>(new Container<T>(value));
    }

    template <class T> DuckLikeObject & operator =(T value)
    {
        DuckLikeObject tmp(value);
        std::swap(tmp, *this);
        return *this;
    }

    void quack()
    {
        m_data->quack();
    }

private:
    std::unique_ptr<ContainerBase> m_data;
};

#endif // DUCKLIKEOBJECT

