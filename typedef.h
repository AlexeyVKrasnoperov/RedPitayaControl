#ifndef _typedef_
#define _typedef_

#include <stdint.h>
#include <vector>

typedef std::vector<uint32_t>  uivector;
typedef uivector::iterator     uivector_itr;
typedef std::vector<uivector>  uimatrix;
typedef uimatrix::iterator     uimatrix_itr;
typedef std::vector<int32_t>    ivector;
typedef std::vector<double>     dvector;


#define BIT(n)       (1ULL << (n))
#define SETBIT(n,i)  ((n) |= BIT(i))
#define CLRBIT(n,i)  ((n) &= ~BIT(i))
#define TESTBIT(n,i) ((bool)(((n) & BIT(i)) != 0))

#endif
