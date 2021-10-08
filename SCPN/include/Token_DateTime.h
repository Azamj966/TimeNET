#ifndef TOKEN_DATETIME_H
#define TOKEN_DATETIME_H

#include "Token.h"
#include "Place.h"
#include "gmdb.h"

class Token_DateTime : public Token {
public:
    Token_DateTime(const DateTime& value);
    Token_DateTime(Token_DateTime& other, bool copyTransition);
    ~Token_DateTime();
    Token_DateTime& operator= (const Token_DateTime& tdatetime);
    bool operator==( const Token_DateTime & T ) const { return value == T.value; }
    bool operator==( const DateTime & x) const { return value == x; }
    const DateTime& getValue() const { return value; }
    const std::string getName() const {return "DateTime";}
    const std::string content() const;
    //virutal void setReferenceAttribute(char* attributename,SMARTPTR(Token) reference);
    static void addInitialTokens(Place* p);
    static void createTokensFromDB(NetClass* net, GMDB* connection, token_info_list &tokens, token_reference_list &refs);

private:
    DateTime value;
};

#endif  //TOKEN_DATETIME_H
