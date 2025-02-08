#ifndef RAIIGUARD_H
#define RAIIGUARD_H
#include <functional>

using namespace std;

/*
 * just make sure that the callback is called when the object goes out of scope
 */

class RAIIGuard
{
public:
    RAIIGuard(const function<void()> &callback) : deletionCallback(callback) {}
    ~RAIIGuard() { deletionCallback(); }

private:
    function<void()> deletionCallback;
};;

#endif // RAIIGUARD_H

