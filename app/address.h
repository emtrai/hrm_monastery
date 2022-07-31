#ifndef ADDRESS_H
#define ADDRESS_H
#include <QString>

class Address
{
public:
    Address();
    Address(const QString& fullAddr, const QString& province, const QString& country);
    const QString &province() const;
    void setProvince(const QString &newProvince);

    const QString &country() const;
    void setCountry(const QString &newCountry);

    const QString &name() const;
    void setName(const QString &newName);

    const QString &fullAddr() const;
    void setFullAddr(const QString &newFullAddr);

private:
    QString mName; // Ten dia danh
    QString mFullAddr; // Dia chi day du
    QString mProvince; // tinh, thanh pho thuoc trung uong
    QString mCountry; // quoc gia
};

#endif // ADDRESS_H
