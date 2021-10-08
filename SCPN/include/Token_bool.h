#ifndef TOKEN_BOOL_H
#define TOKEN_BOOL_H


#include "Token.h"
#include "Place.h"
#include "gmdb.h"

class Token_bool : public Token {
public:
    Token_bool(bool value);
    Token_bool(Token_bool& other, bool copyTransition);
    ~Token_bool();
    Token_bool& operator= (const Token_bool& tbool);
    bool operator==( const Token_bool & T ) const { return value == T.value; }
    bool operator==( const bool & x) const { return value == x; }
    bool getValue() const { return value; }
    const std::string getName() const {return "bool";}
    const std::string content() const;
    //virutal void setReferenceAttribute(char* attributename,SMARTPTR(Token) reference);
    static void addInitialTokens(Place* p);
    static void createTokensFromDB(NetClass* net, GMDB* connection, token_info_list &tokens, token_reference_list &refs);

private:
    bool value;
};

#endif  //TOKEN_BOOL_H
