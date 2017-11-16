#ifndef IFEATURE_H
#define IFEATURE_H

class IFeature{
  protected:
    char status = 0;
  public:
    char getStatus(){ return this->status;}
    virtual char init() = 0;
    virtual char stop() = 0;
};

#endif
