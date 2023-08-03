#pragma once

#include <iostream>
#include <fstream>

namespace imgl
{
void loadImage(const std::string & path)
{
    //char * buffer;
    
    std::ifstream file("cross.txt");
    if (file.is_open())
    {
        /* // Sets the position of the next character to be extracted to file.end + 0 */
        /* file.seekg(0, file.end); */
        /* int size = file.tellg(); // get pos in input sequence */
        /* file.seekg(8); // skip 8 bits png header */

        /* buffer = new char [size - 8]; */
        /* file.read(buffer, size - 8); */
        
        /* file.close(); */

        /* for (uint32_t i = 0; i != size - 8; ++i) */
        /*     std::cout << buffer[i]; */

        /* delete [] buffer; */

        std::cout << "test\n";
    }
}
} // namespace imgl
