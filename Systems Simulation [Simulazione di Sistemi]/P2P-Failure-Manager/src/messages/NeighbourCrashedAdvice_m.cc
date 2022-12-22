//
// Generated file, do not edit! Created by opp_msgc 4.4 from messages/NeighbourCrashedAdvice.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "NeighbourCrashedAdvice_m.h"

USING_NAMESPACE

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




Register_Class(NeighbourCrashedAdvice);

NeighbourCrashedAdvice::NeighbourCrashedAdvice(const char *name, int kind) : ProtocolMessage(name,kind)
{
    this->crashedNode_var = 0;
    crashedNodeNeighbour_arraysize = 0;
    this->crashedNodeNeighbour_var = 0;
}

NeighbourCrashedAdvice::NeighbourCrashedAdvice(const NeighbourCrashedAdvice& other) : ProtocolMessage(other)
{
    crashedNodeNeighbour_arraysize = 0;
    this->crashedNodeNeighbour_var = 0;
    copy(other);
}

NeighbourCrashedAdvice::~NeighbourCrashedAdvice()
{
    delete [] crashedNodeNeighbour_var;
}

NeighbourCrashedAdvice& NeighbourCrashedAdvice::operator=(const NeighbourCrashedAdvice& other)
{
    if (this==&other) return *this;
    ProtocolMessage::operator=(other);
    copy(other);
    return *this;
}

void NeighbourCrashedAdvice::copy(const NeighbourCrashedAdvice& other)
{
    this->crashedNode_var = other.crashedNode_var;
    delete [] this->crashedNodeNeighbour_var;
    this->crashedNodeNeighbour_var = (other.crashedNodeNeighbour_arraysize==0) ? NULL : new int[other.crashedNodeNeighbour_arraysize];
    crashedNodeNeighbour_arraysize = other.crashedNodeNeighbour_arraysize;
    for (unsigned int i=0; i<crashedNodeNeighbour_arraysize; i++)
        this->crashedNodeNeighbour_var[i] = other.crashedNodeNeighbour_var[i];
}

void NeighbourCrashedAdvice::parsimPack(cCommBuffer *b)
{
    ProtocolMessage::parsimPack(b);
    doPacking(b,this->crashedNode_var);
    b->pack(crashedNodeNeighbour_arraysize);
    doPacking(b,this->crashedNodeNeighbour_var,crashedNodeNeighbour_arraysize);
}

void NeighbourCrashedAdvice::parsimUnpack(cCommBuffer *b)
{
    ProtocolMessage::parsimUnpack(b);
    doUnpacking(b,this->crashedNode_var);
    delete [] this->crashedNodeNeighbour_var;
    b->unpack(crashedNodeNeighbour_arraysize);
    if (crashedNodeNeighbour_arraysize==0) {
        this->crashedNodeNeighbour_var = 0;
    } else {
        this->crashedNodeNeighbour_var = new int[crashedNodeNeighbour_arraysize];
        doUnpacking(b,this->crashedNodeNeighbour_var,crashedNodeNeighbour_arraysize);
    }
}

int NeighbourCrashedAdvice::getCrashedNode() const
{
    return crashedNode_var;
}

void NeighbourCrashedAdvice::setCrashedNode(int crashedNode)
{
    this->crashedNode_var = crashedNode;
}

void NeighbourCrashedAdvice::setCrashedNodeNeighbourArraySize(unsigned int size)
{
    int *crashedNodeNeighbour_var2 = (size==0) ? NULL : new int[size];
    unsigned int sz = crashedNodeNeighbour_arraysize < size ? crashedNodeNeighbour_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        crashedNodeNeighbour_var2[i] = this->crashedNodeNeighbour_var[i];
    for (unsigned int i=sz; i<size; i++)
        crashedNodeNeighbour_var2[i] = 0;
    crashedNodeNeighbour_arraysize = size;
    delete [] this->crashedNodeNeighbour_var;
    this->crashedNodeNeighbour_var = crashedNodeNeighbour_var2;
}

unsigned int NeighbourCrashedAdvice::getCrashedNodeNeighbourArraySize() const
{
    return crashedNodeNeighbour_arraysize;
}

int NeighbourCrashedAdvice::getCrashedNodeNeighbour(unsigned int k) const
{
    if (k>=crashedNodeNeighbour_arraysize) throw cRuntimeError("Array of size %d indexed by %d", crashedNodeNeighbour_arraysize, k);
    return crashedNodeNeighbour_var[k];
}

void NeighbourCrashedAdvice::setCrashedNodeNeighbour(unsigned int k, int crashedNodeNeighbour)
{
    if (k>=crashedNodeNeighbour_arraysize) throw cRuntimeError("Array of size %d indexed by %d", crashedNodeNeighbour_arraysize, k);
    this->crashedNodeNeighbour_var[k] = crashedNodeNeighbour;
}

class NeighbourCrashedAdviceDescriptor : public cClassDescriptor
{
  public:
    NeighbourCrashedAdviceDescriptor();
    virtual ~NeighbourCrashedAdviceDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(NeighbourCrashedAdviceDescriptor);

NeighbourCrashedAdviceDescriptor::NeighbourCrashedAdviceDescriptor() : cClassDescriptor("NeighbourCrashedAdvice", "ProtocolMessage")
{
}

NeighbourCrashedAdviceDescriptor::~NeighbourCrashedAdviceDescriptor()
{
}

bool NeighbourCrashedAdviceDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<NeighbourCrashedAdvice *>(obj)!=NULL;
}

const char *NeighbourCrashedAdviceDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int NeighbourCrashedAdviceDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount(object) : 2;
}

unsigned int NeighbourCrashedAdviceDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *NeighbourCrashedAdviceDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "crashedNode",
        "crashedNodeNeighbour",
    };
    return (field>=0 && field<2) ? fieldNames[field] : NULL;
}

int NeighbourCrashedAdviceDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='c' && strcmp(fieldName, "crashedNode")==0) return base+0;
    if (fieldName[0]=='c' && strcmp(fieldName, "crashedNodeNeighbour")==0) return base+1;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *NeighbourCrashedAdviceDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : NULL;
}

const char *NeighbourCrashedAdviceDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int NeighbourCrashedAdviceDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    NeighbourCrashedAdvice *pp = (NeighbourCrashedAdvice *)object; (void)pp;
    switch (field) {
        case 1: return pp->getCrashedNodeNeighbourArraySize();
        default: return 0;
    }
}

std::string NeighbourCrashedAdviceDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    NeighbourCrashedAdvice *pp = (NeighbourCrashedAdvice *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getCrashedNode());
        case 1: return long2string(pp->getCrashedNodeNeighbour(i));
        default: return "";
    }
}

bool NeighbourCrashedAdviceDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    NeighbourCrashedAdvice *pp = (NeighbourCrashedAdvice *)object; (void)pp;
    switch (field) {
        case 0: pp->setCrashedNode(string2long(value)); return true;
        case 1: pp->setCrashedNodeNeighbour(i,string2long(value)); return true;
        default: return false;
    }
}

const char *NeighbourCrashedAdviceDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        NULL,
    };
    return (field>=0 && field<2) ? fieldStructNames[field] : NULL;
}

void *NeighbourCrashedAdviceDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    NeighbourCrashedAdvice *pp = (NeighbourCrashedAdvice *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


