/*
 * SavedSimState.h
 *
 *  Created on: 15.07.2009
 *      Author: AT
 */
#ifndef SIMSAVEDSTATE_H_
#define SIMSAVEDSTATE_H_

#include "EventList.h"
#include "gmnet.h"
#include "BindingDummy.h"

class BindingDummy;

typedef set<BindingDummy> set_BindingDummy;
typedef map<string, set_BindingDummy> map_bindings;
typedef map<string, vector_tokensID> map_markings;
typedef map<string, vector_tokensID> map_notBoundTokens;
typedef map<string, TransitionList> map_transitionList;
typedef map<string, bool> map_transitionInEventList;

class SimSavedState {
public:
	SimSavedState(NetClass *net, int currentLevel, DateTime currentTime);
	~SimSavedState();

	void setSavedMarkings(PlaceMap *places);
    void savePlaceMarking(string current_place_name, TokenList current_marking);
    void restoreMarkings(PlaceMap *places);
    void restorePlaceMarking(Place *place);
    vector_tokensID getSavedTokensID(string placeName);

    void setSavedBindings(TransitionMap *transitions);
    void saveTransitionBindings(Transition *current_transition, EventList *current_bindings);

    void removeSavedBindings();
    void removeSavedBindings(TransitionMap *transitions);
    vector_tokensID createTokensIDList(TokenList current_tokens);
    void addRestoredToken(SMARTPTR(Token) newToken);
    SMARTPTR(Token) getSavedToken(int tokenID);
    SMARTPTR(Token) getRestoredToken(int tokenID);
    TokenList* createTokenList(vector_tokensID savedTokensID);
    void restoreTransitionBindings(Transition *currentTransition);

    void setSavedNotBoundTokens(TransitionMap *transitions);
    void saveTransitionNotBoundTokens(string currentTransitionName, TokenList currentNotBoundTokes);
    void restoreTransitionNotBoundTokens(Transition *currentTransition);

    void setSavedAffectedTransitions(TransitionMap *transitions);
    void restoreSavedAffectedTransitions(TransitionMap *transitions);

    void saveTransitionStates(TransitionMap *transitions);
    void restoreTransitionStates(TransitionMap *transitions);

    int getStateLevel() {return stateLevel;}
    void setStateLevel(int current_level) {stateLevel = current_level;}

    DateTime getStateTime() {return stateTime;}
    void setStateTime(DateTime current_time) {stateTime = current_time;}

    int getTrials() {return trials;}
    void setTrials(int current_trials) {trials = current_trials;}
    void decrementTrials() {trials--;}
private:
	map_markings savedMarkings; // map of markings snapshots
	map_bindings savedBindings; // map of bindings snapshots
	map_notBoundTokens savedNotBoundTokens; // map of saved not bound tokens
	map_tokens savedTokens; // map of saved tokens
	map_tokens restoredTokens; // map of restored tokens
	map_transitionList savedCouldEnableTransitions;
	map_transitionList savedCouldDisableTransitions;
	map_transitionInEventList savedInEventList;

	int stateLevel; // the level of saved state
	DateTime stateTime; // the time of saved stated to jump back
	int trials; // number of saved state's free trials

	NetClass *net;
};

#endif /* SIMSAVEDSTATE_H_ */
