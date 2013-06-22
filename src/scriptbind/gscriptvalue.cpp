#include "cpgf/scriptbind/gscriptvalue.h"
#include "cpgf/scriptbind/gscriptbindapi.h"
#include "cpgf/gmetaapiutil.h"
#include "cpgf/gmetaapi.h"
#include "cpgf/gglobal.h"


namespace cpgf {


GScriptValue::GScriptValue(Type type, const GVariant & value, IMetaItem * metaItem)
	: type(type), value(value), metaItem(metaItem)
{
}

GScriptValue::GScriptValue(Type type, const GVariant & value)
	: type(type), value(value), metaItem()
{
}

GScriptValue::GScriptValue()
	: type(typeEmpty), value(), metaItem()
{
}

GScriptValue::GScriptValue(const GScriptValue & other)
	: type(other.type), value(other.value), metaItem(other.metaItem)
{
}

GScriptValue & GScriptValue::operator = (const GScriptValue & other)
{
	if(this != &other) {
		this->type = other.type;
		this->value = other.value;
		this->metaItem = other.metaItem;
	}
	
	return *this;
}

GScriptValue GScriptValue::fromNull()
{
	return GScriptValue(typeNull, (void *)0);
}

GScriptValue GScriptValue::fromFundamental(const GVariant & fundamental)
{
	return GScriptValue(typeFundamental, fundamental);
}

GScriptValue GScriptValue::fromString(const char * s)
{
	return GScriptValue(typeString, createTypedVariant(createStringVariant(s), createMetaType<char *>()));
}

GScriptValue GScriptValue::fromMetaClass(IMetaClass * metaClass)
{
	GMetaType metaType(metaGetTypedItemMetaType(metaClass));
	metaType.addPointer();
	return GScriptValue(typeMetaClass, createTypedVariant(metaClass, metaType));
}

GScriptValue GScriptValue::fromObject(const GVariant & instance, IMetaClass * metaClass)
{
	GMetaType metaType(metaGetTypedItemMetaType(metaClass));
	metaType.addPointer();
	return GScriptValue(typeObject, createTypedVariant(instance, metaType), metaClass);
}

GScriptValue GScriptValue::fromMethod(void * instance, IMetaMethod * method)
{
	return GScriptValue(typeMethod, instance, method);
}

GScriptValue GScriptValue::fromOverridedMethods(IMetaList * methods)
{
	return GScriptValue(typeOverridedMethods, methods);
}

GScriptValue GScriptValue::fromEnum(IMetaEnum * metaEnum)
{
	return GScriptValue(typeEnum, metaEnum);
}

GScriptValue GScriptValue::fromRaw(const GVariant & raw)
{
	return GScriptValue(typeRaw, raw);
}

GScriptValue GScriptValue::fromAccessible(void * instance, IMetaAccessible * accessible)
{
	return GScriptValue(typeAccessible, instance, accessible);
}

GScriptValue GScriptValue::fromScriptObject(IScriptObject * scriptObject)
{
	return GScriptValue(typeScriptObject, scriptObject);
}

GScriptValue GScriptValue::fromScriptMethod(IScriptFunction * scriptFunction)
{
	return GScriptValue(typeScriptMethod, scriptFunction);
}

void * GScriptValue::toNull() const
{
	return NULL;
}

GVariant GScriptValue::toFundamental() const
{
	if(this->isFundamental()) {
		return this->value;
	}
	else {
		return GVariant();
	}
}

std::string GScriptValue::toString() const
{
	if(this->isString()) {
		return fromVariant<char *>(this->value);
	}
	else {
		return "";
	}
}

IMetaClass * GScriptValue::toMetaClass() const
{
	if(this->isMetaClass()) {
		IMetaClass * metaClass = fromVariant<IMetaClass *>(this->value);
		metaClass->addReference();
		return metaClass;
	}
	else {
		return NULL;
	}
}

GVariant GScriptValue::toObject(IMetaClass ** metaClass) const
{
	if(metaClass != NULL) {
		*metaClass = NULL;
	}

	if(this->isObject()) {
		if(metaClass != NULL) {
			*metaClass = gdynamic_cast<IMetaClass *>(this->metaItem.get());
			this->metaItem->addReference();
		}
		return this->value;
	}
	else {
		return GVariant();
	}
}

void * GScriptValue::toObjectAddress(IMetaClass ** metaClass) const
{
	GVariant instance(this->toObject(metaClass));
	if(! instance.isEmpty()) {
		return fromVariant<void *>(instance);
	}
	else {
		return NULL;
	}
}

IMetaMethod * GScriptValue::toMethod(void ** outInstance) const
{
	if(outInstance != NULL) {
		*outInstance = NULL;
	}

	if(this->isMethod()) {
		IMetaMethod * metaMethod = gdynamic_cast<IMetaMethod *>(this->metaItem.get());
		metaMethod->addReference();
		if(outInstance != NULL) {
			*outInstance = fromVariant<void *>(this->value);
		}
		return metaMethod;
	}
	else {
		return NULL;
	}
}

IMetaList * GScriptValue::toOverridedMethods() const
{
	if(this->isOverridedMethods()) {
		IMetaList * metaList = fromVariant<IMetaList *>(this->value);
		metaList->addReference();
		return metaList;
	}
	else {
		return NULL;
	}
}

IMetaEnum * GScriptValue::toEnum() const
{
	if(this->isEnum()) {
		IMetaEnum * metaEnum = fromVariant<IMetaEnum *>(this->value);
		metaEnum->addReference();
		return metaEnum;
	}
	else {
		return NULL;
	}
}

GVariant GScriptValue::toRaw() const
{
	if(this->isRaw()) {
		return this->value;
	}
	else {
		return GVariant();
	}
}

IMetaAccessible * GScriptValue::toAccessible(void ** outInstance) const
{
	if(outInstance != NULL) {
		*outInstance = NULL;
	}

	if(this->isAccessible()) {
		IMetaAccessible * accessible = gdynamic_cast<IMetaAccessible *>(this->metaItem.get());
		accessible->addReference();
		if(outInstance != NULL) {
			*outInstance = fromVariant<void *>(this->value);
		}
		return accessible;
	}
	else {
		return NULL;
	}
}

IScriptObject * GScriptValue::toScriptObject() const
{
	if(this->isScriptObject()) {
		IScriptObject * scriptObject = fromVariant<IScriptObject *>(this->value);
		scriptObject->addReference();
		return scriptObject;
	}
	else {
		return NULL;
	}
}

IScriptFunction * GScriptValue::toScriptMethod() const
{
	if(this->isScriptMethod()) {
		IScriptFunction * scriptFunction = fromVariant<IScriptFunction *>(this->value);
		scriptFunction->addReference();
		return scriptFunction;
	}
	else {
		return NULL;
	}
}

IMetaTypedItem * getTypedItemFromScriptValue(const GScriptValue & value)
{
	if(value.isObject()) {
		IMetaClass * metaClass;
		value.toObject(&metaClass);
		return metaClass;
	}
	else if(value.isMetaClass()) {
		return value.toMetaClass();
	}
	else if(value.isEnum()) {
		return value.toEnum();
	}
	else {
		return NULL;
	}
}


} // namespace cpgf

