#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <cstring>

#include "GenericTable.h"

#define HASH_TABLE_VOID 0
#define HASH_TABLE_END_CHAIN -1
#define HASH_TABLE_DEFAULT_MIDDLE 1

#define HASH_TABLE_ACCEPTABLE_STATIC_SIZE 1024

#define IS_VOID(element) ((element).data.name==NULL)//&&element.next==NULL
#define IS_END(element) ((element).next==NULL)

struct hashTableElement
{
    dataElement data;
    //char status; // HASH_TABLE_VOID = no element at all, HASH_TABLE_END_CHAIN = only one element or last element, other = pointer to next
    hashTableElement *next; // pointer to next if there is
};

template <int tableSize>
class HashTable : public GenericTable
{
    public:
        HashTable()
        {
            buildConstructor();
        }
        HashTable(const HashTable &source)
        {
            HashTable();

            operator = (source);
        }
        ~HashTable()
        {
            clear();

            delete[] table;
        }

        void buildConstructor(void)
        {
            table = new hashTableElement[tableSize];

            for(unsigned int i=0;i<tableSize;++i)
            {
                table[i].data.name = NULL;
                table[i].next = NULL;
            }
            m_size = 0;
        }

        void clear(void)
        {
            hashTableElement *element, *prev;
            for(unsigned int i=0;i<tableSize;++i)
            {
                element = &table[i];
                if(IS_VOID(*element))
                {
                    continue;
                }
                // for each non void element :
                delete[] element->data.name;
                element->data.name = NULL;
                if(IS_END(*element))
                {
                    continue;
                }
                //else

                prev = element;
                element = element->next;
                prev->next = NULL;
                do
                {
                    if(element->next == NULL)
                    {
                        delete[] element->data.name;
                        delete element;
                        break;
                    }
                    prev = element;
                    element = element->next;
                    delete[] prev->data.name;
                    delete prev;
                }while(1);
            }
        }

        unsigned int size(void){return m_size;}

        bool addData(const char *name, data_str data)
        {
            int h = hash(reinterpret_cast<const unsigned char *>(name));

            hashTableElement *chainElement = &table[h];

            if(IS_VOID(*chainElement)) // if the hash index is free :
            {
                chainElement->data = {copyString(name), data};
                ++m_size;
                return true;
            }
            //else

            while(1)
            {
                if(strcmp(name, chainElement->data.name) == 0)
                {
                    return false; // unable to insert if the name is already taken
                }

                if(IS_END(*chainElement))
                {
                    hashTableElement *n = new hashTableElement;
                    chainElement->next = n;
                    n->next = NULL;
                    n->data = {copyString(name), data};
                    ++m_size;
                    return true;
                }
                chainElement = chainElement->next;
            }
        }

        bool deleteData(const char *name)
        {
            int h = hash(reinterpret_cast<const unsigned char *>(name));

            hashTableElement *chainElement = &table[h];

            if(IS_VOID(*chainElement))
            {
                return false;
            }
            //else


            if(IS_END(*chainElement))
            {
                if(strcmp(name, chainElement->data.name) == 0)
                {
                    delete[] chainElement->data.name;
                    chainElement->data.name = NULL;
                    --m_size;
                    return true;
                }
                return false;
            }

            hashTableElement *previousElement;
            do
            {
                previousElement = chainElement;
                chainElement = chainElement->next;

                if(strcmp(name, chainElement->data.name) == 0)
                {
                    if(IS_END(*chainElement))
                    {
                        delete[] chainElement->data.name;
                        delete chainElement;
                        previousElement->next = NULL;
                        --m_size;
                        return true;
                    }
                    // else

                    previousElement->next = chainElement->next;
                    delete[] chainElement->data.name;
                    delete chainElement;

                    --m_size;
                    return true;
                }
            }while(!IS_END(*chainElement));

            return false;
        }

        bool changeData(const char *name, data_str data)
        {
            int h = hash(reinterpret_cast<const unsigned char *>(name));

            hashTableElement elt = table[h];

            if(IS_VOID(elt))
            {
                return false;
            }
            if(IS_END(elt))
            {
                if(strcmp(name, elt.data.name) == 0)
                {
                    table[h].data.data = data;
                    return true;
                }
                // else
                return false;
            }

                if(strcmp(name, elt.data.name) == 0)
                {
                    table[h].data.data = data;
                    return true;
                }
                else
                {
                    hashTableElement *chainElement = elt.next;
                    while(1)
                    {
                        if(strcmp(name, chainElement->data.name) == 0)
                        {
                            chainElement->data.data = data;
                            return true;
                        }
                        if(IS_END(*chainElement))
                        {
                            return false;
                        }
                        chainElement = chainElement->next;
                    }
                }

        }

        bool retrieveData(const char *name, data_str *where)
        {
            int h = hash(reinterpret_cast<const unsigned char *>(name));

            hashTableElement elt = table[h];


            if(IS_VOID(elt))
            {
                return false;
            }
            if(IS_END(elt)) // common
            {
                if(strcmp(name, elt.data.name) == 0)
                {
                    *where = elt.data.data;
                    return true;
                }
                // else
                return false;
            }
            //else

                if(strcmp(name, elt.data.name) == 0)
                {
                    *where = elt.data.data;
                    return true;
                }
                else
                {
                    hashTableElement *chainElement = elt.next;
                    while(1)
                    {
                        if(strcmp(name, chainElement->data.name) == 0)
                        {
                            *where = chainElement->data.data;
                            return true;
                        }
                        if(IS_END(*chainElement))
                        {
                            return false;
                        }
                        chainElement = chainElement->next;
                    }
                }

        }

        bool dataExists(const char *name)
        {
            int h = hash(reinterpret_cast<const unsigned char *>(name));

            hashTableElement *chainElement = &table[h];

            if(IS_VOID(*chainElement)) // if the hash index is free :
            {
                return false;
            }
            //else

            while(1)
            {
                if(strcmp(name, chainElement->data.name) == 0)
                {
                    return true;
                }

                if(IS_END(*chainElement))
                {
                    return false;
                }
                chainElement = chainElement->next;
            }
        }

        void startIteration(void)
        {
            itCurrentId = 0;
            itCurrentElement = NULL;
        }

        dataElement iterate(void)
        {
            if(itCurrentElement == NULL)
            {
                while(1)
                {
                    if(itCurrentId >= tableSize)
                    {
                        dataElement retValue;
                        retValue.name = NULL;
                        retValue.data.type = FREE;
                        return retValue; // we have finished iterating
                    }
                    if(IS_VOID(table[itCurrentId]))
                    {
                        ++itCurrentId;
                        continue;
                    }
                    break;
                }
                itCurrentElement = &table[itCurrentId];
            }

            hashTableElement *prev = itCurrentElement;
            if(IS_END(*itCurrentElement))
            {
                itCurrentElement = NULL;
                ++itCurrentId;
                return prev->data;
            }

            itCurrentElement = itCurrentElement->next;
            return prev->data;
        }

        /*void fill(GenericTable& other)
        {
            other.startIteration();
            dataElement el;
            while(1)
            {
                el = other.iterate();
                if(el.data == NULL)
                {
                    break;
                }
                addData(el.name, el.data);
            }
        }*/

        HashTable& operator=(GenericTable& source)
        {
            if(&source == this)
                return *this;

            fill(source);

            return *this;
        }

    protected:

        unsigned int itCurrentId;
        hashTableElement *itCurrentElement;


        //hashTableElement table[tableSize];
        hashTableElement *table;

        unsigned int m_size;

        unsigned long hash(const unsigned char *str)
        {
            unsigned long h = 5381;
            int c;

            while ((c = (*(str++))))
                h = ((h << 5) + h) + c; /* hash * 33 + c */

            return h%tableSize;
        }
};

//int getEntityIdCorrection(std::string name, void *entityManager);
//std::string getEntityNameCorrection(int id, void *entityManager);

#endif // DATAMANAGER_H
