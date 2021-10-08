#ifndef TOKEN_INT_H
#define TOKEN_INT_H

#include "Token.h"
#include "Place.h"
#include "gmdb.h"

class Token_int : public Token {
public:
    Token_int(long value);

    // modified constructor for RESTART
    // Token_int(Token_int& other);
    Token_int(Token_int& other, bool copyTransition);

    ~Token_int();
    Token_int& operator= (const Token_int& tint);
    bool operator==( const Token_int & T ) const { return value == T.value; }
    bool operator==( const long & x) const { return value == x; }
    long getValue() const { return value; }
    const std::string getName() const {return "int";}
    const std::string content() const;
    //virutal void setReferenceAttribute(char* attributename,SMARTPTR(Token) reference);
    static void addInitialTokens(Place* p);
    static void createTokensFromDB(NetClass* net, GMDB* connection, token_info_list &tokens, token_reference_list &refs);

private:
    long value;
};

#endif  //TOKEN_INT_H
