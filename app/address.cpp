#include "address.h"

Address::Address()
{

}

Address::Address(const QString& fullAddr, const QString& province, const QString& country)
{
    this->mFullAddr = fullAddr;
    this->mProvince = province;
    this->mCountry = country;
}


const QString &Address::province() const
{
    return mProvince;
}

void Address::setProvince(const QString &newProvince)
{
    mProvince = newProvince;
}

const QString &Address::country() const
{
    return mCountry;
}

void Address::setCountry(const QString &newCountry)
{
    mCountry = newCountry;
}

const QString &Address::name() const
{
    return mName;
}

void Address::setName(const QString &newName)
{
    mName = newName;
}

const QString &Address::fullAddr() const
{
    return mFullAddr;
}

void Address::setFullAddr(const QString &newFullAddr)
{
    mFullAddr = newFullAddr;
}
