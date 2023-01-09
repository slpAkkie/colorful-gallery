#ifndef THUMBNNAIL_H
#define THUMBNNAIL_H

#include <QToolButton>

class Thumbnnail : public QToolButton
{
    Q_OBJECT

public:
    explicit Thumbnnail(QWidget *parent = nullptr);
    ~Thumbnnail();
};

#endif // THUMBNNAIL_H
