#pragma once
#include <QString>

struct Product {
    int id = 0;
    QString name;
    QString description;
    double price = 0.0;
    QString status;
    int stock = 0;
    QString category;
    QString location;
    QString image;
    QString lastUse;
};
