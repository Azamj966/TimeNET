#ifndef TOKEN_STRING_H
#define TOKEN_STRING_H

#include "Token.h"
#include "Place.h"
#include "gmdb.h"

class Token_string : public Token {
public:
    Token_string(string value);
    Token_string(Token_string& other, bool copyTransition);
    ~Token_string();
    Token_string& operator= (const Token_string& tstring);
    bool operator==( const Token_string & T ) const { return value == T.value; }
    bool operator==( const string & x) const { return value == x; }
    const string& getValue() const { return value; }
    const string getName() const {return "string";}
    const string content() const;
    //virutal void setReferenceAttribute(char* attributename,SMARTPTR(Token) reference);
    static void addInitialTokens(Place* p);
    static void createTokensFromDB(NetClass* net, GMDB* connection, token_info_list &tokens, token_reference_list &refs);

private:
    string value;
};

#endif  //TOKEN_STRING_H
