#pragma once

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <vector>
#include "./Level.hpp"
#include "./macro.hpp"
#include "glm/glm.hpp"

namespace ost {

Level loadLevel(const char* level_path)
{
    std::vector<glm::vec2> buffer;
        std::ifstream in;
        in.open(level_path, std::ios::binary);

        if (in.fail()) {
            std::cerr << "Unable to open " << level_path << " I'm out!" << std::endl;
            PANIC("Y out of range in level read");
        }

        // READ LEVEL SIZES
        std::string sizeDefs;
        std::getline(in, sizeDefs);
        std::size_t delimIndex =  sizeDefs.find("x");
        std::size_t levelWidth = std::stoi(sizeDefs.substr(0, delimIndex));
        std::size_t levelHeight = std::stoi(sizeDefs.substr(delimIndex+1, sizeDefs.size()-delimIndex+1));

        float levelSize = (levelWidth > levelHeight) ? levelWidth : levelHeight; //select the biggest size as we'll draw maps in a 1:1

        std::string line;
        for(float y=0; y < levelHeight ;y++) {
            if (in.eof() || in.bad()) {
                PANIC("Y out of range in level read");
            }
            std::getline(in, line);
            std::stringstream ss(line);

            for(float x=0;x < levelWidth;x++) {
                int n;
                if(!(ss >> n)) {
                    PANIC("X out of range while reading level level read");
                } // failed to read, must be end of line
                if(n % 2 == 0) { buffer.push_back(glm::vec2(x-levelWidth*0.5f, (levelSize-y)-levelHeight*0.5f)/levelSize ); } // FILL THE BUFFER WITH Vectors - vertex candidate
            }
        }

    Level lvl(buffer, glm::vec2(levelWidth,levelHeight));

    //TODO return levelSize to be able to set the uniform view matrix
    return lvl;
}

}
