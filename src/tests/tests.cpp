/* The MIT License (MIT)
*  Copyright (c) 2016 Pinaky Bhattacharyya
*  [See LICENSE.txt in project root folder for more details.]
*/


#ifdef DEBUG
    #include "debug.h"
#endif

#include <iostream>
#include "test_header.h"

int main()
{
    try
    {
        aimstesting::test_utilities();
    }
    catch(std::logic_error &e)
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
    catch(...)
    {
        std::cerr << "Error: Unhandled exception has occurred." << std::endl;
        exit(2);
    }

    return 0;
}
