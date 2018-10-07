#include <iostream>
#include <stdlib.h>

#include "HashTable.h"
#include "Data.h"
#include "Object.h"
#include "ObjectUtil.h"

int modifSize = 0;

std::string generateVarName(void)
{
    std::string varname;
    varname = "var_";
    char car;

    for(int j=0;j<1+rand()%5+rand()%5;j++)
    {
        int randChar = rand()%10;
        car = '0'+randChar;

        varname = varname + car;
    }

    return varname;
}

std::string randStr(void)
{
    std::string varname;
    varname = "";
    char car;

    for(int j=0;j<1+rand()%5+rand()%5;j++)
    {
        int randChar = rand()%50;
        car = '0'+randChar;

        varname = varname + car;
    }

    return varname;
}

data_str randomValue(void)
{
    data_str var;
    var = newData();
    switch(rand()%4)
    {
        case 0:
            set<T_STRING>(&var, randStr().c_str());
            break;
        case 1:
            set<T_BOOL>(&var, rand()%2);
            break;
        case 2:
            set<T_FLOAT>(&var, rand());
            break;
        case 3:
            set<T_INT>(&var, rand()%1000);
            break;
    }
    return var;
}

Object *constructTree(int depth = 0)
{
    Object *object = new Object;
    if(depth == 0)
    {
        for(int i=0;i<100;++i)
        {++modifSize;
            if(rand()%10 == 0)
            {
                object->setEntry(generateVarName().c_str(), randomValue());
            }
            else
            {
                data_str var;
                var = newData();set<T_OBJECT>(&var, constructTree(depth+1));
                object->setEntry(generateVarName().c_str(), var);
            }
        }
    }
    else
    {
        int length = rand()%10;
        if(rand()%10 > depth+7)
        {
            length += rand()%100;
        }
        for(int i=0;i<length;++i)
        {++modifSize;
            if(rand()%20 > depth+15)
            {
                data_str var;
                var = newData();set<T_OBJECT>(&var, constructTree(depth+1));
                object->setEntry(generateVarName().c_str(), var);
            }
            else
            {
                object->setEntry(generateVarName().c_str(), randomValue());
            }
        }
    }
    return object;
}

int main()
{
    srand(1);

    Object *obj = constructTree();

    setEntry<T_FLOAT>(obj, "rofl", 58459.8f);

    obj->startIteration();
    dataElement el;
    while(1)
    {
        el=obj->iterate();
        if(EMPTY_DATA(el.data))
        {
            break;
        }
        //std::cerr << el.name << " : " << get<T_NUMBER>(*static_cast<data_str*>(el.data)) << std::endl;
    }

    //std::cerr << "modif : " << modifSize << std::endl;

    //obj->setEntry("var_92601", newData());

    obj->print();

    //std::cerr << "has entry 2 : " << obj->hasEntry("2") << std::endl;

    delete obj;

    return 0;
}
