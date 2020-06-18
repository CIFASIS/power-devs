#include "indpar.h"
#include <string.cpp>
char* indexpar(char* par, int index) {
    string str(par);
    string searchString( "index" ); 
    string replaceString( "1" );
    string::size_type pos = 0;
    while ( (pos = str.find(searchString, pos)) != string::npos ) {
        str.replace( pos, searchString.size(), replaceString );
        pos++;
    }
  return (char*)str;
}
;
 
