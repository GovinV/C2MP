#ifndef GENERIC_TABLE_H_INCLUDED
#define GENERIC_TABLE_H_INCLUDED

#include <iostream>
#include <cstring>

#include "dataStructures.h"

struct dataElement
{
    const char *name;
    data_str data;
};

char *copyString(const char *str);

class GenericTable
{
    public:
        GenericTable();
        virtual ~GenericTable();

        virtual void buildConstructor(void) = 0;
        virtual unsigned int size(void) = 0;
        virtual void clear(void) = 0;

        virtual bool addData(const char *name, data_str data) = 0;
        //virtual bool addDataArray(dataElement *dataToInsert, unsigned int toInsertSize) = 0;
        virtual bool deleteData(const char *name) = 0; // doesn't delete the pointed data
        virtual bool changeData(const char *name, data_str data) = 0;
        virtual bool retrieveData(const char *name, data_str *where) = 0;

        virtual bool dataExists(const char *name) = 0;

        virtual void startIteration(void) = 0;
        virtual dataElement iterate(void) = 0;

        void fill(GenericTable& other)
        {
            other.startIteration();
            dataElement el;
            while(1)
            {
                el = other.iterate();
                if(EMPTY_DATA(el.data))
                {
                    break;
                }
                addData(el.name, el.data);
            }
        }

        GenericTable& operator=(GenericTable& source) = delete;
        /*{
            if(&source == this)
                return *this;

            buildConstructor();

            source.startIteration();
            dataElement el;
            while(1)
            {
                el = source.iterate();
                if(el.data == NULL)
                {
                    break;
                }
                addData(el.name, el.data);
            }

            return *this;
        }*/
};

#endif // GENERIC_TABLE_H_INCLUDED
