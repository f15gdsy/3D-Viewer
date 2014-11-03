//
//  SMFileParser.h
//  3DCocos
//
//  Created by Yifeng Wu on 10/10/14.
//
//

#ifndef ___DCocos__SMFileParser__
#define ___DCocos__SMFileParser__

#include <string>

#include "SMacros.h"
#include "SMeshDataDelegate.h"

NS_S_BEGIN


class SMFileParser {
public:
    void parse (const std::string& fileName);
    void inline setDelegate (SMeshDataDelegate* delegate) {
        _delegate = delegate;
    }
private:
    int getNextInt ();
    float getNextFloat();
    bool checkNextString (const std::string str);
    
    std::string _data;
    int _currentPosition;
    
    SMeshDataDelegate* _delegate;
};

NS_S_END

#endif /* defined(___DCocos__SMFileParser__) */
