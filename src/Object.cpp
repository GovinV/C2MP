#include "Object.h"
#include "Data.h"

Object::Object()
{
    state = 0;

    m_table = allocateFirstTable();
}

Object::Object(Object& source)
{
    Object();

    operator = (source);
}

Object::~Object()
{
    startIteration();
    dataElement el;
    while(1)
    {
        el=iterate();
        if(el.name == NULL)
        {
            break;
        }
        freeData(&el.data);
        //delete el.data;
    }

    delete m_table;
}

data_str Object::getEntry(const char *name)
{
    data_str data;
    if(m_table->retrieveData(name, &data))
    {
        return data;
    }

    return newData();
}

void Object::setEntry(const char *name, data_str data)
{
    data_str oldData;
    if(m_table->retrieveData(name, &oldData))
    {
        freeData(&oldData);
        //delete oldData;
        if(!m_table->changeData(name, data))
        {
            std::cerr << "impossible to get there ?" << std::endl;
        }
    }
    else
    {
        if(!m_table->addData(name, data))
        {
            std::cerr << "Error : unable to add data" << std::endl;
            return;
        }

        checkArrayIncrease();
    }
}

bool Object::hasEntry(const char *name)
{
    return m_table->dataExists(name);
}

void Object::clear(void)
{
    m_table->clear();
}

unsigned int Object::size(void)
{
    return m_table->size();
}

void Object::print(unsigned int depth)
{
    startIteration();
    dataElement el;
    while(1)
    {
        el=iterate();
        if(el.name == NULL)
        {
            break;
        }
        for(unsigned int i=0;i<depth;++i)
        {
            std::cout << "   ";
        }
        data_str str = el.data;
        std::cout << el.name << " : " << get<T_STRING>(str) << std::endl;
        if(str.type == OBJECT)
        {
            static_cast<Object*>(str.value.pointer)->print(depth+1);
        }

    }
}

GenericTable *Object::allocateFirstTable()
{
    return new HashTable<FIRST_OBJECT_SIZE>;
}

GenericTable *Object::allocateSecondTable()
{
    return new HashTable<SECOND_OBJECT_SIZE>;
}

GenericTable *Object::allocateThirdTable()
{
    return new HashTable<THIRD_OBJECT_SIZE>;
}

GenericTable *Object::allocateFourthTable()
{
    return new HashTable<FOURTH_OBJECT_SIZE>;
}

GenericTable *Object::allocateFifthTable()
{
    return new HashTable<FIFTH_OBJECT_SIZE>;
}

GenericTable *Object::allocateSixthTable()
{
    return new HashTable<SIXTH_OBJECT_SIZE>;
}

GenericTable *Object::allocateSeventhTable()
{
    return new HashTable<SEVENTH_OBJECT_SIZE>;
}

GenericTable *Object::allocateEighthTable()
{
    return new HashTable<EIGHTH_OBJECT_SIZE>;
}

GenericTable *Object::allocateNinethTable()
{
    return new HashTable<NINETH_OBJECT_SIZE>;
}

GenericTable *Object::allocateLastTable()
{
    return new HashTable<LAST_OBJECT_SIZE>;
}

void Object::checkArrayIncrease(void)
{
    switch(state)
    {
        case 0:
            if(size() > FIRST_OBJECT_FLOOR)
            {
                GenericTable *newTable = allocateSecondTable();
                newTable->fill(*m_table);
                delete m_table;
                m_table = newTable;
                ++state;
            }
            break;
        case 1:
            if(size() > SECOND_OBJECT_FLOOR)
            {
                GenericTable *newTable = allocateThirdTable();
                newTable->fill(*m_table);
                delete m_table;
                m_table = newTable;
                ++state;
            }
            break;
        case 2:
            if(size() > THIRD_OBJECT_FLOOR)
            {
                GenericTable *newTable = allocateFourthTable();
                newTable->fill(*m_table);
                delete m_table;
                m_table = newTable;
                ++state;
            }
            break;
        case 3:
            if(size() > FOURTH_OBJECT_FLOOR)
            {
                GenericTable *newTable = allocateFifthTable();
                newTable->fill(*m_table);
                delete m_table;
                m_table = newTable;
                ++state;
            }
            break;
        case 4:
            if(size() > FIFTH_OBJECT_FLOOR)
            {
                GenericTable *newTable = allocateSixthTable();
                newTable->fill(*m_table);
                delete m_table;
                m_table = newTable;
                ++state;
            }
            break;
        case 5:
            if(size() > SIXTH_OBJECT_FLOOR)
            {
                GenericTable *newTable = allocateSeventhTable();
                newTable->fill(*m_table);
                delete m_table;
                m_table = newTable;
                ++state;
            }
            break;
        case 6:
            if(size() > SEVENTH_OBJECT_FLOOR)
            {
                GenericTable *newTable = allocateEighthTable();
                newTable->fill(*m_table);
                delete m_table;
                m_table = newTable;
                ++state;
            }
            break;
        case 7:
            if(size() > EIGHTH_OBJECT_FLOOR)
            {
                GenericTable *newTable = allocateNinethTable();
                newTable->fill(*m_table);
                delete m_table;
                m_table = newTable;
                ++state;
            }
            break;
        case 8:
            if(size() > NINETH_OBJECT_FLOOR)
            {
                GenericTable *newTable = allocateLastTable();
                newTable->fill(*m_table);
                delete m_table;
                m_table = newTable;
                ++state;
            }
            break;
        default:

            break;
    }
}

void Object::startIteration(void)
{
    m_table->startIteration();
}

dataElement Object::iterate(void)
{
    return m_table->iterate();
}
