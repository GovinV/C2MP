#ifndef OBJECT_H
#define OBJECT_H

#include "HashTable.h"
#include "Data.h"

#define FIRST_OBJECT_FLOOR 4
#define SECOND_OBJECT_FLOOR 16
#define THIRD_OBJECT_FLOOR 64
#define FOURTH_OBJECT_FLOOR 1024
#define FIFTH_OBJECT_FLOOR 2048
#define SIXTH_OBJECT_FLOOR 4096
#define SEVENTH_OBJECT_FLOOR 8192
#define EIGHTH_OBJECT_FLOOR 16384
#define NINETH_OBJECT_FLOOR 32768

#define LAST_OBJECT_SIZE 65536

#define FIRST_OBJECT_SIZE FIRST_OBJECT_FLOOR
#define SECOND_OBJECT_SIZE SECOND_OBJECT_FLOOR
#define THIRD_OBJECT_SIZE THIRD_OBJECT_FLOOR
#define FOURTH_OBJECT_SIZE FOURTH_OBJECT_FLOOR
#define FIFTH_OBJECT_SIZE FIFTH_OBJECT_FLOOR
#define SIXTH_OBJECT_SIZE SIXTH_OBJECT_FLOOR
#define SEVENTH_OBJECT_SIZE SEVENTH_OBJECT_FLOOR
#define EIGHTH_OBJECT_SIZE EIGHTH_OBJECT_FLOOR
#define NINETH_OBJECT_SIZE NINETH_OBJECT_FLOOR

class Object
{
    public:
        Object();
        Object(Object& source);
        virtual ~Object();

        data_str getEntry(const char *name);
        void setEntry(const char *name, data_str data);
        bool hasEntry(const char *name);
        void clear(void);
        unsigned int size(void);

        void print(unsigned int depth = 0);

        void startIteration(void);
        dataElement iterate(void);

        Object& operator=(Object& source)
        {
            if(&source == this)
                return *this;

            Object();
            source.startIteration();
            dataElement el;
            while(1)
            {
                el=source.iterate();
                if(el.name == NULL)
                {
                    break;
                }
                setEntry(el.name, el.data);
            }

            return *this;
        }

    protected:

        void checkArrayIncrease(void);

        char state;

        GenericTable *allocateFirstTable();
        GenericTable *allocateSecondTable();
        GenericTable *allocateThirdTable();
        GenericTable *allocateFourthTable();
        GenericTable *allocateFifthTable();
        GenericTable *allocateSixthTable();
        GenericTable *allocateSeventhTable();
        GenericTable *allocateEighthTable();
        GenericTable *allocateNinethTable();
        GenericTable *allocateLastTable();

        GenericTable *m_table;

    private:
};

#endif // OBJECT_H
