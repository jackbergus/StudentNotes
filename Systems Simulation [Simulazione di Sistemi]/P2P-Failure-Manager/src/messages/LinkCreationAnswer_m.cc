//
// Generated file, do not edit! Created by opp_msgc 4.4 from messages/LinkCreationAnswer.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "LinkCreationAnswer_m.h"

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




Register_Class(LinkCreationAnswer);

LinkCreationAnswer::LinkCreationAnswer(const char *name, int kind) : ProtocolMessage(name,kind)
{
    this->answer_var = 0;
}

LinkCreationAnswer::LinkCreationAnswer(const LinkCreationAnswer& other) : ProtocolMessage(other)
{
    copy(other);
}

LinkCreationAnswer::~LinkCreationAnswer()
{
}

LinkCreationAnswer& LinkCreationAnswer::operator=(const LinkCreationAnswer& other)
{
    if (this==&other) return *this;
    ProtocolMessage::operator=(other);
    copy(other);
    return *this;
}

void LinkCreationAnswer::copy(const LinkCreationAnswer& other)
{
    this->answer_var = other.answer_var;
}

void LinkCreationAnswer::parsimPack(cCommBuffer *b)
{
    ProtocolMessage::parsimPack(b);
    doPacking(b,this->answer_var);
}

void LinkCreationAnswer::parsimUnpack(cCommBuffer *b)
{
    ProtocolMessage::parsimUnpack(b);
    doUnpacking(b,this->answer_var);
}

int LinkCreationAnswer::getAnswer() const
{
    return answer_var;
}

void LinkCreationAnswer::setAnswer(int answer)
{
    this->answer_var = answer;
}

class LinkCreationAnswerDescriptor : public cClassDescriptor
{
  public:
    LinkCreationAnswerDescriptor();
    virtual ~LinkCreationAnswerDescriptor();

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

Register_ClassDescriptor(LinkCreationAnswerDescriptor);

LinkCreationAnswerDescriptor::LinkCreationAnswerDescriptor() : cClassDescriptor("LinkCreationAnswer", "ProtocolMessage")
{
}

LinkCreationAnswerDescriptor::~LinkCreationAnswerDescriptor()
{
}

bool LinkCreationAnswerDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<LinkCreationAnswer *>(obj)!=NULL;
}

const char *LinkCreationAnswerDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int LinkCreationAnswerDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int LinkCreationAnswerDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *LinkCreationAnswerDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "answer",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int LinkCreationAnswerDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='a' && strcmp(fieldName, "answer")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *LinkCreationAnswerDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *LinkCreationAnswerDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "answer_e";
            return NULL;
        default: return NULL;
    }
}

int LinkCreationAnswerDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    LinkCreationAnswer *pp = (LinkCreationAnswer *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string LinkCreationAnswerDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    LinkCreationAnswer *pp = (LinkCreationAnswer *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getAnswer());
        default: return "";
    }
}

bool LinkCreationAnswerDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    LinkCreationAnswer *pp = (LinkCreationAnswer *)object; (void)pp;
    switch (field) {
        case 0: pp->setAnswer(string2long(value)); return true;
        default: return false;
    }
}

const char *LinkCreationAnswerDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
    };
    return (field>=0 && field<1) ? fieldStructNames[field] : NULL;
}

void *LinkCreationAnswerDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    LinkCreationAnswer *pp = (LinkCreationAnswer *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


