#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include <IFeature.h>
#include <FreematicsBase.h>
#include <FreematicsSD.h>

class Persitence : public IFeature {
  public:
    char init();
    char stop();
    //Persitence();

  private:
    SDClass *card;
};


#endif
