#pragma once
#include <Precompiled.h>

struct Unsigned2
{
    Unsigned2() : x(0), y(0){}
    Unsigned2(size_t X, size_t Y) : x(X), y(Y){}
    size_t x;
    size_t y;

    bool operator==(const Unsigned2& rhs) { return (x == rhs.x && y == rhs.y); }
    bool operator!=(const Unsigned2& rhs) { return (x != rhs.x || y != rhs.y); }
};

template < typename DataType >
class ArrayRange
{
public:
    ArrayRange() : Data(nullptr)
    {
    }
    ArrayRange(Array<DataType>* data) : Data(data), Range(0, (*data).size()){}
    //ArrayRange(ArrayRange<DataType>& data) : *this(data), Range(0, data.size()){}
    ArrayRange(ArrayRange<DataType>& data, Unsigned2 range)
    {
        if (range.y < range.x || range.y > data.Data->size())
        {
            Error("End must be larger than start and not out of Range.");
        }
        if (range.x > range.y)
        {
            Error("Start must be smaller than end and not out of Range.");
        }
        *this = data;
        Range = range;
    }
    ArrayRange(Array<DataType>* data, Unsigned2 range) : Data(data)
    {
        if (range.y < range.x || range.y >(*Data).size())
        {
            Error("End must be larger than start and not out of Range.");
        }
        if (range.x > range.y)
        {
            Error("Start must be smaller than end and not out of Range.");
        }
        Range = range;
    }

    void push_back(const DataType& input)
    {
        ++Range.y;
        (*Data).insertAt(Range.y, input);
    }

    void pop_back()
    {
        (*Data).eraseAt(Range.y);
        --Range.y;
    }

    void push_front(const DataType& input)
    {
        ++Range.y;
        (*Data).insertAt(Range.x, input);
    }

    void pop_front()
    {
        --Range.y;
        (*Data).eraseAt(Range.x);
    }

    void insertAt(const DataType& input, size_t index)
    {
        if (index > size())
        {
            Error("Attempting to access index %i out of range.", index);
        }
        ++Range.y;
        (*Data).insertAt(Range.x + index, input);
    }
    void eraseAt(size_t index)
    {
        if (index > size())
        {
            Error("Attempting to access index %i out of range.", index);
        }
        (*Data).eraseAt(Range.x + index);
        --Range.y;
    }

    size_t size() { return Range.y - Range.x; }

    DataType& operator[] (size_t index)
    {
        if (index > size())
        {
            Error("Attempting to access index %i out of range.", index);
        }
        return (*Data)[Range.x + index];
    }

    Unsigned2 range() { return Range; }
    void SetRangeX(size_t index)
    {
        if (index > Range.y)
        {
            Error("Start must be smaller than end and not out of Range.");
        }
        Range.x = index;
    }
    void SetRangeY(size_t index)
    {
        if (index < Range.x || index >(*Data).size())
        {
            Error("End must be larger than start and not out of Range.");
        }
        Range.y = index;
    }

    void SetRange(Unsigned2& index)
    {
        if ((index.y < index.x) || (index.y >(*Data).size()))
        {
            Error("End must be larger than start and not out of Range.");
        }
        Range = index;
    }

    void Resize(size_t newSize)
    {
        if (newSize > size())
        {
            newSize -= size();
            while (newSize)
            {
                push_back(DataType());
                --newSize;
            }
            
        }
        else
        {
            newSize -= size(); //works because unsigned lol
            while (newSize)
            {
                push_back(DataType());
                ++newSize;
            }
            
        }
        
    }

    unsigned FindFirstGlobalIndexOf(DataType obj)
    {
        for (unsigned i = 0; i < Data->size(); ++i)
        {
            if ((*Data)[i] == obj)
            {
                return i;
            }
        }

        return String::InvalidIndex;
    }
private:
    Unsigned2 Range;
    Array<DataType>* Data;
};