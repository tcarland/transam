/**
  * @file include/Encode.h 
  * @author tcarland@gmail.com
  * 
 **/
#ifndef _TRANSAM_ENCODE_H_
#define _TRANSAM_ENCODE_H_


#include "TransFile.h"


namespace transam {



class Encode {

  public:

    Encode ( encoding_t type );

    ~Encode();



  private:

    encoding_t  _type;

};


}  // namespace

#endif  // _TRANSAM_ENCODE_H_

