#ifndef TOKEN_REAL_H
#define TOKEN_REAL_H


#include "Place.h"
#include "Token.h"
#include "gmdb.h"

class Token_real : public Token {
public:
    Token_real(double value);
    Token_real(Token_real& other, bool copyTransition);
    ~Token_real();
    Token_real& operator= (const Token_real& treal);
    bool operator==( const Token_real & T ) const { return value == T.value; }
    bool operator==( const double & x) const { return value == x; }
    double getValue() const { return value; }
    const std::string getName() const {return "real";}
    const std::string content() const;
    //virutal void setReferenceAttribute(char* attributename,SMARTPTR(Token) reference);
    static void addInitialTokens(Place* p);
    static void createTokensFromDB(NetClass* net, GMDB* connection, token_info_list &tokens, token_reference_list &refs);

private:
    double value;
};

#endif  //TOKEN_REAL_H
