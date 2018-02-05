// #pragma once
//
// #include <stdio.h>
// #include <iostream>
// #include <sstream>
// #include <vector>
// #include "glm/glm.hpp"
//
// namespace ost {
//
// std::vector<glm::vec2> readLevel(const char* level_path)
// {
//     std::vector<glm::vec2> buffer;
//     {
//         const char* fname = "./levels/level0";
//         std::ifstream in;
//         in.open(fname, std::ios::binary);
//
//         if (in.fail()) {
//             std::cerr << "Unable to open " << fname << " I'm out!" << std::endl;
//             exit(-1);
//         }
//
//         // READ LEVEL SIZES
//         std::string sizeDefs;
//         std::getline(in, sizeDefs);
//         std::size_t delimIndex =  sizeDefs.find("x");
//         std::size_t levelWidth = std::stoi(sizeDefs.substr(0, delimIndex));
//         std::size_t levelHeight = std::stoi(sizeDefs.substr(delimIndex+1, sizeDefs.size()-delimIndex+1));
//
//
//         std::string line;
//         for(float y=0; y < levelHeight ;y++) {
//             if (in.eof() || in.bad()) {
//                 PANIC("Y out of range in level read");
//             }
//             std::getline(in, line);
//             std::stringstream ss(line);
//
//             for(float x=0;x < levelWidth;x++) {
//                 int n;
//                 if(!(ss >> n)) {PANIC("X out of range in level read");} //failed to read, must be end of line
//                 if(n % 2 == 0) { buffer.push_back(glm::vec2((x/levelWidth)-.5f, ((levelHeight-y)/levelHeight)-.5f) * 2.0f); } //FILL THE BUFFER WITH Vectors - vertex candidate
//             }
//         }
//     }
//     return buffer;
// }
//
// }
