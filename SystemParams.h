#ifndef SYSTEMPARAMS_H
#define SYSTEMPARAMS_H

/*
* Reza Adhitya Saputra
* reza.adhitya.saputra@gmail.com
*
*/

class SystemParams
{
public:
    SystemParams();
    ~SystemParams();

public:
    static int num_stipples;
    static int cone_slices;
    static int cone_diameter;
    static int max_iter;
};

#endif // SYSTEMPARAMS_H
