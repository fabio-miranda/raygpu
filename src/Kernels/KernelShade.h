#ifndef KernelShade_H
#define KernelShade_H

#include "Kernels/KernelBase.h"

class KernelShade : public KernelBase {

public:
	KernelShade();
	~KernelShade();

	void step();

private:

};


#endif