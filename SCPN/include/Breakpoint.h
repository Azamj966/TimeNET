#ifndef BREAKPOINT_H
#define BREAKPOINT_H

#include "MeasureObj.h"

enum BreakpointRelation {
	Unknown,
	Equal,
	GreaterThan,
	LowerThan,
	GreaterThanOrEqual,
	LowerThanOrEqual
};

#ifdef USE_BOEHM_GC
/* Everything derived from this class will be garbage collected. */
class Breakpoint : public gc_cleanup
#else
class Breakpoint
#endif
{
public:
	Breakpoint(MeasureObj *measure, BreakpointRelation relation, double value);
	Breakpoint();
	bool isTriggered();
	static BreakpointRelation stringToRelation(string relation);

private:
	MeasureObj *measure;
	BreakpointRelation relation;
	double value;
};


typedef vector<Breakpoint> BreakpointList;

#endif /* BREAKPOINT_H */
