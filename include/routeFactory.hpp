#include "IVehicleRouting.hpp"

/**
 * AbstractRouteFactory provides an interface to the client 
 * to use different route generating algorithms present 
 * in this library.
 */
class AbstractRouteFactory {
public:
	virtual RoutesInterface* createNewRouteSets() = 0;
};

/**
 * ConcreteRouteFactory is the frist variant of the 
 * algorithm. More variants can be added here.
 */
class ConcreteRouteFactory : public AbstractRouteFactory {
public:
    ConcreteRouteFactory(string path) : path_(path) {}
    RoutesInterface* createNewRouteSets()
    {
        return new Routes(path_);
    }
    ~ConcreteRouteFactory() 
    {
        path_.clear();
    }
private:
    string path_;
};


